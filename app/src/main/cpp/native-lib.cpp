#include <elf.h>
#include <fcntl.h>
#include <jni.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "android_log.h"
#include "native-lib.h"
#include "encrypt.h"

//指定执行优先级  越小优先级越高
void init_1() __attribute__((constructor (3)));
void init_getString() __attribute__((constructor (2)));
void init_2() __attribute__((constructor (1)));

//指定section
__attribute__((section (".encrypt"))) jstring getString(JNIEnv *env) {
    static const char * txt __attribute__((section (".encrypt2"))) = "Andy Hello from C++";
    return env->NewStringUTF(txt);
}

void init_getString() {
    unsigned long base;
    LOGE("init_getString");
    base = getLibAddr();

    LOGE("init_getString base: %lu", base);
}

void init_1() {
    LOGE("init_1");
}

void init_2() {
    LOGE("init_2");
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_andy_androsecurity_jni_jni_stringFromJNIEncrypt(
        JNIEnv* env,
        jobject ) {
    return getString(env);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_andy_androsecurity_jni_jni_encryptSO(
        JNIEnv* env,
        jobject ) {
    std::string path = "/sdcard/Download/libnative_lib.so";
    return encrypt(path.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_andy_androsecurity_jni_jni_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    init_1();
    init_getString();
    init_2();
    return env->NewStringUTF(hello.c_str());
}

unsigned long getLibAddr(){
    unsigned long ret = 0;
    char name[] = "libnative-lib.so";
    char buf[4096], *temp;
    int pid;
    FILE *fp;
    pid = getpid();
    sprintf(buf, "/proc/%d/maps", pid);
    fp = fopen(buf, "r");
    if(fp == NULL)
    {
        LOGE("open failed");
        goto _error;
    }
    while(fgets(buf, sizeof(buf), fp)){
        if(strstr(buf, name)){
            temp = strtok(buf, "-");
            ret = strtoul(temp, NULL, 16);
            break;
        }
    }
_error:
    fclose(fp);
    return ret;
}

