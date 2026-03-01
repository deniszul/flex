#include <stdint.h>
#include <stdlib.h>

#include "flex.h"

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
