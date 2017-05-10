
#include "stdio.h"
#include "stdarg.h"
#include "time.h"

#ifdef IAMWINDOWS
#include "windows.h"
#else
#include "pthread.h"
#endif

// ���O���L�^���邽�߂̊֐��Q
#define LOG_UNKNOWN 0
#define LOG_CHATSERVER 1
#define LOG_CHATCLIENT 2
#define LOG_USER 3

#define LOG_ERR 1
#define LOG_WARNING 2
#define LOG_INFO 3
#define LOG_TEST 4
#define LOG_ATTACK 5
#define LOG_MAX 6

#define LOG_OK 0
#define LOG_NO 1

void initlog();
int mylog(int log_user, int log_type, const char* buffer, ...);
int mylogd(int log_user, int log_type, const char* buffer, ...);
void releaselog();// init�Ƒ�
void GetTimeYYYYNNDDHHMMSS (char *s);
