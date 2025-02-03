#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cell/fs/cell_fs_file_api.h>
#include <sys/ppu_thread.h>
#include <sys/prx.h>
#include "xmb_plugin2.h"


#define XAI_VERSION "XAI Version 1.13"

void load_cfw_functions()
{
	setNIDfunc(xSettingRegistryGetInterface, "xsetting", 0xD0261D72);
	setNIDfunc(xSettingSystemInfoGetInterface, "xsetting", 0xAF1F161);
	setNIDfunc(xUserGetInterface, "xsetting", 0xCC56EB2D);

	setNIDfunc(NotifyWithTexture, "vshcommon", 0xA20E43DB);
	setNIDfunc(FindTexture, "paf", 0x3A8454FC);

	setNIDfunc(free_, "allocator", 0x77A602DD);
	setNIDfunc(malloc_, "allocator", 0x759E0635);
	setNIDfunc(memalign_, "allocator", 0x6137D196);

	setNIDfunc(FindPlugin, "paf", 0xF21655F3);
	setNIDfunc(wcstombs_, "stdc", 0xB680E240);
	setNIDfunc(stoull_, "stdc", 0xD417EEB5);

	setNIDfunc(fopen_, "stdc", 0x69C27C12);
	setNIDfunc(fclose_, "stdc", 0xE1BD3587);
	setNIDfunc(fprintf_, "stdc", 0xFAEC8C60);
	setNIDfunc(ctime_, "stdc", 0xBC7B4B8E);		

	setNIDfunc(FindString, "paf", 0x89B67B9C);

	setNIDfunc(sceNetCtlGetInfoVsh, "netctl_main", 0x9A528B81);	
}

SYS_MODULE_INFO(xai_plugin, 0, 1, 1);
SYS_MODULE_START(_xai_plugin_prx_entry);
SYS_MODULE_STOP(_xai_plugin_prx_stop);
SYS_MODULE_EXIT(_xai_plugin_prx_exit);

SYS_LIB_DECLARE_WITH_STUB(LIBNAME, SYS_LIB_AUTO_EXPORT, STUBNAME);
SYS_LIB_EXPORT(_xai_plugin_export_function, LIBNAME);

xmb_plugin_xmm0 * xmm0_interface;
xmb_plugin_xmb2 * xmb2_interface;
xmb_plugin_mod0 * mod0_interface;

sys_ppu_thread_t thread_id = 0;
const char * action_thread;

int LoadPlugin(char *pluginname, void *handler)
{
	//log_function("xai_plugin", "1", __FUNCTION__, "(%s)\n", pluginname);
	return xmm0_interface->LoadPlugin3(xmm0_interface->GetPluginIdByName(pluginname), handler, 0); 
}

int GetPluginInterface(const char *pluginname, int interface_)
{
	return plugin_GetInterface(FindPlugin(pluginname), interface_);
}

void * getNIDfunc(const char * vsh_module, uint32_t fnid)
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
			uint16_t count = *(uint16_t*)((char*)export_stru_ptr + 6); // amount of exports

			for(int i = 0; i < count; i++)
			{
				if(fnid == *(uint32_t*)((char*)lib_fnid_ptr + i * 4))								
					return (void*&)*((uint32_t*)(lib_func_ptr) + i); // take adress from OPD				
			}
		}

		table = table + 4;
	}

	return 0;
}

int load_functions()
{	
	setNIDfunc(FindPlugin, "paf", 0xF21655F3);
	setNIDfunc(plugin_GetInterface, "paf", 0x23AFB290);
	setNIDfunc(plugin_SetInterface, "paf", 0xA1DC401);
	setNIDfunc(plugin_SetInterface2, "paf", 0x3F7CB0BF);

	//load_log_functions();
	load_cfw_functions();
	//load_saves_functions();
	
	xmm0_interface = (xmb_plugin_xmm0 *)GetPluginInterface("xmb_plugin", 'XMM0');
	xmb2_interface = (xmb_plugin_xmb2 *)GetPluginInterface("xmb_plugin", 'XMB2');
	
	//setlogpath("/dev_hdd0/tmp/cfw_settings.log"); // Default path

	return 0;
}

// An exported function is needed to generate the project's PRX stub export library
extern "C" int _xai_plugin_export_function(void)
{
    return CELL_OK;
}

extern "C" int _xai_plugin_prx_entry(size_t args, void *argp)
{	
	load_functions();
	//log_function("xai_plugin", "", __FUNCTION__, "()\n", 0);
	plugin_SetInterface2(*(unsigned int*)argp, 1, xai_plugin_functions);

    return SYS_PRX_RESIDENT;
}

extern "C" int _xai_plugin_prx_stop(void)
{
	//log_function("xai_plugin", "", __FUNCTION__, "()\n", 0);
    return SYS_PRX_STOP_OK;
}

extern "C" int _xai_plugin_prx_exit(void)
{
	//log_function("xai_plugin", "", __FUNCTION__, "()\n", 0);
    return SYS_PRX_STOP_OK;
}

void xai_plugin_interface::xai_plugin_init(int view)
{
	//log_function("xai_plugin", "1", __FUNCTION__, "()\n", 0);
	plugin_SetInterface(view, 'ACT0', xai_plugin_action_if);
}

int xai_plugin_interface::xai_plugin_start(void * view)
{
	//log_function("xai_plugin", "1", __FUNCTION__, "()\n", 0);
	return SYS_PRX_START_OK; 
}

int xai_plugin_interface::xai_plugin_stop(void)
{
	//log_function("xai_plugin", "1", __FUNCTION__, "()\n", 0);
	return SYS_PRX_STOP_OK;
}

void xai_plugin_interface::xai_plugin_exit(void)
{
	//log_function("xai_plugin", "1", __FUNCTION__, "()\n", 0);
}


void xai_plugin_interface_action::xai_plugin_action(const char * action)
{	
	thread_id = 0;

	//log_function("xai_plugin", __VIEW__, __FUNCTION__, "(%s)\n", action);
	action_thread = action;
	sys_ppu_thread_create(&thread_id, plugin_thread, 0, 3000, 0x4000, 0, "xai_thread");
}


void ShowMessage(const char *string, const char *plugin, const char *tex_icon)
{
	int teximg, dummy = 0;
	FindPlugin = getNIDfunc("paf", 0xF21655F3, 0);
	FindString = getNIDfunc("paf", 0x89B67B9C, 0);
	wcstombs_= getNIDfunc("stdc", 0xB680E240, 0);		
	FindTexture = getNIDfunc("paf", 0x3A8454FC, 0);
	NotifyWithTexture = getNIDfunc("vshcommon", 0xA20E43DB,0);
	
	int plugin_uint = FindPlugin((char*)plugin);
	int wstring = FindString(plugin_uint, string);

	char conv_str[120];
	wcstombs_((char *)conv_str, (wchar_t *)wstring, 120 + 1);

	//log(conv_str);
	//log("\n");

	FindTexture(&teximg, plugin_uint, tex_icon);
	NotifyWithTexture(0, tex_icon, 0, &teximg, &dummy, "", "", 0, (wchar_t*)wstring, 0, 0, 0);	
}
 