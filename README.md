# Flex String
A lightweight string library for C with a clear separation between:
- fstring → owned string (manages its own memory)
- fstring_view → non-owning slice (zero-copy)
- fstring_builder → dynamic string builder
- fstring_buffer → thread-safe string buffer (C11 + pthread)

Designed to be explicit, predictable, and safe from classic char* pitfalls.

---

## Features

- Immutable-style owned strings (`fstring`)
- Zero-copy substring support (`fstring_view`)
- Slice, trim, compare (case-sensitive & case-insensitive)
- Auto-growing string builder
- Thread-safe buffer (when compiled with C11)
- Ergonomics `_Generic` append API (C11)


---

## Installation
You have two ways to use flex.

### 1. Header-only (recommended)
Copy (`flex.h`)[./flex.h] into your project and define the implementation in one source file:
```c
#define FLEX_STRING_IMPLEMENTATION
#include "flex.h"
```
Then compile normally.

If you want thread-safe support (`fstring_buffer`), compile with:
```
-std=c11 -pthread
```

### 2. Traditional library
Copy both [`flex.h`](./flex.h) and [`flex.c`](./flex.c) into your project and compile them together:
```
cc -std=c11 -pthread your_program.c flex.c

```
The `-pthread` flag is only required if you use the thread-safe API (`fstring_buffer`).


---

## Core Types

`fstring`
An owned string.
```c
typedef struct {
    const uint8_t *data;
    size_t len;
} fstring;
```

Must be freed with:

```c
flex_str_free(&str);
```

---

`fstring_view`
A non-owning view into existing memory.
```c
typedef struct {
    const uint8_t *data;
    size_t len;
} fstring_view;
```

No allocation. No freeing required.
Ideal for parsing and fast slicing.

---

`fstring_builder`
A dynamic, growable string.
```c
typedef struct {
    uint8_t *data;
    size_t len;
    size_t capacity;
} fstring_builder;
```
Auto-resizes using:
```c
FLEX_SB_GROWTH_FACTOR = 2
```

---

`fstring_buffer` (C11)
Thread-safe string builder using `pthread_mutex_t`.

```c
typedef struct {
    uint8_t *data;
    size_t len;
    size_t capacity;
    pthread_mutex_t lock;
} fstring_buffer;
```

Available only when compiling with C11.

---

## Quick Start
### Creating Strings
From C string:
```c
fstring s = flex_str_from_cstr("Hello");
```

From view:

```c 
fstring_view sv = FLEX_SV_LIT("World");
fstring s = flex_str_from_sv(sv);
```

Free when done:
```c
flex_str_free(&s);
```

---

### Using String Views
Literal:
```c
fstring_view sv = FLEX_SV_LIT("hello");
```

Slice:
```c
fstring_view sub = flex_sv_slice(sv, 1, 4);

```

Trim:
```c
fstring_view trimmed = flex_sv_trim(sv);

```

Compare:
```c
flex_sv_equal(a, b);
flex_sv_equali(a, b); // insensitive-case

```

---

### String Builder
#### Initialization
```c
fstring_builder sb = flex_sb_new(0);
```

#### Append (C11 `_Generic`)

```c
flex_sb_append(&sb, "Hello ");
flex_sb_append(&sb, 42);
flex_sb_append(&sb, '!');
```
> `flex_sb_append(sb, x)` fallback to `flex_sb_append_cstr` if `_Generic` not supported.

Explicit versions:

```c
flex_sb_append_cstr(&sb, "Hello");
flex_sb_append_i32(&sb, 123);
```

Formatted:

```c
flex_sb_appendf(&sb, "Value: %d", 10);
```

#### Finalize

```c
fstring result = flex_sb_to_str(sb);
```

Free builder:

```c
flex_sb_free(&sb);
```

---

### Thread-Safe Buffer
If compiled with C11 or greater:

```c
fstring_buffer sbuf = flex_sbuf_new(0);
```

Append:

```c
flex_sbuf_append(&sbuf, "Thread safe");
```

Manual locking:

```c
flex_sbuf_lock(&sbuf);
flex_sbuf_append_locked(&sbuf, "data");
flex_sbuf_unlock(&sbuf);
```

Free:

```c
flex_sbuf_free(&sbuf);
```

---

## Error Handling
All append and reserve functions return:

Code	Meaning

```c
FLEX_EOK	Success
FLEX_E2BIG	Size too large
FLEX_EINVAL	Invalid argument
FLEX_ENOSPC	Allocation failure
```

---

## Useful Macros

Printf formatting:

```c
printf("%.*s\n", FLEX_STR_FMT(str));
```

Literal view:

```c
FLEX_SV_LIT("text")
```

Append literal:

```c
flex_sb_append_lit(&sb, "text");
```

--- 

## Limitations
ASCII-only case conversion
No UTF-8 awareness
Immutability is by convention, not enforced
If you need full Unicode handling or advanced text processing, this is intentionally not that.
