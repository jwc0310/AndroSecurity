//
// Created by Andy on 2019/4/25.
//
#include <fcntl.h>
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

#include "android_log.h"
#include "decrypt.h"

int decrypt(const char *path) {
    char name[15];
    unsigned int nblock;
    unsigned int nsize;
    unsigned long base;
    unsigned long text_addr;
    unsigned int i;

    Elf32_Ehdr *ehdr;
    Elf32_Shdr *shdr;

    base = getLibAddr();

    LOGE("base:%lu", base);
    ehdr = (Elf32_Ehdr *)base;              //获取elf header
    text_addr = base + ehdr->e_shoff;   //获取section地址

    nblock = ehdr->e_entry >> 16;
    nsize = ehdr->e_entry & 0xffff;

    LOGE("nblock = %d", nblock);

    if(mprotect((void *) base, 4096 * nsize, PROT_READ | PROT_EXEC | PROT_WRITE) != 0){
        LOGE("mem privilege change failed");
        return -1;
    }

    for(i=0;i< nblock; i++){
        char *addr = (char*)(text_addr + i);
        *addr = ~(*addr);
    }

    if(mprotect((void *) base, 4096 * nsize, PROT_READ | PROT_EXEC) != 0){
        LOGE("mem privilege change failed");
        return -1;
    }

    LOGE("Decrypt success");
    return 0;
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