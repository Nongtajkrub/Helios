#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t  i8;

typedef float f32;

typedef unsigned int uint;

const u16 PTR_SIZE = sizeof(void*);

typedef void (func)(void*);
typedef bool (funcb)(void*);
