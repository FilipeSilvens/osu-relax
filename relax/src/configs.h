#include "relax.h"

// configuration

// window name
TCHAR newWindowTitle[MAX_PATH];

char config_window_name = GetPrivateProfileString(L"Window", L"windowname", L"osu!relax", newWindowTitle, sizeof(newWindowTitle), L".\\config.ini");

//keys
char config_left_key = GetPrivateProfileInt(L"Keys", L"leftkey", 0x58, L".\\config.ini");
char config_right_key = GetPrivateProfileInt(L"Keys", L"rightkey", 0x5A, L".\\config.ini");

//delays
int config_delaystartmin = GetPrivateProfileInt(L"DelayStart", L"min", 0, L".\\config.ini");
int config_delaystartmax = GetPrivateProfileInt(L"DelayStart", L"max", 0, L".\\config.ini");

int config_delayreleasemin = GetPrivateProfileInt(L"DelayRelease", L"min", 0, L".\\config.ini");
int config_delayreleasemax = GetPrivateProfileInt(L"DelayRelease", L"max", 0, L".\\config.ini");

//tap interval
int config_singletapinterval = GetPrivateProfileInt(L"SingleTapInterval", L"milliseconds", 0, L".\\config.ini");

//100's to issue
int config_total100s = GetPrivateProfileInt(L"100sToIssue", L"100s", 0, L".\\config.ini");
