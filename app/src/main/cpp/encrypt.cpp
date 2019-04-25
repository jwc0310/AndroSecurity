//
// Created by Andy on 2019/4/25.
//
#include <fcntl.h>
#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "android_log.h"
#include "native-lib.h"

int encrypt(const char *path) {
    char target_section[] = ".encrypt";
    char *shstr = NULL;
    char *content = NULL;
    Elf32_Ehdr ehdr;
    Elf32_Shdr shdr;
    int i;
    unsigned int base, length;
    unsigned short nblock;
    unsigned short nsize;
    unsigned char block_size = 16;
    int fd;

    fd = open(path, O_RDWR);
    if(fd < 0){
        LOGE("open %s failed\n", path);
        goto _error;
    }

    if(read(fd, &ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)){
        LOGE("Read ELF header error");
        goto _error;
    }

    lseek(fd, ehdr.e_shoff + sizeof(Elf32_Shdr) * ehdr.e_shstrndx, SEEK_SET);
    if(read(fd, &shdr, sizeof(Elf32_Shdr)) != sizeof(Elf32_Shdr)){
        LOGE("Read ELF section string table error");
        goto _error;
    }

    if((shstr = (char *) malloc(shdr.sh_size)) == NULL){
        LOGE("Malloc space for section string table failed");
        goto _error;
    }

    lseek(fd, shdr.sh_offset, SEEK_SET);
    if(read(fd, shstr, shdr.sh_size) != shdr.sh_size){
        LOGE("Read string table failed");
        goto _error;
    }

    lseek(fd, ehdr.e_shoff, SEEK_SET);
    for(i = 0; i < ehdr.e_shnum; i++){
        if(read(fd, &shdr, sizeof(Elf32_Shdr)) != sizeof(Elf32_Shdr)){
            LOGE("Find section .text procedure failed");
            goto _error;
        }
        if(strcmp(shstr + shdr.sh_name, target_section) == 0){
            base = shdr.sh_offset;
            length = shdr.sh_size;
            LOGE("Find section %s\n", target_section);
            break;
        }
    }

    lseek(fd, base, SEEK_SET);
    content = (char*) malloc(length);
    if(content == NULL){
        LOGE("Malloc space for content failed");
        goto _error;
    }
    if(read(fd, content, length) != length){
        LOGE("Read section .text failed");
        goto _error;
    }

    nblock = length / block_size;
    nsize = base / 4096 + (base % 4096 == 0 ? 0 : 1);
    LOGE("base = %d, length = %d\n", base, length);
    LOGE("nblock = %d, nsize = %d\n", nblock, nsize);

    ehdr.e_entry = (length << 16) + nsize;
    ehdr.e_shoff = base;

    for(i=0;i<length;i++){
        content[i] = ~content[i];
    }

    lseek(fd, 0, SEEK_SET);
    if(write(fd, &ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)){
        LOGE("Write ELFhead to .so failed");
        goto _error;
    }

    lseek(fd, base, SEEK_SET);
    if(write(fd, content, length) != length){
        LOGE("Write modified content to .so failed");
        goto _error;
    }

    LOGE("Completed");
_error:
    free(content);
    free(shstr);
    close(fd);
    return 0;
}