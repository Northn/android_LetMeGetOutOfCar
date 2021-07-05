#ifndef ALLOG_H
#define ALLOG_H

#include <android/log.h>

#define LOG(...) __android_log_print(ANDROID_LOG_INFO, "LMGOOC", __VA_ARGS__)
#define VERBOSE(...) __android_log_print(ANDROID_LOG_VERBOSE, "LMGOOC", __VA_ARGS__)
#define WARN(...) __android_log_print(ANDROID_LOG_WARN, "LMGOOC", __VA_ARGS__)
#define ERROR(...) __android_log_print(ANDROID_LOG_ERROR, "LMGOOC", __VA_ARGS__)
#define FATAL(...) __android_log_print(ANDROID_LOG_FATAL, "LMGOOC", __VA_ARGS__)

#endif // ALLOG_H
