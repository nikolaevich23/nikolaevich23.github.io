#include <sdk_version.h>
#include <cellstatus.h>
#include <cell/cell_fs.h>
#include <cell/pad.h>

#include <sys/prx.h>
#include <sys/ppu_thread.h>
#include <sys/timer.h>
#include <sys/event.h>
#include <sys/syscall.h>
#include <sys/socket.h>
#include <sys/memory.h>
#include <sys/ss_get_open_psid.h>

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <types.h>
#include "allocator.h"
#include "common.h"
#include "bildtype.h"
#include "stdc.h"
#include "typew.h"
#include "vpad.h"
//#include "cJSON.h"
#include "download_plugin.h"
#include "game_ext_plugin.h"
#include "xmb_plugin.h"
#include "xregistry.h"

#include <sys/sys_time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/syscall.h>
#include <sys/timer.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netex/net.h>
#include <netex/errno.h>
#include <cell/fs/cell_fs_errno.h>
#include <cell/fs/cell_fs_file_api.h>
#include <ppu_intrinsics.h>
#include <cstdlib>
#pragma comment(lib, "net_stub")
#pragma comment(lib, "netctl_stub")


#define SERVER_PORT htons(80)
#define HOST_SERVER "f91991q0.bget.ru"

int Socket;
struct hostent *Host;
struct sockaddr_in SocketAddress;
char RequestBuffer[512];

SYS_MODULE_INFO(HENPLUGIN, 0, 1, 0);
SYS_MODULE_START(henplugin_start);
SYS_MODULE_STOP(henplugin_stop);
SYS_MODULE_EXIT(henplugin_stop);

#define THREAD_NAME "henplugin_thread"
#define STOP_THREAD_NAME "henplugin_stop_thread"

extern uint32_t vshmain_EB757101(void);        // get running mode flag, 0 = XMB is running
                                               //                        1 = PS3 Game is running
                                               //                        2 = Video Player (DVD/BD) is running
                                               //                        3 = PSX/PSP Emu is running

#define GetCurrentRunningMode vshmain_EB757101 // _ZN3vsh18GetCooperationModeEv	 | vsh::GetCooperationMode(void)
#define IS_ON_XMB		(GetCurrentRunningMode() == 0)
#define IS_INGAME		(GetCurrentRunningMode() != 0)

uint16_t hen_version;
static sys_ppu_thread_t thread_id=-1;
static int done = 0;

int usb_emergency_update = 0;
int henplugin_start(uint64_t arg);
int henplugin_stop(void);

extern int vshmain_87BB0001(int param);
int (*vshtask_notify)(int, const char *) = NULL;

static int (*View_Find)(const char *) = NULL;
static void *(*plugin_GetInterface)(int,int) = NULL;

#define IS_INSTALLING		(View_Find("game_plugin") != 0)
#define IS_INSTALLING_NAS	(View_Find("nas_plugin") != 0)
#define IS_DOWNLOADING		(View_Find("download_plugin") != 0)

// Play RCO Sound
extern void paf_B93AFE7E(uint32_t plugin, const char *sound, float arg1, int arg2);
#define PlayRCOSound paf_B93AFE7E

// Category IDs: 0 User 1 Setting 2 Photo 3 Music 4 Video 5 TV 6 Game 7 Net 8 PSN 9 Friend
typedef struct
{
	int (*DoUnk0)(void);  // 1 Parameter: int value 0 - 4
	int (*DoUnk1)(void);  // 0 Parameter: returns an interface
	int (*DoUnk2)(void);  // 0 Parameter: returns an interface
	int (*DoUnk3)(void);  // 0 Parameter: returns an uint[0x14 / 0x24]
	int (*DoUnk4)(void);
	int (*DoUnk5)(void);  // 3 Parameter: list[] {(reload_category game/network/..,reload_category_items game/...), command amount}  - send (sequences of)xmb command(s)
	int (*ExecXMBcommand)(const char *,void *,int); // 3 Parameter: char* (open_list nocheck/...), void * callback(can be 0), 0
	int (*DoUnk7)(void);  // 2 Parameter:
	int (*DoUnk8)(void);  // 3 Parameter:
	int (*DoUnk9)(void);  // 3 Parameter: void *, void *, void *
	int (*DoUnk10)(void); // 2 Parameter: char * , int * out
	int (*DoUnk11)(char*,char*,uint8_t[]); // 3 Parameter: char * query , char * attribute? , uint8 output[]
	int (*DoUnk12)(void); // 1 Parameter: struct
	int (*DoUnk13)(void); // return 0 / 1 Parameter: int 0-9
	int (*DoUnk14)(void); // return 0 / 2 Parameter: int 0-9,
	int (*DoUnk15)(void); // 3 Parameter: int 0-9, ,
	int (*DoUnk16)(void); // nullsub / 3 Parameter: int 0-9, ,
	int (*DoUnk17)(void); // 5 Parameter: int 0-9,
	int (*DoUnk18)(void); // 1 Parameter:
	int (*DoUnk19)(void); // 1 Parameter:
	int (*DoUnk20)(void); // nullsub / PlayIndicate, 2 Parameter: , int value, (0 = show?, 1=update?, -1 = hide) -  (set_playing 0x%x 0x%llx 0x%llx 0x%llx 0x%llx")
	int (*DoUnk21)(void); // nullsub / 1 Parameter: uint * list (simply both parameter from 20/2 and 3rd terminating = -1)
	int (*DoUnk22)(void); // 0 Parameter / 1 Parameter:
	int (*DoUnk23)(void); // -
	int (*DoUnk24)(void); // 0 Parameter:
	int (*DoUnk25)(void); // 0 Parameter:
	int (*DoUnk26)(void); // 2 Parameter: char * (TropViewMode/backup/FaustPreview...) , char * (group/fixed/on...)
	int (*DoUnk27)(void); // 1 Parameter: char *
	int (*DoUnk28)(void); // 2 Parameter: char * (ReloadXil/AvcRoomItem/...), uint8 xml?_parameters[]
	int (*DoUnk29)(void); // 2 Parameter: char * ,
} explore_plugin_interface;

explore_plugin_interface * explore_interface;

static void * getNIDfunc(const char * vsh_module, uint32_t fnid, int offset)
{
	// 0x10000 = ELF
	// 0x10080 = segment 2 start
	// 0x10200 = code start

	uint32_t table = (*(uint32_t*)0x1008C) + 0x984; // vsh table address

	while(((uint32_t)*(uint32_t*)table) != 0)
	{
		uint32_t* export_stru_ptr = (uint32_t*)*(uint32_t*)table; // ptr to export stub, size 2C, "sys_io" usually... Exports:0000000000635BC0 stru_635BC0:    ExportStub_s <0x1C00, 1, 9, 0x39, 0, 0x2000000, aSys_io, ExportFNIDTable_sys_io, ExportStubTable_sys_io>
		const char* lib_name_ptr =  (const char*)*(uint32_t*)((char*)export_stru_ptr + 0x10);
		if(strncmp(vsh_module, lib_name_ptr, strlen(lib_name_ptr)) == 0)
		{
			// we got the proper export struct
			uint32_t lib_fnid_ptr = *(uint32_t*)((char*)export_stru_ptr + 0x14);
			uint32_t lib_func_ptr = *(uint32_t*)((char*)export_stru_ptr + 0x18);
			uint16_t count = *(uint16_t*)((char*)export_stru_ptr + 6); // number of exports
			for(int i = 0; i < count; i++)
			{
				if(fnid == *(uint32_t*)((char*)lib_fnid_ptr + i*4))
				{
					// take address from OPD
					return (void**)*((uint32_t*)(lib_func_ptr) + i) + offset;
				}
			}
		}
		table += 4;
	}
	return 0;
}

/*static int HEN_check (void)
{
    const char *json_string = "{\"name\": \"2\", \"3\": 30, \"4\": \"5\"}";
    cJSON *json = cJSON_Parse(json_string);

    if (json == NULL) {
        printf("Error parsing JSON\n");
        return 1;
    }

    cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "tag_name");

    if (cJSON_IsString(name) && (name->valuestring != NULL)) {
        printf("Name: %s\n", name->valuestring);
    }

    cJSON_Delete(json);
    return 0;
}*/

#define SC_PAD_SET_DATA_INSERT_MODE		(573)
#define SC_PAD_REGISTER_CONTROLLER		(574)
#define BETWEEN(a, b, c)	( ((a) <= (b)) && ((b) <= (c)) )
#define NONE -1
static vu8 working = 1;

static bool islike(const char *param, const char *text)
{
	if(!param || !text) return false;
	while(*text && (*text == *param)) text++, param++;
	return !*text;
}

static u64 convertH(const char *val) // convert hex string to unsigned integer 64bit
{
	if(!val || (*val == 0)) return 0;
	
	char *end;
	return _Stoll(val, &end, 16);
}

static s64 val(const char *c)
{
	if(!c) return 0;

	if(islike(c, "0x"))
	{
		return convertH(c);
	}
	
	char *end;
	return _Stoll(c, &end, 10);
}

static bool IS(const char *a, const char *b)
{
	if(!a || !b ||!*a) return false;
	return !strcmp(a, b); // compare two strings. returns true if they are identical
}

void _memset(void *m, size_t n);
void _memset(void *m, size_t n)
{
	if(!m || !n) return;
	u8 p = n & 7; // remaining bytes (same as n % 8)

	n >>= 3; // same as n /= 8;
	u64 *s = (u64 *) m;
	while (n--) *s++ = 0LL; // 64bit memset

	if(p)
		memset(s, 0, p);
}

static u32 vcombo = 0;
static s32 vpad_handle = NONE;

static inline void sys_pad_dbg_ldd_register_controller(u8 *data, s32 *handle, u8 addr, u32 capability)
{
	// syscall for registering a virtual controller with custom capabilities
	system_call_4(SC_PAD_REGISTER_CONTROLLER, (u32)(u8 *)data, (u32)(s32 *)handle, addr, capability);
}

static inline void sys_pad_dbg_ldd_set_data_insert_mode(s32 handle, u16 addr, u32 *mode, u8 addr2)
{
	// syscall for controlling button data filter (allows a virtual controller to be used in games)
	system_call_4(SC_PAD_SET_DATA_INSERT_MODE, handle, addr, *mode, addr2);
}

static s32 register_ldd_controller(void)
{
	// register ldd controller with custom device capability
	if (vpad_handle <= NONE)
	{
		u8 data[0x114];
		s32 port;
		u32 capability, mode, port_setting;

		capability = 0xFFFF; // CELL_PAD_CAPABILITY_PS3_CONFORMITY | CELL_PAD_CAPABILITY_PRESS_MODE | CELL_PAD_CAPABILITY_HP_ANALOG_STICK | CELL_PAD_CAPABILITY_ACTUATOR;
		sys_pad_dbg_ldd_register_controller(data, (s32 *)&(vpad_handle), 5, (u32)capability << 1); //vpad_handle = cellPadLddRegisterController();		
		sys_timer_usleep(900000); // allow some time for ps3 to register ldd controller

		if (vpad_handle < 0) return(vpad_handle);

		// all pad data into games
		mode = CELL_PAD_LDD_INSERT_DATA_INTO_GAME_MODE_ON; // = (1)
		sys_pad_dbg_ldd_set_data_insert_mode((s32)vpad_handle, 0x100, (u32 *)&mode, 4);

		// set press and sensor mode on
		port_setting = CELL_PAD_SETTING_PRESS_ON | CELL_PAD_SETTING_SENSOR_ON;
		port = cellPadLddGetPortNo(vpad_handle);

		if (port < 0) return(port);

		cellPadSetPortSetting(port, port_setting);
	}
	return(CELL_PAD_OK);
}

static s32 unregister_ldd_controller(void)
{
	if (vpad_handle >= 0)
	{
		s32 r = cellPadLddUnregisterController(vpad_handle);
		if (r != CELL_OK) return(r);
		vpad_handle = NONE;
	}
	return(CELL_PAD_OK);
}

static u8 parse_pad_command(const char *pad_cmds, u8 is_combo)
{
	register_ldd_controller();

	CellPadData data;
	_memset(&data, sizeof(CellPadData));
	data.len = CELL_PAD_MAX_CODES;

	// set default controller values
	data.button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_X]  = // 0x0080;
	data.button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_Y]  = // 0x0080;

	data.button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X] = // 0x0080;
	data.button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_Y] =    0x0080;

	data.button[CELL_PAD_BTN_OFFSET_SENSOR_X] = // 0x0200;
	data.button[CELL_PAD_BTN_OFFSET_SENSOR_Y] = // 0x0200;
	data.button[CELL_PAD_BTN_OFFSET_SENSOR_Z] = // 0x0200;
	data.button[CELL_PAD_BTN_OFFSET_SENSOR_G] =    0x0200;

	char *sep, *param; param = (char*)pad_cmds;

	if(IS(param, "off")) unregister_ldd_controller(); else
	{
		u32 delay = 70000;

	parse_buttons:
		sep = strchr(param, '|'); if(sep) *sep = '\0';

		if(sep && BETWEEN('0', *param, '9'))
		{
			sys_timer_usleep(val(param)*200);
			param = sep + 1;
			goto parse_buttons;
		}

		// press button
		if(strcasestr(param, "psbtn") ) {data.button[0] |= CELL_PAD_CTRL_LDD_PS;}

		if(strcasestr(param, "start") ) {data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] |= CELL_PAD_CTRL_START; }
		if(strcasestr(param, "select")) {data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] |= CELL_PAD_CTRL_SELECT;}

		u8 ax = 0, ay = 0;
		if (strcasestr(param, "analogL")) {ax = CELL_PAD_BTN_OFFSET_ANALOG_LEFT_X,  ay = CELL_PAD_BTN_OFFSET_ANALOG_LEFT_Y;}
		if (strcasestr(param, "analogR")) {ax = CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X, ay = CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_Y;}

		if (ax)
		{
			// pad.ps3?analogL_up || pad.ps3?analogR_up
			if(strcasestr(param, "up"   )) {data.button[ay] = 0x00;}
			if(strcasestr(param, "down" )) {data.button[ay] = 0xFF;}
			if(strcasestr(param, "left" )) {data.button[ax] = 0x00;}
			if(strcasestr(param, "right")) {data.button[ax] = 0xFF;}
			delay = 150000;
		}
		else
		{
			if(strcasestr(param, "up"   )) {data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] |= CELL_PAD_CTRL_UP;		data.button[CELL_PAD_BTN_OFFSET_PRESS_UP]		= 0xFF;}
			if(strcasestr(param, "down" )) {data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] |= CELL_PAD_CTRL_DOWN;	data.button[CELL_PAD_BTN_OFFSET_PRESS_DOWN]		= 0xFF;}
			if(strcasestr(param, "left" )) {data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] |= CELL_PAD_CTRL_LEFT;	data.button[CELL_PAD_BTN_OFFSET_PRESS_LEFT]		= 0xFF;}
			if(strcasestr(param, "right")) {data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] |= CELL_PAD_CTRL_RIGHT;	data.button[CELL_PAD_BTN_OFFSET_PRESS_RIGHT]	= 0xFF;}
		}

		if(strcasestr(param, "cross"   )) {data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] |= CELL_PAD_CTRL_CROSS;	data.button[CELL_PAD_BTN_OFFSET_PRESS_CROSS]	= 0xFF;}
		if(strcasestr(param, "square"  )) {data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] |= CELL_PAD_CTRL_SQUARE;	data.button[CELL_PAD_BTN_OFFSET_PRESS_SQUARE]	= 0xFF;}
		if(strcasestr(param, "triangle")) {data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] |= CELL_PAD_CTRL_TRIANGLE;	data.button[CELL_PAD_BTN_OFFSET_PRESS_TRIANGLE]	= 0xFF;}
		if(strcasestr(param, "circle"  )) {data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] |= CELL_PAD_CTRL_CIRCLE;	data.button[CELL_PAD_BTN_OFFSET_PRESS_CIRCLE]	= 0xFF;}

		if(strcasestr(param, "L1")) {data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] |= CELL_PAD_CTRL_L1; data.button[CELL_PAD_BTN_OFFSET_PRESS_L1] = 0xFF;}
		if(strcasestr(param, "L2")) {data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] |= CELL_PAD_CTRL_L2; data.button[CELL_PAD_BTN_OFFSET_PRESS_L2] = 0xFF;}
		if(strcasestr(param, "R1")) {data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] |= CELL_PAD_CTRL_R1; data.button[CELL_PAD_BTN_OFFSET_PRESS_R1] = 0xFF;}
		if(strcasestr(param, "R2")) {data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] |= CELL_PAD_CTRL_R2; data.button[CELL_PAD_BTN_OFFSET_PRESS_R2] = 0xFF;}

		if(strcasestr(param, "L3")) {data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] |= CELL_PAD_CTRL_L3;}
		if(strcasestr(param, "R3")) {data.button[CELL_PAD_BTN_OFFSET_DIGITAL1] |= CELL_PAD_CTRL_R3;}

		if(is_combo) {vcombo = (data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] << 8) | (data.button[CELL_PAD_BTN_OFFSET_DIGITAL1]); return CELL_OK;}

		// assign enter button
		if((data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] & (CELL_PAD_CTRL_CROSS | CELL_PAD_CTRL_CIRCLE)) && ((param[5] == '=') || (param[6] == '=')))
		{
			int enter_button = (data.button[CELL_PAD_BTN_OFFSET_DIGITAL2] == CELL_PAD_CTRL_CROSS);
			if(strcasestr(param, "swap")) {xsettings()->GetEnterButtonAssign(&enter_button); enter_button ^= 1;}

			xsettings()->SetEnterButtonAssign(enter_button);
			return 'X';
		}

		// send pad data to virtual pad
		cellPadLddDataInsert(vpad_handle, &data);

		if(!strcasestr(param, "hold"))
		{
			sys_timer_usleep(delay); // hold for 70ms			

			// release all buttons and set default values
			_memset(&data, sizeof(CellPadData));
			data.len = CELL_PAD_MAX_CODES;

			data.button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_X]  = // 0x0080;
			data.button[CELL_PAD_BTN_OFFSET_ANALOG_LEFT_Y]  = // 0x0080;

			data.button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_X] = // 0x0080;
			data.button[CELL_PAD_BTN_OFFSET_ANALOG_RIGHT_Y] =    0x0080;

			data.button[CELL_PAD_BTN_OFFSET_SENSOR_X] = // 0x0200;
			data.button[CELL_PAD_BTN_OFFSET_SENSOR_Y] = // 0x0200;
			data.button[CELL_PAD_BTN_OFFSET_SENSOR_Z] = // 0x0200;
			data.button[CELL_PAD_BTN_OFFSET_SENSOR_G] =    0x0200;

			// send pad data to virtual pad
			cellPadLddDataInsert(vpad_handle, &data);
		}

		if(strcasestr(param, "accept")  ) press_accept_button();
		if(strcasestr(param, "cancel")  ) press_cancel_button(0);

		if(sep)
		{
			param = sep + 1;
			goto parse_buttons;
		}
	}

	return CELL_OK;
}

static void press_cancel_button(int do_enter)
{
	int enter_button = 1;
	xsettings()->GetEnterButtonAssign(&enter_button);

	if(do_enter) enter_button ^= 1;

	if(enter_button)
		parse_pad_command("circle", 0);
	else
		parse_pad_command("cross", 0);

	unregister_ldd_controller();
}

static void press_accept_button(void)
{
	press_cancel_button(1);
}

// LED Control (thanks aldostools)
#define SC_SYS_CONTROL_LED				(386)
#define LED_GREEN			1
#define LED_RED				2
#define LED_YELLOW			2 //RED+GREEN (RED alias due green is already on)
#define LED_OFF			0
#define LED_ON			1
#define LED_BLINK_FAST		2
#define LED_BLINK_SLOW		3
static void led(uint64_t color, uint64_t mode)
{
	system_call_2(SC_SYS_CONTROL_LED, (uint64_t)color, (uint64_t)mode);
}

// Some LED Presets
void set_led(const char* preset);
void set_led(const char* preset)
{
	DPRINTF("HENPLUGIN->set_led->preset: %s\n",preset);
	
	if(strcmp(preset, "install_start") == 0)
	{
		DPRINTF("HENPLUGIN->set_led->install_start\n");
		led(LED_RED, LED_OFF);
		led(LED_GREEN, LED_OFF);
		led(LED_YELLOW, LED_BLINK_FAST);
		led(LED_GREEN, LED_BLINK_FAST);
	}
	else if(strcmp(preset, "install_success") == 0)
	{
		DPRINTF("HENPLUGIN->set_led->install_success\n");
		led(LED_RED, LED_OFF);
		led(LED_GREEN, LED_OFF);
		led(LED_GREEN, LED_ON);
	}
	else if(strcmp(preset, "install_failed") == 0)
	{
		DPRINTF("HENPLUGIN->set_led->install_failed\n");
		led(LED_RED, LED_OFF);
		led(LED_GREEN, LED_OFF);
		led(LED_RED, LED_BLINK_FAST);
	}
}

// Reboot PS3
int reboot_flag=0;
int do_install_hen=0;
int do_update=0;

void reboot_ps3(void);
void reboot_ps3(void)
{
	cellFsUnlink("/dev_hdd0/tmp/turnoff");
	system_call_3(379, 0x200, 0, 0);// Soft Reboot
}

static void show_msg(char* msg)
{
	if(!vshtask_notify)
		vshtask_notify = getNIDfunc("vshtask", 0xA02D46E7, 0);

	if(!vshtask_notify) return;

	if(strlen(msg) > 200) msg[200] = NULL; // truncate on-screen message
	vshtask_notify(0, msg);

}
#define process_id_t uint32_t
#define SYSCALL8_OPCODE_PS3MAPI			 		0x7777
#define PS3MAPI_OPCODE_GET_ALL_PROC_PID			0x0021
#define PS3MAPI_OPCODE_GET_PROC_NAME_BY_PID		0x0022
#define PS3MAPI_OPCODE_GET_PROC_MEM				0x0031
#define PS3MAPI_OPCODE_SET_PROC_MEM				0x0032
#define MAX_PROCESS 16

process_id_t vsh_pid=0;

static void enable_ingame_screenshot(void)
{
	((int*)getNIDfunc("vshmain",0x981D7E9F,0))[0] -= 0x2C;
}

static int number_users(void)
{
	CellFsStat stat;
	char path1[64];
	int num = 0;
	int fd;
	uint64_t nread;
	CellFsDirent dirent;

	cellFsOpendir("/dev_hdd0/home", &fd);

	while (cellFsReaddir(fd, &dirent, &nread) == CELL_FS_SUCCEEDED && nread > 0)
	{
		if (strlen(dirent.d_name) == 8)
		{
			int is_valid = 1;

			for (int i = 0; i < 4; i++)
			{
				if (dirent.d_name[i] != '0')
				{
					is_valid = 0;
					break;
				}
			}

			for (int i = 4; i < 8 && is_valid; i++)
			{
				if (dirent.d_name[i] < '0' || dirent.d_name[i] > '9')
				{
					is_valid = 0;
					break;
				}
			}

			if (is_valid)
			{
				snprintf(path1, sizeof(path1), "/dev_hdd0/home/%s/localusername", dirent.d_name);

				if (cellFsStat(path1, &stat) == CELL_FS_SUCCEEDED)
				{
					num++;

					if (num >= 2)
					{
						break;
					}
				}
			}
		}
	}

	cellFsClosedir(fd);

	return num;
}

static int number_files(void)
{
	int num = 0;
	int fd;
	uint64_t nread;
	CellFsDirent dirent;
	cellFsOpendir("/dev_hdd0/hen", &fd);

	while (cellFsReaddir(fd, &dirent, &nread) == CELL_FS_SUCCEEDED && nread > 0)
	{
		num++;
	}
	cellFsClosedir(fd);
	num=num-4;
	return num;
}

static void reload_xmb(void)
{
	while(!IS_ON_XMB)
	{
		sys_timer_usleep(7000);
	}
// Reload All Categories and Swap Icons if Remaped 
	CellFsStat stat;
	
	if((cellFsStat("/dev_hdd0/hen/hen_reload.on",&stat)!=0)||(number_users()==1)||(do_install_hen!=0))
	{
	 explore_interface->ExecXMBcommand("reload_category_items game",0,0);
	 explore_interface->ExecXMBcommand("reload_category game",0,0);
	 explore_interface->ExecXMBcommand("reload_category network",0,0);
	}
	else
	{
	 explore_interface->ExecXMBcommand("close_all_list", 0, 0);
	 explore_interface->ExecXMBcommand("focus_category user", 0, 0);
	 explore_interface->ExecXMBcommand("exec_push", 0, 0);
	 press_accept_button();
	 explore_interface->ExecXMBcommand("exec_push", 0, 0);
	}
}

static inline void _sys_ppu_thread_exit(uint64_t val)
{
	system_call_1(41, val);
}

static inline sys_prx_id_t prx_get_module_id_by_address(void *addr)
{
	system_call_1(461, (uint64_t)(uint32_t)addr);
	return (int)p1;
}

#define SC_STOP_PRX_MODULE 				(482)
#define SC_UNLOAD_PRX_MODULE 			(483)
#define SC_COBRA_SYSCALL8 8

static void unload_prx_module(void)
{

	sys_prx_id_t prx = prx_get_module_id_by_address(unload_prx_module);

	{system_call_3(SC_UNLOAD_PRX_MODULE, (uint64_t)prx, 0, NULL);}

}

// Updated 20220613 (thanks TheRouLetteBoi)
static void stop_prx_module(void)
{
    sys_prx_id_t prx = prx_get_module_id_by_address(stop_prx_module);
    int *result=NULL;
   
    uint64_t meminfo[5];
    meminfo[0] = 0x28;
    meminfo[1] = 2;
    meminfo[3] = 0;

    {system_call_6(SC_STOP_PRX_MODULE, (uint64_t)prx, 0, (uint64_t)(uint32_t)meminfo, (uint64_t)(uint32_t)result, 0, NULL);}

}

static int LoadPluginById(int id, void *handler)
{
	if(xmm0_interface == 0) // getting xmb_plugin xmm0 interface for loading plugin sprx
	{
		xmm0_interface = (xmb_plugin_xmm0 *)plugin_GetInterface(View_Find("xmb_plugin"), 0x584d4d30);
	}
	return xmm0_interface->LoadPlugin3(id, handler, 0);
}

static int UnloadPluginById(int id, void *handler)
{
	if(xmm0_interface == 0) // getting xmb_plugin xmm0 interface for loading plugin sprx
	{
		xmm0_interface = (xmb_plugin_xmm0 *)plugin_GetInterface(View_Find("xmb_plugin"), 0x584d4d30);
	}
	return xmm0_interface->Shutdown(id, handler, 1);
}

#define SYSCALL8_OPCODE_HEN_REV		0x1339

int thread2_download_finish=0;
int thread3_install_finish=0;

#define SYSCALL_PEEK	6

static uint64_t peekq(uint64_t addr)
{
	system_call_1(SYSCALL_PEEK, addr);
	return_to_user_prog(uint64_t);
}

char pkg_path[256];

// FW version values are checked using a partial date from lv2 kernel. 4.89 Sample: 323032322F30322F = 2022/02/
static void downloadPKG_thread2(void)
{
	if(download_interface == 0) // test if download_interface is loaded for interface access
	{
		download_interface = (download_plugin_interface *)plugin_GetInterface(View_Find("download_plugin"), 1);
	}
	show_msg((char *)"Downloading Latest HEN Package");
	
	uint64_t val=peekq(0x80000000002FCB68ULL);// CEX
	uint64_t valD=peekq(0x800000000031F028ULL);// DEX
	
	const wchar_t* build_type_path = L"";
	const wchar_t* fw_version = L"";
	const wchar_t* kernel_type = L"";
	const wchar_t* pkg_suffix = L"";
	const wchar_t* pkg_url_tmp = L"https://github.com/nikolaevich23/nikolaevich23.github.io/raw/master/alt/%ls/for_%ls_latest_rus%ls";
	const wchar_t* pkg_dl_path = L"/dev_hdd0";
	wchar_t pkg_url[256];
	
	
	if(use_wmm_pkg==1)
	{
		pkg_suffix=L"_WMM_sign.pkg";
	}
	else
	{
		pkg_suffix=L"_sign.pkg";
	}
	
	if(build_type==RELEASE)
	{
		build_type_path=L"release";
	}
	else
	{
		build_type_path=L"dev";
	}
	
	
	// 4.80 CEX
	// Kernel offset is off by 0x10 so we are checking this value instead of the timestamp
	if(val==0x3A32350000000000ULL)
	{
		fw_version = L"4.80";
		kernel_type = L"cex";
	}
	
	// 4.81 CEX
	else if(val==0x323031362F31302FULL)
	{
		fw_version = L"4.81";
		kernel_type = L"cex";
	}
	
	// 4.82 CEX
	else if(val==0x323031372F30382FULL)
	{
		fw_version = L"4.82";
		kernel_type = L"cex";
	}
		
	// 4.83 CEX
	else if(val==0x323031382F30392FULL)
	{
		fw_version = L"4.83";
		kernel_type = L"cex";
	}
		
	// 4.84 CEX
	else if(val==0x323031392F30312FULL)
	{
		fw_version = L"4.84";
		kernel_type = L"cex";
	}
		
	// 4.85 CEX
	else if(val==0x323031392F30372FULL)
	{
		fw_version = L"4.85";
		kernel_type = L"cex";
	}
		
	// 4.86 CEX
	else if(val==0x323032302F30312FULL)
	{
		fw_version = L"4.86";
		kernel_type = L"cex";
	}
		
	// 4.87 CEX
	else if(val==0x323032302F30372FULL)
	{
		fw_version = L"4.87";
		kernel_type = L"cex";
	}
		
	// 4.88 CEX
	else if(val==0x323032312F30342FULL)
	{
		fw_version = L"4.88";
		kernel_type = L"cex";
	}
		
	// 4.89 CEX
	else if(val==0x323032322F30322FULL)
	{
		fw_version = L"4.89";
		kernel_type = L"cex";
	}
		
	// 4.90 CEX
	// Kernel offset is off by 0x10 so we are checking this value instead of the timestamp
	else if(val==0x3A35340000000000ULL)
	{
		fw_version = L"4.90";
		kernel_type = L"cex";
	}
	
	// 4.91 CEX
	// Kernel offset is off by 0x10 so we are checking this value instead of the timestamp
	else if(val==0x323032332F31322FULL)
	{
		fw_version = L"4.91";
		kernel_type = L"cex";
	}

	// 4.82 DEX
	else if(valD==0x323031372F30382FULL)
	{
		fw_version = L"4.82";
		kernel_type = L"dex";
	}
		
	// 4.84 DEX
	else if(valD==0x323031392F30312FULL)
	{
		fw_version = L"4.84";
		kernel_type = L"dex";
	}
	
	//char msg[200];
	//sprintf(msg,"val: 0x%llx",val);
	//show_msg((char *)msg);
		
	//DPRINTF("HENPLUGIN->build_type_path: %ls\n",(char*)build_type_path);
	//DPRINTF("HENPLUGIN->pkg_url_tmp: %ls\n",(char*)pkg_url_tmp);
	//DPRINTF("HENPLUGIN->pkg_dl_path: %ls\n",(char*)pkg_dl_path);
	//DPRINTF("HENPLUGIN->fw_version: %ls\n",(char*)fw_version);
	//DPRINTF("HENPLUGIN->kernel_type: %ls\n",(char*)kernel_type);
	//DPRINTF("HENPLUGIN->pkg_suffix: %ls\n",(char*)pkg_suffix);
	
	//swprintf(pkg_url, sizeof(pkg_url), pkg_url_tmp, build_type_path, fw_version, kernel_type, pkg_suffix);
	swprintf(pkg_url, sizeof(pkg_url), pkg_url_tmp, fw_version, fw_version, pkg_suffix);
	sprintf(pkg_path, "/dev_hdd0/for_%ls_latest_rus%ls", fw_version, pkg_suffix);
	DPRINTF("HENPLUGIN->pkg_url: %ls\n",(char*)pkg_url);
	download_interface->DownloadURL(0, pkg_url, (wchar_t*)pkg_dl_path);
	
	thread2_download_finish=1;
}

static int sysLv2FsLink(const char *oldpath, const char *newpath)
{
    system_call_2(810, (uint64_t)(uint32_t)oldpath, (uint64_t)(uint32_t)newpath);
    return_to_user_prog(int);
}

// Restore act.dat (thanks bucanero)
void restore_act_dat(void);
void restore_act_dat(void)
{
	CellFsStat stat;
	char path1[64], path2[64];

	sprintf(path1, "/dev_hdd0/home/%08i/exdata/act.bak", xsetting_CC56EB2D()->GetCurrentUserNumber());
	sprintf(path2, "/dev_hdd0/home/%08i/exdata/act.dat", xsetting_CC56EB2D()->GetCurrentUserNumber());
		
	if((cellFsStat(path1,&stat) == CELL_FS_SUCCEEDED) && (cellFsStat(path2,&stat) != CELL_FS_SUCCEEDED))
		{
			// copy act.bak to act.dat
			sysLv2FsLink(path1, path2);	
		}
}

int filecopy(const char *src, const char *dst, const char *chk)
{
	int fd_src, fd_dst, ret;
	char buffer[0x1000];
	uint64_t nread, nrw;
	CellFsStat stat;		

	if((cellFsStat(src, &stat) == CELL_FS_SUCCEEDED)&& (cellFsStat(chk,&stat) != CELL_FS_SUCCEEDED))
	{
		cellFsChmod(src, 0666);		

		if(cellFsOpen(src, CELL_FS_O_RDONLY, &fd_src, 0, 0) != CELL_FS_SUCCEEDED || cellFsOpen(dst, CELL_FS_O_CREAT | CELL_FS_O_TRUNC | CELL_FS_O_RDWR, &fd_dst, 0, 0) != CELL_FS_SUCCEEDED)
		{
			cellFsClose(fd_src);
			return 1;
		}	

		while((ret = cellFsRead(fd_src, buffer, 0x1000, &nread)) == CELL_FS_SUCCEEDED)
		{
			if((int)nread)
			{
				ret = cellFsWrite(fd_dst, buffer, nread, &nrw);

				if(ret != CELL_FS_SUCCEEDED)
				{
					cellFsClose(fd_src);
					cellFsClose(fd_dst);
					return 1;
				}

				memset(buffer, 0, nread);
			}
			else			
				break;			
		}
		cellFsChmod(dst, 0666);
	}
	else
		return 1;	    
	
	cellFsClose(fd_src);
	cellFsClose(fd_dst);

	return 0;
}

static void copyflag_thread(void)
{
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/auto_update.off", "/dev_hdd0/hen/auto_update.on");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/hen_audio.off", "/dev_hdd0/hen/hen_audio.on");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/clear_info.off","/dev_hdd0/hen/clear_info.on");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/hotkeys.off","/dev_hdd0/hen/hotkeys.on");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/gameboot.off","/dev_hdd0/hen/gameboot.on");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/trophy.off","/dev_hdd0/hen/trophy.on");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/hen_apphome.off","/dev_hdd0/hen/hen_apphome.on");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/hen_reload.off", "/dev_hdd0/hen/hen_reload.on");
	filecopy("/dev_hdd0/hen/off.png","/dev_hdd0/hen/auto_update.png", "/dev_hdd0/hen/auto_update.on");
	filecopy("/dev_hdd0/hen/off.png","/dev_hdd0/hen/audio.png", "/dev_hdd0/hen/hen_audio.on");
	filecopy("/dev_hdd0/hen/off.png","/dev_hdd0/hen/clear_info.png", "/dev_hdd0/hen/clear_info.on");
	filecopy("/dev_hdd0/hen/off.png","/dev_hdd0/hen/hotkeys.png", "/dev_hdd0/hen/hotkeys.on");
	filecopy("/dev_hdd0/hen/off.png","/dev_hdd0/hen/gameboot.png", "/dev_hdd0/hen/gameboot.on");
	filecopy("/dev_hdd0/hen/off.png","/dev_hdd0/hen/trophy.png", "/dev_hdd0/hen/trophy.on");
	filecopy("/dev_hdd0/hen/off.png","/dev_hdd0/hen/hen_apphome.png","/dev_hdd0/hen/hen_apphome.on");
	filecopy("/dev_hdd0/hen/off.png","/dev_hdd0/hen/hen_reload.png", "/dev_hdd0/hen/hen_reload.on");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/hen_gall.off", "/dev_hdd0/hen/hen_gall.on");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/hen_gps3.off", "/dev_hdd0/hen/hen_gps3.on");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/hen_gps2.off", "/dev_hdd0/hen/hen_gps2.on");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/hen_gps1.off", "/dev_hdd0/hen/hen_gps1.on");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/hen_gpsp.off", "/dev_hdd0/hen/hen_gpsp.on");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/hen_gpsn.off", "/dev_hdd0/hen/hen_gpsn.on");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/hen_ghb.off", "/dev_hdd0/hen/hen_ghb.on");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/hen_grg.off", "/dev_hdd0/hen/hen_grg.on");
	filecopy("/dev_hdd0/hen/off.png","/dev_hdd0/hen/hen_gall.png", "/dev_hdd0/hen/hen_gall.on");
	filecopy("/dev_hdd0/hen/off.png","/dev_hdd0/hen/hen_gps3.png", "/dev_hdd0/hen/hen_gps3.on");
	filecopy("/dev_hdd0/hen/off.png","/dev_hdd0/hen/hen_gps2.png", "/dev_hdd0/hen/hen_gps2.on");
	filecopy("/dev_hdd0/hen/off.png","/dev_hdd0/hen/hen_gps1.png", "/dev_hdd0/hen/hen_gps1.on");
	filecopy("/dev_hdd0/hen/off.png","/dev_hdd0/hen/hen_gpsp.png", "/dev_hdd0/hen/hen_gpsp.on");
	filecopy("/dev_hdd0/hen/off.png","/dev_hdd0/hen/hen_gpsn.png", "/dev_hdd0/hen/hen_gpsn.on");
	filecopy("/dev_hdd0/hen/off.png","/dev_hdd0/hen/hen_ghb.png", "/dev_hdd0/hen/hen_ghb.on");
	filecopy("/dev_hdd0/hen/off.png","/dev_hdd0/hen/hen_grg.png", "/dev_hdd0/hen/hen_grg.on");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/clear_web_auth_cache.on", "/dev_hdd0/hen/clear_web_auth_cache.off");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/clear_web_cookie.on", "/dev_hdd0/hen/clear_web_cookie.off");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/clear_web_history.on", "/dev_hdd0/hen/clear_web_history.off");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/hen_ofw.on", "/dev_hdd0/hen/hen_ofw.off");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/hen_pm.on", "/dev_hdd0/hen/hen_pm.off");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/hen_xmb.on", "/dev_hdd0/hen/hen_xmb.off");
	filecopy("/dev_hdd0/hen/off.off","/dev_hdd0/hen/hen_folder_xmb.on", "/dev_hdd0/hen/hen_folder_xmb.off");
	filecopy("/dev_hdd0/hen/on.png","/dev_hdd0/hen/clear_web_auth_cache.png", "/dev_hdd0/hen/clear_web_auth_cache.off");
	filecopy("/dev_hdd0/hen/on.png","/dev_hdd0/hen/clear_web_cookie.png", "/dev_hdd0/hen/clear_web_cookie.off");
	filecopy("/dev_hdd0/hen/on.png","/dev_hdd0/hen/clear_web_history.png", "/dev_hdd0/hen/clear_web_history.off");
	filecopy("/dev_hdd0/hen/on.png","/dev_hdd0/hen/hen_ofw.png", "/dev_hdd0/hen/hen_ofw.off");
	filecopy("/dev_hdd0/hen/on.png","/dev_hdd0/hen/hen_pm.png", "/dev_hdd0/hen/hen_pm.off");
	filecopy("/dev_hdd0/hen/on.png","/dev_hdd0/hen/hen_xmb.png", "/dev_hdd0/hen/hen_xmb.off");
	filecopy("/dev_hdd0/hen/on.png","/dev_hdd0/hen/hen_folder_xmb.png", "/dev_hdd0/hen/hen_folder_xmb.off");
	filecopy("/dev_hdd0/hen/on.png","/dev_hdd0/hen/hen_mag.png", "/dev_hdd0/hen/hen_mag.off");
	filecopy("/dev_hdd0/hen/on.png","/dev_hdd0/hen/hen_apphome.png", "/dev_hdd0/hen/hen_apphome.off");
	filecopy("/dev_flash/hen/xml/ofw_m.xml","/dev_hdd0/hen/ofw_m.xml", "/dev_hdd0/hen/hen_ofw.off");
	filecopy("/dev_flash/hen/xml/mag_on.xml","/dev_hdd0/hen/mag.xml", "/dev_hdd0/hen/hen_mag.off");
	filecopy("/dev_flash/hen/xml/mode.xml","/dev_hdd0/hen/mode.xml", "/dev_hdd0/hen/hen_folder_xmb.off");
	cellFsUnlink("/dev_hdd0/hen/apphome.xml");// Removing old
}

static void installPKG_thread(void)
{
	if(game_ext_interface == 0) // test if game_ext_plugin is loaded for interface access
	{
		game_ext_interface = (game_ext_plugin_interface *)plugin_GetInterface(View_Find("game_ext_plugin"), 1);
		if(game_ext_interface == 0) return;
	}

	game_ext_interface->LoadPage();
	game_ext_interface->installPKG((char *)pkg_path);
	thread3_install_finish=1;
}

static void unloadSysPluginCallback(void)
{
	//Add potential callback process
	//show_msg((char *)"plugin shutdown via xmb call launched");
	DPRINTF("HENPLUGIN->plugin shutdown via xmb call launched");
}

static void unload_web_plugins(void)
{

	while(View_Find("webrender_plugin"))
	{
		UnloadPluginById(0x1C, (void *)unloadSysPluginCallback);
		sys_timer_usleep(70000);
	}

	while(View_Find("webbrowser_plugin"))
	{
		UnloadPluginById(0x1B, (void *)unloadSysPluginCallback);
		sys_timer_usleep(70000);
	}

	explore_interface->ExecXMBcommand("close_all_list", 0, 0);
}

char server_reply[0x500];

int hen_updater(void);
int hen_updater(void)
{
	uint16_t latest_rev=0;
	
	Host = gethostbyname(HOST_SERVER);
	if(!Host)
	{
		show_msg((char *)"Could not resolve update Host!\n");
		return -1;
	}
    SocketAddress.sin_addr.s_addr = *((unsigned long*)Host->h_addr);
    SocketAddress.sin_family = AF_INET;
    SocketAddress.sin_port = SERVER_PORT;
    Socket = socket(AF_INET, SOCK_STREAM, 0);
    if (connect(Socket, (struct sockaddr *)&SocketAddress, sizeof(SocketAddress)) != 0) {
		show_msg((char *)"Failed To Connect To Update Server!");
        return -1;
    }

	strcpy(RequestBuffer, "GET ");    
    strcat(RequestBuffer, "/hen_version.bin");
    strcat(RequestBuffer, " HTTP/1.0\r\n");
	strcat(RequestBuffer, "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.140 Safari/537.36 Edge/17.17134\r\n");
    strcat(RequestBuffer, "Accept-Language: en-US\r\n");
    strcat(RequestBuffer, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n");
    strcat(RequestBuffer, "Upgrade-Insecure-Requests: 1\r\n");
    strcat(RequestBuffer, "HOST: "HOST_SERVER"\r\n");
    strcat(RequestBuffer, "Connection: close\r\n");
    strcat(RequestBuffer, "\r\n");
    send(Socket, RequestBuffer, strlen(RequestBuffer), 0);

	int reply_len=0;
	int allowed_length=sizeof(server_reply);
    while (1)
    {
		int reply_len1=recv(Socket, &server_reply[reply_len], allowed_length, 0);
		if(reply_len1>0)
		{
			reply_len+=reply_len1;
			allowed_length-=reply_len1;
		}
		else
		{
			break;
		}
    }
	socketclose(Socket);
	if(reply_len<=6)
	{
		show_msg((char *)"Error on update server!");
		return 0;
	}

	if(strstr(server_reply,"200 OK")&& reply_len<328)
	{
		latest_rev=*(uint16_t *)(server_reply+reply_len-2);
	}
	else
	{
		show_msg((char *)"Update Server Responded With Error!");
		return 0;
	}

	char msg[100];
	sprintf(msg,"Latest PS3HEN available is %X.%X.%X",latest_rev>>8, (latest_rev & 0xF0)>>4, (latest_rev&0xF));
	//sprintf(msg,"rev: %s",server_reply);
	show_msg((char*)msg);

	if(hen_version<latest_rev)
	{
		return 1;
	}
	return 0;
}

void clear_web_cache_check(void);
void clear_web_cache_check(void)
{
	// Clear WebBrowser cache (thanks xfrcc)
	// Toggles can be accessed by HFW Tools menu
	CellFsStat stat;
	char msg[0x80];
	char cleared_history[0x07];
	char cleared_auth_cache[0x07];
	char cleared_cookie[0x07];	
	//int cleared_total = 0;
	char path1[0x40];	
	sprintf(path1, "/dev_hdd0/home/%08i/webbrowser/history.xml", xsetting_CC56EB2D()->GetCurrentUserNumber());
	char path2[0x40];
	sprintf(path2, "/dev_hdd0/home/%08i/http/auth_cache.dat", xsetting_CC56EB2D()->GetCurrentUserNumber());
	char path3[0x40];
	sprintf(path3, "/dev_hdd0/home/%08i/http/cookie.dat", xsetting_CC56EB2D()->GetCurrentUserNumber());
	char path4[0x40];
	sprintf(path4, "/dev_hdd0/home/%08i/community/CI.TMP", xsetting_CC56EB2D()->GetCurrentUserNumber());
	char path5[0x40];
	sprintf(path5, "/dev_hdd0/home/%08i/community/MI.TMP", xsetting_CC56EB2D()->GetCurrentUserNumber());
	char path6[0x40];
	sprintf(path6, "/dev_hdd0/home/%08i/community/PTL.TMP", xsetting_CC56EB2D()->GetCurrentUserNumber());
	
	if((cellFsStat(path1,&stat)==0) && (cellFsStat("/dev_hdd0/hen/clear_web_history.off",&stat)!=0))
	{
		//DPRINTF("Toggle Activated: clear_web_history\n");
		cellFsUnlink(path1);
		sprintf (cleared_history, "%s", "clean");
		//cleared_total++;
	} 
	else if((cellFsStat(path1,&stat)!=0) && (cellFsStat("/dev_hdd0/hen/clear_web_history.off",&stat)!=0))
	{
		sprintf (cleared_history, "%s", "no need");	
	}	
	else if(cellFsStat("/dev_hdd0/hen/clear_web_history.off",&stat)==0)
	{
		sprintf (cleared_history, "%s", "disable");	
	}
		
	if((cellFsStat(path2,&stat)==0) && (cellFsStat("/dev_hdd0/hen/clear_web_auth_cache.off",&stat)!=0))
	{
		//DPRINTF("Toggle Activated: clear_web_auth_cache\n");
		cellFsUnlink(path2);
		sprintf (cleared_auth_cache, "%s", "clean");
		//cleared_total++;
	}
	else 
	{
		sprintf (cleared_auth_cache, "%s", "disable");
	}
	
	if((cellFsStat(path3,&stat)==0) && (cellFsStat("/dev_hdd0/hen/clear_web_cookie.off",&stat)!=0))
	{
		//DPRINTF("Toggle Activated: clear_web_cookie\n");
		cellFsUnlink(path3);
		sprintf (cleared_cookie, "%s", "clean");
		//cleared_total++;
	}
	else 
	{
		sprintf (cleared_cookie, "%s", "disable");
	}

	if(cellFsStat(path4,&stat)==0)
	{
		cellFsUnlink(path4);
	}
	if(cellFsStat(path5,&stat)==0)
	{		
		cellFsUnlink(path5);
	}
	if(cellFsStat(path6,&stat)==0)
	{		
		cellFsUnlink(path6);
	}
	
	if(cellFsStat("/dev_hdd0/hen/clear_info.off",&stat)!=0)
	{
		sprintf(msg, "Clear Web Cache History %-s\nAuth Cache %-s\nCookie %-s", cleared_history, cleared_auth_cache, cleared_cookie);
		show_msg((char*)msg);
	}
}

void set_build_type(void);
void set_build_type(void)
{
	CellFsStat stat;
	if((cellFsStat("/dev_hdd0/hen/dev_build_type.on",&stat)==0) || (cellFsStat("/dev_usb000/dev_build_type.on",&stat)==0) || (cellFsStat("/dev_usb001/dev_build_type.on",&stat)==0)){build_type=DEV;}
	DPRINTF("HENPLUGIN->Setting build_type to %i\n", build_type);
}

// Shamelessly taken and modified from webmanMOD (thanks aldostools)
static void play_rco_sound(const char *sound)
{
	View_Find = getNIDfunc("paf", 0xF21655F3, 0);
	uint32_t plugin = View_Find("system_plugin");
	if(plugin)
	{
		PlayRCOSound(plugin, sound, 1, 0);
		DPRINTF("HENPLUGIN->PlayRCOSound(%0X, %s, 1, 0)\n",plugin,sound);
	}
}

static void close_browser_plugins(void);
static void close_browser_plugins(void)
{
	View_Find = getNIDfunc("paf", 0xF21655F3, 0);
	int is_browser_open=View_Find("webbrowser_plugin");
	
	// Silk
	DPRINTF("HENPLUGIN->close_browser_plugins: silk \n");
	while(is_browser_open)
	{	
		sys_timer_usleep(70000);
		is_browser_open=View_Find("webbrowser_plugin");
	}
	
	// Webkit
	DPRINTF("HENPLUGIN->close_browser_plugins: webkit \n");
	is_browser_open=View_Find("webrender_plugin");
	while(is_browser_open)
	{
		sys_timer_usleep(70000);
		is_browser_open=View_Find("webrender_plugin");
	}
	DPRINTF("HENPLUGIN->unload_web_plugins \n");
	unload_web_plugins();
}

static void package_install(void);
static void package_install(void)
{
	DPRINTF("HENPLUGIN->package_install: %s \n", pkg_path);
	CellFsStat stat;
	
	LoadPluginById(0x16, (void *)installPKG_thread);
	
	if(cellFsStat(pkg_path,&stat)==0)
	{
		if (usb_emergency_update==1)
		{
			char manual_reboot_txt[0x80];
			sprintf(manual_reboot_txt, "HEN_UPD.PKG installing from USB.\nWhen finished, reboot manually.");
			show_msg((char *)manual_reboot_txt);
		}
		else 
		{
			// Original logic for waiting for package installation to complete
			DPRINTF("HENPLUGIN->IS_INSTALLING: %08X\nthread3_install_finish: %i\n",IS_INSTALLING,thread3_install_finish);
			while (!thread3_install_finish || IS_INSTALLING)
			{
				DPRINTF("HENPLUGIN->IS_INSTALLING: %08X\n",IS_INSTALLING);
				sys_timer_usleep(2000000); // check every 2 seconds
			}
			reboot_flag=1;
		}
	}
}

static void henplugin_thread(__attribute__((unused)) uint64_t arg)
{
	set_build_type();
	
	View_Find = getNIDfunc("paf", 0xF21655F3, 0);
	plugin_GetInterface = getNIDfunc("paf", 0x23AFB290, 0);
	int view = View_Find("explore_plugin");
	system_call_1(8, SYSCALL8_OPCODE_HEN_REV); hen_version = (int)p1;
	char henver[0x30];
	if(build_type==DEV)
	{
		sprintf(henver, "PS3HEN %X.%X.%X\nPSPx.Ru Team\nDeveloper Mode", hen_version>>8, (hen_version & 0xF0)>>4, (hen_version&0xF));
	}
	else if (build_type==LITE)
	{
		sprintf(henver, "PS3HEN %X.%X.%X\nPSPx.Ru Team\nLite Mode", hen_version>>8, (hen_version & 0xF0)>>4, (hen_version&0xF));
	}
	else 
	{
		sprintf(henver, "PS3HEN %X.%X.%X\nPSPx.Ru Team", hen_version>>8, (hen_version & 0xF0)>>4, (hen_version&0xF));
	}
	
	//DPRINTF("HENPLUGIN->hen_version: %x\n",hen_version);
	show_msg((char *)henver);
	

	if(view==0)
	{
		view=View_Find("explore_plugin");
		sys_timer_usleep(70000);
	}
	explore_interface = (explore_plugin_interface *)plugin_GetInterface(view, 1);

	CellFsStat stat;
	do_update=(cellFsStat("/dev_hdd0/hen/hen_updater.off",&stat) ? hen_updater() : 0);// 20211011 Added update toggle thanks bucanero for original PR
	//DPRINTF("HENPLUGIN->Checking do_update: %i\n",do_update);
	// If default HEN Check file is missing, assume HEN is not installed
	do_install_hen=(cellFsStat("/dev_flash/hen/PS3HEN.BIN",&stat));
	//DPRINTF("HENPLUGIN->do_install_hen: %x\n",do_install_hen);

	enable_ingame_screenshot();
	reload_xmb();

	// Emergency USB HEN Installer
	for (int i = 0; i < 8; i++) 
	{
		char usb_path[32];
		snprintf(usb_path, sizeof(usb_path), "/dev_usb00%d/HEN_UPD.pkg", i);

		// Check if the package exists on the current USB path
		if (cellFsStat(usb_path, &stat) == 0) 
		{
			DPRINTF("HENPLUGIN->Checking for emergency installer on %s\n", usb_path);
			
			usb_emergency_update = 1; // Set USB Emergency Update Flag
			//set_led("install_start");
			play_rco_sound("snd_trophy");
			DPRINTF("HENPLUGIN->Installing Emergency Package From %s\n", usb_path);
			// Set pkg_path to the found USB path
			memset(pkg_path, 0, 256);
			strcpy(pkg_path, usb_path);
			close_browser_plugins();
			package_install();
			//sys_timer_usleep(20000000); // wait 20 seconds to allow package to finish
			//set_led("install_success");
			goto done;
		} else 
		{
			DPRINTF("HENPLUGIN->Checking for emergency installer on %s\n", usb_path);
		}
	}
	
	restore_act_dat(); // restore act.dat from act.bak backup
	
	// Check for webMAN-MOD
	if((cellFsStat("/dev_hdd0/plugins/webftp_server.sprx",&stat)==0) || (cellFsStat("/dev_hdd0/plugins/webftp_server_lite.sprx",&stat)==0))
	{
		use_wmm_pkg=1;
		DPRINTF("HENPLUGIN->WMM Detected\n");
	}
	
	// Removing temp installer packages so old ones can't be installed
	DPRINTF("HENPLUGIN->Removing Temp Installer Packages\n");
	cellFsUnlink(pkg_path);
	
	if (number_files()<57) // check flag
	{
		copyflag_thread();
		DPRINTF("HENPLUGIN->flag files: %u\n", number_files());
		//reboot_flag=1;
	}
	
	/*if (cellFsStat("/dev_hdd0/hen/ofw_m.xml",&stat)!=0)  // check flag
	{
	do_update=1;
	}*/
	
	if((do_install_hen!=0) || (do_update==1))
	{
		//set_led("install_start");
		int is_browser_open=View_Find("webbrowser_plugin");
		
		while(is_browser_open)
		{	
			sys_timer_usleep(70000);
			is_browser_open=View_Find("webbrowser_plugin");
		}
		is_browser_open=View_Find("webrender_plugin");
		while(is_browser_open)
		{
			sys_timer_usleep(70000);
			is_browser_open=View_Find("webrender_plugin");
		}
		unload_web_plugins();
		
		if (use_wmm_pkg==1)
		{
			DPRINTF("HENPLUGIN->Use WMM Update Package\n");
			memset(pkg_path,0,33);
			strcpy(pkg_path,pkg_path);
		}
		
		DPRINTF("HENPLUGIN->pkg_path=%s\n",pkg_path);

		LoadPluginById(0x29,(void*)downloadPKG_thread2);

		while(thread2_download_finish==0)
		{
			sys_timer_usleep(70000);
		}

		while(IS_DOWNLOADING)
		{
			sys_timer_usleep(500000);
			//DPRINTF("HENPLUGIN->Waiting for package to finish downloading\n");
		}
		
		if(cellFsStat(pkg_path,&stat)==0)
		{
			// After package starts installing, this first loop exits
			LoadPluginById(0x16, (void *)installPKG_thread);
			
			//DPRINTF("IS_INSTALLING: %08X\nthread3_install_finish: %i\n",IS_INSTALLING,thread3_install_finish);
			while (!thread3_install_finish || IS_INSTALLING)
			{
				//DPRINTF("IS_INSTALLING: %08X\n",IS_INSTALLING);
				sys_timer_usleep(2000000); // check every 2 seconds								 
			}
			reboot_flag=1;
			
			goto done;
		}
	}
	else
	{
		// Removing temp packages
		//cellFsUnlink(pkg_path);
	}
	
done:
	DPRINTF("HENPLUGIN->Exiting main thread!\n");
	
	cellFsUnlink("/dev_hdd0/theme/PS3HEN.p3t");// Removing temp HEN installer
	if (usb_emergency_update!=1) 
	{
		cellFsUnlink(pkg_path);
	}
	done=1;
	
	
	if(reboot_flag==1)
	{
		play_rco_sound("snd_trophy");
		
		char reboot_txt[0x80];
		sprintf(reboot_txt, "The operation is completed!\n\n I'm rebooting...");
		show_msg((char *)reboot_txt);
		sys_timer_usleep(8000000);// Wait a few seconds
		reboot_ps3();// Default Soft Reboot
	}
	
	clear_web_cache_check();// Clear WebBrowser cache check (thanks xfrcc)
	
	sys_ppu_thread_exit(0);
}

int henplugin_start(__attribute__((unused)) uint64_t arg)
{
	//sys_timer_sleep(40000);
	sys_ppu_thread_create(&thread_id, henplugin_thread, 0, 3000, 0x4000, SYS_PPU_THREAD_CREATE_JOINABLE, THREAD_NAME);
	// Exit thread using directly the syscall and not the user mode library or we will crash
	_sys_ppu_thread_exit(0);
	return SYS_PRX_RESIDENT;
}

static void henplugin_stop_thread(__attribute__((unused)) uint64_t arg)
{
	uint64_t exit_code;
	sys_ppu_thread_join(thread_id, &exit_code);
	sys_ppu_thread_exit(0);
}


// Updated 20220613 (thanks TheRouLetteBoi)
int henplugin_stop()
{
	sys_ppu_thread_t t_id;
	int ret = sys_ppu_thread_create(&t_id, henplugin_stop_thread, 0, 3000, 0x2000, SYS_PPU_THREAD_CREATE_JOINABLE, STOP_THREAD_NAME);

	uint64_t exit_code;
	if (ret == 0) sys_ppu_thread_join(t_id, &exit_code);

	sys_timer_usleep(7000);
	stop_prx_module();

	_sys_ppu_thread_exit(0);

	return SYS_PRX_STOP_OK;
}
