// core/kernel/lib/string.c
// Real string functions

#include "string.h"

int strcmp(const char* s1, const char* s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

char* strcpy(char* dest, const char* src) {
    char* d = dest;
    while (*src) {
        *d++ = *src++;
    }
    *d = '\0';
    return dest;
}

char* strtok(char* str, const char* delim) {
    static char* last = NULL;
    char* token = str ? str : last;
    
    if (token == NULL) return NULL;
    
    while (*token && strchr(delim, *token)) {
        token++;
    }
    
    if (*token == '\0') {
        last = NULL;
        return NULL;
    }
    
    char* start = token;
    while (*token && !strchr(delim, *token)) {
        token++;
    }
    
    if (*token) {
        *token = '\0';
        last = token + 1;
    } else {
        last = NULL;
    }
    
    return start;
}

char* strchr(const char* str, int c) {
    while (*str) {
        if (*str == c) return (char*)str;
        str++;
    }
    return NULL;
}

void* memcpy(void* dest, const void* src, size_t n) {
    char* d = (char*)dest;
    const char* s = (const char*)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

void* memset(void* ptr, int value, size_t num) {
    unsigned char* p = (unsigned char*)ptr;
    for (size_t i = 0; i < num; i++) {
        p[i] = (unsigned char)value;
    }
    return ptr;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}

int int_to_str(int num, char* str, int base) {
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return 1;
    }
    
    char digits[] = "0123456789";
    char temp[32];
    int i = 0;
    int is_negative = 0;
    
    if (num < 0 && base == 10) {
        is_negative = 1;
        num = -num;
    }
    
    while (num > 0) {
        temp[i++] = digits[num % base];
        num /= base;
    }
    
    if (is_negative) {
        temp[i++] = '-';
    }
    
    int j = 0;
    while (i > 0) {
        str[j++] = temp[--i];
    }
    str[j] = '\0';
    
    return j;
}
