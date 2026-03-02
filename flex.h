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


#ifdef __cplusplus
}
#endif

#endif // FLEX_STRING_H
