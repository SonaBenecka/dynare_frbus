#!/usr/bin/env python3
"""Lightweight parser/conversion tests for the FRB/US Dynare repository.

These tests do not require Dynare or the LONGBASE dataset. They verify that the
BIMETS MDL sources can be parsed and Dynare/MATLAB files generated.
Run from the bundle root:
    python3 -m unittest tests/test_generated_files.py
"""
from __future__ import annotations

import json
import subprocess
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]

class GeneratedFilesTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        subprocess.run([sys.executable, "scripts/parse_frbus_mdl.py", "--all"], cwd=ROOT, check=True)
        cls.meta = json.loads((ROOT / "docs/generated_metadata.json").read_text())

    def test_counts(self):
        for version in ["backward", "mce"]:
            with self.subTest(version=version):
                self.assertEqual(self.meta[version]["n_unique_endogenous"], 284)
                self.assertEqual(self.meta[version]["n_identity_blocks"], 293)
                self.assertEqual(self.meta[version]["n_exogenous_listed"], 83)

    def test_conditionals_collapsed(self):
        expected = {"dmptmax", "dmptr", "qynidn", "rccd", "rcch", "rff", "ynicpn"}
        for version in ["backward", "mce"]:
            self.assertEqual(set(self.meta[version]["repeated_conditional_variables"]), expected)

    def test_mce_leads(self):
        expected = {"zdivgr", "zgap05", "zgap10", "zgap30", "zpi10", "zpi10f", "zpib5", "zpic30", "zpic58", "zpicxfe", "zpieci", "zrff10", "zrff30", "zrff5"}
        self.assertEqual(set(self.meta["mce"]["lead_equation_variables"]), expected)
        self.assertEqual(self.meta["backward"]["lead_equation_variables"], [])

    def test_no_raw_bimets_functions_in_dynare_includes(self):
        bad = ["TSLAG", "TSLEAD", "TSDELTA", "TSDELTALOG", "MOVAVG", "MOVSUM", "LOG(", "EXP("]
        for path in (ROOT / "dynare/includes").glob("*.inc"):
            txt = path.read_text()
            for b in bad:
                self.assertNotIn(b, txt, msg=f"{b} remains in {path}")

    def test_generated_mod_references_existing_includes(self):
        for version in ["backward", "mce"]:
            mod = ROOT / f"dynare/frbus_{version}.mod"
            inc = ROOT / f"dynare/includes/frbus_equations_{version}.inc"
            self.assertTrue(mod.exists())
            self.assertTrue(inc.exists())
            self.assertIn(f'@#include "includes/frbus_equations_{version}.inc"', mod.read_text())

    def test_exercise_scripts_present(self):
        expected = [
            "run_error_propagation_backward.m",
            "run_endogenous_targeting_backward.m",
            "run_stochastic_simulation_backward.m",
            "frbus_endogenous_targeting.m",
            "frbus_stochastic_vars_backward.m",
            "frbus_plot_stochsim_pyfrbus_style.m",
            "frbus_sanitize_dynare_model.m",
        ]
        for name in expected:
            with self.subTest(name=name):
                path = ROOT / "matlab" / name
                self.assertTrue(path.exists(), f"Missing {path}")

    def test_stochastic_variable_count(self):
        path = ROOT / "matlab/frbus_stochastic_vars_backward.m"
        txt = path.read_text()
        import re
        vars_ = re.findall(r"'([^']+)'", txt)
        self.assertEqual(len(vars_), 64)
        self.assertIn("eco", vars_)
        self.assertIn("ynirn", vars_)

if __name__ == "__main__":
    unittest.main()
