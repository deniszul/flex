#ifndef FLEX_STRING_H
#define FLEX_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define FLEX_SB_INIT_CAPACITY 64

#define FLEX_SB_GROWTH_FACTOR 2

#define FLEX_STR_FMT(str) (int)(str).len, (str).data

enum {
    FLEX_EOK = 0,
    FLEX_E2BIG,
    FLEX_EINVAL,
    FLEX_ENOSPC,
};

typedef struct {
    const uint8_t *data;
    size_t len;
} fstring;

typedef struct {
    const uint8_t *data;
    size_t len;
} fstring_view;

fstring flex_str_from_sv(fstring_view sv);
fstring flex_str_from_cstr(const char *cstr);
void flex_str_free(fstring *str);
fstring_view flex_str_slice(fstring str, size_t start, size_t end);
fstring flex_str_toupper(fstring str);
fstring flex_str_tolower(fstring str);

#define FLEX_SV_EMPTY (fstring_view) { NULL, 0 }
#define FLEX_SV_LIT(cstr) (fstring_view) { (uint8_t*)(cstr), sizeof(cstr) - 1 }
#define FLEX_SV_STR(str) (fstring_view) { (str).data, (str).len }
#define FLEX_SV_BUF(data, len) (fstring_view) { (uint8_t*)(data), (len) }
fstring_view flex_sv_slice(fstring_view sv, size_t start, size_t end);
fstring_view flex_sv_ltrim(fstring_view sv);
fstring_view flex_sv_rtrim(fstring_view sv);
fstring_view flex_sv_trim(fstring_view sv);
fstring_view flex_sv_take_n(fstring_view sv, size_t n);
fstring_view flex_sv_take_while(fstring_view sv, bool (*predicate)(char x));
fstring_view flex_sv_take_until(fstring_view sv, char x);
fstring_view flex_sv_take_until_sv(fstring_view sv, fstring_view predicate);
fstring_view flex_sv_adv_n(fstring_view *sv, size_t n);
fstring_view flex_sv_adv_while(fstring_view *sv, bool (*predicate)(char x));
fstring_view flex_sv_adv_until(fstring_view *sv, char x);
fstring_view flex_sv_adv_until_sv(fstring_view *sv, fstring_view predicate);
bool flex_sv_starts_with(fstring_view sv, fstring_view prefix);
bool flex_sv_ends_with(fstring_view sv, fstring_view suffix);
bool flex_sv_equal(fstring_view lhs, fstring_view rhs);
bool flex_sv_equali(fstring_view lhs, fstring_view rhs);
int flex_sv_index_of(fstring_view sv, char x);


typedef struct {
    uint8_t *data;
    size_t len;
    size_t capacity;
} fstring_builder;

fstring_builder flex_sb_new(size_t capacity);
void flex_sb_free(fstring_builder *sb);
fstring flex_sb_to_str(fstring_builder sb);
int flex_sb_reserve(fstring_builder *sb, size_t size);
int flex_sb_appendf(fstring_builder *sb, const char *fmt, ...);
int flex_sb_append_chr(fstring_builder *sb, char);
int flex_sb_append_buf(fstring_builder *sb, const char *buf, size_t len);
int flex_sb_append_i32(fstring_builder *sb, int32_t);
int flex_sb_append_i64(fstring_builder *sb, int64_t);
int flex_sb_append_u32(fstring_builder *sb, uint32_t);
int flex_sb_append_u64(fstring_builder *sb, uint64_t);
int flex_sb_append_f32(fstring_builder *sb, float);
int flex_sb_append_f64(fstring_builder *sb, double);
int flex_sb_append_cstr(fstring_builder *sb, const char*);
#define flex_sb_appendfs(sb, fs) flex_sb_append_buf((sb), (const char*)fs.data, fs.len)
#define flex_sb_append_null(sb) flex_sb_append_chr((sb), '\0')
#define flex_sb_append_lit(sb, lit) flex_sb_append_buf((sb), lit, sizeof(lit) - 1)

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define flex_sb_append(sb, x) _Generic((x), \
    char: flex_sb_append_chr, \
    char*: flex_sb_append_cstr, \
    const char*: flex_sb_append_cstr, \
    int: flex_sb_append_i32, \
    int64_t: flex_sb_append_i64, \
    uint32_t: flex_sb_append_u32, \
    uint64_t: flex_sb_append_u64, \
    float: flex_sb_append_f32, \
    double: flex_sb_append_f64)((sb), x)

#else 
#define flex_sb_append(sb, x) flex_sb_append_cstr((sb), x)
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#include <pthread.h>

typedef struct {
    uint8_t *data;
    size_t len;
    size_t capacity;
    pthread_mutex_t lock;
} fstring_buffer;

fstring_buffer flex_sbuf_new(size_t capacity);
void flex_sbuf_free(fstring_buffer *sbuf);
fstring flex_sbuf_to_str(fstring_buffer sbuf);
int flex_sbuf_reserve(fstring_buffer *sbuf, size_t size);
int flex_sbuf_appendf(fstring_buffer *sbuf, const char *fmt, ...);
int flex_sbuf_append_chr(fstring_buffer *sbuf, char);
int flex_sbuf_append_buf(fstring_buffer *sbuf, const char *buf, size_t len);
int flex_sbuf_append_i32(fstring_buffer *sbuf, int32_t);
int flex_sbuf_append_i64(fstring_buffer *sbuf, int64_t);
int flex_sbuf_append_u32(fstring_buffer *sbuf, uint32_t);
int flex_sbuf_append_u64(fstring_buffer *sbuf, uint64_t);
int flex_sbuf_append_f32(fstring_buffer *sbuf, float);
int flex_sbuf_append_f64(fstring_buffer *sbuf, double);
int flex_sbuf_append_cstr(fstring_buffer *sbuf, const char*);
#define flex_sbuf_appendfs(sbuf, fs) flex_sbuf_append_buf((sbuf), (const char*)fs.data, fs.len)
#define flex_sbuf_append_null(sbuf) flex_sbuf_append_chr((sbuf), '\0')
#define flex_sbuf_append_lit(sbuf, lit) flex_sbuf_append_buf((sbuf), lit, sizeof(lit) - 1)
#define flex_sbuf_append(sbuf, x) _Generic((x), \
    char: flex_sbuf_append_chr, \
    char*: flex_sbuf_append_cstr, \
    const char*: flex_sbuf_append_cstr, \
    int: flex_sbuf_append_i32, \
    int64_t: flex_sbuf_append_i64, \
    uint32_t: flex_sbuf_append_u32, \
    uint64_t: flex_sbuf_append_u64, \
    float: flex_sbuf_append_f32, \
    double: flex_sbuf_append_f64)((sbuf), x)


#define flex_sbuf_lock(sbuf) pthread_mutex_lock(&(sbuf)->lock)
int flex_sbuf_reserve_locked(fstring_buffer *sbuf, size_t size);
int flex_sbuf_appendf_locked(fstring_buffer *sbuf, const char *fmt, ...);
int flex_sbuf_append_chr_locked(fstring_buffer *sbuf, char);
int flex_sbuf_append_buf_locked(fstring_buffer *sbuf, const char *buf, size_t len);
int flex_sbuf_append_i32_locked(fstring_buffer *sbuf, int32_t);
int flex_sbuf_append_i64_locked(fstring_buffer *sbuf, int64_t);
int flex_sbuf_append_u32_locked(fstring_buffer *sbuf, uint32_t);
int flex_sbuf_append_u64_locked(fstring_buffer *sbuf, uint64_t);
int flex_sbuf_append_f32_locked(fstring_buffer *sbuf, float);
int flex_sbuf_append_f64_locked(fstring_buffer *sbuf, double);
int flex_sbuf_append_cstr_locked(fstring_buffer *sbuf, const char*);
#define flex_sbuf_appendfs_locked(sbuf, fs) flex_sbuf_append_buf_locked((sbuf), (const char*)fs.data, fs.len)
#define flex_sbuf_append_null_locked(sbuf) flex_sbuf_append_chr_locked((sbuf), '\0')
#define flex_sbuf_append_lit_locked(sbuf, lit) flex_sbuf_append_buf_locked((sbuf), lit, sizeof(lit) - 1)
#define flex_sbuf_append_locked(sbuf, x) _Generic((x), \
    char: flex_sbuf_append_chr_locked, \
    char*: flex_sbuf_append_cstr_locked, \
    const char*: flex_sbuf_append_cstr_locked, \
    int: flex_sbuf_append_i32_locked, \
    int64_t: flex_sbuf_append_i64_locked, \
    uint32_t: flex_sbuf_append_u32_locked, \
    uint64_t: flex_sbuf_append_u64_locked, \
    float: flex_sbuf_append_f32_locked, \
    double: flex_sbuf_append_f64_locked)((sbuf), x)
#define flex_sbuf_unlock(sbuf) pthread_mutex_unlock(&(sbuf)->lock)
#endif // C11 or GREATER

#ifdef FLEX_STRING_IMPLEMENTATION
#include <stdint.h>
#include <stdlib.h>

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)

static inline int is_spaces(char ch) {
    return ch == '\t' || ch == '\r' || ch == '\n' || ch == ' ' || ch == '\v' || ch == '\f';
}

fstring flex_str_from_sv(fstring_view sv) {
    fstring fstr = { 0 };
    if (sv.len > 0) {
        void *data = malloc(sv.len);
        if (!data)
            return fstr;

        memcpy(data, sv.data, sv.len);
        fstr.data = data;
        fstr.len = sv.len;
    }

    return fstr;
}

fstring flex_str_from_cstr(const char *cstr) {
    fstring fstr = { 0 };
    size_t n = strlen(cstr);
    if (n != 0) {
        void *data = malloc(n);
        if (!data)
            return fstr;

        memcpy(data, cstr, n);
        fstr.data = data;
        fstr.len = n;
    }

    return fstr;
}

void flex_str_free(fstring *str) {
    free((void*)str->data);
    str->data = NULL;
    str->len = 0;
}

fstring_view flex_str_slice(fstring str, size_t start, size_t end) {
    end = MIN(end, str.len);
    start = MIN(start, end);

    return FLEX_SV_BUF(str.data + start, end - start);
}

fstring flex_str_toupper(fstring str) {
    fstring fstr = { 0 };
    uint8_t *new_data = malloc(str.len);
    if (!new_data)
        return fstr;

    memcpy(new_data, str.data, str.len);
    for (size_t i = 0; i < str.len; i++) {
        if ('a' <= new_data[i] && new_data[i] <= 'z')
            new_data[i] -= 32;
    }

    return (fstring) { new_data, str.len };
}

fstring flex_str_tolower(fstring str) {
    fstring fstr = { 0 };
    uint8_t *new_data = malloc(str.len);
    if (!new_data)
        return fstr;

    memcpy(new_data, str.data, str.len);
    for (size_t i = 0; i < str.len; i++) {
        if ('A' <= new_data[i] && new_data[i] <= 'Z')
            new_data[i] += 32;
    }

    return (fstring) { new_data, str.len };
}

fstring_view flex_sv_slice(fstring_view sv, size_t start, size_t end) {
    end = MIN(end, sv.len);
    start = MIN(start, end);

    return FLEX_SV_BUF(sv.data + start, end - start);
}

fstring_view flex_sv_ltrim(fstring_view sv) {
    if (sv.len == 0)
        return FLEX_SV_EMPTY;

    size_t i = 0;
    while (i < sv.len && is_spaces(sv.data[i]))
        i++;

    return FLEX_SV_BUF(sv.data + i, sv.len - i);
}

fstring_view flex_sv_rtrim(fstring_view sv) {
    if (sv.len == 0)
        return FLEX_SV_EMPTY;

    size_t i = 0;
    while (i < sv.len && is_spaces(sv.data[sv.len - 1 - i]))
        i++;

    return FLEX_SV_BUF(sv.data, sv.len - i);
}

fstring_view flex_sv_trim(fstring_view sv) {
    if (sv.len == 0)
        return FLEX_SV_EMPTY;

    return flex_sv_rtrim(flex_sv_ltrim(sv));
}

fstring_view flex_sv_take_n(fstring_view sv, size_t n) {
    n = MIN(n, sv.len);
    return FLEX_SV_BUF(sv.data, n);
}

fstring_view flex_sv_take_while(fstring_view sv, bool (*predicate)(char x)) {
    if (sv.len == 0)
        return FLEX_SV_EMPTY;

    size_t i = 0;
    while (i < sv.len && predicate(sv.data[i]))
        i++;

    return FLEX_SV_BUF(sv.data, i); // Not including th last char
}

fstring_view flex_sv_take_until(fstring_view sv, char x) {
    if (sv.len == 0)
        return FLEX_SV_EMPTY;

    size_t i = 0;
    while (i + 1 <= sv.len && sv.data[i] != x)
        i++;

    fstring_view res = FLEX_SV_EMPTY;
    if (i + 1 <= sv.len)
        res = FLEX_SV_BUF(sv.data, i + 1);

    return res;
}

fstring_view flex_sv_take_until_sv(fstring_view sv, fstring_view predicate) {
    if (sv.len == 0)
        return FLEX_SV_EMPTY;

    fstring_view window = FLEX_SV_BUF(sv.data, predicate.len);
    size_t i = 0;
    while (i + predicate.len <= sv.len && !flex_sv_equal(window, predicate))
        i++, window.data++;

    fstring_view res = FLEX_SV_EMPTY;
    if (i + predicate.len <= sv.len)
        res = FLEX_SV_BUF(sv.data, i + predicate.len);

    return res;
}

fstring_view flex_sv_adv_n(fstring_view *sv, size_t n) {
    n = MIN(n, sv->len);

    fstring_view res = FLEX_SV_BUF(sv->data, n);
    sv->data += n;
    sv->len -= n;

    return res;
}

fstring_view flex_sv_adv_while(fstring_view *sv, bool (*predicate)(char x)) {
    if (sv->len == 0)
        return FLEX_SV_EMPTY;

    size_t i = 0;
    while (i < sv->len && predicate(sv->data[i]))
        i++;

    fstring_view res = FLEX_SV_BUF(sv->data, i);
    sv->data += i;
    sv->len -= i;

    return res;
}

fstring_view flex_sv_adv_until(fstring_view *sv, char x) {
    if (sv->len == 0)
        return FLEX_SV_EMPTY;

    size_t i = 0;
    while (i + 1 <= sv->len && sv->data[i] != x)
        i++;

    fstring_view res = FLEX_SV_EMPTY;
    if (i + 1 <= sv->len) {
        res = FLEX_SV_BUF(sv->data, i + 1);
        sv->data += i + 1;
        sv->len -= i + 1;
    }

    return res;
}

fstring_view flex_sv_adv_until_sv(fstring_view *sv, fstring_view predicate) {
    if (sv->len == 0)
        return FLEX_SV_EMPTY;

    fstring_view window = FLEX_SV_BUF(sv->data, predicate.len);
    size_t i = 0;
    while (i + predicate.len <= sv->len && !flex_sv_equal(window, predicate))
        i++, window.data++;

    fstring_view res = FLEX_SV_EMPTY;
    if (i + predicate.len <= sv->len) {
        res = FLEX_SV_BUF(sv->data, i + predicate.len);
        sv->data += i + predicate.len;
        sv->len -= i + predicate.len;
    }

    return res;
}

bool flex_sv_starts_with(fstring_view sv, fstring_view prefix) {
    if (sv.len >= prefix.len) {
        fstring_view actual_prefix = FLEX_SV_BUF(sv.data, prefix.len);
        return flex_sv_equal(sv, actual_prefix);
    }

    return false;
}

bool flex_sv_ends_with(fstring_view sv, fstring_view suffix) {
    if (sv.len >= suffix.len) {
        fstring_view actual_suffix = FLEX_SV_BUF(sv.data + sv.len - suffix.len, suffix.len);
        return flex_sv_equal(suffix, actual_suffix);
    }

    return false;
}

bool flex_sv_equal(fstring_view lhs, fstring_view rhs) {
    if (!lhs.data || !rhs.data)
        return false;
    if (lhs.len != rhs.len)
        return false;
    if (lhs.len == 0)
        return true;

    return memcmp(lhs.data, rhs.data, lhs.len) == 0;
}

bool flex_sv_equali(fstring_view lhs, fstring_view rhs) {
    if (!lhs.data || !rhs.data)
        return false;
    if (lhs.len != rhs.len)
        return false;
    if (lhs.len == 0)
        return true;

    char a, b;
    for (size_t i = 0; i < lhs.len; i++) {
        uint8_t l = lhs.data[i];
        uint8_t r = rhs.data[i];
        a = 'A' <= l && l <= 'Z' ? l + 32 : l;
        b = 'A' <= r && r <= 'Z' ? r + 32 : r;

        if (a != b)
            return false;
    }

    return true;
}

int flex_sv_index_of(fstring_view sv, char x) {
    size_t i = 0;
    while (i < sv.len && sv.data[i] != x)
        i++;

    if (i < sv.len)
        return i;

    return -1;
}

static int flex_i32toa(int32_t N, char *restrict buf, size_t size) {
    uint32_t num;
    int is_negative = 0;

    if (N < 0) {
        is_negative = 1;
        num = (uint32_t)(-(N + 1)) + 1;
    } else {
        num = (uint32_t)N;
    }

    uint32_t tmp = num;
    int digits = 0;
    do {
        tmp /= 10;
        digits++;
    } while (tmp != 0);

    int total = digits + is_negative;

    if (size < (size_t)total || buf == NULL)
        return total;

    char *p = buf;

    if (is_negative)
        *p++ = '-';

    for (int i = digits - 1; i >= 0; --i) {
        p[i] = '0' + (num % 10);
        num /= 10;
    }

    return total;
}

static int flex_i64toa(int64_t N, char *restrict buf, size_t size) {
    uint64_t num;
    int is_negative = 0;

    if (N < 0) {
        is_negative = 1;
        num = (uint64_t)(-(N + 1)) + 1;
    } else {
        num = (uint64_t)N;
    }

    uint64_t tmp = num;
    int digits = 0;
    do {
        tmp /= 10;
        digits++;
    } while (tmp != 0);

    int total = digits + is_negative;

    if (size < (size_t)total || buf == NULL)
        return total;

    char *p = buf;

    if (is_negative)
        *p++ = '-';

    for (int i = digits - 1; i >= 0; --i) {
        p[i] = '0' + (num % 10);
        num /= 10;
    }

    return total;
}

static int flex_u32toa(uint32_t N, char *restrict buf, size_t size) {
    uint32_t num = N;

    uint32_t tmp = num;
    int digits = 0;
    do {
        tmp /= 10;
        digits++;
    } while (tmp != 0);

    if (size < (size_t)digits || buf == NULL)
        return digits;

    for (int i = digits - 1; i >= 0; --i) {
        buf[i] = '0' + (num % 10);
        num /= 10;
    }

    return digits;
}

static int flex_u64toa(uint64_t N, char *restrict buf, size_t size) {
    uint64_t num = N;

    uint64_t tmp = num;
    int digits = 0;
    do {
        tmp /= 10;
        digits++;
    } while (tmp != 0);

    if (size < (size_t)digits || buf == NULL)
        return digits;

    for (int i = digits - 1; i >= 0; --i) {
        buf[i] = '0' + (num % 10);
        num /= 10;
    }

    return digits;
}

static int flex_buf_reserve(uint8_t **data, size_t *capacity, size_t size) {
    if (!data || !capacity)
        return FLEX_EINVAL;

    if (*capacity < size) {
        if (*capacity == 0)
            *capacity = FLEX_SB_INIT_CAPACITY;

        size_t new_capacity = *capacity;
        while (new_capacity < size)
            new_capacity *= FLEX_SB_GROWTH_FACTOR;

        void *new_data = realloc(*data, new_capacity);
        if (!new_data)
            return FLEX_ENOSPC;

        *data = new_data;
        *capacity = new_capacity;
    }

    return FLEX_EOK;
}

static int flex_buf_vappendf(
    uint8_t **data, size_t *len, size_t *capacity,
    const char *restrict fmt, va_list args
) {
    va_list args_temp;
    va_copy(args_temp, args);

    int n = vsnprintf(NULL, 0, fmt, args_temp);
    va_end(args_temp);

    if (n < 0)
        return FLEX_EINVAL;

    if ((size_t)n > SIZE_MAX - *len - 1)
        return FLEX_E2BIG;

    size_t needed = n + *len + 1;
    if (needed > *capacity) {
        int retval = flex_buf_reserve(data,capacity, needed);
        if (retval != FLEX_EOK)
            return retval;
    }

    n = vsnprintf((char*)((*data) + *len), n + 1, fmt, args);
    if (n < 0)
        return FLEX_EINVAL;

    *len += n;
    return FLEX_EOK;
}

static int flex_buf_append_chr(uint8_t **data, size_t *len, size_t *capacity, char ch) {
    int retval = flex_buf_reserve(data, capacity, *len + 1);
    if (retval == FLEX_EOK)
        (*data)[(*len)++] = ch;

    return retval;
}

static int flex_buf_append_buf(uint8_t **data, size_t *len, size_t *capacity, const char *src_buf, size_t src_len) {
    if (src_len == 0)
        return FLEX_EOK;

    if (!data)
        return FLEX_EINVAL;

    int retval = flex_buf_reserve(data, capacity, *len + src_len);
    if (retval != FLEX_EOK)
        return retval;

    memcpy((*data) + *len, src_buf, src_len);
    *len += src_len;

    return retval;
}

static int flex_buf_append_i32(uint8_t **data, size_t *len, size_t *capacity, int32_t N) {
    enum { I32_BUF_MIN_SIZE = 11 }; // MAX int32_t (10) -> 10 + (minus sign) = 11
    char nbuf[I32_BUF_MIN_SIZE] = { 0 };
    int n = flex_i32toa(N, nbuf, sizeof(nbuf));
    if (n > 0)
        return flex_buf_append_buf(data, len, capacity, nbuf, n);

    return FLEX_EOK;
}

static int flex_buf_append_i64(uint8_t **data, size_t *len, size_t *capacity, int64_t N) {
    enum { I64_BUF_MIN_SIZE = 21 }; // MAX int64_t (20) + (minus sign) = 21
    char nbuf[I64_BUF_MIN_SIZE] = { 0 };
    int n = flex_i64toa(N, nbuf, sizeof(nbuf));
    if (n > 0)
        return flex_buf_append_buf(data, len, capacity, nbuf, n);

    return FLEX_EOK;
}

static int flex_buf_append_u32(uint8_t **data, size_t *len, size_t *capacity, uint32_t N) {
    enum { U32_BUF_MIN_SIZE = 10 }; // MAX int64_t (10) = 10
    char nbuf[U32_BUF_MIN_SIZE] = { 0 };
    int n = flex_u32toa(N, nbuf, sizeof(nbuf));
    if (n > 0)
        return flex_buf_append_buf(data, len, capacity, nbuf, n);

    return FLEX_EOK;
}

static int flex_buf_append_u64(uint8_t **data, size_t *len, size_t *capacity, uint64_t N) {
    enum { U64_BUF_MIN_SIZE = 20 }; // MAX int64_t (20) = 20
    char nbuf[U64_BUF_MIN_SIZE] = { 0 };
    int n = flex_u64toa(N, nbuf, sizeof(nbuf));
    if (n > 0)
        return flex_buf_append_buf(data, len, capacity, nbuf, n);

    return FLEX_EOK;
}

static int flex_buf_append_f32(uint8_t **data, size_t *len, size_t *capacity, float N) {
    enum {
        F32_PRECISION = 7,
        // sign + dot + e+-00 (4) + digits (7)
        F32_BUF_MIN_SIZE = 6 + F32_PRECISION
    };

    char nbuf[F32_BUF_MIN_SIZE] = { 0 };
    int n = snprintf(nbuf, sizeof(nbuf), "%.*g", F32_PRECISION, N);
    if (n > 0)
        return flex_buf_append_buf(data, len, capacity, nbuf, n);
    else if (n < 0)
        return FLEX_EINVAL;

    return FLEX_EOK;
}

static int flex_buf_append_f64(uint8_t **data, size_t *len, size_t *capacity, double N) {
    enum {
        F64_PRECISION = 15,
        // sign + dot + e(+-)000 (5) + digits (15)
        F64_BUF_MIN_SIZE = 7 + F64_PRECISION
    };

    char nbuf[F64_BUF_MIN_SIZE] = { 0 };
    int n = snprintf(nbuf, sizeof(nbuf), "%.*g", F64_PRECISION, N);
    if (n > 0)
        return flex_buf_append_buf(data, len, capacity, nbuf, n);
    else if (n < 0)
        return FLEX_EINVAL;

    return FLEX_EOK;
}

static int flex_buf_append_cstr(uint8_t **data, size_t *len, size_t *capacity, const char *restrict cstr) {
    size_t n = strlen(cstr);
    if (n > 0)
        return flex_buf_append_buf(data, len, capacity, cstr, n);

    return FLEX_EOK;
}

static fstring flex_buf_to_str(uint8_t *buf, size_t size) {
    fstring fstr = { 0 };
    if (size > 0) {
        uint8_t *data = malloc(size);
        if (!data)
            return fstr;

        memcpy(data, buf, size);
        fstr.data = data;
        fstr.len = size;
    }

    return fstr;
}

fstring_builder flex_sb_new(size_t capacity) {
    fstring_builder sb = { 0 };
    if (capacity > 0) {
        sb.data = malloc(capacity);
        sb.capacity = capacity;
    }

    return sb;
}

void flex_sb_free(fstring_builder *sb) {
    free(sb->data);
    sb->data = NULL;
    sb->len = 0;
    sb->capacity = 0;
}

fstring flex_sb_to_str(fstring_builder sb) {
    return flex_buf_to_str(sb.data, sb.len);
}

int flex_sb_reserve(fstring_builder *sb, size_t size) {
    return flex_buf_reserve(&sb->data, &sb->capacity, size);
}

int flex_sb_appendf(fstring_builder *sb, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int retval = flex_buf_vappendf(&sb->data, &sb->len, &sb->capacity, fmt, args);
    va_end(args);

    return retval;
}

int flex_sb_append_chr(fstring_builder *sb, char ch) {
    return flex_buf_append_chr(&sb->data, &sb->len, &sb->capacity, ch);
}

int flex_sb_append_buf(fstring_builder *sb, const char *buf, size_t len) {
    return flex_buf_append_buf(&sb->data, &sb->len, &sb->capacity, buf, len);
}

int flex_sb_append_i32(fstring_builder *sb, int32_t N) {
    return flex_buf_append_i32(&sb->data, &sb->len, &sb->capacity, N);
}

int flex_sb_append_i64(fstring_builder *sb, int64_t N) {
    return flex_buf_append_i64(&sb->data, &sb->len, &sb->capacity, N);
}

int flex_sb_append_u32(fstring_builder *sb, uint32_t N) {
    return flex_buf_append_u32(&sb->data, &sb->len, &sb->capacity, N);
}

int flex_sb_append_u64(fstring_builder *sb, uint64_t N) {
    return flex_buf_append_u64(&sb->data, &sb->len, &sb->capacity, N);
}

int flex_sb_append_f32(fstring_builder *sb, float N) {
    return flex_buf_append_f32(&sb->data, &sb->len, &sb->capacity, N);
}

int flex_sb_append_f64(fstring_builder *sb, double N) {
    return flex_buf_append_f64(&sb->data, &sb->len, &sb->capacity, N);
}

int flex_sb_append_cstr(fstring_builder *sb, const char *cstr) {
    return flex_buf_append_cstr(&sb->data, &sb->len, &sb->capacity, cstr);
}


#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
fstring_buffer flex_sbuf_new(size_t capacity) {
    fstring_buffer sbuf = { 0 };
    pthread_mutex_init(&sbuf.lock, NULL);
    if (capacity > 0) {
        sbuf.data = malloc(capacity);
        sbuf.capacity = capacity;
    }

    return sbuf;
}

void flex_sbuf_free(fstring_buffer *sbuf) {
    free(sbuf->data);
    sbuf->data = NULL;
    sbuf->len = 0;
    sbuf->capacity = 0;
    pthread_mutex_destroy(&sbuf->lock);
}

int flex_sbuf_reserve(fstring_buffer *sbuf, size_t size) {
    int retval = FLEX_EOK;
    if (size > sbuf->capacity) {
        pthread_mutex_lock(&sbuf->lock);
        retval = flex_buf_reserve(&sbuf->data, &sbuf->capacity, size);
        pthread_mutex_unlock(&sbuf->lock);
    }

    return retval;
}

fstring flex_sbuf_to_str(fstring_buffer sb) {
    return flex_buf_to_str(sb.data, sb.len);
}

int flex_sbuf_appendf(fstring_buffer *sbuf, const char *restrict fmt, ...) {
    va_list args;
    va_start(args, fmt);
    pthread_mutex_lock(&sbuf->lock);
    int retval = flex_buf_vappendf(&sbuf->data, &sbuf->len, &sbuf->capacity, fmt, args);
    pthread_mutex_unlock(&sbuf->lock);
    va_end(args);

    return retval;
}

int flex_sbuf_append_chr(fstring_buffer *sbuf, char ch) {
    pthread_mutex_lock(&sbuf->lock);
    int retval = flex_buf_append_chr(&sbuf->data, &sbuf->len, &sbuf->capacity, ch);
    pthread_mutex_unlock(&sbuf->lock);
    return retval;
}

int flex_sbuf_append_buf(fstring_buffer *sbuf, const char *restrict buf, size_t len) {
    pthread_mutex_lock(&sbuf->lock);
    int retval = flex_buf_append_buf(&sbuf->data, &sbuf->len, &sbuf->capacity, buf, len);
    pthread_mutex_unlock(&sbuf->lock);
    return retval;
}

int flex_sbuf_append_i32(fstring_buffer *sbuf, int32_t N) {
    pthread_mutex_lock(&sbuf->lock);
    int retval = flex_buf_append_i32(&sbuf->data, &sbuf->len, &sbuf->capacity, N);
    pthread_mutex_unlock(&sbuf->lock);

    return retval;
}

int flex_sbuf_append_i64(fstring_buffer *sbuf, int64_t N) {
    pthread_mutex_lock(&sbuf->lock);
    int retval = flex_buf_append_i64(&sbuf->data, &sbuf->len, &sbuf->capacity, N);
    pthread_mutex_unlock(&sbuf->lock);

    return retval;
}

int flex_sbuf_append_u32(fstring_buffer *sbuf, uint32_t N) {
    pthread_mutex_lock(&sbuf->lock);
    int retval = flex_buf_append_u32(&sbuf->data, &sbuf->len, &sbuf->capacity, N);
    pthread_mutex_unlock(&sbuf->lock);

    return retval;
}

int flex_sbuf_append_u64(fstring_buffer *sbuf, uint64_t N) {
    pthread_mutex_lock(&sbuf->lock);
    int retval = flex_buf_append_u64(&sbuf->data, &sbuf->len, &sbuf->capacity, N);
    pthread_mutex_unlock(&sbuf->lock);

    return retval;
}

int flex_sbuf_append_f32(fstring_buffer *sbuf, float N) {
    pthread_mutex_lock(&sbuf->lock);
    int retval = flex_buf_append_f32(&sbuf->data, &sbuf->len, &sbuf->capacity, N);
    pthread_mutex_unlock(&sbuf->lock);

    return retval;
}

int flex_sbuf_append_f64(fstring_buffer *sbuf, double N) {
    pthread_mutex_lock(&sbuf->lock);
    int retval = flex_buf_append_f64(&sbuf->data, &sbuf->len, &sbuf->capacity, N);
    pthread_mutex_unlock(&sbuf->lock);

    return retval;
}

int flex_sbuf_append_cstr(fstring_buffer *sbuf, const char *restrict cstr) {
    pthread_mutex_lock(&sbuf->lock);
    int retval = flex_buf_append_cstr(&sbuf->data, &sbuf->len, &sbuf->capacity, cstr);
    pthread_mutex_unlock(&sbuf->lock);

    return retval;
}

int flex_sbuf_reserve_locked(fstring_buffer *sbuf, size_t size) {
    return flex_buf_reserve(&sbuf->data, &sbuf->capacity, size);
}

int flex_sbuf_appendf_locked(fstring_buffer *sbuf, const char *restrict fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int retval = flex_buf_vappendf(&sbuf->data, &sbuf->len, &sbuf->capacity, fmt, args);
    va_end(args);

    return retval;
}

int flex_sbuf_append_chr_locked(fstring_buffer *sbuf, char ch) {
    return flex_buf_append_chr(&sbuf->data, &sbuf->len, &sbuf->capacity, ch);
}

int flex_sbuf_append_buf_locked(fstring_buffer *sbuf, const char *restrict buf, size_t len) {
    return flex_buf_append_buf(&sbuf->data, &sbuf->len, &sbuf->capacity, buf, len);
}

int flex_sbuf_append_i32_locked(fstring_buffer *sbuf, int32_t N) {
    return flex_buf_append_i32(&sbuf->data, &sbuf->len, &sbuf->capacity, N);
}

int flex_sbuf_append_i64_locked(fstring_buffer *sbuf, int64_t N) {
    return flex_buf_append_i64(&sbuf->data, &sbuf->len, &sbuf->capacity, N);
}

int flex_sbuf_append_u32_locked(fstring_buffer *sbuf, uint32_t N) {
    return flex_buf_append_u32(&sbuf->data, &sbuf->len, &sbuf->capacity, N);
}

int flex_sbuf_append_u64_locked(fstring_buffer *sbuf, uint64_t N) {
    return flex_buf_append_u64(&sbuf->data, &sbuf->len, &sbuf->capacity, N);
}

int flex_sbuf_append_f32_locked(fstring_buffer *sbuf, float N) {
    return flex_buf_append_f32(&sbuf->data, &sbuf->len, &sbuf->capacity, N);
}

int flex_sbuf_append_f64_locked(fstring_buffer *sbuf, double N) {
    return flex_buf_append_f64(&sbuf->data, &sbuf->len, &sbuf->capacity, N);
}

int flex_sbuf_append_cstr_locked(fstring_buffer *sbuf, const char *restrict cstr) {
    return flex_buf_append_cstr(&sbuf->data, &sbuf->len, &sbuf->capacity, cstr);
}
#endif // C11 or GREATER

#endif

#ifdef __cplusplus
}
#endif

#endif // FLEX_STRING_H
