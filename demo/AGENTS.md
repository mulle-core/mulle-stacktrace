# mulle-sde Build System Guidelines

## Project Crafting

mulle-sde enforces a particular project structure. It also manages dependencies
and libraries.

### 0.1 AI Support

At the start of each session, run once:

```bash
mulle-sde vibecoding on
```

### 1.1 Standard Build

To compile the project:

```bash
mulle-sde craft -- --no-hook
```

### 1.2 Clean Build

For a full, clean build, including reinstalling dependencies:

```bash
mulle-sde craft -g -- --no-hook
```

*(`-g` is shorthand for `mulle-sde clean gravetidy`)*

### 1.3 Testing

To run a specific test case:

```bash
mulle-sde test run <path_to_test_file>
```

*Example: `mulle-sde test run test/NSTableView/01_initialization.m`*

---

## Project Dependencies

The canonical source for dependency management is the `mulle-sde sourcetree`
configuration, reflected in the auto-generated `cmake/reflect/_Dependencies.cmake`.
Use `mulle-sde dependency list` for a JSON output of dependencies.

---

## Dependency Source Code Resolution

To find and access external dependency source code:

1. **Run `mulle-sde dependency find api <header>`** to locate a specific header.
2. **Run `mulle-sde dependency headers -f`** to get all dependency headers as
   absolute paths.

### Part 1: Finding the True Source Dependency Project

The `mulle-sde` build system may use "envelope" or "amalgamated" projects.
Follow these steps:

1. **Run `mulle-sde env get MULLE_FETCH_SEARCH_PATH`** to get a list of local
   directories where projects may reside.

2. **Run `mulle-sourcetree list`** to get dependency URLs:

   Example output:

   ```
   address          url
   -------          ---
   mulle-mmap       https://github.com/mulle-core/mulle-mmap.git
   mulle-container  https://github.com/mulle-c/mulle-container.git
   ```

To locate `mulle-container`, check if it belongs to `mulle-c` in the path
`/home/src/srcO/mulle-c`.

---

## Build Troubleshooting Guidelines

This section outlines the procedure for diagnosing build failures.

### Hierarchical Debugging Workflow

When `mulle-sde craft` fails:

### 1. Identify the Failing Component

Examine the build log to identify whether the failure is in the main project
or a dependency.

### 2. Attempt to Repair the Dependency

If the failure is within a dependency:

1. Navigate to the dependency's source directory.

2. Run a clean build:

   ```bash
   cd ../MulleUIView
   mulle-sde craft -c -- --no-hook
   ```

3. If the clean build fails, run a deep clean:

   ```bash
   mulle-sde craft -g -- --no-hook
   ```

### 3. Escalate to the User

If the deep clean fails, **do not attempt to fix** the dependency code.
Report the issue with the specific compiler error and ask for permission
to analyze and fix the code.

### 4. Handling Non-Symlinked Dependencies

If a dependency is symlinked, its build artifacts are cached and may be stale.
So after fixing and rebuilding the dependency at its directory.
Clean the stale artifacts from the main project:

```bash
# Run from the main project directory
mulle-sde clean <DependencyName>
```

Then, run a normal `mulle-sde craft -- --no-hook` .
This is as an executable product. You can run it with `mulle-sde run`.
This is a C project.
# Syntax

mulle-c libaries (should) use a consistent naming scheme for their data
structures and functions. After a bit of use, you should easily figure out
how a function is supposed to be named, that performs a certain action.
Conversely just by looking at the function name, you can discern the scope of
the function, its arguments and the order in which they appear.


## Data structure

A [data structure](https://en.wikipedia.org/wiki/Data_structure)
is a collection of a struct and functions operating on it.


### Struct

If a data structure interfaces with mulle-allocator
(see: mulle-allocator) it is significant. It is distinguished by the syntax,
if the data structure keeps track of the allocator in its struct:

``` c
struct my__data
{
   void                    *bytes;
   size_t                  length;
   // no allocator, therefore my__data and not my_data
};
```

``` c
struct my_data
{
   void                    *bytes;
   size_t                  length;
   struct mulle_allocator  *allocator;
};
```


The `struct` is name-spaced with a string, which is always "mulle"
in mulle projects.

```
struct                   : struct_with_allocator
                         | struct_without_allocator
                         ;

struct_with_allocator    : namespace '_' struct_name
                         ;

struct_without_allocator : namespace '_'  '_' struct_name
                         ;

namespace                : "mulle"

struct_name              : [a-z][a-z0-9]+
                         ;
```

Notice that there are no uppercase characters and no '`_`' underscore character
in a struct_name.

A tightly coupled struct inherits the allocator identification, even if itself
has no allocator or adds an allocator. A typical case is an enumerator:

``` c
struct my_dataenumerator
{
   void     *curr;
   size_t   length;  // has no allocator, but belongs to `my_data`
                     // therefore not my__dataenumerator
};

```

#### Fields

Structs are composed of fields. It's distinguished, if a field is supposed to
be accessed via accessor functions only (private).

``` c
struct mulle__string
{
   char     *s;      // public
   size_t   _len;    // private
};

size_t   mulle__string_get_length( struct mulle__string *p);
```


```
field            : public_field
                 | private_field
                 ;

public_field     : field_name
                 ;

private_field    : '_'  field_name
                 ;

field_name       : [a-z][a-z0-9_]*
                 ;
```

Notice that no uppercase characters are part of a field_name.
Underscores following an initial letter are fine, but should not be the last
character.


### Functions

Functions of a data structure are prefixed with the name of the struct.
What the function does, is expressed in its function_name which is most often a
verb/object combination with a predefined meaning:

``` c
size_t   mulle__string_get_length( struct mulle__string *s);
```

An unsafe_function function does not actively check the given arguments for
NULL pointers. A safe_function guarantees to not crash on NULL pointer input.


```
function            : unsafe_function
                    | safe_function
                    ;

unsafe_function     :  '_' struct '_' function_name
                    ;

safe_function       :  struct '_' function_name
                    ;

scoped_function_name :  scope '_' function_name
                     | function_name
                     ;

scope               : [a-z][a-z0-9]+
                    | "global"
                    | "universe"
                    | "thread"
                    ;

function_name       : verb '_' object modifiers
                    | verb modifiers
                    ;

verb                : [a-z][a-z0-9]+
                    | "advance"
                    | "alloc"
                    | "copy"
                    | "count"
                    | "create"
                    | "destroy"
                    | "done"
                    | "enumerate"
                    | "extract"
                    | "find"
                    | "free"
                    | "get"
                    | "guarantee"
                    | "init"
                    | "insert"
                    | "lookup"
                    | "make"
                    | "member"
                    | "push"
                    | "pop"
                    | "next"
                    | "register"
                    | "remove"
                    | "reset"
                    | "reserve"
                    | "search"
                    | "set"
                    ;

object              : [a-z][a-z0-9_]*
                    | "allocator"
                    | "bytes"
                    | "count"
                    | "data"
                    | "items"
                    | "last"
                    | "length"
                    | "notakey"
                    | "string"
                    | "size"
                    | "size_as_length"
                    | "used"
                    | "used_as_length"
                    ;

modifiers           :
                    : '_' modifier modifiers
                    ;

modifier            : [a-z][a-z0-9]+
                    | inline
                    | nofail
                    ;
```

# Semantics

## Verbs

Generally verbs act on the first parameter of the function call.
(e.g. mulle_map_get( map)). If that is not the case the verb is
preceded by the scope of the verb. That could be "thread_get" for
example for accessing thread-local storage.

### Verb considerations

* If there is a function where there are source and destination arguments, the
destination argument comes first.
* If a pointer along with a length are being passed, it is preferred to use
something like `struct mulle_data` and pass the couple by value (sic!). Otherwise pass
the pointer first, immediately followed by the length.
* a by-reference return value is always the last argument (multiple reference values can be
avoided with little structs)
* an allocator is the last argument, unless a by-reference return value is present


### Initialization and Destruction

| Verb    | Meaning
|---------|---------------
| alloc   | allocate memory from heap
| copy    | create + copy items
| done    | tear down struct on stack or heap
| free    | deallocate memory to heap
| init    | setup struct on stack or heap
| make    | construct on the fly (e.g. return a struct by value)

| Verb    | Decomposition
|---------|---------------
| create  | alloc + init
| destroy | done + free
| reset   | remove all, keep allocation though

| Verb    | Opposite
|---------|----------
| alloc   | free
| copy    | destroy
| create  | destroy
| init    | done


### Memory management

| Verb      | Opposite
|-----------|----------
| guarantee | Ensure that there is space for 'n' elements without realloc. A pointer to the guaranteed memory area is returned
| advance   | Reserve space for 'n' elements, returning a pointer to the first space
| reserve   | Reserve space for 1 element, returning a pointer to it

### Element Access Read

| Verb        | Meaning
|-------------|---------------
| get         | random access read
| next        | sequential access read
| member      | returns 1 (yes) or 0 (no) depending on presence in container


### Element Access Write

| Verb        | Meaning
|-------------|---------------
| add         | sequential access write, into empty space
| extract     | destructive get, with ownership transfer (caller must free)
| insert      | random access write, not overwriting (may return success)
| member      | returns 1 (yes) or 0 (no) depending on presence in container
| push        | add to end
| pop         | get and remove from end
| register    | a get or set operation, returns the previous value , with ownership | transfer (caller must free)
| remove      | random access remove, always returns `void` (sic!)
| set         | a destructive insert (does not return previous value)
| update      | a destructive insert (returns the previous value, if ownership transfer permits it)

A register does not destroy a previously inserted value, an exchange does.


### Element Searches

Cache verbs are slightly different (see below)

| Verb        | Meaning
|-------------|---------------
| enumerate   | create enumerator
| find        | a search that is linear, returns an index (mulle_not_found_e) or key
| lookup      | a hashtable or similiar indexing (but not a get)
| search      | a search that's not linear, probably a binary search, returns the value
| count       | get number of elements (non char or byte), probably a "find"


| Verb/Object | Meaning
|-------------|---------------
| copy_items  | copy each item from source to destination struct [ dst, src]


### Misc. Operations on Caches and Containers with Caches

A cache is a non-complete subset of a larger backing store. The backing store
can be part of the data-structure or it can be separate.

| Verb        | Meaning
|-------------|-----------
| fill        | just a cache update with no previous probe or search
| find        | a search that is linear, returns an index (mulle_not_found_e) or key
| lookup      | a cache lookup probe followed possibly by a search and refresh if the lookup failed.
| probe       | a hashtable or similiar indexing (but not a get)
| refresh     | a search with a cache update (must not probe)
| search      | a search that's not linear, probably a binary search, returns the value


## Objects

The verb may be followed by an object, like for example in `get_length`, get
is the **verb** and **length** is the object. That this is called an "object"
is purely linguistically, you can also call it "attribute" or "field" or
"property".
There are a few standardized objects with specific meanings. Underscores
following an initial letter are fine, but should not be the last character.

| Object         | Meaning
|----------------|----------
| capacity       | the desired user capacity, not the actual size, which will be larger when allocated
| count          | a quantity of something other than bytes (usually pointers)
| length         | a quantity in bytes or characters
| notakey        | the value used for indicating an invalid key (often NULL)
| size           | the current maximum quantity (max count)
| size_as_length | as above but in bytes
| used           | the amount of the maximum quantity in use (like count, but there could be holes that make used larger than count)
| used_as_length | as above but in bytes

If there are multiple verb modifiers, append them in sorted order.


## Modifiers

After the object, or if no object is present after the verb, there can be a
number of modifiers. These are concatenated in sorted order.

| Verb        | Meaning
|-------------|----------
| inline      | A inlining variant of another non-inlining function
| nofail      | The function will never fail, like a search that is guaranteed
|             | to find something. The function may throw, abort or exit

## CMake Guidelines

This section outlines the structure and conventions for CMake in this
project. Following these guidelines ensures a stable and predictable build.

### 1. File and Directory Roles

#### `CMakeLists.txt` (Project Root)

* **Purpose:** Main entry point for the CMake build process.
* **Guidelines:** Edit for high-level project settings. Use `mulle-sde` to manage
  source files (e.g., `mulle-sde add src/foo.m`).

#### `cmake/reflect/`

* **Purpose:** Auto-generated files by `mulle-sde reflect` to map the sourcetree
  into CMake-readable lists.
* **Guidelines:** **Do not edit**. Changes will be overwritten by `mulle-sde reflect`.

#### `cmake/share/`

* **Purpose:** Contains standard shared CMake modules.
* **Guidelines:** **Do not edit**. Changes will be overwritten by `mulle-sde upgrade`.

#### `cmake/`

* **Purpose:** Project-specific customizations for the build system.
* **Guidelines:** Copy and edit files from `cmake/share/` to override default modules.

### 2. Summary of Editing Rules

| Path             | Editable? | Notes                                                        |
| ---------------- | --------- | ------------------------------------------------------------ |
| `CMakeLists.txt` | Yes       | High-level project settings.                                 |
| `cmake/`         | Yes       | Override modules by copying from `cmake/share/`.             |
| `cmake/share/`   | No        | Managed by `mulle-sde upgrade`. Changes will be lost.        |
| `cmake/reflect/` | No        | Auto-generated by `mulle-sde reflect`. Changes will be lost. |

## Coding Style Guide

This document outlines the coding conventions. All contributions must adhere to
these rules to maintain code consistency and readability.

### 1. Formatting

#### 1.1. Indentation
-   Use three (3) spaces for each level of indentation. Do not use tabs.

#### 1.2. Bracing
-   Use the Allman style for braces. The opening and closing braces for a block
    are placed on their own lines and are vertically aligned.

    ```c
    if( condition)
    {
       ...
    }
    ```

#### 1.3. Spacing
-   **Control Structures & Function Calls:** No space between a
    keyword/function name and the opening parenthesis. A space follows the
    opening parenthesis.
    -   `if( condition)`
    -   `while( i < n)`
    -   `[self doSomethingWith: arg]`
-   **Expressions:** No spaces immediately inside parentheses for grouping
    expressions.
    -   `x = (a + b) * c;`

#### 1.4. Columnar Alignment
-   Align similar elements vertically for readability, especially assignment
    operators.

    ```c
    char  *a;
    int   b;

    a = "foo";
    b = 1848;
    ```

#### 1.5. `return` Statements
-   Parenthesize the return expression: `return( expr);`.
-   Do not use parentheses when there is no return value: `return;`.

### 2. Declarations & Initialization

#### 2.1. Variable Declaration
-   Declare all local variables at the top of the function block (C89 style).
-   Sort variable declarations alphabetically by name.
-   Declare one variable per line, except for trivial counters
    (e.g., `int i, n;`).

#### 2.2. Initialization
-   Variables should not be initialized at declaration by default.
-   **Exceptions:**
    1.  When initializing from a function argument, often involving a cast.
    2.  When initializing an aggregate type (struct, array) to zero, which
        replaces a subsequent `memset(..., 0, ...)`. If an aggregate is
        initialized this way, the corresponding `memset` must be removed.

### 3. Control Flow

#### 3.1. `case` and `goto` Labels
-   `case` statements and `goto` labels are not indented relative to the
    block they are in. They align with the opening brace of the `switch` or
    function block.

    ```c
    switch( value)
    {
    case 0:
       ...
       break;

    default:
       ...
       break;
    }
    ```

#### 3.2. Single-Statement Blocks
-   Avoid single-line statements for blocks (e.g., `if(condition) statement;`).
-   Use braces for all blocks, except for single-line statements, when it has
    no continuation and its visual association with the control structure is
    unambiguous.

