#!/usr/bin/env python3
"""
Function Signature Checker for AVR64DD32 Project

This script compares function declarations in .h files with their definitions
in .c files to ensure consistency in the (void) parameter style.

Author: Code Review Assistant
Date: January 26, 2026
"""

import os
import re
import sys
from pathlib import Path
from typing import List, Dict, Tuple, Set


class FunctionSignature:
    """Represents a function signature."""
    
    def __init__(self, return_type: str, name: str, params: str, line_num: int, full_line: str):
        self.return_type = return_type.strip()
        self.name = name.strip()
        self.params = params.strip()
        self.line_num = line_num
        self.full_line = full_line.strip()
    
    def __repr__(self):
        return f"{self.return_type} {self.name}({self.params})"
    
    def has_void_params(self) -> bool:
        """Check if function uses (void) for no parameters."""
        return self.params == "void"
    
    def has_empty_params(self) -> bool:
        """Check if function uses () for no parameters."""
        return self.params == ""
    
    def is_no_param_function(self) -> bool:
        """Check if this is a no-parameter function."""
        return self.has_void_params() or self.has_empty_params()


def extract_functions_from_file(filepath: Path, is_header: bool = True) -> List[FunctionSignature]:
    """
    Extract function declarations/definitions from a file.
    
    Args:
        filepath: Path to the .h or .c file
        is_header: True if this is a header file, False for source file
        
    Returns:
        List of FunctionSignature objects
    """
    functions = []
    
    try:
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            lines = f.readlines()
    except Exception as e:
        print(f"Error reading {filepath}: {e}")
        return functions
    
    # Pattern for function declarations/definitions
    # Matches: return_type function_name(parameters);
    # or:      return_type function_name(parameters) {
    if is_header:
        # In headers, look for prototypes ending with ;
        pattern = r'^\s*([a-zA-Z_][\w\s\*]*?)\s+([a-zA-Z_]\w*)\s*\(([^)]*)\)\s*;'
    else:
        # In source files, look for definitions (may have { on same line or next)
        pattern = r'^\s*([a-zA-Z_][\w\s\*]*?)\s+([a-zA-Z_]\w*)\s*\(([^)]*)\)\s*(?:\{|$)'
    
    in_multiline_comment = False
    
    for line_num, line in enumerate(lines, start=1):
        # Skip multi-line comments
        if '/*' in line:
            in_multiline_comment = True
        if '*/' in line:
            in_multiline_comment = False
            continue
        if in_multiline_comment:
            continue
            
        # Skip single-line comments
        if line.strip().startswith('//'):
            continue
        
        # Skip preprocessor directives
        if line.strip().startswith('#'):
            continue
        
        # Skip typedef function pointers
        if 'typedef' in line or '(*' in line:
            continue
        
        # Skip lines that are clearly struct/enum definitions
        if re.match(r'^\s*(struct|enum|typedef)\s+', line):
            continue
        
        match = re.search(pattern, line)
        if match:
            return_type = match.group(1)
            func_name = match.group(2)
            params = match.group(3)
            
            # Skip if return type contains keywords that indicate this isn't a function
            skip_keywords = ['struct', 'enum', 'typedef', 'union']
            if any(keyword in return_type.lower() for keyword in skip_keywords):
                continue
            
            # Skip common false positives
            if func_name in ['if', 'while', 'for', 'switch', 'sizeof']:
                continue
            
            func = FunctionSignature(return_type, func_name, params, line_num, line)
            functions.append(func)
    
    return functions


def find_matching_c_file(h_file: Path, project_root: Path) -> Path:
    """
    Find the corresponding .c file for a given .h file.
    
    Args:
        h_file: Path to .h file
        project_root: Root directory of the project
        
    Returns:
        Path to .c file or None if not found
    """
    # Try same directory first
    c_file = h_file.with_suffix('.c')
    if c_file.exists():
        return c_file
    
    # Try searching in subdirectories
    module_name = h_file.stem
    for c_file in project_root.rglob(f"{module_name}.c"):
        return c_file
    
    return None


def compare_signatures(h_funcs: List[FunctionSignature], 
                       c_funcs: List[FunctionSignature],
                       h_file: Path,
                       c_file: Path) -> List[Dict]:
    """
    Compare function signatures between header and source files.
    
    Returns:
        List of dictionaries describing mismatches
    """
    mismatches = []
    
    # Create lookup dictionary for C functions
    c_func_dict = {f.name: f for f in c_funcs}
    
    for h_func in h_funcs:
        # Only check functions with no parameters
        if not h_func.is_no_param_function():
            continue
        
        # Find matching C function
        c_func = c_func_dict.get(h_func.name)
        
        if c_func is None:
            # Function declared in .h but not defined in .c (maybe extern or inline)
            continue
        
        # Check if parameter styles match
        h_has_void = h_func.has_void_params()
        c_has_void = c_func.has_void_params()
        
        if h_has_void != c_has_void:
            mismatch = {
                'function': h_func.name,
                'h_file': str(h_file),
                'c_file': str(c_file),
                'h_line': h_func.line_num,
                'c_line': c_func.line_num,
                'h_signature': str(h_func),
                'c_signature': str(c_func),
                'h_style': 'void' if h_has_void else 'empty',
                'c_style': 'void' if c_has_void else 'empty',
                'h_full_line': h_func.full_line,
                'c_full_line': c_func.full_line
            }
            mismatches.append(mismatch)
    
    return mismatches


def format_report(mismatches: List[Dict], verbose: bool = False) -> str:
    """
    Format the mismatch report.
    
    Args:
        mismatches: List of mismatch dictionaries
        verbose: Include full file paths and line contents
        
    Returns:
        Formatted report string
    """
    if not mismatches:
        return "✓ All function signatures match! No issues found."
    
    report_lines = []
    report_lines.append("=" * 80)
    report_lines.append(f"FUNCTION SIGNATURE MISMATCH REPORT")
    report_lines.append(f"Found {len(mismatches)} mismatch(es)")
    report_lines.append("=" * 80)
    report_lines.append("")
    
    # Group by file
    by_file = {}
    for m in mismatches:
        key = (m['h_file'], m['c_file'])
        if key not in by_file:
            by_file[key] = []
        by_file[key].append(m)
    
    for (h_file, c_file), file_mismatches in sorted(by_file.items()):
        h_name = Path(h_file).name
        c_name = Path(c_file).name
        
        report_lines.append(f"File Pair: {h_name} / {c_name}")
        report_lines.append("-" * 80)
        
        for m in file_mismatches:
            report_lines.append(f"  Function: {m['function']}")
            report_lines.append(f"    Header (.h):  Line {m['h_line']:4d} - ({m['h_style']:5s})  {m['h_signature']}")
            report_lines.append(f"    Source (.c):  Line {m['c_line']:4d} - ({m['c_style']:5s})  {m['c_signature']}")
            
            if verbose:
                report_lines.append(f"      .h: {m['h_full_line']}")
                report_lines.append(f"      .c: {m['c_full_line']}")
            
            report_lines.append("")
        
        report_lines.append("")
    
    # Summary
    report_lines.append("=" * 80)
    report_lines.append("SUMMARY")
    report_lines.append("=" * 80)
    report_lines.append(f"Total mismatches: {len(mismatches)}")
    
    # Count by type
    h_void_c_empty = sum(1 for m in mismatches if m['h_style'] == 'void' and m['c_style'] == 'empty')
    h_empty_c_void = sum(1 for m in mismatches if m['h_style'] == 'empty' and m['c_style'] == 'void')
    
    if h_void_c_empty > 0:
        report_lines.append(f"  - Header has (void), Source has ():  {h_void_c_empty}")
    if h_empty_c_void > 0:
        report_lines.append(f"  - Header has (), Source has (void):  {h_empty_c_void}")
    
    report_lines.append("")
    report_lines.append("RECOMMENDATION:")
    report_lines.append("  Update the .c files to match the .h files (use void for consistency)")
    
    return "\n".join(report_lines)


def generate_fix_script(mismatches: List[Dict], output_file: Path):
    """
    Generate a Python script to automatically fix the mismatches.
    
    Args:
        mismatches: List of mismatch dictionaries
        output_file: Path where to save the fix script
    """
    script_lines = [
        "#!/usr/bin/env python3",
        '"""',
        "Auto-generated script to fix function signature mismatches.",
        "Generated by check_function_signatures.py",
        '"""',
        "",
        "import re",
        "from pathlib import Path",
        "",
        "def fix_function_signature(filepath, line_num, old_pattern, new_pattern):",
        '    """Fix a single function signature."""',
        "    with open(filepath, 'r', encoding='utf-8') as f:",
        "        lines = f.readlines()",
        "    ",
        "    if line_num <= len(lines):",
        "        lines[line_num - 1] = lines[line_num - 1].replace(old_pattern, new_pattern)",
        "    ",
        "    with open(filepath, 'w', encoding='utf-8') as f:",
        "        f.writelines(lines)",
        "    ",
        "    print(f'Fixed: {filepath}:{line_num}')",
        "",
        "# Fixes to apply:",
        ""
    ]
    
    for m in mismatches:
        if m['h_style'] == 'void' and m['c_style'] == 'empty':
            # Need to change .c from () to (void)
            old = f"{m['function']}()"
            new = f"{m['function']}(void)"
            script_lines.append(f"fix_function_signature(")
            script_lines.append(f"    Path(r'{m['c_file']}'),")
            script_lines.append(f"    {m['c_line']},")
            script_lines.append(f"    '{old}',")
            script_lines.append(f"    '{new}'")
            script_lines.append(f")")
            script_lines.append("")
    
    script_lines.append('print("All fixes applied!")')
    
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write("\n".join(script_lines))
    
    # Make executable on Unix systems
    try:
        os.chmod(output_file, 0o755)
    except:
        pass


def main():
    """Main function."""
    import argparse
    
    parser = argparse.ArgumentParser(
        description='Check function signature consistency between .h and .c files',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s /path/to/AVR64DD32/AVR64DD32
  %(prog)s . --verbose
  %(prog)s . --generate-fix fix_signatures.py
        """
    )
    
    parser.add_argument('project_dir', 
                       help='Path to the project directory (e.g., AVR64DD32/AVR64DD32)')
    parser.add_argument('-v', '--verbose', 
                       action='store_true',
                       help='Show full file paths and line contents')
    parser.add_argument('-g', '--generate-fix',
                       metavar='SCRIPT',
                       help='Generate a Python script to auto-fix mismatches')
    parser.add_argument('--exclude',
                       action='append',
                       default=[],
                       help='Exclude directories (can be used multiple times)')
    
    args = parser.parse_args()
    
    project_root = Path(args.project_dir).resolve()
    
    if not project_root.exists():
        print(f"Error: Directory not found: {project_root}")
        sys.exit(1)
    
    print(f"Scanning project directory: {project_root}")
    print()
    
    # Default exclusions
    exclude_dirs = set(args.exclude)
    exclude_dirs.update(['Debug', 'Release', 'bin', 'obj', '.git', '.vs'])
    
    # Find all .h files (excluding system/library headers)
    h_files = []
    for h_file in project_root.rglob('*.h'):
        # Skip excluded directories
        if any(ex in h_file.parts for ex in exclude_dirs):
            continue
        # Skip utility/system files
        if any(skip in str(h_file) for skip in ['utils', 'include', 'modules']):
            continue
        h_files.append(h_file)
    
    print(f"Found {len(h_files)} header files to check")
    print()
    
    all_mismatches = []
    files_checked = 0
    
    for h_file in sorted(h_files):
        # Find corresponding .c file
        c_file = find_matching_c_file(h_file, project_root)
        
        if c_file is None:
            continue
        
        files_checked += 1
        
        # Extract functions from both files
        h_funcs = extract_functions_from_file(h_file, is_header=True)
        c_funcs = extract_functions_from_file(c_file, is_header=False)
        
        # Compare signatures
        mismatches = compare_signatures(h_funcs, c_funcs, h_file, c_file)
        all_mismatches.extend(mismatches)
    
    print(f"Checked {files_checked} file pairs")
    print()
    
    # Generate report
    report = format_report(all_mismatches, verbose=args.verbose)
    print(report)
    
    # Generate fix script if requested
    if args.generate_fix and all_mismatches:
        fix_script = Path(args.generate_fix)
        generate_fix_script(all_mismatches, fix_script)
        print()
        print(f"Generated fix script: {fix_script}")
        print(f"Run it with: python {fix_script}")
    
    # Exit with error code if mismatches found
    sys.exit(len(all_mismatches))


if __name__ == '__main__':
    main()
