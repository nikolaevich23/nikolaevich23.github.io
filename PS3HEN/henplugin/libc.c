#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <math.h>
#include <errno.h>
#include <time.h>

// external
extern long int stdc_D14ECE90(const char *str, char **endptr, int base);              // strtol()
#define strtol stdc_D14ECE90

extern uint64_t stdc_36C067C1(const char *a, char **b, int x);                        // _Stoll
#define _Stoll stdc_36C067C1

extern int stdc_E1E83C65(const char *str1, const char *str2, size_t num);				// strncmp()
int strncmp(const char *str1, const char *str2, size_t num) {if(!str1 || !str2) return -1; return stdc_E1E83C65(str1, str2, num);}

extern int stdc_3D85D6F8(const char *str1, const char *str2);							// strcmp()
int strcmp(const char *str1, const char *str2) {if(!str1 || !str2) return -1; return stdc_3D85D6F8(str1, str2);}

extern size_t stdc_2F45D39C(const char *str);											// strlen()
#define __strlen	stdc_2F45D39C
size_t strlen(const char *str) {if(!str) return 0; return __strlen(str);}

extern void *stdc_5909E3C4(void *str, int c, size_t n);									// memset()
#define __memset	stdc_5909E3C4
void* memset(void *str, int c, size_t n) {if(!str || !n) return str; return __memset(str, c, n);}

extern void *stdc_831D70A5(void *dest, const void *src, size_t num);					// memcpy()
void* memcpy(void *dest, const void *src, size_t num) {if(!dest || !src || !num) return dest; return stdc_831D70A5(dest, src, num);}

extern char *stdc_C5C09834(const char *str1, const char *str2);							// strstr()
char* strstr(const char *str1, const char *str2) {if(!str1 || !str2) return NULL; return stdc_C5C09834(str1, str2);}

extern int* stdc_44115DD0(void);
inline int* _Geterrno(void){return stdc_44115DD0();}									// _Geterrno

extern void allocator_77A602DD(void *ptr);												// free()
void free(void *ptr) {if(!ptr) return; allocator_77A602DD(ptr);}

extern void *allocator_759E0635(size_t size);											// malloc()
void* malloc (size_t size) {if(!size) return NULL; return allocator_759E0635(size);}

extern void *allocator_6137D196(size_t alignment, size_t size);							// memalign()
inline void* memalign(size_t alignment, size_t size) {return allocator_6137D196(alignment, size);}

extern void *allocator_A72A7595(size_t nitems, size_t size);							// calloc()
inline void* calloc(size_t nitems, size_t size) {return allocator_A72A7595(nitems, size);}

extern void *allocator_F7A14A22(void *ptr, size_t size);								// realloc()
inline void* realloc(void *ptr, size_t size) {return allocator_F7A14A22(ptr, size);}

extern void *stdc_5B162B7F(void *str1, const void *str2, size_t n);						// memmove()
void* memmove(void *str1, const void *str2, size_t n) {if(!str1 || !str2) return str1; return stdc_5B162B7F(str1, str2, n);}

extern char *stdc_FC0428A6(const char *s);												// strdup()
char* strdup(const char *s) {if(!s) return NULL; return stdc_FC0428A6(s);}

extern char *stdc_44796E5C(int errnum);													// strerror()
inline char* strerror(int errnum) {return stdc_44796E5C(errnum);}

extern double stdc_519EBB77(double x);													// floor()
inline double floor(double x) {return stdc_519EBB77(x);}

extern double stdc_21E6D304(double x);													// ceil()
inline double ceil(double x) {return stdc_21E6D304(x);}

extern time_t stdc_89F6F026(time_t *timer);												// time()
inline time_t time(time_t *timer) {return stdc_89F6F026(timer);}

//extern wchar_t *stdc_F06EED36(wchar_t *ptr, wchar_t wc, size_t num);                  // wmemset()
//inline wchar_t *wmemset(wchar_t *ptr, wchar_t wc, size_t num) {if(!ptr) return NULL; return stdc_F06EED36(ptr, wc, num);}

extern size_t stdc_FCAC2E8E(wchar_t *dest, const char *src, size_t max);				// mbstowcs()
size_t mbstowcs(wchar_t *dest, const char *src, size_t max) {if(!dest || !src) return 0; return stdc_FCAC2E8E(dest, src, max);}

extern size_t stdc_12A55FB7(wchar_t *restrict pwc, const char *restrict s, size_t n, mbstate_t *restrict ps); // mbrtowc
#define mbrtowc  stdc_12A55FB7
int mbtowc(wchar_t * restrict pwc, const char * restrict s, size_t n)
{
	static mbstate_t mbs;
	size_t rval;

	if (s == NULL) {
		__memset(&mbs, 0, sizeof(mbs));											//memset
		return (0);
	}
	rval = mbrtowc(pwc, s, n, &mbs);												//mbrtowc
	if (rval == (size_t)-1 || rval == (size_t)-2)
		return (-1);
	return ((int)rval);
}

extern size_t stdc_B2702E15(char *pmb, wchar_t wc, mbstate_t *ps);						// wcrtomb()
#define wcrtomb  stdc_B2702E15
int wctomb(char *s, wchar_t wchar)
{
	static mbstate_t mbs;
	size_t rval;

	if (s == NULL) {
		__memset(&mbs, 0, sizeof(mbs));											//memset
		return (0);
	}
	if ((rval = wcrtomb(s, wchar, &mbs)) == (size_t)-1)							//wcrtomb
		return (-1);
	return ((int)rval);
}

extern int stdc_C3E14CBE(const void *ptr1, const void *ptr2, size_t num);				// memcmp()
#define __memcmp	stdc_C3E14CBE
int memcmp(const void *ptr1, const void *ptr2, size_t num) {if(!ptr1 || !ptr1 || !num) return -1; return __memcmp(ptr1, ptr2, num);}

extern char *stdc_DEBEE2AF(const char *str, int c);										// strchr()
char* strchr(const char *str, int c) {if(!str) return NULL; return stdc_DEBEE2AF(str, c);}

extern char *stdc_73EAE03D(const char *s, int c);										// strrchr()
char* strrchr(const char *s, int c) {if(!s) return NULL; return stdc_73EAE03D(s, c);}

extern char *stdc_04A183FC(char *dest, const char *src);								// strcpy()
char* strcpy(char *dest, const char *src) {if(!dest || !src) return dest; return stdc_04A183FC(dest, src);}

extern char *stdc_8AB0ABC6(char *dest, const char *src, size_t num);					// strncpy()
char* strncpy(char *dest, const char *src, size_t num) {if(!dest || !src || !num) return dest; return stdc_8AB0ABC6(dest, src, num);}

extern char *stdc_AA9635D7(char *dest, const char *src);								// strcat()
char* strcat(char *dest, const char *src) {if(!dest || !src) return dest; return stdc_AA9635D7(dest, src);}

extern int stdc_B6257E3D(const char *s1, const char *s2, size_t n);						// strncasecmp()
#define __strncasecmp	stdc_B6257E3D
int strncasecmp(const char *s1, const char *s2, size_t n) {if(!s2 || !s2) return -1; return __strncasecmp(s1, s2, n);}

extern int stdc_B6D92AC3(const char *s1, const char *s2);								// strcasecmp()
int strcasecmp(const char *s1, const char *s2) {if(!s2 || !s2) return -1; return stdc_B6D92AC3(s1, s2);}

//extern char *stdc_E40BA755(char *str, const char *delimiters);							// strtok()
//inline char* strtok(char *str, const char *delimiters) {return stdc_E40BA755(str, delimiters);}

//extern struct tm *stdc_266311A0(const time_t *timer);									// localtime()
//inline struct tm* localtime(const time_t *timer) {return stdc_266311A0(timer);}

//internal

/*
void *memset(void *m, int c, size_t n)
{
	char *s = (char *) m;

	while (n--)
		*s++ = (char) c;

	return m;
}

void *memcpy(void *dst0, const void *src0, size_t n)
{
	char *dst = (char *)dst0;
	char *src = (char *)src0;

	void *save = dst0;

	while (n--)
		*dst++ = *src++;

	return save;
}

int memcmp(const void* s1, const void* s2, size_t n)
{
	const unsigned char *p1 = s1, *p2 = s2;
	while(n--)
		if( *p1 != *p2 )
			return *p1 - *p2;
		else
			p1++, p2++;

	return 0;
}


size_t strlen(const char *str)
{
	const char *start = str;
	while (*str) str++;
	return str - start;
}


char *strchr(const char *s, int c)
{
	if(!s) return 0;

	while (*s != (char)c)
		if (!*s++)
			return 0;

	return (char *)s;
}

char *strrchr(const char *s, int c)
{
	if(!s) return 0;

	char *ret = 0;
	char cc = (char)c;

	do
	{
		if( *s == cc ) ret = (char*)s;
	}
	while(*s++);

	return ret;
}

char *strstr(const char *s1, const char *s2)
{
	if(!s1) return 0;

	size_t n = strlen(s2); if(n == 0) return 0;

	while(*s1)
		if(!memcmp(s1++, s2, n))
			return (char*)(s1 - 1);

	return 0;
}

int strncasecmp (__const char *s1, __const char *s2, size_t n)
{
	int c1, c2;

	while(n--)
	{
		c1 = *((unsigned char *)(s1++)); if (c1 >= 'A' && c1 <= 'Z') c1 += 0x20; // ('a' - 'A')
		c2 = *((unsigned char *)(s2++)); if (c2 >= 'A' && c2 <= 'Z') c2 += 0x20; // ('a' - 'A')
		if (c1 != c2)   return (c1 - c2);
		if (c1 == '\0') return 0;
	}

	return 0;
}

int strcasecmp (__const char *s1, __const char *s2)
{
	int c1, c2;

	while(*s1)
	{
		c1 = *((unsigned char *)(s1++)); if (c1 >= 'A' && c1 <= 'Z') c1 += 0x20; // ('a' - 'A')
		c2 = *((unsigned char *)(s2++)); if (c2 >= 'A' && c2 <= 'Z') c2 += 0x20; // ('a' - 'A')
		if (c1 != c2)   return (c1 - c2);
	}

	return 0;
}
*/
char *strcasestr(const char *s1, const char *s2);
char *strcasestr(const char *s1, const char *s2)
{
	if(!s1 || !s2) return NULL;

	size_t n = __strlen(s2); if(n == 0) return 0;

	while(*s1)
		if(!__strncasecmp(s1++, s2, n))
			return (char*)(s1 - 1);

	return 0;
}
/*
int strncmp(const char *s1, const char *s2, size_t n)
{
	while((n > 0) && *s1 && (*s1==*s2)) {s1++, s2++, n--;} if(n == 0) return 0;

	return *(const unsigned char*)s1-*(const unsigned char*)s2;
}

int strcmp(const char *s1, const char *s2)
{
	while(*s1 && (*s1==*s2)) s1++, s2++;

	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}


char *strcpy(char *dest, const char* src)
{
	char *ret = dest;

	while ((*dest++ = *src++)) ;

	return ret;
}

char *strncpy(char *dest, const char *src, size_t n)
{
	char *ret = dest;

	do
	{
		if (!n--) return ret;
	}
	while ((*dest++ = *src++));

	while (n--) *dest++ = 0;

	return ret;
}

char *strcat(char *dest, const char *src)
{
	char *ret = dest;

	while (*dest) dest++;

	while ((*dest++ = *src++)) ;

	return ret;
}

char *strtok(char *str, const char *delim)
{
	static char* p = 0;

	if(str) p = str;
	else
	if(!p) return 0;

	str = p + strspn(p, delim);
	p = str + strcspn(str, delim);

	if(p == str) return p = 0;

	p = *p ? *p = 0, p + 1 : 0;
	return str;
}

size_t strspn(const char *s1, const char *s2)
{
	size_t ret = 0;
	while(*s1 && strchr(s2, *s1++))
		ret++;
	return ret;
}

size_t strcspn(const char *s1, const char *s2)
{
	register const char *p, *spanp;
	register char c, sc;
	for (p = s1;;) {
		c = *p++;
		spanp = s2;
		do {
			if ((sc = *spanp++) == c)
				return (p - 1 - s1);
		} while (sc != 0);
	}
	// * NOTREACHED * //
}
*/
int extcasecmp(const char *s1, const char *s2, size_t n);
int extcasecmp(const char *s1, const char *s2, size_t n)
{
	if(!s1 || !s2) return -1;
	int s = __strlen(s1) - n;
	if(s < 0) return -1;
	return __strncasecmp(s1 + s, s2, n);
}

int extcmp(const char *s1, const char *s2, size_t n);
int extcmp(const char *s1, const char *s2, size_t n)
{
	if(!s1 || !s2) return -1;
	int s = __strlen(s1) - n;
	if(s < 0) return -1;
	if(n == 8 && !__memcmp(s2, ".BIN.ENC", n)) return __strncasecmp(s1 + s, s2, n);
	return __memcmp(s1 + s, s2, n);
}
