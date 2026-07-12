#!/usr/bin/env python3
"""
Convert the BIMETS MDL text version of FRB/US into Dynare files.

This script is intentionally conservative: it performs syntax conversion and
writes a transparent metadata report, but it does not try to solve the model.
The economic equations are kept one-to-one with the BIMETS MDL source except
for a small set of BIMETS IF> branches that are collapsed to max() formulas.

Run from the bundle root:
    python3 scripts/parse_frbus_mdl.py --all
"""
from __future__ import annotations

import argparse
import csv
import json
import re
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Iterable, List, Sequence, Tuple

ROOT = Path(__file__).resolve().parents[1]
SRC_DIR = ROOT / "source"
DYN_DIR = ROOT / "dynare"
INC_DIR = DYN_DIR / "includes"
MATLAB_DIR = ROOT / "matlab"
DOC_DIR = ROOT / "docs"

FUNCTIONS = {"TSLAG", "TSLEAD", "TSDELTA", "TSDELTALOG", "MOVAVG", "MOVSUM", "LOG", "EXP"}
DYNARE_FUNCS = {"log", "exp", "max", "min"}

@dataclass
class EqBlock:
    name: str
    condition: str | None
    lhs: str
    rhs: str
    raw_eq: str
    description: List[str]

@dataclass
class ParsedModel:
    name: str
    text: str
    equations: List[EqBlock]
    endo: List[str]
    exo: List[str]
    descriptions: Dict[str, str]
    raw_exog_lines: List[str]


def read_model_text(path: Path) -> str:
    """Read the uploaded R data dump / CSV-like file and return the MDL text."""
    with path.open(newline="", encoding="utf-8") as f:
        rows = list(csv.reader(f))
    if len(rows) >= 2:
        # frbus_model.txt has columns [x], [MODEL...].
        # frbus_model_mce.txt has columns [rowname,x], [1,MODEL...].
        candidates = rows[1]
        return max(candidates, key=len).replace("\r\n", "\n").replace("\r", "\n")
    return path.read_text(encoding="utf-8")


def strip_outer_parens(s: str) -> str:
    s = s.strip()
    changed = True
    while changed and s.startswith("(") and s.endswith(")"):
        changed = False
        depth = 0
        ok = True
        for i, ch in enumerate(s):
            if ch == "(":
                depth += 1
            elif ch == ")":
                depth -= 1
                if depth == 0 and i != len(s) - 1:
                    ok = False
                    break
        if ok:
            s = s[1:-1].strip()
            changed = True
    return s


def split_top_level_commas(s: str) -> List[str]:
    out: List[str] = []
    depth = 0
    start = 0
    for i, ch in enumerate(s):
        if ch == "(":
            depth += 1
        elif ch == ")":
            depth -= 1
        elif ch == "," and depth == 0:
            out.append(s[start:i].strip())
            start = i + 1
    out.append(s[start:].strip())
    return out


def find_matching_paren(s: str, open_idx: int) -> int:
    depth = 0
    for i in range(open_idx, len(s)):
        if s[i] == "(":
            depth += 1
        elif s[i] == ")":
            depth -= 1
            if depth == 0:
                return i
    raise ValueError(f"No matching parenthesis in: {s[open_idx:open_idx+100]}")


def first_function_call(s: str) -> Tuple[int, int, str, str] | None:
    # Return start, end_exclusive, function_name, inner_args
    for m in re.finditer(r"\b(TSLAG|TSLEAD|TSDELTA|TSDELTALOG|MOVAVG|MOVSUM|LOG|EXP)\s*\(", s):
        name = m.group(1).upper()
        open_idx = s.find("(", m.start())
        close_idx = find_matching_paren(s, open_idx)
        return m.start(), close_idx + 1, name, s[open_idx + 1:close_idx]
    return None


def apply_shift_to_dynare_expr(expr: str, shift: int, varnames: set[str]) -> str:
    """Add a time shift to all model variables already in Dynare expression syntax."""
    if shift == 0:
        return expr
    # Matches variable with optional existing Dynare time index immediately after it.
    token_re = re.compile(r"\b([A-Za-z_]\w*)\b(\((-?\d+)\))?")

    def repl(m: re.Match) -> str:
        tok = m.group(1)
        if tok.lower() not in varnames or tok.lower() in DYNARE_FUNCS:
            return m.group(0)
        old = int(m.group(3)) if m.group(3) is not None else 0
        new = old + shift
        return tok if new == 0 else f"{tok}({new:+d})"

    return token_re.sub(repl, expr)


def convert_expr_dynare(expr: str, varnames: set[str]) -> str:
    """Convert one BIMETS expression into Dynare expression syntax."""
    expr = expr.strip()
    while True:
        call = first_function_call(expr)
        if call is None:
            break
        start, end, name, inner = call
        args = split_top_level_commas(inner)
        if name == "LOG":
            repl = f"log({convert_expr_dynare(args[0], varnames)})"
        elif name == "EXP":
            repl = f"exp({convert_expr_dynare(args[0], varnames)})"
        elif name in {"TSLAG", "TSLEAD"}:
            n = int(args[1]) if len(args) > 1 and args[1] else 1
            if name == "TSLAG":
                n = -n
            conv = convert_expr_dynare(args[0], varnames)
            repl = f"({apply_shift_to_dynare_expr(conv, n, varnames)})"
        elif name == "TSDELTA":
            n = int(args[1]) if len(args) > 1 and args[1] else 1
            conv = convert_expr_dynare(args[0], varnames)
            repl = f"(({conv}) - ({apply_shift_to_dynare_expr(conv, -n, varnames)}))"
        elif name == "TSDELTALOG":
            n = int(args[1]) if len(args) > 1 and args[1] else 1
            conv = convert_expr_dynare(args[0], varnames)
            repl = f"(log({conv}) - log({apply_shift_to_dynare_expr(conv, -n, varnames)}))"
        elif name in {"MOVAVG", "MOVSUM"}:
            if len(args) < 2:
                raise ValueError(f"{name} requires length: {expr}")
            length = int(float(args[1]))
            conv = convert_expr_dynare(args[0], varnames)
            terms = [f"({apply_shift_to_dynare_expr(conv, -i, varnames)})" for i in range(length)]
            repl = "(" + " + ".join(terms) + ")"
            if name == "MOVAVG":
                repl = f"({repl}/{length})"
        else:
            raise ValueError(name)
        expr = expr[:start] + repl + expr[end:]
    expr = re.sub(r"\bLOG\b", "log", expr, flags=re.I)
    expr = re.sub(r"\bEXP\b", "exp", expr, flags=re.I)
    return expr


def convert_expr_matlab(expr: str, varnames: set[str], shift: int = 0, prefix: str = "D") -> str:
    """Convert BIMETS expression into scalar MATLAB syntax evaluated at index t."""
    expr = expr.strip()
    call = first_function_call(expr)
    if call is not None:
        start, end, name, inner = call
        args = split_top_level_commas(inner)
        if name == "LOG":
            repl = f"log({convert_expr_matlab(args[0], varnames, shift, prefix)})"
        elif name == "EXP":
            repl = f"exp({convert_expr_matlab(args[0], varnames, shift, prefix)})"
        elif name in {"TSLAG", "TSLEAD"}:
            n = int(args[1]) if len(args) > 1 and args[1] else 1
            shift2 = shift + (n if name == "TSLEAD" else -n)
            repl = f"({convert_expr_matlab(args[0], varnames, shift2, prefix)})"
        elif name == "TSDELTA":
            n = int(args[1]) if len(args) > 1 and args[1] else 1
            repl = f"(({convert_expr_matlab(args[0], varnames, shift, prefix)}) - ({convert_expr_matlab(args[0], varnames, shift - n, prefix)}))"
        elif name == "TSDELTALOG":
            n = int(args[1]) if len(args) > 1 and args[1] else 1
            repl = f"(log({convert_expr_matlab(args[0], varnames, shift, prefix)}) - log({convert_expr_matlab(args[0], varnames, shift - n, prefix)}))"
        elif name in {"MOVAVG", "MOVSUM"}:
            length = int(float(args[1]))
            terms = [f"({convert_expr_matlab(args[0], varnames, shift - i, prefix)})" for i in range(length)]
            repl = "(" + " + ".join(terms) + ")"
            if name == "MOVAVG":
                repl = f"({repl}/{length})"
        else:
            raise ValueError(name)
        return convert_expr_matlab(expr[:start] + repl + expr[end:], varnames, shift=shift, prefix=prefix)

    # Plain expression: replace variable names with D.name(t+shift).
    token_re = re.compile(r"(?<!\.)\b([A-Za-z_]\w*)\b")

    def repl_tok(m: re.Match) -> str:
        tok = m.group(1)
        low = tok.lower()
        if low in varnames:
            if shift == 0:
                return f"{prefix}.{low}(t)"
            sign = "+" if shift > 0 else ""
            return f"{prefix}.{low}(t{sign}{shift})"
        if low == "log":
            return "log"
        if low == "exp":
            return "exp"
        if low == "max":
            return "max"
        if low == "min":
            return "min"
        return tok

    expr = re.sub(r"\bLOG\b", "log", expr, flags=re.I)
    expr = re.sub(r"\bEXP\b", "exp", expr, flags=re.I)
    return token_re.sub(repl_tok, expr)


def parse_model(text: str, name: str) -> ParsedModel:
    lines = text.splitlines()
    equations: List[EqBlock] = []
    descriptions: Dict[str, str] = {}
    endo: List[str] = []
    raw_exog_lines: List[str] = []
    in_exog = False
    i = 0
    pending_desc: List[str] = []

    while i < len(lines):
        line = lines[i].rstrip()
        stripped = line.strip()
        if stripped.upper().startswith("$ EXOGENOUS SECTION"):
            in_exog = True
            i += 1
            continue
        if in_exog:
            if stripped == "END":
                break
            if stripped.startswith("$") and " - " in stripped:
                raw_exog_lines.append(stripped[1:].strip())
            i += 1
            continue
        if not stripped:
            i += 1
            continue
        if stripped.startswith("$"):
            body = stripped[1:].strip()
            if "ENDOGENOUS SECTION" in body.upper():
                pending_desc = []
            elif body and not set(body) <= {"-"}:
                pending_desc.append(body)
            i += 1
            continue
        if stripped.upper().startswith("IDENTITY>"):
            var = stripped.split(">", 1)[1].strip().lower()
            if var not in endo:
                endo.append(var)
                descriptions[var] = " ".join(pending_desc).strip()
            desc = pending_desc
            pending_desc = []
            cond = None
            i += 1
            if i < len(lines) and lines[i].strip().upper().startswith("IF>"):
                cond = lines[i].strip().split(">", 1)[1].strip()
                i += 1
            if i >= len(lines) or not lines[i].strip().upper().startswith("EQ>"):
                raise ValueError(f"Expected EQ> after IDENTITY> {var} near line {i}")
            first = lines[i].strip().split(">", 1)[1]
            eq_lines = [first]
            i += 1
            while i < len(lines):
                nxt = lines[i].strip()
                if nxt.upper().startswith("IDENTITY>") or nxt.upper().startswith("$ EXOGENOUS SECTION") or nxt == "END":
                    break
                if nxt.startswith("$"):
                    # End of block comments before next identity; do not include them.
                    # Keep them for next equation description.
                    body = nxt[1:].strip()
                    if body and not set(body) <= {"-"}:
                        pending_desc.append(body)
                    i += 1
                    # Usually comments only appear between blocks, so stop if next is IDENTITY.
                    continue
                if nxt:
                    eq_lines.append(nxt)
                i += 1
            raw_eq = " ".join(eq_lines).strip()
            if "=" not in raw_eq:
                raise ValueError(f"Equation has no = for {var}: {raw_eq[:100]}")
            lhs, rhs = raw_eq.split("=", 1)
            equations.append(EqBlock(var, cond, lhs.strip(), rhs.strip(), raw_eq, desc))
            continue
        i += 1

    exo: List[str] = []
    for ln in raw_exog_lines:
        var = ln.split(" - ", 1)[0].strip().lower()
        if var and var != "name" and var not in exo:
            exo.append(var)

    return ParsedModel(name=name, text=text, equations=equations, endo=endo, exo=exo, descriptions=descriptions, raw_exog_lines=raw_exog_lines)


def build_equation_map(model: ParsedModel) -> Dict[str, EqBlock | List[EqBlock]]:
    d: Dict[str, List[EqBlock]] = {}
    for b in model.equations:
        d.setdefault(b.name, []).append(b)
    return {k: v[0] if len(v) == 1 else v for k, v in d.items()}


def collapsed_conditional_dynare(var: str, varnames: set[str]) -> Tuple[str, str] | None:
    # Return lhs, rhs in Dynare syntax, without add-factor.
    if var == "dmptmax":
        return "dmptmax", "max(dmptlur, dmptpi)"
    if var == "dmptr":
        return "dmptr", "max(dmptmax, dmptr(-1))"
    if var == "qynidn":
        return "log(qynidn)", "(-0.9155533588082586) + (0.3548225925232601)*d79a + log(max(ynicpn - tcin, 0.01))"
    if var == "rccd":
        return "rccd", "max(100*jrcd + rcar - zpi5, 0.01)"
    if var == "rcch":
        return "rcch", "max(100*jrh + (1-trfpm/100)*(rme + 100*trspp) - zpi10, 0.1)"
    if var == "rff":
        return "rff", "(1-dmptrsh)*max(rffrule, rffmin) + dmptrsh*max(dmptr(-1)*rffrule + (1-dmptr(-1))*rffmin, rffmin)"
    if var == "ynicpn":
        return "ynicpn", "max(ynin - yniln - ynirn + uynicpnr*xgdpn, tcin + 0.01*xgdpn)"
    return None


def collapsed_conditional_matlab(var: str, varnames: set[str]) -> Tuple[str, str] | None:
    # Return lhs, rhs in MATLAB scalar syntax at index t, without add-factor.
    D = lambda x, sh=0: f"D.{x}(t{('+'+str(sh)) if sh>0 else (str(sh) if sh<0 else '')})"
    if var == "dmptmax":
        return D("dmptmax"), f"max({D('dmptlur')}, {D('dmptpi')})"
    if var == "dmptr":
        return D("dmptr"), f"max({D('dmptmax')}, {D('dmptr',-1)})"
    if var == "qynidn":
        return f"log({D('qynidn')})", f"(-0.9155533588082586) + (0.3548225925232601)*{D('d79a')} + log(max({D('ynicpn')} - {D('tcin')}, 0.01))"
    if var == "rccd":
        return D("rccd"), f"max(100*{D('jrcd')} + {D('rcar')} - {D('zpi5')}, 0.01)"
    if var == "rcch":
        return D("rcch"), f"max(100*{D('jrh')} + (1-{D('trfpm')}/100)*({D('rme')} + 100*{D('trspp')}) - {D('zpi10')}, 0.1)"
    if var == "rff":
        return D("rff"), f"(1-{D('dmptrsh')})*max({D('rffrule')}, {D('rffmin')}) + {D('dmptrsh')}*max({D('dmptr',-1)}*{D('rffrule')} + (1-{D('dmptr',-1)})*{D('rffmin')}, {D('rffmin')})"
    if var == "ynicpn":
        return D("ynicpn"), f"max({D('ynin')} - {D('yniln')} - {D('ynirn')} + {D('uynicpnr')}*{D('xgdpn')}, {D('tcin')} + 0.01*{D('xgdpn')})"
    return None


def wrap_line(s: str, indent: str = "    ", width: int = 100) -> str:
    s = s.strip()
    if len(s) <= width:
        return indent + s
    parts = re.split(r"( \+ | - )", s)
    lines: List[str] = []
    cur = indent
    for part in parts:
        if len(cur) + len(part) > width and cur.strip():
            lines.append(cur.rstrip())
            cur = indent + "    " + part.lstrip()
        else:
            cur += part
    if cur.strip():
        lines.append(cur.rstrip())
    return "\n".join(lines)


def write_inc(model: ParsedModel, filename: str) -> None:
    varnames = set(model.endo) | set(model.exo) | {f"a_{v}" for v in model.endo}
    eqmap = build_equation_map(model)
    out: List[str] = []
    out.append("// Auto-generated FRB/US equations translated from BIMETS MDL.")
    out.append("// Every equation includes a tracking residual/add-factor a_<endogenous variable>.")
    out.append("// Conditional BIMETS IF> blocks are collapsed to max() formulas where possible.")
    out.append("")
    for var in model.endo:
        desc = model.descriptions.get(var, "")
        if desc:
            out.append(f"// {var}: {desc[:220]}")
        item = eqmap[var]
        collapsed = collapsed_conditional_dynare(var, varnames)
        if collapsed is not None:
            lhs, rhs = collapsed
        else:
            assert isinstance(item, EqBlock)
            lhs = convert_expr_dynare(item.lhs, varnames)
            rhs = convert_expr_dynare(item.rhs, varnames)
        line = f"{lhs} = {rhs} + a_{var};"
        out.append(wrap_line(line))
        out.append("")
    (INC_DIR / filename).write_text("\n".join(out), encoding="utf-8")


def chunked(names: Sequence[str], n: int = 8) -> Iterable[Sequence[str]]:
    for i in range(0, len(names), n):
        yield names[i:i+n]


def write_mod(model: ParsedModel, version: str) -> None:
    add = [f"a_{v}" for v in model.endo]
    out: List[str] = []
    out.append(f"// Dynare translation of the FRB/US {version} model from BIMETS MDL.")
    out.append("// Generated by scripts/parse_frbus_mdl.py.")
    out.append("// This file defines the model only. Scenario scripts populate oo_.endo_simul and oo_.exo_simul.")
    out.append("")
    out.append("var")
    for ch in chunked(model.endo, 8):
        out.append("    " + " ".join(ch))
    out.append(";")
    out.append("")
    out.append("varexo")
    for ch in chunked(model.exo + add, 6):
        out.append("    " + " ".join(ch))
    out.append(";")
    out.append("")
    out.append("model(no_static);")
    out.append(f"@#include \"includes/frbus_equations_{version}.inc\"")
    out.append("end;")
    out.append("")
    out.append("// No steady-state block is supplied. Use the MATLAB scripts in ../matlab")
    out.append("// to load a baseline path, compute tracking residuals, and call the perfect-foresight solver.")
    (DYN_DIR / f"frbus_{version}.mod").write_text("\n".join(out), encoding="utf-8")


def max_lag_lead_in_text(text: str) -> Tuple[int, int]:
    max_lag = 0
    max_lead = 0
    for m in re.finditer(r"TSLAG\s*\(([^()]|\([^()]*\))*\)", text, flags=re.I):
        args = split_top_level_commas(m.group(0)[m.group(0).find("(")+1:-1])
        n = int(args[1]) if len(args) > 1 and args[1].strip() else 1
        max_lag = max(max_lag, n)
    for m in re.finditer(r"TSLEAD\s*\(([^()]|\([^()]*\))*\)", text, flags=re.I):
        args = split_top_level_commas(m.group(0)[m.group(0).find("(")+1:-1])
        n = int(args[1]) if len(args) > 1 and args[1].strip() else 1
        max_lead = max(max_lead, n)
    return max_lag, max_lead


def write_names_m(model: ParsedModel, version: str) -> None:
    out = []
    out.append(f"function n = frbus_names_{version}()")
    out.append(f"%FRBUS_NAMES_{version.upper()} Variable metadata for the FRB/US Dynare translation.")
    out.append("n.endo = {")
    for v in model.endo:
        out.append(f"    '{v}'")
    out.append("};")
    out.append("n.exo = {")
    for v in model.exo:
        out.append(f"    '{v}'")
    out.append("};")
    out.append("n.add = {")
    for v in model.endo:
        out.append(f"    'a_{v}'")
    out.append("};")
    out.append("n.all_exo = [n.exo; n.add];")
    out.append("end")
    (MATLAB_DIR / f"frbus_names_{version}.m").write_text("\n".join(out), encoding="utf-8")


def write_addfactor_m(model: ParsedModel, version: str) -> None:
    varnames = set(model.endo) | set(model.exo)
    eqmap = build_equation_map(model)
    out = []
    out.append(f"function addf = frbus_compute_addfactors_{version}(D, idx_range)")
    out.append(f"%FRBUS_COMPUTE_ADDFACTORS_{version.upper()} Compute baseline tracking residuals from data.")
    out.append("% D is a struct with one numeric column vector per FRB/US variable, all lower-case names.")
    out.append("% idx_range is a vector of integer row indices at which residuals are computed.")
    out.append("% The formula is addf.var(t) = lhs_data(t) - rhs_data(t), matching equations lhs = rhs + a_var.")
    out.append("if nargin < 2 || isempty(idx_range)")
    out.append("    nobs = numel(D.xgdp);")
    out.append("    idx_range = 5:nobs;")
    out.append("end")
    out.append("nobs = numel(D.xgdp);")
    for v in model.endo:
        out.append(f"addf.{v} = nan(nobs, 1);")
    out.append("")
    out.append("for t = idx_range(:)'")
    for var in model.endo:
        collapsed = collapsed_conditional_matlab(var, varnames)
        if collapsed is not None:
            lhs, rhs = collapsed
        else:
            item = eqmap[var]
            assert isinstance(item, EqBlock)
            lhs = convert_expr_matlab(item.lhs, varnames)
            rhs = convert_expr_matlab(item.rhs, varnames)
        out.append(f"    addf.{var}(t) = ({lhs}) - ({rhs});")
    out.append("end")
    out.append("end")
    (MATLAB_DIR / f"frbus_compute_addfactors_{version}.m").write_text("\n".join(out), encoding="utf-8")


def write_metadata(models: Dict[str, ParsedModel]) -> None:
    meta = {}
    for version, model in models.items():
        eqmap = build_equation_map(model)
        repeated = {k: len(v) for k, v in ((k, val if isinstance(val, list) else [val]) for k, val in eqmap.items()) if len(v) > 1}
        lead_vars = []
        for b in model.equations:
            if "TSLEAD" in b.raw_eq.upper():
                if b.name not in lead_vars:
                    lead_vars.append(b.name)
        # used variables rough scan
        tokens = set(re.findall(r"\b[a-zA-Z_]\w*\b", model.text.lower()))
        used_exo = sorted(set(model.exo) & tokens)
        max_lag, max_lead = max_lag_lead_in_text(model.text)
        meta[version] = {
            "n_identity_blocks": len(model.equations),
            "n_unique_endogenous": len(model.endo),
            "n_exogenous_listed": len(model.exo),
            "n_used_exogenous_rough": len(used_exo),
            "repeated_conditional_variables": repeated,
            "lead_equation_variables": lead_vars,
            "max_lag_detected": max_lag,
            "max_lead_detected": max_lead,
            "endogenous": model.endo,
            "exogenous": model.exo,
        }
    (DOC_DIR / "generated_metadata.json").write_text(json.dumps(meta, indent=2), encoding="utf-8")


def write_report(models: Dict[str, ParsedModel]) -> None:
    meta = json.loads((DOC_DIR / "generated_metadata.json").read_text(encoding="utf-8"))
    out = []
    out.append("# Generated conversion report")
    out.append("")
    out.append("This report is generated by `scripts/parse_frbus_mdl.py` and records the translated model inventory.")
    out.append("")
    for version, m in meta.items():
        out.append(f"## {version}")
        out.append("")
        out.append(f"- Identity blocks in source MDL: {m['n_identity_blocks']}")
        out.append(f"- Unique endogenous variables: {m['n_unique_endogenous']}")
        out.append(f"- Exogenous variables listed: {m['n_exogenous_listed']}")
        out.append(f"- Maximum lag detected: {m['max_lag_detected']}")
        out.append(f"- Maximum lead detected: {m['max_lead_detected']}")
        out.append(f"- Repeated conditional variables: {', '.join(m['repeated_conditional_variables'].keys())}")
        if m["lead_equation_variables"]:
            out.append(f"- Lead equations: {', '.join(m['lead_equation_variables'])}")
        out.append("")
    (DOC_DIR / "generated_conversion_report.md").write_text("\n".join(out), encoding="utf-8")


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("--all", action="store_true", help="Generate all translated model files")
    args = ap.parse_args()
    if not args.all:
        ap.print_help()
        return

    INC_DIR.mkdir(parents=True, exist_ok=True)
    MATLAB_DIR.mkdir(parents=True, exist_ok=True)
    DOC_DIR.mkdir(parents=True, exist_ok=True)

    b_text = read_model_text(SRC_DIR / "frbus_model.txt")
    m_text = read_model_text(SRC_DIR / "frbus_model_mce.txt")
    models = {
        "backward": parse_model(b_text, "backward"),
        "mce": parse_model(m_text, "mce"),
    }
    for version, model in models.items():
        write_inc(model, f"frbus_equations_{version}.inc")
        write_mod(model, version)
        write_names_m(model, version)
        write_addfactor_m(model, version)
    write_metadata(models)
    write_report(models)
    print("Generated Dynare/MATLAB model files.")
    for version, model in models.items():
        print(f"  {version}: {len(model.endo)} endogenous, {len(model.exo)} listed exogenous, {len(model.equations)} MDL blocks")

if __name__ == "__main__":
    main()
