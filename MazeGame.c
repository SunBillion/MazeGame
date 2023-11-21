#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<windows.h>
#include<conio.h >

#include"maze.h"

int main(){
	srand(time(NULL));
	
	printf("\n\n\n\n                         step to strat");
	getchar();
	
	Maze map1;
	//creatMap(&map1, 111, 111);
	creatMap(&map1, 27, 27);
	
	printMap(&map1);
	
	char ch;
	while (1){
		if (_kbhit()){ //如果有按鍵按下，則_kbhit()函數傳回真   
            ch = _getch(); //使用_getch()函數取得按下的鍵值
            playerMove(&map1, ch);
            if(ch == 27) break;//當按下ESC時循環，ESC鍵的鍵值時27.
            printMap(&map1);
            Sleep(10);
        }
	}
	
	system ("pause") ;
	return 0;
} 
