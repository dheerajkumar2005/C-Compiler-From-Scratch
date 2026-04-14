import os
from socket import timeout
import sys
from enum import Enum
from threading import TIMEOUT_MAX
import argparse
import tarfile
import subprocess
import glob
import re
import csv
import shutil
import mimetypes
import hashlib
from pathlib import Path

### LIST OF PHASES (TENTATIVE): ###
# TOK PASS      <--- A1
# TOK GEN       <--- A1
# PARSE PASS    <--- A1
# PARSE GEN     <--- A2
# TAC PASS      <--- A3
# TAC GEN       <--- A3
# RTL PASS      <--- A4
# RTL GEN       <--- A4
# ASM PASS      <--- A5?
# ASM GEN       <--- A5?

## DEFINITIONS ##

class PhaseName(Enum):
    TOK   = 'TOK'
    PARSE = 'PARSE'
    AST   = 'AST'
    TAC   = 'TAC'
    RTL   = 'RTL'
    ASM   = 'ASM'
    FULL  = 'FULL' # <- This tells the given function to not apply any --sa-XXXX
                   # Flag at all, i.e. not put any flag to stop after any phase.
                   # This is only compatible with PhaseType.PASS

class PhaseType(Enum):
    PASS         = 'PASS'          # If SCLP runs successfully, return PASS
    INVALID_FAIL = 'INVALID_FAIL'  # If SCLP fails on invalid input, return PASS
    GEN          = 'GEN'           # Checked generated IR of SCLP.

class CheckResult(Enum):
    PASS    = 0
    FAIL    = 1
    TIMEOUT = 2

## EDITABLE
## ADD PHASES OVER HERE ########################################################

eval_config_list = [
    (PhaseName.TOK, PhaseType.GEN),
    (PhaseName.PARSE, PhaseType.GEN),
    (PhaseName.TAC, PhaseType.GEN),
    (PhaseName.RTL, PhaseType.GEN),
    (PhaseName.ASM, PhaseType.GEN),
    (PhaseName.TOK, PhaseType.PASS),
    (PhaseName.PARSE, PhaseType.PASS),
    (PhaseName.AST, PhaseType.PASS),
    (PhaseName.RTL, PhaseType.PASS),
    # (PhaseName.ASM, PhaseType.PASS),
    (PhaseName.FULL, PhaseType.PASS)
]

invalid_eval_config_list = [
    (PhaseName.FULL, PhaseType.INVALID_FAIL)
]

## EDITABLE
## ADD SHA256SUM HASHES FOR SCLP OVER HERE #####################################
## Generate in terminal prompt using `sha256sum <path to file>`

suspicious_hashes = {
    "A1-sclp": "ee5c81cc11cc6e9d872fab4d089b98c148a722c4ef7123ebcf08b365766f3f1b",
    "A2-sclp": "2d23a2147b21bb5e7d9b8c4c05788ab1026604c671c5f0f452f5d31d68b37c31",
    "A3-sclp": "83501a3a7348f17500daa51cf2ef5c4c247d738f4e2a0542f39fdeeeef141a1a", 
    "A4-sclp": "2bc79fa659a3dc373cafc9a785ca0fb2b407832b296988111fc8e175fd1c5227",
    "A5-sclp": "3591b009c845db7a8091d23997f2687da0cb65f2832d82e46819d82933d3ff8c",
    "L1-sclp": "9d58d0e4d702842fefc6e2dfbf47c4cfd6e9c8aa7118c91b64da91ef63278a18",
    "L2-sclp": "b4ce6a10ff8584def4738047a24c573bb4c73b6730ac039e954ddb9f18f1ed36",
    "L3-sclp": "c05da3fb58f4a288c5b2e2df21b6978c11e1ddb0cd228e4e96fc4c2e0d8d0fd4",
    "L4-sclp": "2d6bd647bc6421343d3162a12ca9927d806ff974ac2a42e6ec1f60c4e4663346",
    "L5-sclp": "3591b009c845db7a8091d23997f2687da0cb65f2832d82e46819d82933d3ff8c"

}

## EDITABLE
## ADD any more whitelisted filenames and file extensions here #################

whitelisted_extensions = set({
    ".y",
    ".l",
    ".o"
})

whitelisted_filenames = set({
    "Makefile",
    "y.output"
})

## CONSTANTS ##

# Result csv file name
RESULT_CSV_FILENAME = "results.csv"

## EDITABLE
# This is the current ref impl. executable name.
CURRENT_REFIMPL = "A5-sclp"

# Testcase execution will time out in the following time.
TESTCASE_TIMEOUT_PARAM = 2

# Cosmetics
SEP_START ="========================================================================================="
SEP_END   ="\n\n=========================================================================================\n"
TEST_SEP  ="\n************************************************************\n"

## GLOBALS ##

# This path is used by default for finding an executable if a path has not been
# specified on the command line
default_path_executables = Path.home() / "reference_implementations"

# Default reference implementation executable
default_path_refimpl = default_path_executables / CURRENT_REFIMPL
current_path_refimpl = Path.cwd() / CURRENT_REFIMPL

# Testcase Directory
testcases_path = Path('./Testcases')
testcases_nonerror_path = testcases_path / "NonError"
testcases_error_path = testcases_path / "Error"

# Expected Output Directories
exp_out_path = Path('./Expected_Output')
tok_out_path = exp_out_path / "TOK"
ast_out_path = exp_out_path / "AST"
tac_out_path = exp_out_path / "TAC"
rtl_out_path = exp_out_path / "RTL"
asm_out_path = exp_out_path / "ASM"

# Submissions Directory
submissions_path = Path('./Submissions')

# Submission Logs Directory
exec_logs_path = Path('./Submission_Execution_Logs')

# Submission Outputs Directory
exec_out_path = Path('./Submission_Execution_Outputs')

## MUTABLE GLOBAL
# Output log file (Global Variable). currently empty. Is modified by the
# evaluator.
output_log_file = None

## MUTABLE GLOBAL
# Lists for testcases. Handled by the code.
nonerror_testcase_list = []
error_testcase_list = []

## MUTABLE GLOBAL
# sha256sum of the current sclp binary supplied. Used in the sanity check as
# well.
current_sclp_sha256sum = None

## MUTABLE GLOBAL
# Lists suspicious files for each group
suspicious_files = dict()

## FUNCTIONS ##

# Log message to file (general)
def log_gen(message):
    print(message, file=output_log_file, flush=True)

# Log message to file (pass)
def log_pass(message):
    print(f"[PASS]: {message}", file=output_log_file, flush=True)

# Log message to file (warn)
def log_warn(message):
    print(f"[WARN]: {message}")
    print(f"[WARN]: {message}", file=output_log_file, flush=True)

# Log message to file (fail)
def log_fail(message):
    print(f"[FAIL]: {message}", file=output_log_file, flush=True)

# Log message to file (error)
def log_err(message):
    print(f"[ERROR]: {message}")
    print(f"[ERROR]: {message}", file=output_log_file, flush=True)

# Log suspicions
def log_sus(message):
    print(f"\033[1;31m[SUSP]: @@@@ {message} @@@@\033[0m")
    print(f"[SUSP]: @@@@ {message} @@@@", file=output_log_file, flush=True)

# Log message to file (info)
def log_info(message):
    print(f"[INFO]: {message}", file=output_log_file, flush=True)

# General function to check for existence of a required path
def assert_path(path: Path):
    if not path.exists():
        print(f"Required path {path} does not exist.")
        sys.exit(1)

# Validate the naming scheme of a submission
def validate_submission_name(name: str):
    regex = r"JAN2026_group_[0-9]+.tar.gz"
    out = re.search(regex, tar)
    return out != None

# Extract tar file
def extract_tar_file(file: Path, dest: Path):
    tar = tarfile.open(file)
    tar.extractall(dest)
    tar.close()

# Check tar file for a given file name
def tar_file_has(file: Path, name: str):
    tar = tarfile.open(file)
    return name in tar.getnames() or f'./{name}' in tar.getnames()

def get_group_list():                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
    ret = []
    incorrect_dir_names = []
    for archive_path in submissions_path.glob("*.tar.gz"):
        # Check group name
        
        match = re.fullmatch(r"A5-JAN2026_group_[0-9]+-(JAN2026_group_[0-9]+)\.tar", str(archive_path.stem))
        # match = re.fullmatch(r"group([0-9]|[1-9][0-9]|100|101)\.tar", str(archive_path.stem))
        # pattern = re.compile(r'^(group\d+)\.tar$')
        # match = pattern.match(archive_path.stem)

        if match == None:
            incorrect_dir_names.append({ 'name': archive_path.stem , 'path': archive_path })
            continue
        
        # Retrieve group name
        group_name = match.group(1)    
        
        ret.append({ 'name': group_name , 'path': archive_path })
    return ret, incorrect_dir_names

# Attempt to decode a given file as unicode text. If it fails there is a
# good chance that the file is binary.
def check_unicode_decodability(file_path: Path):
    try:
        with open(file_path, 'r') as f:
            # Read a 
            f.read(4096)
        return True
    except UnicodeDecodeError:
        return False

# Generate an sha256sum of the given file.
def get_sha256sum(file_path: Path) -> str:
    with open(file_path, 'rb') as file:
        digest = hashlib.file_digest(file, 'sha256')
        return digest.hexdigest()

# Specifically check for sclp hashes at the given file path.
def check_sclp_hashes(file_path: Path) -> bool:
    is_sus: bool = False

    file_hash = get_sha256sum(file_path)
        
    for key in suspicious_hashes.keys():
        if file_hash == suspicious_hashes[key]:
            log_sus(f"File {file_path} matched the hash of '{key}'")
            is_sus = True
        
    if current_sclp_sha256sum:
        if file_hash == current_sclp_sha256sum:
            log_sus(f"File {file_path} matched the hash of the reference implementation supplied to this script.")
            is_sus = True
    
    return is_sus

# Check the submission for any funny business.
def sanity_check_submission(path: Path) -> set:
    ret = set()

    sclp_expected_path = (path / "sclp")
    if sclp_expected_path.exists():
        log_sus(f"File called {sclp_expected_path} already exists in submission folder.")
        ret.add(sclp_expected_path)

    for file in path.rglob("*"):

        if file.is_dir():
            if file.name == path.name:
                log_warn(\
                    "Incorrect directory structure detected. Please tell student "\
                    "to fix the structure and/or temporarily adjust the structure yourself.")
            else:
                continue

        is_sus: bool = False
        
        file_type, encoding = mimetypes.guess_type(file)
        
        if not file_type:
            if (file.suffix in whitelisted_extensions) or \
               (file.name in whitelisted_filenames):
               log_info(f"Could not detect mime type for {file}. But either file name or suffix is whitelisted.")
            else:
                log_sus(f"Could not detect mime type for {file}. File name or extension not whitelisted.")
                is_sus = True
        
        if os.access(file, os.X_OK):
            log_sus(f"File {file} is marked executable")
            is_sus = True
        
        if not check_unicode_decodability(file):
            log_sus(f"File {file} is likely binary")
            is_sus = True
        
        if check_sclp_hashes(file):
            is_sus = True

        if is_sus:
            ret.add(file)

    if sclp_expected_path.exists():
        log_sus(f"Removing {sclp_expected_path} before build")
        file.unlink()

    return ret

def clean_submission(path: Path):
    result = subprocess.run(
        ["make", "clean", "-C", f"{path}"],
        stdout = output_log_file,
        stderr = output_log_file)
    if result.returncode != 0:
        log_err(f"Couldn't find rule for clean {path}")

def build_submission(path: Path) -> bool:
    print(path)
    result = subprocess.run(
        ["make", "-C", f"{path}"], 
        stdout = output_log_file,
        stderr = output_log_file)
    
    if result.returncode != 0:
        log_err(f"Couldn't build the project in {path}")
        return False
    
    exec_path: Path = path / "sclp"
    if not exec_path.is_file():
        log_err(f"Couldn't find sclp executable at {exec_path}")
        return False
    
    return True

def check_shift_reduce_conflicts(path: Path) -> int:
    matches = list(path.rglob("*.y"))
    
    if len(matches) > 1:
        log_err(f"Multiple yacc files found in {path}: {matches}")
        return -1
    
    if len(matches) < 1:
        log_err(f"No yacc file found in {path}")
        return -1

    yacc_file = matches[0]
    yacc_out_file = Path(yacc_file.parent / "y.output")

    result = subprocess.run(
        ["yacc", "-dv", f"{yacc_file}", "-o", f"{yacc_out_file}"], 
        stdout = output_log_file,
        stderr = output_log_file)

    if result.returncode != 0:
        log_err(f"[ERROR]: Couldn't execute yacc -dv in {path}")
        return -1
        
    if not yacc_out_file.is_file():
        log_err(f"[ERROR]: Couldn't find yacc output ({yacc_out_file})")
        return -1
    
    count = 0
    
    with open(f"{yacc_out_file}", 'r') as file:
        for line in file: 
            words = line.split() 
            for i in range(len(words) - 2): 
                if (words[i] == 'conflicts:'): 
                    count = count + int(words[i + 1])   
    
    log_info(f"Shift-reduce conflicts in {yacc_file}: {count}")
    return count
    
# Compares two files. Prints out the diff if there are differences.
def run_diff(file: Path, expected: Path, rtl_gen: bool, asm_gen: bool) -> bool:
    if rtl_gen:
        temp1 = Path("./expected_rtl_output")
        temp1.touch(exist_ok=True)
        temp2 = Path("./rtl_output")
        temp2.touch(exist_ok=True)
        with open(file,"r") as fin, open(temp1,"w") as fout:
            for l in fin:
                clean = l.split(";;",1)[0].rstrip()
                if clean:
                    fout.write(clean+'\n')
        with open(expected,"r") as fin, open(temp2,"w") as fout:
            for l in fin:
                clean = l.split(";;",1)[0].rstrip()
                if clean:
                    fout.write(clean+'\n')
        file = temp1
        expected = temp2
    if asm_gen:
        temp1 = Path("./expected_asm_output")
        temp1.touch(exist_ok=True)
        temp2 = Path("./asm_output")
        temp2.touch(exist_ok=True)
        with open(file,"r") as fin, open(temp1,"w") as fout:
            for l in fin:
                clean = l.split("#",1)[0].rstrip()
                if clean:
                    fout.write(clean+'\n')
        with open(expected,"r") as fin, open(temp2,"w") as fout:
            for l in fin:
                clean = l.split("#",1)[0].rstrip()
                if clean:
                    fout.write(clean+'\n')
        file = temp1
        expected = temp2
    result = subprocess.run(
        ["diff", "-Bwu", f"{file}", f"{expected}"], 
        stdout = subprocess.PIPE,
        stderr = subprocess.STDOUT)
    
    if result.returncode != 0:
        log_fail("[FAIL] Output does not match expected.")
        log_info("DIFF OUTPUT:")
        log_info("============")
        log_gen(result.stdout.decode())
        log_info("============")
        return False
    else:
        return True    

def cleanup_testcase_dir():
    for file in testcases_nonerror_path.glob("*.c"):
        if Path(f"{file}.toks").is_file(): Path(f"{file}.toks").unlink()
        if Path(f"{file}.ast").is_file():  Path(f"{file}.ast").unlink()
        if Path(f"{file}.tac").is_file():  Path(f"{file}.tac").unlink()
        if Path(f"{file}.rtl").is_file():  Path(f"{file}.rtl").unlink()
        if Path(f"{file}.spim").is_file(): Path(f"{file}.spim").unlink()

# Please make note of the enums above when using this function.
def check_testcase(
    exec_path: Path,
    exec_out_path: Path,
    testcase_path: Path,
    expected_output_path: Path,
    phase_name: PhaseName,
    phase_type: PhaseType) -> CheckResult:
    cleanup_testcase_dir()

    flag = "INVALID"
    output_file_path = "INVALID"
    final_flags = []

    no_flags = False

    if phase_type == PhaseType.PASS or phase_type == PhaseType.INVALID_FAIL:
        if phase_name == PhaseName.TOK:
            flag = "--sa-scan"
        elif phase_name == PhaseName.PARSE:
            flag = "--sa-parse"
        elif phase_name == PhaseName.TAC:
            flag = "--sa-tac"
        elif phase_name == PhaseName.AST:
            flag = "--sa-ast"
        elif phase_name == PhaseName.RTL:
            flag = "--sa-rtl"
        elif phase_name == PhaseName.ASM:
            flag = "--sa-asm"
        elif phase_name == PhaseName.FULL:
            no_flags = True
        else:
            raise ValueError(f"Invalid phase_name supplied: {phase_name}")

    elif phase_type == PhaseType.GEN:
        if phase_name == PhaseName.TOK:
            flag = "--show-tokens"
            output_file_path = Path(f"{testcase_path}.toks")
        elif phase_name == PhaseName.PARSE:
            flag = "--show-ast"
            output_file_path = Path(f"{testcase_path}.ast")
        elif phase_name == PhaseName.TAC:
            flag = "--show-tac"
            output_file_path = Path(f"{testcase_path}.tac")
        elif phase_name == PhaseName.RTL:
            flag = "--show-rtl"
            output_file_path = Path(f"{testcase_path}.rtl") 
        elif phase_name == PhaseName.ASM:
            flag = "--show-asm"
            output_file_path = Path(f"{testcase_path}.spim")
        elif phase_name == PhaseName.FULL:
            raise ValueError(f"For {PhaseName.FULL}, the only valid phase type is {PhaseType.PASS}")
        else:
            raise ValueError(f"Invalid phase_name supplied: {phase_name}")

    else:
        raise ValueError(f"Invalid phase_type supplied: {phase_type}")
        
    if no_flags:
        final_flags = [f"{exec_path}", f"{testcase_path}"]
    else:
        final_flags = [f"{exec_path}", flag, f"{testcase_path}"]
    
    phase_info_string = f"{phase_name.value}_{phase_type.value}"

    try:
        tc_res = subprocess.run(
            final_flags,
            timeout=TESTCASE_TIMEOUT_PARAM,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT)
    except subprocess.TimeoutExpired as e:
        log_err("Sclp submission timed out.")
        log_info("Exception contents:")
        log_gen(e)
        log_fail(f"Failed {phase_info_string} {testcase_path}")
        return CheckResult.TIMEOUT
        
    if phase_type == PhaseType.PASS:
        if tc_res.returncode == 0:
            log_pass(f"Passed {phase_info_string} {testcase_path}")
            return CheckResult.PASS
        else:
            log_fail(f"Failed {phase_info_string} {testcase_path}")
            return CheckResult.FAIL
    elif phase_type == PhaseType.INVALID_FAIL:
        if tc_res.returncode == 0:
            log_fail(f"Failed {phase_info_string} {testcase_path}")
            return CheckResult.FAIL 
        else:
            log_pass(f"Passed {phase_info_string} {testcase_path}")
            return CheckResult.PASS

    if tc_res.returncode != 0:
        log_warn(f"Sclp submission exited with returncode {tc_res.returncode}")
        log_info("Console output:")
        log_gen(tc_res.stdout.decode())

    rtl_gen = False
    if phase_name == PhaseName.RTL and phase_type == PhaseType.GEN and output_file_path.exists():
        rtl_gen = True
    asm_gen = False
    if phase_name == PhaseName.ASM and phase_type == PhaseType.GEN and output_file_path.exists():
        asm_gen = True
    if not run_diff(expected_output_path, output_file_path, rtl_gen, asm_gen):
        log_fail(f"Failed {phase_info_string} {testcase_path}")
        if output_file_path.exists():
            storage_path = exec_out_path / output_file_path.name
            shutil.move(output_file_path, storage_path)
        else:
            log_info(f"Submission did not generate output at expected location: {output_file_path}")
        return CheckResult.FAIL
    else:
        log_pass(f"Passed {phase_info_string} {testcase_path}")
        if output_file_path.exists():
            storage_path = exec_out_path / output_file_path.name
            shutil.move(output_file_path, storage_path)
        else:
            log_info(f"Submission did not generate output at expected location: {output_file_path}")
        return CheckResult.PASS

def run_spim(spim_file: Path, out_file: Path):
    try:
        with open(out_file, "w") as fout:
            result = subprocess.run(
                ["spim", "-quiet", "-file", str(spim_file)],
                stdout = fout,
                stderr = subprocess.DEVNULL,
                timeout=5
            )
            print("Ran spim for " + str(spim_file))
            if result.returncode == 0:
                print(str(spim_file) + " - success")
        return result.returncode
    except subprocess.TimeoutExpired:
        return -1

def clean_output(p):
    with open(p) as f:
        lines = [line.strip() for line in f if line.strip()]
    with open(p, "w") as f:
        f.write("\n".join(lines) + "\n")

def evaluate_archive(name: str, path: Path) -> dict | None:
    global output_log_file
    global suspicious_files

    # SET LOG FILE
    output_log_file = open(exec_logs_path / f"{name}.log", 'w+')
    
    result_dict = dict()
    result_dict['Group'] = name
    
    log_info(f"GROUP NAME: {name}")
    log_info(f"=================================================================")

    # Check if tar file has a valid folder name
    if not tar_file_has(path, name):
        log_err(f"Expected folder named '{name}' not found in {path}")
        log_info(f"Skipping evaluation for '{name}' due to errors")
        return None
    
    log_info(f"Extractng archive at {path}")
    extract_tar_file(path, path.parent)

    build_dir: Path = submissions_path / name

    log_info(f"Running make clean in {build_dir}")
    clean_submission(build_dir)

    log_info(f"Running sanity check on {build_dir}")
    sus_list = sanity_check_submission(build_dir)

    if len(sus_list) > 0:
        if name not in suspicious_files:
            suspicious_files[name] = set()
        suspicious_files[name] = suspicious_files[name].union(sus_list)

    log_info(f"Building {build_dir}")
    if not build_submission(build_dir):
        log_info(f"Skipping evaluation for '{name}' due to errors")
        return None
    
    log_info(f"Checking shift-reduce conflicts in {build_dir}")
    shift_reduce_count = check_shift_reduce_conflicts(build_dir)
    result_dict["SR_Conflicts"] = shift_reduce_count

    exec_path: Path = build_dir / "sclp"
    exec_group_out_path = exec_out_path / name
    exec_group_out_path.mkdir(parents=True, exist_ok=True)

    # Check for post-build suspicions.
    if check_sclp_hashes(exec_path):
        log_sus(f"File with hash that matches one of the reference implementations present AFTER building the submission.")
        if name not in suspicious_files:
            suspicious_files[name] = set()
        suspicious_files[name].add(exec_path)

    log_info(f"Starting testcase evaluation of {build_dir}")

    total_count = 0

    for testcase in nonerror_testcase_list:
        for eval_config in eval_config_list:
            testcase_expected = None
            
            if eval_config[0] != PhaseName.FULL:
                testcase_expected = testcase['expected_' + eval_config[0].value] 
            
            result = check_testcase(
                exec_path, exec_group_out_path, testcase['path'],
                testcase_expected,
                eval_config[0], eval_config[1])

            col_name = f"{eval_config[0].value}_{eval_config[1].value}_{testcase['name']}"
            
            if result == CheckResult.PASS:
                total_count += 1
                result_dict[col_name] = 1
            else:
                result_dict[col_name] = 0

    for testcase in error_testcase_list:
        for eval_config in invalid_eval_config_list:
            result = check_testcase(
                exec_path, exec_group_out_path, testcase['path'],
                None,
                eval_config[0], eval_config[1])

            col_name = f"{eval_config[0].value}_{eval_config[1].value}_{testcase['name']}"
            
            if result == CheckResult.PASS:
                total_count += 1
                result_dict[col_name] = 1
            else:
                result_dict[col_name] = 0
    BASE_DIR = Path(__file__).resolve().parent
    SPIM_DIR = Path(BASE_DIR / "Expected_Output") / "SPIM"
    STU_SPIM_DIR = Path(BASE_DIR / "Submission_Execution_Outputs/") / name.strip().replace("\r", "").replace("\n", "")
    spim_file_list = ["tc_valid_08_01.c.spim", "tc_valid_08_03.c.spim"]
                         # ,SPIM_DIR+"tc_valid_08_10.c.spim"]

    # if STU_SPIM_DIR.exists():
    #     print("Dir contents:", list(STU_SPIM_DIR.iterdir()))
    # else: print("Dir missing")
    col_name = f"SPIM_"
    for stu_file in spim_file_list:
       
        stu_path = STU_SPIM_DIR / stu_file
        ref_path = SPIM_DIR / stu_file
        print(stu_path)
        print(ref_path)
        if not stu_path.exists():
            print(f"Missing file {stu_file}")
            log_fail(f"Missing file {stu_file}")
            result_dict[col_name+stu_file.split('.')[0]] = 0
            continue
        
            # call spim, get output for ref
            # call spim for stud
            # diff
        stu_out = Path(f"tmp_stu_{stu_file}.out")
        ref_out = Path(f"tmp_ref_{stu_file}.out")

        rc1 = run_spim(stu_path, stu_out)
        rc2 = run_spim(ref_path, ref_out)

        clean_output(stu_out)
        clean_output(ref_out)

        if rc1 != 0 or rc2 != 0:
            log_fail(f"SPIM failed for {stu_file}")
            result_dict[col_name + stu_file.split('.')[0]] = 0
            continue
        if run_diff(stu_out, ref_out, rtl_gen=False, asm_gen=False):
            log_pass(f"{stu_file} PASSED")
            total_count += 1
            result_dict[col_name + stu_file.split('.')[0]] = 1
        else:
            log_fail(f"{stu_file} FAILED")
            result_dict[col_name + stu_file.split('.')[0]] = 0

    
    result_dict["Total"] = total_count

    output_log_file.close()
    return result_dict


# This function creates a set of records for the testcase files.
# These will be used everywhere.
def generate_testcase_lists() -> list:
    global nonerror_testcase_list
    global error_testcase_list
    
    for file in testcases_nonerror_path.glob("*.c"):
        nonerror_testcase_list.append({
            'name': file.stem,
            'path': file,
            'expected_TOK':   None,
            'expected_PARSE': None,
            'expected_AST' : None,
            'expected_TAC':   None,
            'expected_RTL':   None,
            'expected_ASM':   None
        })
    
    nonerror_testcase_list.sort(key=lambda x: x['name'])
    
    for file in testcases_error_path.glob("*.c"):
        error_testcase_list.append({
            'name': file.stem,
            'path': file
        })
    
    error_testcase_list.sort(key=lambda x: x['name'])

def generate_csv_header() -> list:
    header = []
    header.append("Group")

    for testcase in nonerror_testcase_list:
        for eval_config in eval_config_list:
            col_name = f"{eval_config[0].value}_{eval_config[1].value}_{testcase['name']}"
            header.append(col_name)

    for testcase in error_testcase_list:
        for eval_config in invalid_eval_config_list:
            col_name = f"{eval_config[0].value}_{eval_config[1].value}_{testcase['name']}"
            header.append(col_name)

    spim_file_list = ["tc_valid_08_01.c.spim", "tc_valid_08_03.c.spim"]
    col_name = f"SPIM_"
    header.append(col_name+spim_file_list[0].split('.')[0])
    header.append(col_name+spim_file_list[1].split('.')[0])

    header.append("Total")
    header.append("SR_Conflicts")

    return header

def generate_csv_row(header, result) -> list:
    ret = []
    for col in header:
        ret.append(result[col])
    return ret

# EDITABLE
# This function should be gradually uncommented or updated.
# Select the text, then use CTRL + / to uncomment the required lines.
def verify_expected_outputs():
    for testcase in nonerror_testcase_list:
        file = testcase['path']
        
        # Get Expected output path
        expected_path = tok_out_path / f"{file.name}.toks"
        # And verify that it exists
        assert_path(expected_path)
        testcase['expected_TOK'] = expected_path

        ## AST OUTPUT GENERATION ##
        # Get Expected output path
        expected_path = ast_out_path / f"{file.name}.ast"
        # And verify that it exists
        assert_path(expected_path)
        testcase['expected_PARSE'] = expected_path

        # ## TAC OUTPUT GENERATION ##
        # Get Expected output path
        expected_path = tac_out_path / f"{file.name}.tac"
        # And verify that it exists
        assert_path(expected_path)
        testcase['expected_TAC'] = expected_path

        # ## RTL OUTPUT GENERATION ##
        # # Get Expected output path
        expected_path = rtl_out_path / f"{file.name}.rtl"
        # # And verify that it exists
        assert_path(expected_path)
        testcase['expected_RTL'] = expected_path

        # ## ASM OUTPUT GENERATION ##
        # # Get Expected output path
        expected_path = asm_out_path / f"{file.name}.spim"
        # # And verify that it exists
        assert_path(expected_path)
        testcase['expected_ASM'] = expected_path



def main():
    ## MAKE DEFAULT DIRECTORIES ##
    exec_logs_path.mkdir(parents=True, exist_ok=True)
    exec_out_path.mkdir(parents=True, exist_ok=True)

    ## CHECK DIRECTORY INTEGRITY ##
    assert_path(testcases_path)
    assert_path(testcases_nonerror_path)
    assert_path(testcases_error_path)
    assert_path(exec_logs_path)
    assert_path(exec_out_path)

    ## ARGPARSE ##
    parser = argparse.ArgumentParser(
        description="Automatic Evaluator for CS316 Lab Assignments.")

    parser.add_argument(
        "--sclp_path",
        help=f"Specify the path to {CURRENT_REFIMPL}",
        default="")

    parser.add_argument(
        "--csv",
        help="Generate the csv file with evaluation result for all groups available",
        action='store_true')
    
    args = parser.parse_args()

    sclp_actual_path = None

    if args.sclp_path == "":
        if current_path_refimpl.exists():
            sclp_actual_path = current_path_refimpl
        elif default_path_refimpl.exists():
            sclp_actual_path = default_path_refimpl
        else:
            print(f"[ERROR] No Reference Implementation Found. Please supply one.")
            sys.exit(1)

        print(f"[WARN]: Path to Reference Implementation not supplied explicitly.")
    else:
        sclp_actual_path = Path(args.sclp_path)
        assert_path(sclp_actual_path)

    print(f"[INFO]: Using Sclp Path: {sclp_actual_path}")

    # Store the hash of the supplied SCLP binary
    global current_sclp_sha256sum
    current_sclp_sha256sum = get_sha256sum(sclp_actual_path)

    ######

    # Get information about all testcases, and sort it.
    generate_testcase_lists()
    print(f"[INFO] Total Nonerror Testcases: {len(nonerror_testcase_list)}")
    print(f"[INFO] Total Error Testcases: {len(error_testcase_list)}")

    # Verify that all expected outputs are present.
    verify_expected_outputs()

    # UNCOMMENT TO LIST CONTENTS OF TABLES
    # for i in nonerror_testcase_list:
    #     print(f"{i['name']}\t{i['path']}\t{i['expected_TOK']}")
    # for i in error_testcase_list:
    #     print(f"{i['name']}\t{i['path']}")

    #######

    print("[INFO] Finding Groups...")
    group_list, incorrect_dir_names = get_group_list()
    group_list.sort(key=lambda x: 
    x['name'])
    incorrect_dir_names.sort(key=lambda x: x['name'])
    print("[INFO] Groups found:")
    print(f"{'NAME':<16} PATH TO ARCHIVE")
    for i in group_list:
        print(f"{i['name']:<16} {i['path']}")
    print("\n")
    if len(incorrect_dir_names) > 0:
        print("[INFO] Incorrectly named directories:")
        print(f"{'NAME':<16} PATH TO ARCHIVE")

        for i in incorrect_dir_names:
            print(f"{i['name']:<16} {i['path']}")
        
        print("\n")
    #######

    summary_table = []
    csv_table = []

    header = generate_csv_header()
    csv_table.append(header)
    
    print("[INFO] Starting Evaluation...")
    for i in group_list:
        print(f"[INFO] Evaluating {i['name']}")
        result = evaluate_archive(i['name'], i['path'])
        if result == None:
            print(f"[INFO] Evaluation skipped for {i['name']}")
            dummy_result = [i['name']]
            dummy_result.extend([0]*(len(header) - 1))
            csv_table.append(dummy_result)
        else:
            csv_table.append(generate_csv_row(header, result))
            summary_table.append([ result['Group'], result['Total'], result['SR_Conflicts'] ])

    for i in incorrect_dir_names:
        print(f"[INFO] Assigning zero to {i['name']}")
        dummy_result = [i['name']]
        dummy_result.extend([0]*(len(header) - 1))
        csv_table.append(dummy_result)
    
    if Path("./expected_rtl_output").exists:
        Path("./expected_rtl_output").unlink(missing_ok=True)
    if Path("./rtl_output").exists:
        Path("./rtl_output").unlink(missing_ok=True)

    print("")
    print("SUMMARY:")
    print("========")
    print("TOTAL TESTCASES: ", len(nonerror_testcase_list) + len(error_testcase_list))
    print("MAX POSSIBLE SCORE: ", len(header) - 3)
    print("")
    print(f"{'GROUP':<16} {'SCORE':<10} SR CONFLICTS")

    for row in summary_table:
        print(f"{row[0]:<16} {row[1]:<10} {row[2]}")

    print("")

    if len(suspicious_files) > 0:
        print("SUSPICIOUS FILES DETECTED:")
        for group in suspicious_files.keys():
            print(f"  {group}:")
            for file in suspicious_files[group]:
                print(f"    * {file}")
        print("Please see the logs for more details.")
        print("If suspicions are valid, REMOVE these submissions from the folder and run the script again.")
    else:
        print("No suspicious activity detected by the automated checker.")

    print("")
    
    if args.csv:
        with open('results.csv', mode='w') as outfile:
            writer = csv.writer(outfile)
            writer.writerows(csv_table)
        
        print(f"Results written to results.csv")
    else:
        print("Use the --csv flag to export results to 'results.csv'.")

if __name__ == '__main__':
    main()
