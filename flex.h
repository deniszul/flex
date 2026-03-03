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


#ifdef __cplusplus
}
#endif

#endif // FLEX_STRING_H
