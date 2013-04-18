#ifdef PACKAGE
	#define SHEBANGNAME PACKAGE
#else
	#define SHEBANGNAME "shebang"
#endif

#ifdef VERSION
	#define SB_VERSION VERSION
#else
	#define SB_VERSION "0.3"
#endif

#define MAXBUFSIZE 1024

static const char* TMP = "/tmp";
static const char* WSTR = " ";
static const char* GNABEHS = "@!";

int str2argv(const char* str, int* argc, char* argv[], char** cmd, int* gnabehs);
void Fatal(char* fmt, ...);
