#ifndef INCLUDE_STRING_H_
#define INCLUDE_STRING_H_

#include "types.h"

void memcpy(uint8_t *dest, const uint8_t *src, uint32_t len);

void memset(void *dest, uint8_t val, uint32_t len);

void bzero(void *dest, uint32_t len);

int strcmp(const char *, const char *);

char *strcpy(char *dest, const char *src);

char *strcat(char *dest, const char *src);

size_t strlen(const char *);

#endif	// INCLUDE_STRING_H_