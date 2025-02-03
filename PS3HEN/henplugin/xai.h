#ifndef __XAI_H__
#define __XAI_H__

#define TEX_ERROR						"tex_error_ws"
#define TEX_SUCCESS						"tex_check2_ws"
#define TEX_WARNING						"tex_warning"
#define TEX_INFO						"tex_notification_info"
#define TEX_INFO2						"tex_notification_info2"
#define EXPLORE_PLUGIN					"explore_plugin"
#define XAI_PLUGIN						"xai_plugin"

#define setNIDfunc(name, module, nid) (void*&)(name) = (void*)((int)getNIDfunc(module, nid))

// TODO: reference additional headers your program requires here
void *getNIDfunc(const char *vsh_module, uint32_t fnid);
int GetPluginInterface(const char *pluginname, int interface_);
int LoadPlugin(char *pluginname, void *handler);

// NID Functions prx
static int (*FindPlugin)(const char *plugin);
static int (*plugin_GetInterface)(int view,int interface);
static int (*plugin_SetInterface)(int view, int interface, void *Handler);
static int (*plugin_SetInterface2)(int view, int interface, void *Handler);

static int (*FindString)(int plugin, const char *text);
static uint32_t (*sceNetCtlGetInfoVsh)(uint32_t size, const char *ip);

static size_t (*wcstombs_)(char *dest, const wchar_t *src, size_t max);

static xsetting_D0261D72_class*(*xSettingRegistryGetInterface)() = 0;
static xsetting_AF1F161_class*(*xSettingSystemInfoGetInterface)() = 0;
static xsetting_CC56EB2D_class*(*xUserGetInterface)() = 0;

static void (*NotifyWithTexture)(int32_t, const char *eventName, int32_t, int32_t *texture, int32_t*, const char*, const char*, float, const wchar_t *text, int32_t, int32_t, int32_t);
static uint32_t (*FindTexture)(int32_t *texptr, uint32_t plugin, const char *name);


class xai_plugin_interface_action
{	
	public:
		static void xai_plugin_action(const char *action);
};

static void *xai_plugin_action_if[3] = { (void*)xai_plugin_interface_action::xai_plugin_action, 0, 0 };

class xai_plugin_interface
{
	public:	
		static void xai_plugin_init(int view);
		static int xai_plugin_start(void *view);
		static int xai_plugin_stop(void);
		static void xai_plugin_exit(void);
};

static void *xai_plugin_functions[4] = 
{
	(void*)xai_plugin_interface::xai_plugin_init,
	(void*)xai_plugin_interface::xai_plugin_start,
	(void*)xai_plugin_interface::xai_plugin_stop,
	(void*)xai_plugin_interface::xai_plugin_exit
};

#endif
