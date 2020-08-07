#include <windows.h>

// Sleep in posix uses seconds instead of milis
#define sleep(X) Sleep(X * 1000)