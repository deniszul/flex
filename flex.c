#include <stdint.h>
#include <stdlib.h>

#include "flex.h"

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
