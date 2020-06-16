#include <stdio.h>
#include <errno.h>  // errno 설정 누가 햇어
#include <unistd.h>  // open~ 등
#include <stdlib.h> // exit(1)
#include <string.h> // strcmp
#include <fcntl.h>  // 권한
#include "overlap_codes.h"
int SearchChar(int fd, Player * p) { //캐릭터를 검색하는 함수. 캐릭터가 존재하면 1, 없으면 0 리턴
   int bytes;
   Player tmp;

   while(1) {
      if((bytes =read(fd, &tmp, sizeof(Player)))==-1) {
         perror("read fail");
         exit(1);
      }
      else if(bytes==0)  //읽은 바이트 수가 0이면 탈출 == 다 읽으면
         return 0;
      if(p==NULL) //p==NULL이면 캐릭터 리스트 출력
         printf("캐릭터 이름 : %-10s 공격력 : %-5d\n", tmp.name, tmp.power);
		
      /*이름이 같다면
       * 파워정보를 가져옴
       * 1반환
       */
      else if(strcmp(tmp.name, p->name)==0) {
         p->power=tmp.power;
         return 1;
      }
   }
}


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
		fprintf(stderr, "%c", buf[i]); 
}
