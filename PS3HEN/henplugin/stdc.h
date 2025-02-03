#ifndef __STDC_H__
#define __STDC_H__


/*
extern uint64_t stdc_1ACBEFAC;  // ? dword_747E48
extern uint64_t stdc_26A34F81;  // ? dword_6F7B38
extern uint64_t stdc_57DBCF27;  // _Inf
extern uint64_t stdc_6524499E;  // _FInf
extern uint64_t stdc_7BC88211;  // ? off_707E80
extern uint64_t stdc_985FC057;  // ? dword_6F7B80
extern uint64_t stdc_AAAACE57;  // ? dword_6D5368
extern uint64_t stdc_B5D2F53B;  // ? off_707E84
extern uint64_t stdc_D59C193C;  // _Nan
extern uint64_t stdc_210B2F6E;  // _FNan
extern uint64_t stdc_D97B0687;  // _Ctype
extern uint64_t stdc_67D1406B;  // __ctype_ptr
extern uint64_t stdc_32E56B1A;  // _Stdin
extern uint64_t stdc_FB2BD688;  // _Stdout
extern uint64_t stdc_FEFBE065;  // _Stderr
*/


extern int stdc_24C9E021(int n);                                                      // abs()
#define abs stdc_24C9E021

extern double stdc_58EB9E57(double x);                                                // fabs()
#define fabs stdc_58EB9E57

extern int stdc_EE303936(double);                                                     // _Dsign()
#define _Dsign stdc_EE303936

extern double stdc_0F428F0F(double x);                                                // rint()
#define rint stdc_0F428F0F

extern int stdc_3AD203FA(double x);                                                   // lrint()
#define lrint stdc_3AD203FA

extern long int stdc_9558ED08(float x);                                               // lrintf()
#define lrintf stdc_9558ED08

extern double stdc_21E6D304(double x);                                                // ceil()
#define ceil stdc_21E6D304

extern float stdc_BAF11866(float x);                                                  // ceilf()
#define ceilf stdc_BAF11866

extern double stdc_475D855B(double x);                                                // trunc()
#define trunc stdc_475D855B

extern double stdc_519EBB77(double x);                                                // floor()
#define floor stdc_519EBB77

extern float stdc_23B985F7(float x);                                                  // floorf()
#define floorf stdc_23B985F7

extern double stdc_F3EC0258(double x);                                                // round()
#define round stdc_F3EC0258

extern float stdc_C984BF53(float x);                                                   // roundf()
#define roundf stdc_C984BF53

extern long int stdc_772F1E4D(double x);                                              // lround()
#define lround stdc_772F1E4D

extern long int stdc_FBB4047A(float x);                                               // lroundf()
#define lroundf stdc_FBB4047A

extern float stdc_7C2EAEB5(float x, float y);                                         // fminf()
#define fminf stdc_7C2EAEB5

extern float stdc_895CDB49(float x, float y);                                         // fmaxf()
#define fmaxf stdc_895CDB49

extern double stdc_A713F8CF(double x, double* intpart);                               // modf()
#define modf stdc_A713F8CF

extern double stdc_E769E5CF(double numer, double denom);                              // fmod()
#define fmod stdc_E769E5CF

extern float stdc_9A81E583(float numer, float denom);                                 // fmodf()
#define fmodf stdc_9A81E583

extern ldiv_t stdc_AD62A342(long int numer, long int denom);                          // ldiv()
#define ldiv stdc_AD62A342

extern int stdc_15BDCC00(void);                                                       // rand()
#define rand stdc_15BDCC00

extern void stdc_0D2A593B(uint32_t seed);                                             // srand()
#define srand stdc_0D2A593B

extern double stdc_18668CE3(double x);                                                // exp()
#define exp stdc_18668CE3

extern float stdc_FCF08193(float x);                                                  // expf()
#define expf stdc_FCF08193

extern double stdc_7F381837(double x, int *exp);                                      // frexp()
#define frexp stdc_7F381837

extern double stdc_38E69F09(double x, double y);                                      // pow()
#define pow stdc_38E69F09

extern float stdc_4DDB926B(float x, float y);                                         // powf()
#define powf stdc_4DDB926B

extern float stdc_00FDE072(float x, float y);                                         // f_powf()
#define f_powf stdc_00FDE072

extern double stdc_659E011E(double x);                                                // sqrt()
#define sqrt stdc_659E011E

extern double stdc_8451EDF0(double x);                                                // sqrtf()
#define sqrtf stdc_8451EDF0

extern double stdc_CA463458(double, int);                                              // _Log()
#define _Log stdc_CA463458

// stdc_71293B71  // _FLog()

// stdc_742F12B4  // _Sin()

extern float stdc_B54CC9A1(float x);                                                  // f_sinf()
#define f_sinf stdc_B54CC9A1

extern float stdc_B5E28191(float, unsigned int);                                      // _FSin()
#define _FSin stdc_B5E28191

extern double stdc_05EFC660(double x);                                                // asin()
#define asin stdc_05EFC660

extern float stdc_411434BB(float x);                                                  // asinf()
#define asinf stdc_411434BB

extern float stdc_7AB679DA(float x);                                                  // f_cosf()
#define f_cosf stdc_7AB679DA

extern double stdc_1313A420(double x);                                                // acos()
#define acos stdc_1313A420

extern float stdc_DECE76A6(float x);                                                  // acosf()
#define acosf stdc_DECE76A6

extern float stdc_705D9E24(float);                                                    // f_acosf()
#define f_acosf stdc_705D9E24

extern double stdc_D50277AD(double x);                                                // tan()
#define tan stdc_D50277AD

extern float stdc_9379E36E(float x);                                                  // tanf()
#define tanf stdc_9379E36E

extern double stdc_7F579E03(double x);                                                // atan()
#define atan stdc_7F579E03

extern float stdc_3C057FBD(float x);                                                  // atanf()
#define atanf stdc_3C057FBD

extern double stdc_31DB8C89(double y, double x);                                      // atan2()
#define atan2 stdc_31DB8C89

extern float stdc_FB932A56(float y, float x);                                         // atan2f()
#define atan2f stdc_FB932A56

extern float stdc_ABDCCC7A(float, float);                                             // f_atan2f()
#define f_atan2f stdc_ABDCCC7A

float stdc_B7696143(float x, float y);                                                // nextafterf()
#define nextafterf stdc_B7696143

// stdc_44115DD0  // _Geterrno()

extern void stdc_8B439438(FILE *stream);                                              // clearerr()
#define clearerr stdc_8B439438

extern void stdc_692B497F(const char *str);                                           // perror()
#define perror stdc_692B497F

extern int stdc_33D6AE54(FILE *stream);                                               // ferror()
#define ferror stdc_33D6AE54

extern char *stdc_44796E5C(int errnum);                                               // strerror()
#define strerror stdc_44796E5C

extern size_t stdc_2F45D39C(const char *str);                                         // strlen()
#define strlen stdc_2F45D39C

extern size_t stdc_65E8D4D0(const wchar_t *ws);                                       // wcslen()
#define wcslen stdc_65E8D4D0

extern void *stdc_CBAC7AD7(void *ptr, int value, size_t num);                         // memchr()
#define memchr stdc_CBAC7AD7

extern wchar_t *stdc_4E72F810(const wchar_t *s, wchar_t c, size_t n);                 // wmemchr()
#define wmemchr stdc_4E72F810;

extern int stdc_C3E14CBE(const void *ptr1, const void *ptr2, size_t num);             // memcmp()
#define memcmp stdc_C3E14CBE

extern int stdc_AC758D20(const wchar_t *ptr1, const wchar_t *ptr2, size_t num);       // wmemcmp()
#define wmemcmp stdc_AC758D20

#define memset stdc_5909E3C4
extern void *stdc_5909E3C4(void *str, int c, size_t n);                               // memset()

extern wchar_t *stdc_F06EED36(wchar_t *ptr, wchar_t wc, size_t num);                  // wmemset()
#define wmemset stdc_F06EED36

extern void *stdc_5B162B7F(void *str1, const void *str2, size_t n);                   // memmove()
#define memmove stdc_5B162B7F

extern wchar_t *stdc_99B38CE7(wchar_t *dest, const wchar_t *src, size_t num);         // wmemmove()
#define wmemmove stdc_99B38CE7

extern void *stdc_831D70A5(void *dest, const void *src, size_t num);                  // memcpy()
#define memcpy stdc_831D70A5

extern wchar_t *stdc_A8B07F1B(wchar_t *dest, const wchar_t *src, size_t num);         // wmemcpy()
#define wmemcpy stdc_A8B07F1B

extern wchar_t *stdc_84378DDC(wchar_t *dest, const wchar_t *src, size_t num);         // wcsncpy()
#define wcsncpy stdc_84378DDC

extern int stdc_3D85D6F8(const char *str1, const char *str2);                         // strcmp()
#define strcmp stdc_3D85D6F8

extern int stdc_F0776A44(const wchar_t *wcs1, const wchar_t *wcs2);                   // wcscmp()
#define wcscmp stdc_F0776A44

extern int stdc_E1E83C65(const char *str1, const char *str2, size_t num);             // strncmp()
#define strncmp stdc_E1E83C65

extern int stdc_6D5115B0(const wchar_t *ws1, const wchar_t *ws2, size_t n);           // wcsncmp()
#define wcsncmp stdc_6D5115B0

extern int stdc_B6D92AC3(const char *s1, const char *s2);                             // strcasecmp()
#define strcasecmp stdc_B6D92AC3

extern int stdc_B6257E3D(const char *s1, const char *s2, size_t n);                   // strncasecmp()
#define strncasecmp stdc_B6257E3D

extern char *stdc_04A183FC(char *dest, const char *src);                              // strcpy()
#define strcpy stdc_04A183FC

extern wchar_t *stdc_F41355F9(wchar_t *dest, const wchar_t *src);                     // wcscpy()
#define wcscpy stdc_F41355F9

extern char *stdc_8AB0ABC6(char *dest, const char *src, size_t num);                  // strncpy()
#define strncpy stdc_8AB0ABC6

extern char *stdc_FC0428A6(const char *s);                                            // strdup()
#define strdup stdc_FC0428A6

extern char *stdc_AA9635D7(char *dest, const char *src);                              // strcat()
#define strcat stdc_AA9635D7

extern wchar_t *stdc_7FDCF73E(wchar_t *dest, const wchar_t *src);                     // wcscat()
#define wcscat stdc_7FDCF73E

extern char *stdc_589B5314(char *dest, const char *src, size_t n);                    // strncat()
#define strncat stdc_589B5314

extern wchar_t *stdc_9439E4CD(wchar_t* dest, const wchar_t* source, size_t num);      // wcsncat()
#define wcsncat stdc_9439E4CD

extern char *stdc_DEBEE2AF(char *str, int c);                                         // strchr()
#define strchr stdc_DEBEE2AF

extern wchar_t *stdc_82A3CC30(wchar_t* ws, wchar_t wc);                               // wcschr()
#define wcschr stdc_82A3CC30

extern char *stdc_73EAE03D(const char *s, int c);                                     // strrchr()
#define strrchr stdc_73EAE03D

extern wchar_t *stdc_B7AB5127(const wchar_t *ws, wchar_t wc);                         // wcsrchr()
#define wcsrchr stdc_B7AB5127

extern char *stdc_C5C09834(char *str1, const char *str2);                             // strstr()
#define strstr stdc_C5C09834

extern wchar_t *stdc_A797790F(wchar_t* wcs1, const wchar_t* wcs2);                    // wcsstr()
#define wcsstr stdc_A797790F

extern size_t stdc_F3EF3678(const wchar_t *wcs1, const wchar_t *wcs2);                // wcscspn()
#define wcscspn stdc_F3EF3678

extern char *stdc_E40BA755(char *str, const char *delimiters);                        // strtok()
#define strtok stdc_E40BA755

extern char *stdc_B738027A(char *str, const char *delim, char **saveptr);             // strtok_r()
#define strtok_r stdc_B738027A

extern long int stdc_D14ECE90(const char *str, char **endptr, int base);              // strtol()
#define strtol stdc_D14ECE90

extern double stdc_D1D69CB8(const char *a, char **b, long x);                         // _Stod()
#define _Stod stdc_D1D69CB8

extern uint64_t stdc_36C067C1(const char *a, char **b, int x);                        // _Stoll
#define _Stoll stdc_36C067C1

extern unsigned long stdc_867275D7(const char *a, char **b, int x);                   // _Stoul()
#define _Stoul stdc_867275D7

extern uint64_t stdc_D417EEB5(const char *a, char **b, int x);                        // _Stoull()
#define _Stoull stdc_D417EEB5

extern long stdc_116CDA13(const wchar_t *restrict nptr, wchar_t **restrict endptr, int base);  // wcstol
#define wcstol stdc_116CDA13

extern long long int stdc_A30D4797(const wchar_t* str, wchar_t** endptr, int base);   // wcstoll()
#define wcstoll stdc_A30D4797

/*
extern size_t stdc_B2702E15(char *pmb, wchar_t wc, mbstate_t *ps);                    // wcrtomb()
#define wcrtomb stdc_B2702E15

extern size_t stdc_12A55FB7(wchar_t *restrict pwc, const char *restrict s, size_t n, mbstate_t *restrict ps);  // mbrtowc
#define mbrtowc stdc_12A55FB7
*/

extern size_t stdc_FCAC2E8E(wchar_t *dest, const char *src, size_t max);              // mbstowcs()
#define mbstowcs stdc_FCAC2E8E

extern size_t stdc_B680E240(char *dest, const wchar_t *src, size_t max);              // wcstombs()
#define wcstombs stdc_B680E240

extern int stdc_C01D9F97(const char *fmt, ...);                                       // printf()
#define printf stdc_C01D9F97

extern int stdc_E48348E9(const char *format, va_list arg);                            // vprintf()
#define vprintf stdc_E48348E9

extern int stdc_273B9711(char *str, const char *fmt, ...);                            // sprintf()
#define sprintf stdc_273B9711

extern int stdc_3A840AE3(char *str, size_t size, const char *fmt, ...);               // snprintf()
#define snprintf stdc_3A840AE3

extern int stdc_7AE82E0F(char *str, const char *format, va_list arg);                 // vsprintf()
#define vsprintf stdc_7AE82E0F

extern int stdc_FAEC8C60(FILE *stream, const char *fmt, ...);                         // fprintf()
#define fprintf stdc_FAEC8C60

extern int stdc_FB2081FD(FILE *stream, const char *fmt, va_list arg);                 // vfprintf()
#define vfprintf stdc_FB2081FD

extern int stdc_99A72146(char *s, size_t n, const char *fmt, va_list arg);            // vsnprintf()
#define vsnprintf stdc_99A72146

extern int stdc_E9137453(FILE *stream, const wchar_t *fmt, ...);                      // fwprintf()
#define fwprintf stdc_E9137453

extern int stdc_62BF1D6C(wchar_t *wcs, size_t maxlen, const wchar_t *format, ...);    // swprintf()
#define swprintf stdc_62BF1D6C

extern int stdc_4B6A4010(wchar_t * ws, size_t len, const wchar_t * fmt, va_list arg); // vswprintf()
#define vswprintf stdc_4B6A4010

extern int stdc_B1F4779D(sys_spu_thread_t id, uint32_t arg_addr);                     // spu_thread_printf()
#define spu_thread_printf stdc_B1F4779D

extern int stdc_F0E022C6(FILE *stream);                                               // getc()
#define getc stdc_F0E022C6

extern int stdc_A5BC0E19(void);                                                       // getchar()
#define getchar stdc_A5BC0E19

extern char *stdc_90010029(char *str);                                                // gets()
#define gets stdc_90010029

extern char *stdc_AF44A615(char *str, int num, FILE * stream);                        // fgets()
#define fgets stdc_AF44A615

extern int stdc_AC893127(FILE *stream);                                               // fgetc()
#define fgetc stdc_AC893127

extern wchar_t *stdc_55D4866E(wchar_t *ws, int n, FILE *stream);                      // fgetws()
#define fgetws stdc_55D4866E

// stdc_82A4561A  // _put_fd()

extern int stdc_2677568C(int c);                                                      // putchar()
#define putchar stdc_2677568C

extern int stdc_79819DBF(int c, FILE *stream);                                        // fputc()
#define fputc stdc_79819DBF

extern int stdc_E3CC73F3(const char *str);                                            // puts()
#define puts stdc_E3CC73F3

extern int stdc_4D348427(const char *str, FILE *stream);                              // fputs()
#define fputs stdc_4D348427

extern int stdc_FE88E97E(FILE *stream, const char *fmt, ...);                         // fscanf()
#define fscanf stdc_FE88E97E

extern int stdc_E9B560A5(const char *s, const char *fmt, ...);                        // sscanf()
#define sscanf stdc_E9B560A5

extern int stdc_3A210C93(const wchar_t* ws, const wchar_t* fmt, ...);                 // swscanf
#define swscanf stdc_3A210C93

extern int stdc_3BD9CE0A(int fd);                                                     // fsync()
#define fsync stdc_3BD9CE0A

extern int stdc_F356418C(const char *, int, ...);                                     // open()
#define open stdc_F356418C

extern FILE *stdc_69C27C12(const char *filename, const char *mode);                   // fopen()
#define fopen stdc_69C27C12

extern FILE *stdc_E3812672(int fd, const char *mode);                                 // fdopen()
#define fdopen stdc_E3812672

extern int stdc_4FFBA189(FILE *stream);                                               // feof()
#define feof stdc_4FFBA189

extern int stdc_26F023D5(FILE *stream);                                               // ftell()
#define ftell stdc_26F023D5

extern int stdc_D360DCB4(FILE *stream);                                               // fileno()
#define fileno stdc_D360DCB4

extern void stdc_54F57626(FILE *stream);                                              // rewind()
#define rewind stdc_54F57626

extern int stdc_69FF1B9B(FILE *stream, long int offset, int whence);                  // fseek()
#define fseek stdc_69FF1B9B

extern off_t stdc_AEC7C970(int fd, off_t offset, int whence);                         // lseek()
#define lseek stdc_AEC7C970

extern size_t stdc_FA00D211(int fd, void *buf, size_t nbytes);                        // read()
#define read stdc_FA00D211

extern size_t stdc_D40723D6(void *ptr, size_t size, size_t count, FILE *stream);      // fread()
#define fread stdc_D40723D6

extern size_t stdc_526A496A(int fildes, const void *buf, size_t nbytes);              // write()
#define write stdc_526A496A

extern size_t stdc_F88F26C4(const void *ptr, size_t size, size_t count, FILE *stream);  // fwrite()
#define fwrite stdc_F88F26C4

extern int stdc_814D8CB0(FILE * stream);                                              // fflush()
#define fflush stdc_814D8CB0

extern int stdc_B120F6CA(int fd);                                                     // close()
#define close stdc_B120F6CA

extern int stdc_E1BD3587(FILE *stream);                                               // fclose()
#define fclose stdc_E1BD3587

extern int stdc_153B364A(const char *path, mode_t mode);                              // mkdir()
#define mkdir stdc_153B364A

extern int stdc_7E7017B1(const char *path);                                           // rmdir()
#define rmdir stdc_7E7017B1

extern int stdc_75D4485C(const char *old_filename, const char *new_filename);         // rename()
#define rename stdc_75D4485C

extern int stdc_EF110B6B(const char *path);                                           // unlink()
#define unlink stdc_EF110B6B

extern void stdc_8A6830E7(void);                                                      // abort()
#define abort stdc_8A6830E7

extern void stdc_C291E698(int status);                                                // exit()
#define exit stdc_C291E698

// stdc_1A00F889  // _ZNSt9exceptionD2Ev

extern clock_t stdc_24F6CBDD(void);                                                   // clock()
#define clock stdc_24F6CBDD

extern time_t stdc_89F6F026(time_t *timer);                                           // time()
#define time stdc_89F6F026

extern char *stdc_BC7B4B8E(const time_t *timer);                                      // ctime()
#define ctime stdc_BC7B4B8E

extern struct tm *stdc_EB26298C(const time_t *timer);                                 // gmtime()
#define gmtime stdc_EB26298C

extern time_t stdc_19CCBB81(struct tm *timeptr);                                      // mktime()
#define mktime stdc_19CCBB81

extern struct tm *stdc_266311A0(const time_t *timer);                                 // localtime()
#define localtime stdc_266311A0

extern double stdc_4217B4CF(time_t time1, time_t time2);                              // difftime()
#define difftime stdc_4217B4CF

extern size_t stdc_CB9C535B(char *ptr, size_t maxsize, const char *fmt, const struct tm *timeptr);  // strftime()
#define strftime stdc_CB9C535B

extern void stdc_A9F68EFF(void *base, size_t num, size_t size, int (*compar)(const void*, const void*));  // qsort()
#define qsort stdc_A9F68EFF

extern void *stdc_5E7888F0(const void *key, const void *base, size_t nitems, size_t size, int (*compar)(const void *, const void *));  // bsearch()
#define bsearch stdc_5E7888F0

/*
extern DIR *stdc_3DBC3BEE(const char *name);                                          // opendir()
#define opendir stdc_3DBC3BEE

extern struct dirent *stdc_2B81FB7F(DIR *dirp);                                       // readdir()
#define readdir stdc_2B81FB7F

extern int stdc_A3440924(DIR *dirp);                                                  // closedir()
#define closedir stdc_A3440924
*/

// stdc_2D8BE7E8  // _ZNKSt9exception6_RaiseEv

// stdc_3B5489A9  // ?

// stdc_3CA81C76  // _Iswctype

// stdc_3EEEDB0E  // _Dclass()

// stdc_418BDFE1  // _get_fd()

extern double stdc_45034943(const char* tagp);                                        // nan()
#define nan stdc_45034943

// stdc_683CA70A  // _ZNKSt12_String_base5_XlenEv

extern int stdc_703EC767(FILE *restrict stream, char *restrict buf, int type, size_t size);  // setvbuf()
#define setvbuf stdc_703EC767

// stdc_7670FF88  // ?

extern double stdc_8ECAE294(double x, double y);                                      // nextafter()
#define nextafter stdc_8ECAE294

extern short stdc_9232BAEA(float *);                                                  // _FDtes()
#define _FDtest stdc_9232BAEA

// stdc_9CB73EE0  // _ZSt6_ThrowRKSt9exception

extern void stdc_AF89FDBD(const char *, const char *);                                // _Assert()
#define _Assert stdc_AF89FDBD

extern void stdc_DDC71A75(const char *, const char *);                                // _SCE_Assert()
#define _SCE_Assert stdc_DDC71A75

extern short stdc_B94B9D13(double *);                                                 // _Dtest()
#define _Dtest stdc_B94B9D13

// stdc_C7931798  // _ZNKSt12_String_base5_XranEv


#endif // __STDC_H__
