#pragma once

#include <stdint.h>

#ifndef NULL
#define NULL 0
#endif

typedef int8_t				s8;
typedef uint8_t				u8;
typedef int16_t				s16;
typedef uint16_t			u16;
typedef int32_t				s32;
typedef uint32_t			u32;
typedef int64_t				s64;
typedef uint64_t			u64;
typedef float				f32;
typedef double				f64;
typedef volatile int8_t		vs8;
typedef volatile uint8_t	vu8;
typedef volatile int16_t	vs16;
typedef volatile uint16_t	vu16;
typedef volatile int32_t	vs32;
typedef volatile uint32_t	vu32;
typedef volatile int64_t	vs64;
typedef volatile uint64_t	vu64;
typedef volatile float		vf32;
typedef volatile double		vf64;

typedef struct opd64 {
	void* 	func;
	void* 	rtoc;
	u64 	zero;
} opd64;

typedef struct opd32 {
	u32 func;
	u32 rtoc;
} opd32;

extern opd64 opd64_start[];
extern opd32 opd32_start[];
#define OPD32(ptr) (opd32_start + ((opd64*)ptr - opd64_start))

typedef u32 mem_container_t;

// 32bit types
typedef u32 lv2_void;
typedef u32 lv2_size_t;

typedef s64 system_time_t;

struct timeval_32 {
	u32 tv_sec;
	s32 tv_usec;
};


#ifdef __cplusplus
#define EXTERN_BEGIN	extern "C" {
#define EXTERN_END		}
#else
#define EXTERN_BEGIN
#define EXTERN_END
#endif
