#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<stdbool.h>
#include<windows.h>
#include<conio.h >

#include"maze.h"


Player player1 = {"Ekiro", 1000, 1000, 6, 10, 1000, 1000, 0, 0, 10, 0};
Backpack backpack1 = {0, {0}};
time_t start_time;
bool is_win, is_die;


int main(){																																	
    srand(time(NULL));
	start_time = time(NULL);
	
	prelude();
	
	PlayGame(50, 50);
	
	ending();		
	
	
	system ("pause") ;
	return 0;
}

