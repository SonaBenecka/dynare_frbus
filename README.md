# FRB/US in Dynare

This repository is a path-simulation replication of the Federal Reserve
FRB/US macroeconomic model translated from BIMETS MDL into Dynare. It contains
both the backward-looking model and the model-consistent-expectations (MCE)
variant.

The project is designed for deterministic perfect-foresight simulations with
data-derived tracking residuals (add-factors). It is not an estimation package.

## Requirements

- MATLAB with Dynare 7.1 or a compatible recent Dynare release.
- Python 3 for the parser and static tests. Python is not needed for MATLAB
  simulations after the generated files are present.
- A LaTeX installation is optional and is only needed to rebuild the report.

The runtime helper searches common Windows Dynare locations. If Dynare is in a
different location, set the MATLAB environment variable before running:

```matlab
setenv('FRBUS_DYNARE_PATH', 'C:\path\to\dynare\matlab')
```

## Quick start

1. Open MATLAB and change to the repository root:

```matlab
cd('C:\path\to\dynare_frbus_v4_github')
```

2. Run the complete workflow:

```matlab
main
```

`main.m` runs the baseline check, the backward-looking monetary-policy shock,
the MCE shock, error propagation, endogenous targeting, and stochastic
simulation. The stochastic step defaults to 1000 replications and can take a
substantial amount of time.

For a quick stochastic smoke test, run:

```matlab
setenv('FRBUS_NREPL', '10')
run_stochastic_simulation_backward
```

The scenario scripts can also be run individually from the repository root
after adding the MATLAB folder:

```matlab
addpath('matlab')
run_baseline_check_backward
run_mp_shock_backward
run_mp_shock_mce
run_error_propagation_backward
run_endogenous_targeting_backward
```

## Data

The included `data/frbus_data.csv` is a row-index-only LONGBASE export. Its
first row is interpreted as 1962Q1, matching the 848-quarter sample used by
the exercises. The scenario scripts prefer `data/LONGBASE.TXT` when that file
is present, and otherwise use the included CSV. A row-index-only replacement
can be loaded with a different start date by passing `[year quarter]` to
`frbus_load_longbase`.

## What the code does

The Python converter reads the two source MDL files in `source/` and writes
the Dynare model files and MATLAB add-factor functions. Each translated
equation has the form

```text
endogenous_variable = model_rhs + a_endogenous_variable;
```

For a baseline data path, the add-factor is computed as the observed left-hand
side minus the model right-hand side. The resulting perfect-foresight solve
reproduces the baseline path before a scenario shock is applied.

The conditional MDL blocks are retained as exact `max()` expressions. The
MCE model retains the forward-looking equations from the source and requires
terminal values; the included demonstration reports the early part of a short
simulation window.

## Exercises and outputs

The four primary exercises are documented in
`report/frbus_us_dynare_report.pdf` and use the charts in `docs/`:

1. A 100 bp `rffintay` monetary-policy shock in the backward-looking model.
2. The corresponding short MCE shock.
3. Propagation of historical model residuals with persistence.
4. Endogenous targeting of five trajectories with a Newton controller.

The stochastic bootstrap is included as an optional extension. Its chart and
bands are written to `docs/` when the script is run.

The v4 runtime validation found a backward baseline maximum absolute error of
approximately `9.50e-9` and a mean absolute error of `4.17e-11` over the
2040Q1-2045Q4 test window.

## Regeneration and tests

From the repository root, regenerate the translated files with:

```bash
python scripts/parse_frbus_mdl.py --all
```

Run the static tests with:

```bash
python -m unittest tests/test_generated_files.py
```

On Windows, use `py -3` instead of `python` if required. These tests verify
the model inventory, conversion rules, conditional blocks, MCE leads, and the
presence of the exercise helpers. They do not run MATLAB or Dynare.

## Report

The report source is `report/frbus_us_dynare_report.tex`. From the repository
root, compile it with:

```bash
pdflatex -interaction=nonstopmode -halt-on-error -output-directory report report/frbus_us_dynare_report.tex
pdflatex -interaction=nonstopmode -halt-on-error -output-directory report report/frbus_us_dynare_report.tex
```

The committed PDF is a draft technical description of the translation,
solution method, validation results, and four exercises.

## Repository layout

```text
data/       bundled LONGBASE-compatible data
docs/       exercise charts and CSV diagnostics
dynare/     source .mod files and translated equation includes
matlab/     add-factor, solver, plotting, and exercise scripts
report/     LaTeX source and compiled report
scripts/    MDL-to-Dynare converter
source/     original MDL text sources
tests/      parser and generated-file tests
```
