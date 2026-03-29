#!/usr/bin/python3

import argparse
import itertools
import os
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path

GREEN = "\033[32m"
RED = "\033[31m"
YELLOW = "\033[33m"
CYAN = "\033[36m"
BOLD = "\033[1m"
RESET = "\033[0m"

SA_FLAGS = ["--sa-scan", "--sa-parse", "--sa-ast", "--sa-tac"]
SHOW_FLAGS = ["--show-tokens", "--show-ast", "--show-tac", "--show-rtl"]

SHOW_SUBSETS = [
    list(combo)
    for r in range(1, len(SHOW_FLAGS) + 1)
    for combo in itertools.combinations(SHOW_FLAGS, r)
]

ALL_COMBOS = [(sa, shows) for sa in SA_FLAGS for shows in SHOW_SUBSETS]

SHOW_EXT = {
    "--show-tokens": ".toks",
    "--show-ast": ".ast",
    "--show-tac": ".tac",
    "--show-rtl": ".rtl",
}

def run_combo(binary: Path, sa: str, shows: list[str], testcase: Path, workdir: Path) -> tuple[int, dict[str, Path], Path]:
    
    tc_copy = workdir / testcase.name
    shutil.copy2(testcase, tc_copy)

    stderr_log = workdir / "stderr.log"
    stderr_log.touch()

    cmd = [str(binary), sa] + shows + [str(tc_copy)]

    try:
        with open(stderr_log, "w") as ferr:
            result = subprocess.run(cmd, stderr=ferr, stdout=subprocess.DEVNULL)
        rc = result.returncode
    except PermissionError:
        stderr_log.write_text(f"ERROR: could not execute binary: {binary}\n")
        rc -= 1
    except FileNotFoundError:
        stderr_log.write_text(f"ERROR: binary not found: {binary}\n")
        rc -= 1

    logs: dict[str, Path] = {}
    for show_flag in shows:
        ext = SHOW_EXT[show_flag]
        path = tc_copy.with_suffix(ext)
        logs[show_flag] = path

    return rc, logs, stderr_log


def diff_logs(path_a: Path, path_b: Path) -> bool:
    r = subprocess.run(
        ["diff", "-Bw", str(path_a), str(path_b)],
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
    )
    return r.returncode == 0


def progress_bar(done: int, total: int, width: int = 40) -> str:
    frac = done / total if total else 1
    filled = int(frac * width)
    bar = ">" * filled + " " * (width - filled)
    pct = frac * 100
    return f"[{bar}] {done}/{total} ({pct:.1f}%)"


def sanitise(name: str) -> str:
    return re.sub(r"[^A-Za-z0-9_\-]", "_", name)


class Failure:
    def __init__(self, reason: str, detail: str):
        self.reason = reason
        self.detail = detail


def test_one(impl: Path, ref: Path, testcase: Path, sa: str, shows: list[str]) -> "Failure | None":
    combo_tag = sa + " " + " ".join(shows)

    with tempfile.TemporaryDirectory(prefix="sclp_impl_") as td_impl, tempfile.TemporaryDirectory("sclp_ref_") as td_ref:
        rc_impl, logs_impl, err_impl = run_combo(impl, sa, shows, testcase, Path(td_impl))
        rc_ref, logs_ref, err_ref = run_combo(ref, sa, shows, testcase, Path(td_ref))

        err_impl_text = (
            err_impl.read_text(errors="replace").strip() if err_impl.exists() else ""
        )
        err_ref_text = (
            err_ref.read_text(errors="replace").strip() if err_ref.exists() else ""
        )
    
        if (rc_impl == 0) ^ (rc_ref == 0):
            return Failure(
                "exit_code_mismatch", 
                (
                    f"Command: {combo_tag}\n"
                    f"impl rc: {rc_impl}\n"
                    f"ref rc:  {rc_ref}\n"
                ),
            )

        if rc_impl != 0:
            # err_text = err_impl.read_text(errors="replace").strip()
            # if not err_text:
            #     return Failure(
            #         "empty_stderr_on_error",
            #         (
            #             f"Command: {combo_tag}\n"
            #             f"Both returned rc != 0 but impl stderr is empty.\n"
            #         ),
            #     )
            
            return None
    
        diff_issues: list[str] = []
        for show_flag in shows:
            ext = SHOW_EXT[show_flag]
            p_impl = logs_impl[show_flag]
            p_ref = logs_ref[show_flag]

            if not p_impl.exists() and not p_ref.exists():
                continue
            if not p_impl.exists():
                diff_issues.append(f"  {show_flag} ({ext}): impl produced no file, ref did")
                continue
            if not p_ref.exists():
                diff_issues.append(f"  {show_flag} ({ext}): ref produced no file, impl did")
                continue
            
            if not diff_logs(p_impl, p_ref):
                diff_out = subprocess.run(
                    ["diff", "-Bw", "--color=never", str(p_impl), str(p_ref)],
                    capture_output=True,
                    text=True
                ).stdout
                diff_issues.append(
                    f"  {show_flag} ({ext}): outputs differ\n" + "\n".join("    " + l for l in diff_out.splitlines())
                )

        if diff_issues:
            return Failure(
                "log_mismatch",
                f"Command : {combo_tag}\n" + "\n".join(diff_issues) + "\n",
            )
        
        return None

def main() -> None:
    parser = argparse.ArgumentParser(description="sclp regression tester")
    parser.add_argument("testcase_dir", type=Path)
    parser.add_argument("--impl", type=Path, default=Path("src/sclp"))
    parser.add_argument("--ref", type=Path, default=Path("reference-implementations/A4-sclp"))
    parser.add_argument("--failed", type=Path, default=Path("failed-logs"))
    args = parser.parse_args()

    for p, label in [(args.impl, "impl"), (args.ref, "ref"), (args.testcase_dir, "testcase_dir")]:
        if not p.exists():
            sys.exit(f"{RED}ERROR:{RESET} {label} path not found: {p}")
        
    testcases = sorted(args.testcase_dir.glob("*.c"))
    if not testcases:
        sys.exit(f"{YELLOW}No *.c files foudn in {args.testcase_dir}{RESET}")
    
    if args.failed.exists():
        shutil.rmtree(args.failed)
    args.failed.mkdir(parents=True)

    total_combos = len(testcases) * len(ALL_COMBOS)
    done_combos = 0
    passed_combos = 0

    failures: dict[str, list[tuple[str, str, Failure]]] = {}

    print(f"\n{BOLD}sclp regression runner{RESET}")
    print(f"  impl : {args.impl}")
    print(f"  ref  : {args.ref}")
    print(f"  tests: {len(testcases)} *.c files x {len(ALL_COMBOS)} combos = {total_combos} runs")

    for tc in testcases:
        tc_failures: list[tuple[str, str, Failure]] = []

        for sa, shows in ALL_COMBOS:
            shows_str = " ".join(shows)
            failure = test_one(args.impl, args.ref, tc, sa, shows)

            done_combos += 1
            if failure is None:
                passed_combos += 1
            else:
                tc_failures.append((sa, shows_str, failure))

            bar = progress_bar(done_combos, total_combos)
            status_char = (
                f"{GREEN}G{RESET}" if failure is None else f"{RED}R{RESET}"
            )
            print(f"\r  {bar}  {status_char}  {tc.name:<30}", end="", flush=True)
        
        if tc_failures:
            failures[tc.stem] = tc_failures
    
    print()

    for tc_stem, tc_failures in failures.items():
        tc_path = args.testcase_dir / f"{tc_stem}.c"
        subdir = args.failed / sanitise(tc_stem)
        subdir.mkdir(parents=True, exist_ok=True)

        shutil.copy2(tc_path, subdir / tc_path.name)

        log_lines: list[str] = [
            f"Testcase : {tc_path}\n",
            f"Failures : {len(tc_failures)} / {len(ALL_COMBOS)} combos\n",
            "=" * 72 + "\n",
        ]
        for sa, shows_str, f in tc_failures:
            log_lines += [
                f"\n[{f.reason}]\n",
                f"  sa    : {sa}\n",
                f"  shows : {shows_str}\n",
                f.detail,
                "-" * 72 + "\n",
            ]

        (subdir / "failure.log").write_text("".join(log_lines))

    failed_combos = total_combos - passed_combos
    failed_files = len(failures)

    print()
    print(f"{BOLD}{'-' * 55}{RESET}")
    print(
        f"  Combinations : "
        f"{GREEN}{passed_combos} passed{RESET} / "
        f"{RED}{failed_combos} failed{RESET} / "
        f"{total_combos} total"
    )
    print(
        f"  Testcase files: "
        f"{GREEN}{len(testcases) - failed_files} clean{RESET} / "
        f"{RED}{failed_files} with failures{RESET} / "
        f"{len(testcases)} total"
    )
    if failed_files:
        print(f"\n Failure reports -> {CYAN}{args.failed}/{RESET}")
    print(f"{BOLD}{'-' * 55}{RESET}\n")

    sys.exit(0 if failed_combos == 0 else 1)

if __name__ == "__main__":
    main()
