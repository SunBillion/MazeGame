#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<windows.h>
#include<conio.h >


/*

**注意**

座標表示格式

	(0,0)-------->(0,n)j方向
	|
	|
	|
	|
	|
	|
	(n,0)
	i方向

*/

//儲存迷宮相關資訊的struct
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

//儲存角色能力素質相關資訊的struct
typedef struct Player Player;
struct Player{
	char* name;
	int max_hp, max_mp, max_level, levelup_exp;
	int hp, mp, level, exp, attack, defense;
};

//儲存背包內道具資訊的struct
typedef struct Backpack Backpack;
struct Backpack{
	int tool_num;
	int tool[100];
};

extern Player player1;
extern Backpack backpack1;
extern time_t start_time;
extern bool is_win, is_die;


void gotoxy(int, int);//改變游標位置()
void clrscr();//清除畫面 (先用system("cls")就好)
void printline(int, char*);//文字動畫
void printMap (Maze*);//印出整張地圖
void creatMap (Maze*, int, int);//生成地圖
void makeRoad (Maze*, int, int);//生成道路
void creatRoom(Maze*, int, int , int, int);//生成房間
void printCamera (Maze*, int, int, int, int);//印出部分地圖(位置，範圍)
void playerMove (Maze*, char);//角色移動
void Monster(int);
void Event();
void prelude();
void ending();
void expup(int);


void gotoxy(int xpos, int ypos){
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = ypos; scrn.Y = xpos;
	SetConsoleCursorPosition(hOuput,scrn);
}

void clrscr(){
	gotoxy(0, 0);
	int i, j;
	for(i=0; i<100; i++) for(j=0; j<100; j++) printf(" ");
	gotoxy(0, 0);
}

void PlayGame(int map_high, int map_wide){
	Maze map1;
	//creatMap(&map1, 111, 111);
	creatMap(&map1, map_high, map_wide);

	printCamera(&map1, 0, 0, 30, 50);
	char ch;
    is_win = is_die = false;

	while (!is_win && !is_die){
		if (_kbhit()){ //如果有按鍵按下，則_kbhit()函數傳回真
            ch = _getch(); //使用_getch()函數取得按下的鍵值
            playerMove(&map1, ch);
        	Sleep(10);

            switch(map1.Map[map1.Player_position[0]][map1.Player_position[1]]){
				case 5:
				    is_win =  true;
					return;
            		break;
            	case 6:
            		Monster(rand()%8);
            		map1.Map[map1.Player_position[0]][map1.Player_position[1]] = 1;
            		break;
            	case 7:
            		Event(0);
            		map1.Map[map1.Player_position[0]][map1.Player_position[1]] = 1;
            		break;
			}
        }
	}
}

void printline(int t, char s[]){
	int screan_wide = 250, screan_high = 25, char_num = (screan_wide-20)/2;

	system("cls");

	printf("\n\n\n\n\n\n\n\n");

	int i, j, len, nl = 0;
	for(i=0, len=strlen(s); i<len; i++){
		if(i%80==0 || (i%80==1 && s[i]<0)){
			printf("\n");
			nl++;
			for(j =0; j<(screan_wide-char_num*2)/2; j++) printf(" ");
			if(len-i<80) for(j=0; j<(80-len%80)/2; j++) printf(" ");
		}
		printf("%c", s[i]);
		if(s[i]<0)  printf("%c", s[++i]);
		Sleep(t);
	}

	Sleep(1000);
	for(i=0; i<screan_high-nl; i++)printf("\n");

	//提示字動畫
	s = "step enter to continue ->";
	for(i=0, len=strlen(s); i<len; i++){
		printf("%c", s[i]);
		Sleep(20);
	}

	//確清空輸入緩衝區
	char c = '\n';
	time_t count = clock();
	while((clock()-count)/(double)(CLOCKS_PER_SEC) < 0.2){
		gotoxy(13, 0);//固定游標位置
		getchar();
	}
}

void printMap(Maze* map){
	//clrscr();
	//gotoxy(0, 0);
	printCamera(map, 0, 0, map->map_high, map->map_wide);
}

void creatMap(Maze* map, int high, int wide){
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

	//設定
	for(i=1; i<high-1; i++) for(j=1; j<wide-1; j++) if(map->Map[i][j] == 0)map->Map[i][j] = 3;
	for(i=-2; i<=2; i++) for(j=-2; j<=2; j++) map->Map[map->Exit_position[0]+i][map->Exit_position[1]+j] = 1;
	map->Map[map->Exit_position[0]][map->Exit_position[1]] = 5;

}

void makeRoad(Maze* map, int x, int y){
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
		map->Map[x][y]=rand()%2+6;
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

void printCamera (Maze* map, int x, int y, int high, int wide){
	system("cls");
	//clrscr();
	int i, j;
	for(i=x; i<x+high; i++){
		for(j=y; j<y+wide; j++){
			if(i==map->Player_position[0] && j==map->Player_position[1]){
				printf("Ek");
			}else{
				switch(map->Map[i][j]){
					case 0:
						printf("[]"); break;
					case 1:
						printf("  "); break;
					case 3:
						printf("[]"); break;
					case 5:
						printf("{}"); break;
					case 6:
						printf("MM"); break;
					case 7:
						printf("EE"); break;
				}
			}
		}
		printf("\n");
	}
	printf("\n%s", player1.name);
	printf("\nhp %-3d / %-3d", player1.max_hp, player1.hp);
	printf("\nmp %-3d / %-3d", player1.max_mp, player1.mp);
	printf("\nAttack %-3d", player1.attack);
	printf("\nDefese %-3d", player1.defense);
	printf("\nlevel %-2d exp %-3d / %-3d", player1.level, player1.levelup_exp, player1.exp);
	printf("\n\n{m}enu     {b}ackpack     {p}ause     {h}elp     {r}etry");
	//printf("\ngame_time: %d", time(NULL)-start_time);
}

void playerMove(Maze* map, char ch){
    switch(ch){
    	case 'w':
    		if(map->Map[map->Player_position[0]-1][map->Player_position[1]] != 3)
    			map->Player_position[0]--;
    		break;
    	case 'a':
    		if(map->Map[map->Player_position[0]][map->Player_position[1]-1] != 3)
    			map->Player_position[1]--;
    		break;
    	case 's':
    		if(map->Map[map->Player_position[0]+1][map->Player_position[1]] != 3)
    			map->Player_position[0]++;
    		break;
    	case 'd':
    		if(map->Map[map->Player_position[0]][map->Player_position[1]+1] != 3)
    			map->Player_position[1]++;
    		break;
    	case 'h':
    		system("cls");
    		printline(20, "操作方式\n\n\n   W - 上移\n   A - 左移\n   S - 下移\n   D - 右移");
    		printline(20, "地圖符號\n\n\n   Ek - 艾奇羅（Ekiro）\n   {} - 出口大門\n   MM - 怪物\n   EE - 事件");
			break;
    	case 'p'://暫停
    		system("cls");
    		printline(0, "||                               ");
    		break;
    	case 'r'://回起點
    		system("cls");
    		printf("\n\n\n\n\n\n\n\n\n\n\n                               ");
			char s[] = "reset(y/n)";
			int i, len;
			for(i=0, len=strlen(s); i<len; i++){
				printf("%c", s[i]);
				Sleep(20);
			}
    		char c;
    		do{
    			c = getchar();
			}while(c!='y' && c!='n');
    		if(c == 'y'){
	    		map->Player_position[0] = 2;
	    		map->Player_position[1] = 2;
			}
    		break;
	}
	//設定鏡頭位置
	int screan_high = 30, screan_wide = 50;
	int x = map->Player_position[0];
	int y = map->Player_position[1];

	if(x < screan_high/2){
		x = 0;
	}else if(x > map->map_high-(screan_high+1)/2){
		x =  map->map_high-screan_high;
	}else{
		x -= screan_high/2;
	}

	if(y < screan_wide/2){
		y = 0;
	}else if(y > map->map_wide-(screan_wide+1)/2){
		y =  map->map_wide-screan_wide;
	}else{
		y -= screan_wide/2;
	}

	//印出畫面
	printCamera(map, x, y, screan_high, screan_wide);

}

void Monster(int n){
	Player monster;	
	system("cls");
	switch(n){
		case 0:
			printline(30, "在迷宮深處，艾奇羅的耳邊突然響起震撼的節奏。巨大的節奏巨人從陰影中浮現，手中握著巨大的節奏槌。音符之力在它身上閃爍，節奏巨人的雙眼閃耀著音樂的魔力。每一擊都伴隨著強烈的節奏，地面也隨之震動。艾奇羅必須時刻保持警惕，迎擊這位音樂之巨的攻擊。");
			monster.name = "節奏巨人";
			monster.attack = 15;
			monster.defense = 7;
			monster.max_hp = monster.hp = 500;
			monster.exp = 60;
			break;
		case 1:
			printline(30, "在一個音樂迴廊中，艾奇羅感受到一陣微風，似乎有什麼快速移動的音符在靠近。突然間，一隻身軀敏捷的音符狩獵者出現在他面前。牠散發著音樂的優雅，手中的音符之刃舞動如流水，攻擊著艾奇羅。艾奇羅需要運用自己的音樂力量與之對抗，預測牠的攻擊節奏，並找出反擊的機會。");
			monster.name = "音符狩獵者";
			monster.attack = 20;
			monster.defense = 5;
			monster.max_hp = monster.hp = 200;
			monster.exp = 50;
			break;
		case 2:
			printline(30, "在花調的房間裡，艾奇羅遇到了一群小巧的花調妖精。牠們在空中翩翩起舞，每一次飛舞都散發出治癒的音律。然而，這也是一種危險，因為牠們能夠為其他怪物提供庇護。艾奇羅需要在攻擊花調妖精的同時，抓住機會打擊其他受庇護的怪物。");
			monster.name = "花調妖精";
			monster.attack = 5;
			monster.defense = 0;
			monster.max_hp = monster.hp = 100;
			monster.exp = 20;
			break;
		case 3:
			printline(30, "當艾奇羅走進一個幽暗的走廊，突然間一道淡淡的光影浮現。音階幽靈以漂浮的姿態出現，身上彷彿有五道光影，每一道代表著不同的音階。當戰鬥開始，音階幽靈將發出音階的音波攻擊，形成美妙的音律迷宮。艾奇羅必須巧妙地按照音階的順序閃避攻擊，並找到安全的區域。");
			monster.name = "音階幽靈";
			monster.attack = 10;
			monster.defense = 4;
			monster.max_hp = monster.hp = 200;
			monster.exp = 30;
			break;
		case 4:
			printline(30, "在一個充滿迴旋音的空間中，艾奇羅遇到了轉音幻獸，外形類似獅子但擁有能夠產生迷幻音波的能力。這種音波能夠影響艾奇羅的感知，使他看到虛幻的景象。艾奇羅必須依靠直覺和耳朵，找到正確的道路，避免被轉音幻獸的音波迷惑。");
			monster.name = "轉音幻獸";
			monster.attack = 17;
			monster.defense = 6;
			monster.max_hp = monster.hp = 160;
			monster.exp = 50;
			break;
		case 5:
			printline(30, "當艾奇羅進入一個荒廢的音樂廳時，突然間噪音亡靈出現，他們曾經是樂手，現在卻受到失落音樂的詛咒。這些亡靈能夠發出令人心碎的音律，影響艾奇羅的精神。艾奇羅需要在淒美的音律中找到對抗噪音亡靈的方法。");
			monster.name = "噪音亡靈";
			monster.attack = 12;
			monster.defense = 3;
			monster.max_hp = monster.hp = 100;
			monster.exp = 20;
			break;
		case 6:
			printline(30, "在迷宮的最深處，艾奇羅遇到了融音獸，這是一種能夠吸收周圍音樂元素的生物。融音獸的力量與迷宮的音律緊密相連，牠能夠改變戰場的音樂格局。艾奇羅需要不斷適應，防止融音獸利用音樂元素的轉變進行意想不到的攻擊。");
			monster.name = " 融音獸";
			monster.attack = 20;
			monster.defense = 6;
			monster.max_hp = monster.hp = 180;
			monster.exp = 60;
			break;
		case 7:
			printline(30, "當艾奇羅來到一個廣大的音樂大廳時，地面突然震動起來。一條巨大的震撼蛇魔出現，身上的音符在光影中閃爍。蛇魔能夠製造強烈的音波，令艾奇羅難以穩定站立。他必須找到躲避音波的安全區域，並在恰當時機反擊這位震撼蛇魔。");
			monster.name = "震撼蛇魔";
			monster.attack = 21;
			monster.defense = 5;
			monster.max_hp = monster.hp = 190;
			monster.exp = 70;
			break;
	}
	
	int i, j, len;
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n\n                                           ");
	char* a = monster.name;
	for(i=0, len=strlen(a); i<len; i++){
		printf("%c", a[i]);
		Sleep(20);
	}
	printf("\n                                           ");
	a = "挑戰(y/n)";
	for(i=0, len=strlen(a); i<len; i++){
		printf("%c", a[i]);
		Sleep(20);
	}
	char c;
	do{
		c = getchar();
	}while(c!='y' && c!='n');
	
	if(c == 'n'){
		return;
	}


	int A[30][50] = {0};
	char ch;
	//for(j=0; j<50; j++) A[25][j] = 1;
	int score = 0;
	while(monster.hp>0 && player1.hp>0){
		int r = 10*(1+rand()%4);
		if(rand()%2 && A[1][r]+A[2][r]+A[3][r]+A[4][r] == 0) A[0][r] = 1;
		Sleep(200);
		if (_kbhit()){ 
        	ch = _getch();
        	switch(ch){
        		case 'd':
        			for(i=25; i>=0; i--) if(A[i][10] == 1) break;
        			if(i == 25){
        				monster.hp -= player1.attack-monster.defense;
					}else if(i==24 || i==26){
        				monster.hp -= player1.attack-monster.defense-2;
					}else{
						player1.hp -= monster.attack-player1.defense;
					}
					A[i][10] = 0;
        			break;
        		case 'f':
        			for(i=25; i>=0; i--) if(A[i][20] == 1) break;
        			if(i == 25){
        				monster.hp -= player1.attack-monster.defense;	
					}else if(i==24 || i==26){
        				monster.hp -= player1.attack-monster.defense-2;
					}else{
						player1.hp -= monster.attack-player1.defense;
					}
					A[i][20] = 0;
        			break;
        		case 'j':
        			for(i=25; i>=0; i--) if(A[i][30] == 1) break;
        			if(i == 25){
        				monster.hp -= player1.attack-monster.defense;
					}else if(i==24 || i==26){
        				monster.hp -= player1.attack-monster.defense-2;
					}else{
						player1.hp -= monster.attack-player1.defense;
					}
					A[i][30] = 0;
        			break;
        		case 'k':
        			for(i=25; i>=0; i--) if(A[i][40] == 1) break;
        			if(i == 25){
        				monster.hp -= player1.attack-monster.defense;
					}else if(i==24 || i==26){
        				monster.hp -= player1.attack-monster.defense-2;
					}else{
						player1.hp -= monster.attack-player1.defense;
					}
					A[i][40] = 0;
        			break;
			}
		}
		if(A[29][10]==1 || A[29][20]==1 || A[29][30]==1 || A[29][40]==1){
			player1.hp -= monster.attack-player1.defense;
		}
		system("cls");
		for(i=7; i<24; i++){
			for(j=0; j<50; j++){
				switch(A[i][j]){
					case 0:
						printf("  ");
						break;
					case 1:
						printf("[]");
						break;
				}
			}
			printf("\n");
		}
		printf("--------------------[d]-----------------[f]-----------------[j]-----------------[k]-------------------\n");
		for(i=25; i<30; i++){
			for(j=0; j<50; j++){
				switch(A[i][j]){
					case 0:
						printf("  ");
						break;
					case 1:
						printf("[]");
						break;
				}
			}
			printf("\n");
		}
		printf("\n%s", player1.name);
		printf("\nhp %-3d / %-3d", player1.max_hp, player1.hp);
		printf("\nmp %-3d / %-3d", player1.max_mp, player1.mp);
		printf("\nAttack %-3d", player1.attack);
		printf("\nDefese %-3d", player1.defense);
		printf("\nlevel %-2d exp %-3d / %-3d", player1.level, player1.levelup_exp, player1.exp);
		printf("\n\n%s", monster.name);
		printf("\nhp %-3d / %-3d", monster.max_hp, monster.hp);
		printf("\nAttack %-3d", monster.attack);
		printf("\nDefese %-3d", monster.defense);
		int B[30][50] = {0}; 
		for(i=1; i<30; i++) for(j=0; j<50; j++) if(A[i-1][j] == 1) B[i][j] = 1;
		for(i=0; i<30; i++) for(j=0; j<50; j++) A[i][j] = B[i][j];
	}
	if(player1.hp > 0){ 
		expup(monster.exp);
	}else{
		is_die = true;
	} 
}

void Event(int n){
	switch(n){
		case 0:
		    printline(30, "在迷宮的某個房間，艾奇羅遇到了一位神祕的猜拳大師。\n");
		    printline(30, "這位大師擁有驚人的猜拳技巧和預知能力，他能在猜拳遊戲中預測對手的舉動，征服任何挑戰者。\n");
		    printline(30, "他坐在一張古老的石桌前，目光深邃。這位大師穿著一流螢般的紫袍，手中抱著一個裝有三種奇異符號的古老盒子\n");
		    
			char gamer;
		    int computer;
		    int result;
		    int gamerWins = 0;
		    int computerWins = 0;
		    int round = 1;
			while (gamerWins < 2 && computerWins < 2)
		    {
		        printf("                                       ======= 第 %d 回合 =======\n", round);
		        printf("                                           A:剪刀\n                                           B:石頭\n                                           C:布\n                                           D:我就爛\n");
		        scanf(" %c%*c", &gamer);
		
		        switch (gamer)
		        {
		            case 'A':
		            case 'a':
		                gamer = '4';
		                break;
		            case 'B':
		            case 'b':
		                gamer = '7';
		                break;
		            case 'C':
		            case 'c':
		                gamer = '10';
		                break;
		            case 'D':
		            case 'd':
		                return;
		            default:
		                printf("                                           Please Go Die One Die!\n");
		                getchar();
		                system("cls");
		                return;
		        }
		
		        srand((unsigned)time(NULL));
		        computer = rand() % 3;
		        result = (int)(gamer - '0') + computer;
		
		        printf("                                           猜拳大師出-->");
		        switch (computer)
		        {
		            case 0:
		                printf("剪刀\n");
		                break;
		            case 1:
		                printf("石頭\n");
		                break;
		            case 2:
		                printf("布\n");
		                break;
		        }
		        printf("                                           你出-->");
		        switch (gamer)
		        {
		            case '4':
		                printf("剪刀\n");
		                break;
		            case '7':
		                printf("石頭\n");
		                break;
		            case '10':
		                printf("布\n");
		                break;
		        }
		
		        if (result == 6 || result == 7 || result == 11)
		        {
		            printf("                                           這回合你贏了...\n");
		            gamerWins++;
		        }
		        else if (result == 5 || result == 9 || result == 10)
		        {
		            printf("                                           這回合你輸了哈哈\n");
		            computerWins++;
		        }
		        else
		        {
		            printf("                                           這回合平手!\n");
		        }
		
		        round++;
		        printf("                                     =============================\n");
		    }
		
		    if (gamerWins > computerWins){
		        printf("                                           你竟然贏了...");
		        while(rand()%5){
		        	switch(rand()%3){
		        		case 0:
		        			expup(rand()%20);
		        			break;
		        		case 1:
		        			hpup(rand()%800);
		        			break;
		        		case 2:
		        			max_hpup(rand()%400);
		        			break;
		        		case 3:
		        			attackup(rand()%10);
		        			break;
		        		case 4:
		        			defenceup(rand()%3);
		        			break;
					}
				}
			}else{
		        printf("                                           你好爛喔哈哈");
			}
			Sleep(3000);
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			break;
	}
}

void prelude(){
	printline(60, "《音律之迷》");
	printline(60,"悠音鎮的夜晚總是寧靜而美好，星光如音符一樣翩翩起舞，彷彿彈奏著一首優美的樂章。艾奇羅，一個對音樂充滿熱愛的年輕人，習慣在悠音廣場漫步，聆聽星光下樂曲的旋律。");
	printline(60,"有一個晚上，當他漫步在悠音廣場上時，忽然間感覺到一陣不同尋常的音律。悠音廣場的中央，他發現一位神祕的老者，身上散發著星光，手持一把鑲有奇特符號的長杖。");
	printline(60,"星光投射在老者的臉上，形成一把光暈，彷彿他是這片星空的一部分。「年輕人，你的心靈敏銳，能夠感受到音樂的召喚。」老者的聲音像是風吹過樹梢的聲音，柔和而富有力量。");
	printline(60,"艾奇羅驚訝地注視著老者。「你感受到了吧，悠音鎮面臨變革的時刻。只有你，被選中的冒險者，能夠解開這個謎之奧秘。」老者的眼中閃耀著星光，彷彿蘊藏著宇宙的秘密。");
	printline(60,"老者將一把鑰匙遞給艾奇羅，鑰匙上鑲嵌著閃爍的音符。「這是通往音樂之謎的鑰匙，只有你能夠解開它的奧秘。」老者微笑著，星光下，他的身影漸漸淡去，彷彿融入了夜空。");
	printline(60,"艾奇羅手中的音符鑰匙散發出微弱的光芒，星光中的音符也回應著，好像在吟唱著一首古老的咒語。夜晚，他站在悠音廣場上，決定追隨著音樂的呼喚，踏上了通往未知的冒險之路。?是一場以音樂之名的奇幻旅程，悠音鎮的命運將在這少年的手中重新織成美妙的樂章。");

}

void ending(){
	if(is_win && !is_die){
		printline(60,"艾奇羅穿過最後的迷宮區域，抵達了迷宮的核心，一座巨大的音樂之心。這心臟般的器官散發著溫暖的光芒，並發出奇妙的音樂。");
		printline(60,"艾奇羅感受到一陣奇異的力量，他舉起手中的音符鑰匙，將其輕輕插入音樂之心。當音符鑰匙進入心臟，整個迷宮都在發出動人的音樂，彷彿大地和空氣都融入了旋律之中。");
		printline(60,"隨著音樂之心的轉動，悠音鎮也開始變化。原本昏暗的天空被燦爛的星光照亮，星星之間彷彿跳動著音符。悠音廣場的燈籠散發出更璀璨的光芒，整個小鎮充滿了魔法的氛圍。");
		printline(60,"音樂之心發出最後一陣悠揚的旋律，艾奇羅感受到一股溫暖的力量注入心中。悠音鎮的居民們開始歡呼，他們感受到了這場音樂奇蹟帶來的愉悅和和諧。");
		printline(60,"艾奇羅放下音符鑰匙，站在悠音廣場上，眺望著這個重新充滿生機的小鎮。他知道，這場冒險不僅帶來了對悠音鎮的拯救，還讓他深刻理解了音樂的力量。");
		printline(60,"悠音鎮成為了一個迷人的音樂仙境，而艾奇羅則成為了傳說中的音樂冒險者。他的冒險可能結束了，但悠音鎮的音樂奇蹟將永遠流傳下去。");		
		
		system("cls");
		printf("\n\n\n\n\n\n                                             致謝");
		Sleep(2000); 
		system("cls");
		
		int i, j;
		for(i=29; i>-60; i--){
			for(j=0; j<i; j++) printf("\n");
			if(i<0) for(j=0; j<4-(i*(-1)+3)%4; j++) printf("\n");
			if(i<30 && i>=0)   printf("                                             遊戲編程: 陳尚義\n\n\n\n");
			if(i<26 && i>=-4)  printf("                                             遊戲編程: 楊程恩\n\n\n\n");
			if(i<22 && i>=-8)  printf("                                             專案管理: 楊承恩\n\n\n\n");
			if(i<18 && i>=-12) printf("                                             遊戲除錯: chatGPT\n\n\n\n");
			if(i<14 && i>=-16) printf("                                             遊戲設計: chatGPT\n\n\n\n");
			if(i<10 && i>=-20) printf("                                             遊戲顧問: chatGPT\n\n\n\n");
			if(i<6 && i>=-24)  printf("                                             遊戲編劇: chatGPT\n\n\n\n");
			if(i<2 && i>=-28)  printf("                                             迷宮生成: 陳尚義\n\n\n\n");
			if(i<-2 && i>=-32) printf("                                             打怪機制: 陳尚義\n\n\n\n");
			if(i<-6 && i>=-36) printf("                                             角色控制: 陳尚義\n\n\n\n");
			if(i<-10 && i>=-40)printf("                                             事件機制: 楊承恩\n\n\n\n");
			if(i<-14 && i>=-44)printf("                                             音樂遊戲: 楊承恩\n\n\n\n");
			if(i<-18 && i>=-48)printf("                                             遊戲動畫: 楊承恩\n\n\n\n");
			if(i<-22 && i>=-52)printf("                                             文字動畫: 陳尚義\n\n\n\n");
			Sleep(300); 
			system("cls");
		}
		printline(60,"Thank you");
		
		
	}else{
		printline(60,"失敗之音樂開始迴盪，伴隨著主角的最後一絲掙扎。在他的視線中，迷宮的景象開始扭曲，音符也逐漸褪去色彩。身體感受到一陣無形的力量，無法抵抗地拉扯著他進入音樂的深淵。");
		printline(60,"悠音鎮的居民感受到一股不安的氛圍，星光中的音符也變得陰沉。感受到了某種不尋常的變化。");
		printline(60,"艾奇羅的視界逐漸淡去，最後只剩下一片黑暗。他感受到自己仿佛被音樂之墜吞噬，一陣失重的感覺伴隨著漆黑降臨。在這片暗影中，他的冒險宣告了失敗。");
	}
}


void expup(int n){
	player1.exp += n;	
	
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n\n                                           xep + %d", n);
	
	while(player1.exp > player1.levelup_exp){
		player1.exp -= player1.levelup_exp;
		player1.level++;
		player1.max_hp += 100;
		player1.hp = player1.max_hp;
		player1.max_mp += 100;
		player1.mp = player1.max_mp;
		
		player1.levelup_exp = 10*(player1.level+1);
	}
}

void hpup(int n){
	player1.hp += n;	
	player1.hp %= player1.max_hp;
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n\n                                           hp + %d", n);
}

void max_hpup(int n){
	player1.max_hp += n;	
	
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n\n                                           max_hp + %d", n);
}

void attackup(int n){
	player1.attack += n;	
	
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n\n                                           attack + %d", n);
}

void defenceup(int n){
	player1.defense += n;	
	
	system("cls");
	printf("\n\n\n\n\n\n\n\n\n\n\n                                           defence + %d", n);
}

