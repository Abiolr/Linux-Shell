# Linux Shell

## Overview

The **Linux Shell** is a custom Unix-like shell written in C as part of a university project for *COMP 3659 – Operating Systems*. It supports essential shell features such as command execution, I/O redirection, pipelines, signal handling, and a custom memory allocator. This project explores key operating system concepts including process management, inter-process communication, and signal control.

## Features

- **Command Execution**: Launch any executable with arguments
- **I/O Redirection**: Support for input (`<`) and output (`>`) redirection
- **Pipelines**: Execute chained commands using pipes (`|`)
- **Background Execution**: Run processes in the background using `&`
- **Built-in Commands**: Includes `cd` for directory navigation and `exit` to quit
- **Signal Handling**: Gracefully handles `SIGINT` (Ctrl+C) without killing the shell
- **Custom Memory Management**: Lightweight heap allocator for internal operations

## Code Structure

- `shell.c`: Main shell loop and command parsing
- `job.c` / `job.h`: Job and pipeline management
- `str_lib.c` / `str_lib.h`: Utility functions for string operations
- `mylib.c` / `mylib.h`: Custom heap allocator implementation
- `signal_handle.c` / `signal_handle.h`: Custom signal handling logic
- `struct.h`: Data structure definitions
- `constants.h`: Configuration values and limits

## Build Instructions

Use the provided `Makefile` to build the project.

```bash
make        # Build the shell
make clean  # Remove compiled files
```

This will generate an executable named `shell`.

## Usage

Run the shell:

```bash
./shell
```

### Examples

**Basic Command**
```bash
mysh$ /bin/ls
```

**I/O Redirection**
```bash
mysh$ /bin/ls > output.txt
mysh$ /bin/cat < input.txt
```

**Pipelines**
```bash
mysh$ /bin/ls -l | /usr/bin/grep .c
```

**Background Processes**
```bash
mysh$ /bin/sleep 10 &
```

**Built-in Commands**
```bash
mysh$ cd /tmp
mysh$ exit
```

## Testing

A test script `test_cases.txt` is included with sample inputs to validate core functionality.

To test the memory management module:

```bash
make test_mylib
./test_mylib
```

## Limitations

- No support for environment variables or wildcard expansion
- Argument count and pipeline depth are limited by `MAX_ARGS` and `MAX_PIPELINE_LEN`
- No support for freeing individual allocations (entire heap reset only)

## Implementation Notes

### Custom Memory Management

MyShell uses a fixed-size heap with custom functions:
- `my_alloc(size_t size)`: Allocates memory
- `my_free_all()`: Resets the heap

This approach reduces fragmentation and simplifies cleanup, but lacks granularity.

### Job Execution Flow

1. Parse input into a `job_t` structure
2. Set up pipes (if pipeline is present)
3. Fork processes for each stage
4. Redirect input/output as required
5. Execute using `execve`
6. Wait for child processes unless running in background

### Signal Handling

Custom `SIGINT` handler allows Ctrl+C to interrupt foreground jobs without killing the shell process itself.

## Authors

- **Abiola Raji**
- **Jacky On**
- **Cameron Goit**

**Class**: COMP 3659 – Operating Systems
