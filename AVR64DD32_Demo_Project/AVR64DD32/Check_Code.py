#!/usr/bin/env python3
import os
import re
import sys

# --------------------------
# Naming rules (EDIT THESE)
# --------------------------

# function foo_bar
FUNC_PATTERN = re.compile(r'^[A-Z][A-Za-z0-9_]*$')


# global g_foo_bar
GLOBAL_PATTERN = re.compile(r'^g_[a-z0-9_]+$')

# static s_foo_bar
STATIC_PATTERN = re.compile(r'^s_[a-z0-9_]+$')

# MACRO FOO_BAR
MACRO_PATTERN = re.compile(r'^[A-Z][A-Z0-9_]*$')

# typedef FooBar_t
TYPE_PATTERN = re.compile(r'^[A-Z][A-Za-z0-9]*_t$')

# --------------------------
# Ignore lists
# --------------------------

# Any directory whose *path* contains one of these substrings will be skipped.
EXCLUDED_DIR_KEYWORDS = [
    '.git',
    'Debug',
    'Release',
    'build',
    'mcc_generated_files',  # common Microchip MCC dir
    'config',               # add/rename as fits your tree
]

# Any file whose base name is in this list will be skipped
EXCLUDED_FILE_BASENAMES = [
    # Examples:
    # 'device.h',
    # 'clock_config.h',
    'interrupt_avr8.h',
    'compiler.h',
    'Delay.h',
    'ccp.h',
    'usart_basic.c',
    'utils_assert.h',
    'iar.h',
]


# --------------------------
# Simple C parsing helpers
# --------------------------

# Heuristic: keywords that look like functions but aren't
CONTROL_KEYWORDS = {
    'if', 'for', 'while', 'switch', 'return', 'sizeof', 'main','atmel_start_init','system_init'
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


def is_probable_function(line):
    """Return function name if this line looks like a function definition."""
    m = FUNC_DEF_RE.match(line)
    if not m:
        return None
    name = m.group('name')
    if name in CONTROL_KEYWORDS:
        return None
    # Reject prototypes (no opening brace on this line and ends with ;)
    if line.strip().endswith(';'):
        return None
    return name


def extract_var_name_from_decl(line):
    """
    Extract the identifier for a simple variable declaration line like:
        int foo;
        static uint16_t bar = 0;
    Very heuristic, but fine for style checks.
    """
    # Remove 'static' and type-ish stuff at the front
    # Then take token before '=' or ';'
    line = line.strip().rstrip(';')
    # Ignore obvious typedefs
    if line.startswith('typedef'):
        return None, None

    is_static = line.startswith('static ')
    if is_static:
        line_no_static = line[len('static '):]
    else:
        line_no_static = line

    # Split at '=' to drop initializer
    before_eq = line_no_static.split('=', 1)[0].strip()
    # Split by whitespace; last token is usually the var name (maybe with [])
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

    for lineno, raw in enumerate(lines, start=1):
        line = raw.rstrip('\n')

        stripped = line.strip()

        # Track brace depth (scope)
        # Do this AFTER using current depth so we see global-scope lines as depth 0
        current_depth = brace_depth

        # -----------------
        # Macros (#define)
        # -----------------
        if stripped.startswith('#define'):
            parts = stripped.split()
            if len(parts) >= 2:
                macro_name = parts[1]
                # Ignore parameterized macros like FOO(x) -> "FOO(x"
                macro_name = macro_name.split('(')[0]
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
        # Global-scope stuff only
        # -----------------
        if current_depth == 0:
            # Function definitions
            func_name = is_probable_function(line)
            if func_name:
                if not FUNC_PATTERN.match(func_name):
                    errors.append(
                        f'{path}:{lineno}: function "{func_name}" '
                        f'does not match pattern {FUNC_PATTERN.pattern}'
                    )

            # Global / static variables
            if GLOBAL_VAR_LINE_RE.match(line):
                var_name, is_static = extract_var_name_from_decl(line)
                if var_name:
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

        # Update brace depth
        brace_depth += line.count('{')
        brace_depth -= line.count('}')


def collect_c_files(root='.'):
    c_files = []
    for dirpath, dirnames, filenames in os.walk(root):
        # Normalize path for portable checks
        norm_dirpath = dirpath.replace('\\', '/')

        # Skip any directory whose path contains one of our excluded keywords
        if any(keyword in norm_dirpath for keyword in EXCLUDED_DIR_KEYWORDS):
            continue

        for fn in filenames:
            # Skip specific file basenames
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
