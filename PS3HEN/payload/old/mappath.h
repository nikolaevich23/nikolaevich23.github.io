#ifndef __MAPPATH_H__
#define __MAPPATH_H__

#include <lv2/memory.h>

#define SYS_MAP_PATH	35
#define SYS_MAP_GAME	36

#define NPSIGNIN_LOCK  			"/dev_flash/vsh/resource/npsignin_plugin.lck"
#define NPSIGNIN_UNLOCK 		"/dev_flash/vsh/resource/npsignin_plugin.rco"

extern uint8_t allow_restore_sc; // allow re-create cfw syscalls accessing system update on XMB

#define FLAG_COPY		1
#define FLAG_TABLE		2
#define FLAG_PROTECT	4

void    map_path_slot(char *old, char *newp, int slot);
void	map_path_patches(int syscall);
int  	map_path(char *oldpath, char *newpath, uint32_t flags);
int 	map_path_user(char *oldpath, char *newpath, uint32_t flags);
int 	sys_map_path(char *oldpath, char *newpath);
int 	sys_map_paths(char *paths[], char *new_paths[], unsigned int num);
int get_map_path(unsigned int num, char *path, char *new_path);
/*int	sys_map_game(char *path);
int	sys_hermes_pathtable(uint64_t pathtable);*/

int 	sys_aio_copy_root(char *src, char *dst);
void create_syscalls(void);
void restore_syscalls(const char *path);
void check_signin(const char *path);
void unhook_all_map_path(void);

#endif /* __MAPPATH_H_ */

