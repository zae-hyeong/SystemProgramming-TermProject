/*
  작성자: 201912325박찬규
  내용: mutex 와 condition variable을 통하여 번갈아 실행되는 스레드 구현
*/ 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <fcntl.h>
#include "overlap_codes.h"
/*
 * argv[0] : "Boss_Stage_Process"
 * argv[1] : 아이디 char string - 크기 10바이트
 * argv[2] : power
 */

static pthread_mutex_t lock_m = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t lock_c = PTHREAD_COND_INITIALIZER;
static int check_num =1 ;
static int life=0;
static int boss_life=0;
//userThread가 실행할 함수
static void *doUserCommand(void *arg)
{	
   char ** info = (char**)arg;
   int power = atoi(info[2]);
   int health = 1001;
   int boss_power=100;

   fprintf(stderr, "%s님의 공격!!!\n", info[1]);
   fprintf(stderr, "보스에게 %d 피해를 입힙니다!!\n", power);
   sleep(1);
   
   pthread_mutex_lock(&lock_m);
   while(1) {
      while( check_num != 0 )  // userThread의 차례까 아니라면 기다린다.
         pthread_cond_wait(&lock_c, &lock_m);
      if( boss_life == 1 ) {  // 보스 몬스터가 체력이 0 이하라면 종료
         fprintf(stderr, "승리했습니다!!!");
         break;
      }

      fprintf(stderr, "보스에게 %d 피해를 입습니다!!\n", boss_power);
      health -= boss_power;
      if(health <=0) {  // 캐릭터의 체력이 0 이하라면
         life=1;  // 캐릭터가 죽음을 표시
         check_num=1;  // bossThread가 실행할 수 있도록 함
         fprintf(stderr, "패배했습니다....\n\n");
         pthread_cond_signal(&lock_c);  // bossThread를 깨운다.
         break;
      }
      fprintf(stderr, "남은 체력 %d!!!!!\n\n\n", health);
      sleep(1);
      fprintf(stderr, "%s님의 공격!!!\n", info[1]);
      fprintf(stderr, "보스에게 %d 피해를 입힙니다!!\n", power);
      sleep(1);

      sleep(1);
      check_num=1;
      pthread_cond_signal(&lock_c);
   }
   pthread_mutex_unlock(&lock_m);

   return NULL;
	
}
// bossThread가 실행할 함수
void *doBossCommand(void *arg)
{
   int fd_boss_img = open("boss_img", O_RDWR |O_APPEND);
   int fd_boss_img_sick = open("boss_img_sick", O_RDWR |O_APPEND);
   int boss_health = 1000;

   char ** info = arg;
   int power = atoi(info[2]);
  
   int i=0;

   sleep(1);
   pthread_mutex_lock(&lock_m);
   while(1) {
      while( check_num != 1 )  // bossThread의 차례가 아니라면 기다린다.
         pthread_cond_wait(&lock_c, &lock_m);
      if(life==1)  // 캐릭터의 체력이 모두 떨어졌다면 종료한다.
         break;
      for(i=0;i<3;i++) {
         system("clear");
	 printfmonster(fd_boss_img);
         lseek(fd_boss_img, (off_t)0, SEEK_SET);
	 system("clear");
         printfmonster(fd_boss_img_sick);
         lseek(fd_boss_img_sick, (off_t)0, SEEK_SET);
      }
      fprintf(stderr, "\n\n보스몬스터가 공격받았다!!!\n\n");
      boss_health-=power;
      fprintf(stderr, "보스 체력 %d!!!!!!\n\n", boss_health);

      if(boss_health<=0) {  // 보스몬스터의 체력 값이 0 이하라면
	 boss_life=1;  // 보스 사망
	 check_num=0; // userThread 가 실행될 수 있도록 함 
	 pthread_cond_signal(&lock_c); // userThread를 깨운다.
         break;
      }
      sleep(1);

      for(i=0;i<3;i++) {
         system("clear");
	 printfmonster(fd_boss_img);
         lseek(fd_boss_img, (off_t)0, SEEK_SET);
	 system("clear");
         printfmonster(fd_boss_img_sick);
         lseek(fd_boss_img_sick, (off_t)0, SEEK_SET);
      }
      fprintf(stderr, "\n\n보스몬스터가 공격한다!!!\n\n");
      sleep(1);

      check_num=0;  // userThread가 실행될 수 있도록 함
      pthread_cond_signal(&lock_c); // userThread를 깨운다
   }
   pthread_mutex_unlock(&lock_m);

   close(fd_boss_img);
   close(fd_boss_img_sick);
   return NULL;
}
	      



int main(int argc, char * argv[])
{
	pthread_t userThread, bossThread;
       
	fprintf(stderr, "Boss Stage를 시작합니다!!!\n\n");
        sleep(2);

        pthread_create(&userThread, NULL, doUserCommand,argv);
	pthread_create(&bossThread, NULL, doBossCommand,argv);

	pthread_join(userThread, NULL);
	pthread_join(userThread, NULL);
	fprintf(stderr, "complete");
        return 0;
}
