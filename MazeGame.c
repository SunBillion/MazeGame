#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<windows.h>
#include<conio.h >

#include"maze.h"

int main(){
	srand(time(NULL));
	
	printf("\n\n\n\n\n\n\n\n                         step to strat");
	getchar();
	
	Maze map1;
	//creatMap(&map1, 111, 111);
	creatMap(&map1, 27, 27);
	
	printMap(&map1);
	
	char ch;
	bool win;
	while (1){
		if (_kbhit()){ //�p�G��������U�A�h_kbhit()��ƶǦ^�u   
            ch = _getch(); //�ϥ�_getch()��ƨ��o���U�����
            playerMove(&map1, ch);
            if(ch == 27) break;//����UESC�ɴ`���AESC�䪺��Ȯ�27.
            printMap(&map1);
            Sleep(10);
            if(map1.Player_position[0]==map1.Exit_position[0] && map1.Player_position[1]==map1.Exit_position[1]){
            	win = true;
				break;
			}
        }
	}
	if(win){
		system("cls");
		printf("\n\n\n\n\n\n\n\n                         you win!!\n\n\n\n\n\n\n\n");
	}else{
		system("cls");
		printf("\n\n\n\n\n\n\n\n                         you die!!\n\n\n\n\n\n\n\n");
	}
	
	system ("pause") ;
	return 0;
} 
