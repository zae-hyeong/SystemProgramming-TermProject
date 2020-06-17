#include<stdio.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

typedef struct _player {
	char name[10];
	int power;
}Player;

int CreatChar(int fd);
int SearchChar(int fd, Player *p);
int SelectChar(int fd);

int main() {
	int select, fd, t;
	printf("시스템 프로그래밍 텀 프로젝트. Team 6 : RPG 게임\n");
	Player play;
	Player tmp;
	while(1) {
		printf(" 실행할 메뉴를 선택해 주세요\n");
		printf("1. 캐릭터 생성   2. 캐릭터 목록   3. 캐릭터 선택  4. 종료\n\n선택 : ");
		scanf("%d", &select);
		
		//fd = open("list.bin", O_RDWR | O_CREAT |O_APPEND, S_IRWXU | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
		
		switch(select) {
			case 1 :
				fd = open("list.bin", O_RDWR | O_CREAT |O_APPEND, S_IRWXU | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
				CreatChar(fd);
				break;
			
			case 2 :
				fd = open("list.bin", O_RDWR | O_CREAT, S_IRWXU | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
				SearchChar(fd, NULL);
				printf("\n");
				break;	
			case 3 :
				fd = open("list.bin", O_RDWR | O_CREAT, S_IRWXU | S_IRUSR | S_IWGRP | S_IRGRP | S_IROTH);
				SelectChar(fd);
				//wait(NULL);
				break;
			case 4 :
				printf("프로그램을 종료합니다\n");
				return 0;
			default :
				printf("잘못 선택하셨습니다. 다시 선택해주세요\n");
		}
		close(fd);
	}
}

int CreatChar(int fd) {
	system("clear");
	char name[11];
	int i, yes;
	Player p;

	printf("캐릭터 생성 화면으로 이동했습니다.\n");
	while(1) {
		printf("생성할 캐릭터의 이름을 입력해주세요(최대 10자) : ");
		scanf("%s", p.name);
		printf("%s로 괜찮으신가요?(괜찮으면 1입력)", p.name);
		scanf("%d", &yes);
		if(SearchChar(fd, &p)) {
			printf("같은 이름의 캐릭터가 존재합니다. 다시 입력해주세요.\n");
			continue;
		}
		if(yes == 1) break;
	}
	p.power = 10;
	
	write(fd, &p, sizeof(Player));
	
	printf("캐릭터 생성 완료. 잠시후 메인창으로 이동합니다.\n");
	for(i=3; i>0; i--) {
		fprintf(stderr, "%d... ", i);
		sleep(1);
	}
	system("clear");
	return 1;
}

int SearchChar(int fd, Player *p) { //캐릭터를 검색하는 함수. 캐릭터가 존재하면 1, 없으면 0 리턴
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
		else if(strcmp(tmp.name, p->name)==0) {//이름이 같은게 있으면 1 리턴
			p->power=tmp.power;
			return 1;
		}
	}
}

int Powerup(int fd, Player *p) { //캐릭터를 검색해서 공격력 올리는  함수. 캐릭터가 존재하면 1, 없으면 0 리턴
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
		else if(strcmp(tmp.name, p->name)==0) {//이름이 같은게 있으면 1 리턴
			tmp.power = p->power;
			lseek(fd, -1*sizeof(Player), SEEK_CUR);
			printf("이름 : %s, 공격력 : %d", tmp.name, tmp.power);
			if(write(fd, &tmp, sizeof(Player))==-1)
				perror("공격력 업데이트 실패");
			return 1;
		}
	}
}

int SelectChar(int fd) {
	int i, powerup;
	Player player;
	int n;
	pid_t childpid;
 	char power[11];
	printf("캐릭터 선택 창입니다. 플레이 하실 캐릭터의 이름을 입력하세요\n");	
	while(1) {
		printf("선택>>");
		scanf("%s", player.name);
		if(SearchChar(fd, &player)) {
			fprintf(stderr, "%s님 어서오세요. 지금부터 RPG의 세계로 들어갑니다.\n", player.name);
			for(i=3; i>0; i--) {
				fprintf(stderr, "%d... ", i);
				sleep(1);
			}
			//전투 프로세스 시작
			system("clear");
			while(1) {
				printf("\n실행할 행동을 고르세요.\n 1.사냥 2. 보스 사냥 3.로그아웃\n");
                                scanf("%d", &n);
                                if(n==3)
                                        
					return 1;
                                else if(n==2) {
					SearchChar(fd, &player);
					sprintf(power, "%d", player.power);
                                        childpid = fork();
                                        if (childpid == -1) {
                                                perror(" 보스 프로세스 부르기 실패");
                                                exit(0); //

                                        }
                                        // 보스 프로세스
                                        if(childpid == 0) {
                                                //sprintf(power, "%d", player.power);
						printf("%s",power);
                                                execl("./Boss_Stage_Process", "Boss_Stage_Process", player.name, power, NULL);
                                        }
                                        if (childpid != wait(NULL)) {
                                                perror("메인 프로세스가 보스 프로세스 기다리기 실패");
                                                exit(0);
                                        }
				}
				else if(n==1){
					printf("\n사냥 전 공격력: %d\n\n", player.power);

					SearchChar(fd, &player);
					sprintf(power, "%d", player.power);
                                        childpid = fork();
                                        if (childpid == -1) {
                                                perror(" 사냥 프로세스 부르기 실패");
                                                exit(0); // 
                                        }
                                        // 사냥 프로세스
                                        if(childpid == 0) {
                                                execl("./hunt_process", "hunt_process", NULL);
                                        }
                                        if (childpid != wait(NULL)) {
                                                perror("메인 프로세스가 사냥  프로세스 기다리기 실패");
                                                exit(0);
                                        }
					srand(time(NULL));
					powerup = rand()%10 + 50;  // 공격력 올라가는 수치 50~60
					printf("\n사냥으로 올라간 공격력: %d\n\n", powerup);
					player.power += powerup;
					printf("현재 공격력: %d \n\n", player.power);
					
					lseek(fd, (off_t)0, SEEK_SET);
					Powerup(fd, &player);
				}
				else {
					printf("잘못 선택하셨습니다. 다시 선택하십시오.\n");
					continue;
				}
				
			}
			exit(0);
		}
		else {
			lseek(fd, (off_t)0, SEEK_SET);
			printf("해당 이름의 캐릭터가 없습니다. 다시 입력해 주세요.\n");
		}
	}
}
