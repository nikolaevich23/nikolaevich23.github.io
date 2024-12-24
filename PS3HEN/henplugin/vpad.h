#ifdef VIRTUAL_PAD
#define VIRTUAL_PAD

// /pad.ps3?off           turns off virtual pad
// /pad.ps3?<buttons>     press the specified buttons
// /pad.ps3?hold<buttons> keeps the specified buttons pressed
// /pad.ps3?release       release all the buttons

// /pad.ps3?cross=enter   Sets X as enter, O as cancel  (PS3 is rebooted)
// /pad.ps3?cross=swap    Swaps the behavior of X and O (PS3 is rebooted)
// /pad.ps3?circle=enter  Sets O as enter, X as cancel  (PS3 is rebooted)
// /pad.ps3?circle=swap   Swaps the behavior of X and O (PS3 is rebooted)

#include <cell/pad/libpad_dbg.h>

#define SC_PAD_SET_DATA_INSERT_MODE		(573)
#define SC_PAD_REGISTER_CONTROLLER		(574)

static u32 vcombo = 0;

static s32 vpad_handle = NONE;

static inline void sys_pad_dbg_ldd_register_controller(u8 *data, s32 *handle, u8 addr, u32 capability);
static inline void sys_pad_dbg_ldd_set_data_insert_mode(s32 handle, u16 addr, u32 *mode, u8 addr2);
static s32 register_ldd_controller(void);
static s32 unregister_ldd_controller(void);
static void press_cancel_button(int do_enter);
static void press_accept_button(void);
static u8 parse_pad_command(const char *pad_cmds, u8 is_combo);

#endif // #ifdef VIRTUAL_PAD
