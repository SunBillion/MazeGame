#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<windows.h>
#include<conio.h >

typedef struct Maze Maze;

struct Maze{
    int Map[200][200];
    int Player_position[2];
    int Exit_position[2];
    int map_high;
    int map_wide;
    int room_num;
    int road_num;
    int roomroad_num;
};

void gotoxy(int, int);//改變游標位置 
void clrscr();//清除畫面 
void printline(int, char*);//文字動化 
void printMap (Maze*);//印出整張地圖 
void creatMap (Maze*, int, int);//生成地圖 
void makeRoad (Maze*, int, int);//生成道路 
void creatRoom(Maze*, int, int , int, int);//生成房間 
void printCamera (Maze*, int, int, int, int);//印出部分地圖(位置，範圍) 
void playerMove (Maze*, char);//角色移動 



void gotoxy(int xpos, int ypos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = ypos; scrn.Y = xpos;
	SetConsoleCursorPosition(hOuput,scrn);
}

void clrscr()
{
	gotoxy(0, 0);
	int i;
	for(i=0; i<10*10; i++)printf(" ");
	gotoxy(0, 0);
}

void printline(int t, char s[])
{
	int screan_wide = 140, screan_high = 100, char_num = (screan_wide-20)/2;
	
	system("cls");
	
	printf("\n\n\n\n\n\n\n\n");
	
	int i, j, len;
	for(i=0, len=strlen(s); i<len; i++){
		if(i%80==0 || (i%80==1 && s[i]<0)){
			printf("\n");
			for(j =0; j<(screan_wide-char_num*2)/2; j++) printf(" ");
			if(len-i<80) for(j=0; j<(80-len%80)/2; j++) printf(" "); 
		}
		printf("%c", s[i]); 
		if(s[i]<0)  printf("%c", s[++i]); 
		Sleep(t);
	}
	
	Sleep(1000);
	printf("\n\n\n\n\n\n\n\n");
	
	s = "step space to continue ->";
	for(i=0, len=strlen(s); i<len; i++){
		printf("%c", s[i]); 
		Sleep(20);
	}
	
	char c = '\n';
	time_t count = clock();
	while((clock()-count)/(double)(CLOCKS_PER_SEC) < 0.2){
		gotoxy(screan_high-1, 0);
		getchar();
	}
} 

void printMap(Maze* map)
{
	//clrscr();
	//gotoxy(0, 0);
	printCamera(map, 0, 0, map->map_high, map->map_wide);
}

void creatMap(Maze* map, int high, int wide)
{
	//初始各項屬性 
	map->map_high = high;
	map->map_wide = wide;
	map->Player_position[0] = 2;
	map->Player_position[1] = 2;
	int i, j;
	for(i=1; i<high-1; i++) for(j=1; j<wide-1; j++) map->Map[i][j] = 0;
	map->room_num = 0;
	map->road_num = 0;
	map->roomroad_num = 0;
	
	//生成外圍 
	for(i=0; i<high; i++) map->Map[i][0]      = 3;
	for(i=0; i<high; i++) map->Map[i][wide-1] = 3;
	for(j=0; j<wide; j++) map->Map[0][j]      = 3;
	for(j=0; j<wide; j++) map->Map[high-1][j] = 3;
	
	printMap(map);
	
	//生成道路 
	makeRoad(map, 2, 2);
}


void makeRoad(Maze* map, int x, int y)
{
	if(map->Map[x+1][y]+map->Map[x-1][y]+map->Map[x][y+1]+map->Map[x][y-1]+map->Map[x][y]<=1){
		//Sleep(3);
		if(rand()%100<100){
			//printMap(map, true);
			map->Map[x][y] = 1;
			map->road_num++;
			if(rand()%100 < 10) creatRoom(map, x, y, 4, 4);
			/*gotoxy(x, y*2);
			printf("  ");*/ 
			if(rand()%2) 
				if(rand()%2) makeRoad(map, x-1, y); 
				else makeRoad(map, x+1, y);
			else
				if(rand()%2) makeRoad(map, x, y-1);
				else makeRoad(map, x, y+1);
			if(rand()%100 < 10) creatRoom(map, x, y, 4, 4);
			if(map->Map[x+2][y]+map->Map[x+3][y] == 0) 
			//if(rand()%100<70)
				makeRoad(map, x+1, y);
			if(map->Map[x-2][y]+map->Map[x-3][y] == 0) 
			//if(rand()%100<70)
				makeRoad(map, x-1, y);
			//creatRoom(x, y);
			if(map->Map[x][y+2]+map->Map[x][y+3] == 0) 
			//if(rand()%100<70)
				makeRoad(map, x, y+1);
			//creatRoom(x, y);
			if(map->Map[x][y-2]+map->Map[x][y-3] == 0) 
			//if(rand()%100<70)
				makeRoad(map, x, y-1);
			//creatRoom(x, y);
		}
	}
}

void creatRoom(Maze* map, int x, int y, int room_high, int room_wide){
	bool ok = true;
	int sum = 0;
	int i, j;
	for(i=x-(room_high-1);ok && i<=x+(room_high-1);i++){
		for(j=y-(room_wide-1);ok && j<=y+(room_wide-1);j++){
			if(map->Map[i][j] == 3) ok = false;
		}
	}
	for(i=x-(room_high-2);ok && i<x+(room_high-2);i++) 
		if(map->Map[i][y+(room_wide-2)]+map->Map[i+1][y+(room_wide-2)] == 2)ok = false;
	for(i=x-(room_high-2);ok && i<x+(room_high-2);i++) 
		if(map->Map[i][y-(room_wide-2)]+map->Map[i+1][y-(room_wide-2)] == 2)ok = false;
	for(j=y-(room_wide-2);ok && j<y+(room_wide-2);j++) 
		if(map->Map[x+(room_high-2)][j]+map->Map[x+(room_high-2)][j+1] == 2)ok = false;
	for(j=y-(room_wide-2);ok && j<y+(room_wide-2);j++) 
		if(map->Map[x-(room_high-2)][j]+map->Map[x-(room_high-2)][j+1] == 2)ok = false;
	if(ok){
		map->Exit_position[0] = x;
		map->Exit_position[1] = y;
		for(i=x-(room_high-3);i<=x+(room_high-3);i++){
			for(j=y-(room_wide-3);j<=y+(room_wide-3);j++){
				map->Map[i][j]=1;
				/*gotoxy(i, j*2);
				printf("  ");*/
			}
		}
		/*
		if(rand()%5 && map->Map[x+(room_high)][y]==0){
			map->Map[x+(room_high-2)][y]=1;
			makeRoad(map, x+(room_high-1), y);
		}
		if(rand()%5 && map->Map[x-(room_high)][y]==0){
			map->Map[x-(room_high-2)][y]=1;
			makeRoad(map, x-(room_high-1), y);
		}
		if(rand()%5 && map->Map[x][y+(room_wide)]==0){
			map->Map[x][y+(room_wide-2)]=1;
			makeRoad(map, x, y+(room_wide-1));
		}
		if(rand()%5 && map->Map[x][y-(room_wide)]==0){
			map->Map[x][y-(room_wide-2)]=1;
			makeRoad(map, x, y-(room_wide-1));
		}
		*/
		/*int sum = 0; 
		for(i=x-(room_high-2);i<=x+(room_high-2);i++){
			for(j=y-(room_wide-2);j<=y+(room_wide-2);j++){
				sum += map->Map[i][j];
			}
		}*/
		//roadofroom += sum-3*25;
		//room_num++;
	}
}

void printCamera (Maze* map, int x, int y, int high, int wide)
{
	system("cls");
	int i, j;
	for(i=x; i<x+high; i++){
		for(j=y; j<y+wide; j++){
			if(i==map->Player_position[0] && j==map->Player_position[1]){
				printf("Ek");
			}else if(i==map->Exit_position[0] && j==map->Exit_position[1]){
				printf("{}");
			}else{
				switch(map->Map[i][j]){
					case 0:
						printf("[]"); break;					
					case 1:
						printf("  "); break;
					case 3:
						printf("  "); break;
				}
			}
		}
		printf("\n");
	}
}


void playerMove(Maze* map, char ch){
    switch(ch){
    	case 'w':
    		if(map->Map[map->Player_position[0]-1][map->Player_position[1]]==1)
    			map->Player_position[0]--;
    		break;
    	case 'a':
    		if(map->Map[map->Player_position[0]][map->Player_position[1]-1]==1)
    			map->Player_position[1]--;
    		break;
    	case 's':
    		if(map->Map[map->Player_position[0]+1][map->Player_position[1]]==1)
    			map->Player_position[0]++;
    		break;
    	case 'd':
    		if(map->Map[map->Player_position[0]][map->Player_position[1]+1]==1)
    			map->Player_position[1]++;
    		break;
	}
	int x = map->Player_position[0];
	int y = map->Player_position[1]; 
	if(x < 13){
		x = 0;
	}else{
		x -= 13;
	}
	if(y < 13){
		y = 0;
	}else{
		y -= 13;
	}
	printCamera(map, x, y, 25, 25);
}
