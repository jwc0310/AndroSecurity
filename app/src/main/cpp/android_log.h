//
// Created by Andy on 2019/4/25.
//

#ifndef ANDROSECURITY_ANDROID_LOG_H
#define ANDROSECURITY_ANDROID_LOG_H
#include <android/log.h>

#define MODULE_NAME  "Andy encrypt"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, MODULE_NAME, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, MODULE_NAME, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, MODULE_NAME, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,MODULE_NAME, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,MODULE_NAME, __VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,MODULE_NAME, __VA_ARGS__)

#endif //ANDROSECURITY_ANDROID_LOG_H
