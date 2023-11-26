#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<windows.h>
#include<conio.h >

#include"maze.h"

int main(){
	srand(time(NULL));
	
	printline(60, "《音律之迷》");
	
	printline(60, "在一個被稱為「和聲之地」的奇幻王國中，存在一座神秘的迷宮， 被人稱為「音律之迷」。這座迷宮擁有著神秘的音樂力量，能夠影響整個王國的和諧。傳說中，只有能夠通過音樂解謎的冒險者，才能到達迷宮核心，解開王國的命運之謎。");
	
	printline(60, "艾奇羅（Ekiro）是一位年輕的冒險者，他來自被山脈環繞的小鎮「悠音鎮」。夜晚，悠音鎮的廣場上舉辦著音樂節，人們聚在一起共度歡樂時光。艾奇羅坐在廣場的長椅上，聆聽著美妙的音樂，心中充滿著對音樂的熱愛。");	
	
	printline(60, "星星點綴的夜空中，悠音廣場被燈光點亮，華麗的燈籠在微風中搖曳，投下璀璨的光影。小鎮居民穿著華麗的服裝，每個人臉上都洋溢著歡笑，舞臺上的樂手演奏出美妙的樂曲。");
	
	printline(60, "廣場中央的音符雕塑，是悠音鎮的象徵，散發著微弱的音樂之光。");
	
	printline(60, "艾奇羅的目光被一位神秘的老者吸引，在悠音廣場一角，一位身穿古老長袍的老者坐在一張木質椅子上，眼神深邃。他手中拿著一把鑲有奇特符號的長杖。");
	
	printline(60, "艾奇羅靠近老者，發現他的眼睛中閃爍著神秘的光芒。老者注視著艾奇羅，彷彿能看透他的靈魂。");
	
	printline(60, "老者起身，伸出一隻手，手中浮現一個微光的音符鑰匙。他輕聲說：「勇敢的冒險者，這是通往音樂迷宮的鑰匙，只有你能夠揭示音樂之力的奧秘。」隨著老者的話語，音符鑰匙散發出溫暖的光芒，融入艾奇羅的手中。");
	
	printline(60, "老者指向小鎮邊緣的一片叢林，說道：「音樂迷宮的入口就在那裡，但這不僅是冒險，更是一場音樂的啟示。」艾奇羅注視著叢林，心中充滿了好奇和決心。他決定啟程，去解開音樂迷宮的神秘謎底。");
	
	gotoxy(0, 0);
	
	Maze map1;
	//creatMap(&map1, 111, 111);
	creatMap(&map1, 100, 100);
	
	printCamera(&map1, 0, 0, 25, 25);
	
	char ch;
	bool win;
	while (1){
		if (_kbhit()){ //如果有按鍵按下，則_kbhit()函數傳回真   
            ch = _getch(); //使用_getch()函數取得按下的鍵值
            playerMove(&map1, ch);
            if(ch == 27) break;//當按下ESC時循環，ESC鍵的鍵值時27.
            //printMap(&map1);
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
