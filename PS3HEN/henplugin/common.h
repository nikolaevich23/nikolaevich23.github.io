#ifndef __COMMON_H__
#define __COMMON_H__

#include "typew.h"
#include <cell/pad/libpad_dbg.h>

#define SC_PAD_SET_DATA_INSERT_MODE		(573)
#define SC_PAD_REGISTER_CONTROLLER		(574)

#define MAP_BASE (0x80000000007FAE00ULL)		// lv2 memory area to store path redirections table
#define	MAP_ADDR (0xE8)							// address in lv2 to store MAP_BASE address (0x0000 ---> 0xFFFF)

#ifdef DEBUG

#define DPRINTF		printf

#else

#define DPRINTF(...)

#endif

//static int HEN_check (void);
char *strcasestr(const char *s1, const char *s2);
static s64 val(const char *c);
static inline void sys_pad_dbg_ldd_register_controller(u8 *data, s32 *handle, u8 addr, u32 capability);
static inline void sys_pad_dbg_ldd_set_data_insert_mode(s32 handle, u16 addr, u32 *mode, u8 addr2);
static s32 register_ldd_controller(void);
static s32 unregister_ldd_controller(void);
static void press_cancel_button(int do_enter);
static void press_accept_button(void);
static u8 parse_pad_command(const char *pad_cmds, u8 is_combo);
static int number_users(void);
int filecopy(const char *src, const char *dst, const char *chk);

#endif /* __COMMON_H__ */

