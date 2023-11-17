#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>


#include"maze.h"


int Map[100][100] , map_high, map_wide, Player_position[2] = {1, 1}, room_num, roadofroom, road_num;


void creatRoom(int x, int y){
	bool ok = true;
	int sum = 0, room_high = 5, room_wide = 5;
	for(int i=x-(room_high-1);ok && i<=x+(room_high-1);i++){
		for(int j=y-(room_wide-1);ok && j<=y+(room_wide-1);j++){
			if(Map[i][j] == 3)ok = false;
		}
	}
	for(int i=x-(room_high-2);ok && i<x+(room_high-2);i++) 
		if(Map[i][y+(room_wide-2)]+Map[i+1][y+(room_wide-2)] == 2)ok = false;
	for(int i=x-(room_high-2);ok && i<x+(room_high-2);i++) 
		if(Map[i][y-(room_wide-2)]+Map[i+1][y-(room_wide-2)] == 2)ok = false;
	for(int j=y-(room_wide-2);ok && j<y+(room_wide-2);j++) 
		if(Map[x+(room_high-2)][j]+Map[x+(room_high-2)][j+1] == 2)ok = false;
	for(int j=y-(room_wide-2);ok && j<y+(room_wide-2);j++) 
		if(Map[x-(room_high-2)][j]+Map[x-(room_high-2)][j+1] == 2)ok = false;
	if(ok){
		for(int i=x-(room_high-3);i<=x+(room_high-3);i++){
			for(int j=y-(room_wide-3);j<=y+(room_wide-3);j++){
				Map[i][j]=3;
				/*gotoxy(j*2, i);
				printf("  ");*/
			}
		}
		int sum = 0; 
		for(int i=x-(room_high-2);i<=x+(room_high-2);i++){
			for(int j=y-(room_wide-2);j<=y+(room_wide-2);j++){
				sum += Map[i][j];
			}
		}
		roadofroom += sum-3*25;
		room_num++;
	}
}

void makeRoad(int x, int y){
	if(Map[x+1][y]+Map[x-1][y]+Map[x][y+1]+Map[x][y-1] <= 1){
		//Sleep(3);
		if(rand()%100<95){
			//print();
			Map[x][y] = 1;
			road_num ++;
			if(rand()%100 < 95) creatRoom(x, y);
			/*gotoxy(y*2, x);
			printf("  "); */
			if(rand()%2) makeRoad(x-1+rand()%3, y);
			else makeRoad(x, y-1+rand()%3);
			if(Map[x+1][y]+Map[x+2][y] == 0) 
			creatRoom(x, y);
			//if(rand()%100<70)
				makeRoad(x+1, y);
			if(Map[x-1][y]+Map[x-2][y]== 0) 
			//if(rand()%100<70)
				makeRoad(x-1, y);
			//creatRoom(x, y);
			if(Map[x][y+1]+Map[x][y+2]== 0) 
			//if(rand()%100<70)
				makeRoad(x, y+1);
			//creatRoom(x, y);
			if(Map[x][y-1]+Map[x][y-2]== 0) 
			//if(rand()%100<70)
				makeRoad(x, y-1);
			//creatRoom(x, y);
		/*}else if(r<40){
			makeRoad(x+1, y);
		}else if(r<60){
			makeRoad(x-1, y);
		}else if(r<80){
			makeRoad(x, y+1);
		}else{
			makeRoad(x, y-1);*/
		}
	}
}

void creatMap(int high, int wide){
	map_high = high;
	map_wide = wide;
	
	
	for(int i=0; i<map_high; i++) Map[i][0]          = 3;
	for(int i=0; i<map_high; i++) Map[i][map_wide-1] = 3;
	for(int j=0; j<map_wide; j++) Map[0][j]          = 3;
	for(int j=0; j<map_wide; j++) Map[map_high-1][j] = 3;
	
	do{
		//print();
		for(int i=1; i<map_high-1; i++) for(int j=1; j<map_wide-1; j++) Map[i][j] = 0;
		room_num = 0;
		road_num = 0;
		roadofroom = 0;
		makeRoad(2, 2);
		//print();
		printf("%d %d",roadofroom, room_num);
		//Sleep(5000);
	}while(roadofroom<30);
}

int main(){
	srand(time(NULL));	
		creatMap(40, 40);
		print();
		gotoxy(0, map_high+2);
	/*int n;
	scanf("%d",&n);
	for(int i=0;i<n;i++){
		printf("\n%d\n",i+1);
	}	 */
} 
