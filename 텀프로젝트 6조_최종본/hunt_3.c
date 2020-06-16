#include <stdio.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include <unistd.h>
#include <errno.h>
#define BLKSIZE 1024
int copyfile(int fromfd, int tofd) { //p.143 copyfile1.c
   char *bp;
   char buf[BLKSIZE];
   int bytesread;
   int byteswritten = 0;
   int totalbytes = 0;

   for (  ;  ;  ) {
      while (((bytesread = read(fromfd, buf, BLKSIZE)) == -1) &&
             (errno == EINTR)) ;         
      if (bytesread <= 0)          
         break;
      bp = buf;
      while (bytesread > 0) {
         while(((byteswritten = write(tofd, bp, bytesread)) == -1 ) &&
              (errno == EINTR)) ; 
         if (byteswritten < 0)     
            break;
         totalbytes += byteswritten;
         bytesread -= byteswritten;
         bp += byteswritten;
      }
      if (byteswritten == -1)      
          break;
   }
   return totalbytes;
}

int printfmonster(int mon)
{
	int i = 0;
	int numread;
	char buf[BLKSIZE];
	numread = read(mon, buf, BLKSIZE);
	
	if(numread == -1)
	{
		perror("에러발생: ");
		return -1;
	}
	for(i=0; i<numread; i++)
		printf("%c", buf[i]); 
}
int minigame_hunt(int mon)
{
	int power;
	int count = 0;
	char monster, attack;

	printf("적절한 키를 입력하여 몬스터를 공격하세요!\n");
	//srand(time(NULL));

	while(1)
	{
		monster = rand()%26 + 'a';
		printf("\n%c\n", monster);
		scanf("%c", &attack);

		if(attack==monster)
		{
			printf("\n공격 성공!!\n\n");
			printfmonster(mon);
			count++;
			printf("%d회 공격\n\n", count);

			lseek(mon,(off_t)0, SEEK_SET);
			copyfile(mon, STDOUT_FILENO);
			
		}
		else
			printf("\n공격 실패! \n");

		if(count>=5) {
			printf("\n사냥  성공!! 결과는 다음과 같습니다.\n");
			return 0;
		}
		while(getchar() != '\n');
	}
	return -1;
}
int main(){
	int fd_mon_read, fd_mon_delete, fd_mon_dead;
	char buf_power[10];

	if((fd_mon_read = open("monster.bin", O_RDWR |O_APPEND))==-1) {
                perror("monter.bin 파일 오픈 실패");
                exit(-1);
        }
	if((fd_mon_dead = open("deadmonster.bin", O_RDWR |O_APPEND))==-1) {
                perror("deadmonter.bin 파일 오픈 실패");
                exit(-1);
        }
        if((fd_mon_delete = open("kill_monster.bin",  O_RDWR | O_CREAT |O_TRUNC  , S_IRWXU | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH))==-1) {
                perror("kill_mon 파일 오픈 실패");
                exit(-1);
        }	

	copyfile(fd_mon_read, fd_mon_delete);

	lseek(fd_mon_delete,(off_t)0, SEEK_SET);
	copyfile(fd_mon_delete, STDOUT_FILENO);

	if(minigame_hunt(fd_mon_delete) == -1)
		printf("미니게임 오류 발생. \n");

	lseek(fd_mon_delete,(off_t)0, SEEK_SET);
	copyfile(fd_mon_dead, fd_mon_delete);
	lseek(fd_mon_delete,(off_t)0, SEEK_SET);
	copyfile(fd_mon_delete, STDOUT_FILENO);

	 
	close(fd_mon_read);
	close(fd_mon_dead);
	close(fd_mon_delete);

	return 0;
}
