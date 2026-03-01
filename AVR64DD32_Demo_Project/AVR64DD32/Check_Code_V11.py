#!/usr/bin/env python3
import os
import re
import sys

# --------------------------
# Naming rules (EDIT THESE)
# --------------------------

# Functions – your style: e.g. ADC_Initialize, CPU_Interface_Monitor
FUNC_PATTERN = re.compile(r'^[A-Z][A-Za-z0-9_]*$')

# lowerCamelCase body: value, adcResult, cpuInterfaceStateMachine
CAMEL_BODY = r'[a-z]+(?:[A-Z][a-z0-9]*)*'

# locals, statics, struct members, struct tags
LOCAL_VAR_PATTERN = re.compile(r'^' + CAMEL_BODY + r'$')
STRUCT_TAG_PATTERN = LOCAL_VAR_PATTERN

# globals: g_ + lowerCamelCase
GLOBAL_PATTERN = re.compile(r'^g_' + CAMEL_BODY + r'$')

# statics: same rule as locals (NO s_ prefix)
STATIC_PATTERN = LOCAL_VAR_PATTERN

# macros: ALL_CAPS_WITH_UNDERSCORES
MACRO_PATTERN = re.compile(r'^[A-Z][A-Z0-9_]*$')

# typedef names: CamelCase_t
TYPE_PATTERN = re.compile(r'^[A-Z][A-Za-z0-9]*_t$')

# --------------------------
# Ignore lists
# --------------------------

# Skip these directories (any path containing one of these substrings)
EXCLUDED_DIR_KEYWORDS = [
    '.git',
    'Debug',
    'Release',
    'build',
    'mcc_generated_files',  # common Microchip MCC dir – edit as needed
]

# Skip specific filenames (basenames only)
EXCLUDED_FILE_BASENAMES = [
    # 'device.h',
    # 'clock_config.h',
]

# Specific variable/function names to ignore (if needed)
EXCLUDED_VARIABLE_NAMES = set()
EXCLUDED_FUNCTION_NAMES = {'main'}

# --------------------------
# Simple C parsing helpers
# --------------------------

# Keywords that look like functions but aren't
CONTROL_KEYWORDS = {
    'if', 'for', 'while', 'switch', 'return', 'sizeof', 'main'
}

FUNC_DEF_RE = re.compile(
    r'''^
        \s*
        (?:[a-zA-Z_][\w\s\*\(\)]*)   # return type + maybe modifiers
        \s+
        (?P<name>[a-zA-Z_]\w*)       # function name
        \s*\(                        # opening parenthesis
    ''', re.VERBOSE
)

# typedef ... Name_t;
TYPEDEF_RE = re.compile(
    r'''\btypedef\b.*?\b(?P<name>[A-Za-z_]\w*)\s*;'''
)

# Very rough var decl at global scope (no typedef, no function)
# We'll split later to extract last identifier before '=' or ';'
GLOBAL_VAR_LINE_RE = re.compile(
    r'^\s*(?:static\s+)?[A-Za-z_][\w\s\*\[\]]*;'
)

# Rough local variable declaration inside a function scope
LOCAL_VAR_LINE_RE = re.compile(
    r'^\s*[A-Za-z_][\w\s\*\[\]]*\s+[A-Za-z_]\w*\s*(=|;|,)'  # type ... name[=...];
)


def is_probable_function(line: str):
    """Return function name if this line looks like a function definition."""
    m = FUNC_DEF_RE.match(line)
    if not m:
        return None
    name = m.group('name')
    if name in CONTROL_KEYWORDS:
        return None
    # Reject prototypes: no opening brace here, line ends with ';'
    stripped = line.strip()
    if stripped.endswith(';'):
        return None
    return name


def extract_var_name_from_decl(line: str):
    """
    Extract the identifier for a simple variable declaration line like:
        int foo;
        static uint16_t bar = 0;
    Very heuristic, but fine for style checks.
    """
    line = line.strip().rstrip(';')
    if line.startswith('typedef'):
        return None, None

    is_static = line.startswith('static ')
    if is_static:
        line_no_static = line[len('static '):]
    else:
        line_no_static = line

    # Split at '=' to drop initializer
    before_eq = line_no_static.split('=', 1)[0].strip()
    tokens = before_eq.split()
    if not tokens:
        return None, None

    last = tokens[-1]
    # Remove pointer stars and array brackets
    last = last.strip()
    last = last.rstrip('[]')
    last = last.lstrip('*')

    if not last or not re.match(r'^[A-Za-z_]\w*$', last):
        return None, None

    return last, is_static


def check_file(path, errors):
    with open(path, encoding='utf-8', errors='ignore') as f:
        lines = f.readlines()

    brace_depth = 0
    in_struct = False

    for lineno, raw in enumerate(lines, start=1):
        line = raw.rstrip('\n')
        stripped = line.strip()

        # Track brace depth (scope)
        # Do this BEFORE modifying brace_depth so depth 0 is "file scope"
        current_depth = brace_depth

        # -----------------
        # Macros (#define)
        # -----------------
        if stripped.startswith('#define'):
            parts = stripped.split()
            if len(parts) >= 2:
                macro_name = parts[1]
                macro_name = macro_name.split('(')[0]  # drop parameters
                if not MACRO_PATTERN.match(macro_name):
                    errors.append(
                        f'{path}:{lineno}: macro "{macro_name}" '
                        f'does not match pattern {MACRO_PATTERN.pattern}'
                    )

        # -----------------
        # Typedefs
        # -----------------
        for m in TYPEDEF_RE.finditer(line):
            type_name = m.group('name')
            if not TYPE_PATTERN.match(type_name):
                errors.append(
                    f'{path}:{lineno}: typedef "{type_name}" '
                    f'does not match pattern {TYPE_PATTERN.pattern}'
                )

        # -----------------
        # Enum constants (UPPER_CASE)
        # -----------------
        if 'enum' in stripped or current_depth > 0:
            # Very simple enum value match: VALUE, or VALUE = x, or VALUE}
            enum_const_match = re.match(
                r'^\s*([A-Za-z_][A-Za-z0-9_]*)\s*(=|,|})', stripped
            )
            if enum_const_match:
                enum_name = enum_const_match.group(1)
                # Ignore closing brace '}' lines where match may be struct/enclosing tokens
                if enum_name not in {'enum'}:
                    if not re.match(r'^[A-Z][A-Z0-9_]*$', enum_name):
                        errors.append(
                            f'{path}:{lineno}: enum constant "{enum_name}" '
                            f'does not match UPPER_CASE pattern ^[A-Z][A-Z0-9_]*$'
                        )

        # -----------------
        # Struct tags & members
        # -----------------
        struct_decl_match = re.match(r'^\s*struct\s+([A-Za-z_]\w*)', stripped)
        if struct_decl_match:
            struct_name = struct_decl_match.group(1)
            if not STRUCT_TAG_PATTERN.match(struct_name):
                errors.append(
                    f'{path}:{lineno}: struct name "{struct_name}" '
                    f'does not match pattern {STRUCT_TAG_PATTERN.pattern}'
                )
            in_struct = True

        # Detect end of struct (line with just '}' or '};')
        if in_struct and stripped.startswith('}'):
            in_struct = False

        # Struct members – only when inside a struct block
        if in_struct and ';' in stripped and not stripped.startswith('//') and not stripped.startswith('/*'):
            member_line = stripped.rstrip(';')
            before_eq = member_line.split('=', 1)[0].strip()
            tokens = before_eq.split()
            if tokens:
                last = tokens[-1]
                last = last.strip().rstrip('[]').lstrip('*')
                if re.match(r'^[A-Za-z_]\w*$', last):
                    member_name = last
                    if not LOCAL_VAR_PATTERN.match(member_name):
                        errors.append(
                            f'{path}:{lineno}: struct member "{member_name}" '
                            f'does not match camelCase pattern {LOCAL_VAR_PATTERN.pattern}'
                        )

        # -----------------
        # Global-scope stuff only
        # -----------------
        if current_depth == 0:
            # Function definitions
            func_name = is_probable_function(line)
            if func_name and func_name not in EXCLUDED_FUNCTION_NAMES:
                if not FUNC_PATTERN.match(func_name):
                    errors.append(
                        f'{path}:{lineno}: function "{func_name}" '
                        f'does not match pattern {FUNC_PATTERN.pattern}'
                    )

            # Global / static variables
            if GLOBAL_VAR_LINE_RE.match(line):
                var_name, is_static = extract_var_name_from_decl(line)
                if var_name and var_name not in EXCLUDED_VARIABLE_NAMES:
                    if is_static:
                        if not STATIC_PATTERN.match(var_name):
                            errors.append(
                                f'{path}:{lineno}: static variable "{var_name}" '
                                f'does not match pattern {STATIC_PATTERN.pattern}'
                            )
                    else:
                        if not GLOBAL_PATTERN.match(var_name):
                            errors.append(
                                f'{path}:{lineno}: global variable "{var_name}" '
                                f'does not match pattern {GLOBAL_PATTERN.pattern}'
                            )

        # -----------------
        # Function-scope (local) variables
        # -----------------
        if current_depth > 0:
            # Skip preprocessor and obvious control structures
            if stripped.startswith('#'):
                pass
            elif re.match(r'^\s*(if|for|while|switch)\b', stripped):
                pass
            else:
                # Heuristic: looks like "type ... name;" or "type ... name = ..."
                if LOCAL_VAR_LINE_RE.match(stripped):
                    line_no_semicolon = stripped.rstrip(';')
                    before_eq = line_no_semicolon.split('=', 1)[0].strip()
                    tokens = before_eq.split()
                    if tokens:
                        last = tokens[-1]
                        last = last.strip().rstrip('[]').lstrip('*')
                        if re.match(r'^[A-Za-z_]\w*$', last):
                            local_name = last
                            if local_name not in EXCLUDED_VARIABLE_NAMES:
                                if not LOCAL_VAR_PATTERN.match(local_name):
                                    errors.append(
                                        f'{path}:{lineno}: local variable "{local_name}" '
                                        f'does not match camelCase pattern {LOCAL_VAR_PATTERN.pattern}'
                                    )

        # Update brace depth at end of line
        brace_depth += line.count('{')
        brace_depth -= line.count('}')


def collect_c_files(root='.'):
    c_files = []
    for dirpath, dirnames, filenames in os.walk(root):
        norm_dirpath = dirpath.replace('\\', '/')

        # Skip excluded directories
        if any(keyword in norm_dirpath for keyword in EXCLUDED_DIR_KEYWORDS):
            continue

        for fn in filenames:
            if fn in EXCLUDED_FILE_BASENAMES:
                continue
            if fn.lower().endswith(('.c', '.h')):
                c_files.append(os.path.join(dirpath, fn))

    return c_files


def main():
    root = '.'
    if len(sys.argv) > 1:
        root = sys.argv[1]

    files = collect_c_files(root)
    if not files:
        print(f'No .c/.h files found under {root}')
        return 0

    errors = []
    for path in files:
        check_file(path, errors)

    if errors:
        print('Naming violations found:')
        for e in errors:
            print('  ' + e)
        print(f'\nTotal: {len(errors)} issue(s).')
        return 1
    else:
        print('All names conform to the configured patterns.')
        return 0


if __name__ == '__main__':
    sys.exit(main())
