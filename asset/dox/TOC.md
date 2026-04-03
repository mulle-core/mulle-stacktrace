# mulle-stacktrace Library Documentation for AI
<!-- Keywords: debugging, stacktrace -->

## 1. Introduction & Purpose

`mulle-stacktrace` is a C library designed to provide a simple, cross-platform way to capture and print a stack trace of the current execution path. A stack trace is a list of the active function calls at a certain point in time, which is an essential tool for debugging crashes and understanding program flow.

The library abstracts away the platform-specific details of stack walking. On POSIX-compliant systems like Linux and macOS, it currently wraps the functionality provided by `<execinfo.h>`. The goal is to provide a single, unified API that works across different operating systems.

## 2. Key Concepts & Design Philosophy

- **Simplicity:** The library exposes a very straightforward API. The primary function captures the stack trace, and a secondary function prints it in a human-readable format.
- **Abstraction:** It hides the underlying implementation (e.g., `backtrace`, `backtrace_symbols_fd`) from the user, providing a portable interface.
- **On-Demand Symbolization:** The conversion of return addresses to function names, file names, and line numbers (a process called symbolization) is handled by the underlying system libraries. The quality of the output depends on the presence of debugging symbols in the executable and its linked libraries.

## 3. Core API & Data Structures

The API is contained entirely within `mulle-stacktrace.h`.

### 3.1. `mulle-stacktrace.h`

#### Stack Trace Retrieval
- `mulle_stacktrace_get(ignore)`: Captures the current stack trace.
  - `ignore`: An `unsigned int` specifying the number of the most recent stack frames to ignore in the output. This is useful for hiding the stack trace functions themselves from the result. A typical value is 1 or 2.
  - **Returns**: A `char **` (an array of strings), where each string represents a single frame of the stack trace. The caller is responsible for freeing this array and the strings it contains using `mulle_stacktrace_free()`. Returns `NULL` on failure.

#### Stack Trace Printing
- `mulle_stacktrace_print(ignore)`: Captures the current stack trace and immediately prints it to `stderr`. This is a convenience function that combines `mulle_stacktrace_get` and printing.
  - `ignore`: Same as in `mulle_stacktrace_get`.

#### Memory Management
- `mulle_stacktrace_free(trace)`: Frees the array of strings returned by `mulle_stacktrace_get`.
  - `trace`: The `char **` array to be freed.

## 4. Performance Characteristics

- **Overhead:** Capturing a stack trace can be a relatively slow operation, as it may require the operating system to walk the stack and perform symbol lookups. It should not be used in performance-critical code paths.
- **Memory Usage:** The memory required is proportional to the depth of the stack trace, as an array of strings is allocated to hold the symbol names for each frame.
- **Thread-Safety:** The underlying `backtrace` functions are generally considered thread-safe.

## 5. AI Usage Recommendations & Patterns

- **Debugging and Error Reporting:** The primary use case is for debugging. Call `mulle_stacktrace_print` in signal handlers (for crashes like `SIGSEGV`), in assertion failure macros, or in error-handling code paths to provide context about where the error occurred.
- **Ignoring Frames:** When calling from a helper function, use the `ignore` parameter to produce cleaner output. For example, if you have a function `my_error_reporter()` that calls `mulle_stacktrace_print()`, you would call it with `ignore=1` to exclude `my_error_reporter` from the trace itself.
- **Memory Management:** If you use `mulle_stacktrace_get` to retrieve the trace for custom processing, it is **critical** to call `mulle_stacktrace_free` on the result to avoid memory leaks. For simple printing, `mulle_stacktrace_print` is safer as it handles its own memory.
- **Compiler Flags:** For the stack trace to be useful, the program must be compiled with debugging symbols. For GCC and Clang, this is the `-g` flag. Without debug symbols, the output will likely only contain raw addresses, which are much less helpful.

## 6. Integration Examples

### Example 1: Printing a Stack Trace on Demand

This example demonstrates how to call the stack trace function from within a deeply nested function to see the call chain.
*Source: `demo/src/main.c` (adapted for clarity)*

```c
#include <mulle-stacktrace/mulle-stacktrace.h>
#include <stdio.h>

void function_c(void)
{
    printf("Printing stack trace from function_c:\n");
    // Ignore 0 frames, so we see the call to mulle_stacktrace_print itself.
    mulle_stacktrace_print(0);
}

void function_b(void)
{
    function_c();
}

void function_a(void)
{
    function_b();
}

int main(int argc, char *argv[])
{
    function_a();
    return 0;
}
```
**Execution Output (will vary by platform and compiler):**

```
Printing stack trace from function_c:
0   my_program                          0x000000010f1b1e8c mulle_stacktrace_print + 44
1   my_program                          0x000000010f1b1d28 function_c + 24
2   my_program                          0x000000010f1b1d48 function_b + 24
3   my_program                          0x000000010f1b1d68 function_a + 24
4   my_program                          0x000000010f1b1d90 main + 16
5   libdyld.dylib                       0x00007fff6c3e5cc9 start + 1
```

### Example 2: Retrieving and Freeing a Stack Trace

This example shows how to get the stack trace as data, process it (in this case, just print it), and then correctly free the memory.

```c
#include <mulle-stacktrace/mulle-stacktrace.h>
#include <stdio.h>

void process_stack_trace(void)
{
    char **trace;
    int i;

    // Get the stack trace, ignoring this function itself.
    trace = mulle_stacktrace_get(1);
    if (!trace)
    {
        fprintf(stderr, "Could not get stack trace.\n");
        return;
    }

    printf("--- Custom Stack Trace Report ---\n");
    for (i = 0; trace[i]; i++)
    {
        printf("Frame %d: %s\n", i, trace[i]);
    }
    printf("--- End of Report ---\n");

    // CRITICAL: Free the memory allocated by mulle_stacktrace_get.
    mulle_stacktrace_free(trace);
}

int main(int argc, char *argv[])
{
    process_stack_trace();
    return 0;
}
```

## 7. Dependencies

- `mulle-dlfcn`
- `libbacktrace` (optional, for more detailed traces on some platforms)
