#include <string.h>

inline void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len)
{
    for (; len != 0; --len)
        *dest ++ = *src ++;
}

inline void memset(void *dest, uint8_t val, uint32_t len)
{
    uint8_t *dst = (uint8_t *)dest;
    
    for (; len != 0; --len)
        *dst ++ = val;
}

inline void bzero(void *dest, uint32_t len)
{
    memset(dest, 0, len);
}

inline size_t strlen(const char *src)
{
    size_t size = 0;
    while (*src ++)
        size ++;
    
    return size;
}

int (strcmp)(const char *s1, const char *s2)
{
    for (; *s1 == *s2; ++s1, ++s2)
        if (*s1 == '\0')
            return (0);
    return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1); 
}