#ifndef FLEX_STRING_H
#define FLEX_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define FLEX_STR_FMT(str) (int)(str).len, (str).data

typedef struct {
    const uint8_t *data;
    size_t len;
} fstring;

fstring flex_str_from_cstr(const char *cstr);
void flex_str_free(fstring *str);
fstring flex_str_toupper(fstring str);
fstring flex_str_tolower(fstring str);


#ifdef __cplusplus
}
#endif

#endif // FLEX_STRING_H
