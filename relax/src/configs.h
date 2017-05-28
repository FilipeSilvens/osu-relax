#include "relax.h"

// configuration path
#define configPath L".\\config.ini" // hardcode a directory if you're using the cheat as the default program to open .osu files

// window name
TCHAR newWindowTitle[MAX_PATH];

char config_window_name = GetPrivateProfileString(L"Window", L"windowname", L"osu!relax", newWindowTitle, sizeof(newWindowTitle), L".\\config.ini");

//keys
char config_left_key = GetPrivateProfileInt(L"Keys", L"leftkey", 0x58, configPath);
char config_right_key = GetPrivateProfileInt(L"Keys", L"rightkey", 0x5A, configPath);

//delays
int config_delaystartmin = GetPrivateProfileInt(L"DelayStart", L"min", 0, configPath);
int config_delaystartmax = GetPrivateProfileInt(L"DelayStart", L"max", 0, configPath);

int config_delayreleasemin = GetPrivateProfileInt(L"DelayRelease", L"min", 0, configPath);
int config_delayreleasemax = GetPrivateProfileInt(L"DelayRelease", L"max", 0, configPath);

//tap interval
int config_singletapinterval = GetPrivateProfileInt(L"SingleTapInterval", L"milliseconds", 0, configPath);

//100's to issue
int config_total100s = GetPrivateProfileInt(L"100sToIssue", L"100s", 0, configPath);
