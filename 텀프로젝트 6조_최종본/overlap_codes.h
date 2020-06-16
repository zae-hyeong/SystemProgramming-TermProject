/*
 * overlap_codes.h
 * 헤더파일 내 코드
 * 1. player 구조체
 * 2. searchChar(int fd, Player * p)
 * 3. list.bin 파일 접근 권한
 */
#ifndef __OVERLAP_CODES_H__
#define __OVERLAP_CODES_H__

#include<stdio.h>

#define LIST_BIN_FLAGS (O_RDWR | O_CREAT |O_APPEND, S_IRWXU | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH)
#define BLKSIZE 1024

typedef struct _player {
	char name[10];
	int power;
} Player;

int SearchChar(int fd, Player * p);
int copyfile(int fromfd, int tofd);
int printfmonster(int mon);
#endif
