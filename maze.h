#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<windows.h>
#include<conio.h >


/*

**�`�N**

�y�Ъ�ܮ榡

	(0,0)-------->(0,n)j��V
	|
	|
	|
	|
	|
	|
	(n,0)
	i��V

*/

//�x�s�g�c������T��struct
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

//�x�s�����O���������T��struct
typedef struct Player Player;
struct Player{
	char* name;
	int max_hp, max_mp, max_level, levelup_exp;
	int hp, mp, level, exp, attack, defense;
};

//�x�s�I�]���D���T��struct
typedef struct Backpack Backpack;
struct Backpack{
	int tool_num;
	int tool[100];
};

extern Player player1;
extern Backpack backpack1;
extern time_t start_time;
extern bool is_win, is_die;


void gotoxy(int, int);//���ܴ�Ц�m()
void clrscr();//�M���e�� (����system("cls")�N�n)
void printline(int, char*);//��r�ʵe
void printMap (Maze*);//�L�X��i�a��
void creatMap (Maze*, int, int);//�ͦ��a��
void makeRoad (Maze*, int, int);//�ͦ��D��
void creatRoom(Maze*, int, int , int, int);//�ͦ��ж�
void printCamera (Maze*, int, int, int, int);//�L�X�����a��(��m�A�d��)
void playerMove (Maze*, char);//���Ⲿ��
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
		if (_kbhit()){ //�p�G��������U�A�h_kbhit()��ƶǦ^�u
            ch = _getch(); //�ϥ�_getch()��ƨ��o���U�����
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

	//���ܦr�ʵe
	s = "step enter to continue ->";
	for(i=0, len=strlen(s); i<len; i++){
		printf("%c", s[i]);
		Sleep(20);
	}

	//�T�M�ſ�J�w�İ�
	char c = '\n';
	time_t count = clock();
	while((clock()-count)/(double)(CLOCKS_PER_SEC) < 0.2){
		gotoxy(13, 0);//�T�w��Ц�m
		getchar();
	}
}

void printMap(Maze* map){
	//clrscr();
	//gotoxy(0, 0);
	printCamera(map, 0, 0, map->map_high, map->map_wide);
}

void creatMap(Maze* map, int high, int wide){
	//��l�U���ݩ�
	map->map_high = high;
	map->map_wide = wide;
	map->Player_position[0] = 2;
	map->Player_position[1] = 2;
	int i, j;
	for(i=1; i<high-1; i++) for(j=1; j<wide-1; j++) map->Map[i][j] = 0;
	map->room_num = 0;
	map->road_num = 0;
	map->roomroad_num = 0;

	//�ͦ��~��
	for(i=0; i<high; i++) map->Map[i][0]      = 3;
	for(i=0; i<high; i++) map->Map[i][wide-1] = 3;
	for(j=0; j<wide; j++) map->Map[0][j]      = 3;
	for(j=0; j<wide; j++) map->Map[high-1][j] = 3;

	printMap(map);

	//�ͦ��D��
	makeRoad(map, 2, 2);

	//�]�w
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
    		printline(20, "�ާ@�覡\n\n\n   W - �W��\n   A - ����\n   S - �U��\n   D - �k��");
    		printline(20, "�a�ϲŸ�\n\n\n   Ek - ��_ù�]Ekiro�^\n   {} - �X�f�j��\n   MM - �Ǫ�\n   EE - �ƥ�");
			break;
    	case 'p'://�Ȱ�
    		system("cls");
    		printline(0, "||                               ");
    		break;
    	case 'r'://�^�_�I
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
	//�]�w���Y��m
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

	//�L�X�e��
	printCamera(map, x, y, screan_high, screan_wide);

}

void Monster(int n){
	Player monster;	
	system("cls");
	switch(n){
		case 0:
			printline(30, "�b�g�c�`�B�A��_ù�������M�T�_�_�٪��`���C���j���`�����H�q���v���B�{�A�⤤���ۥ��j���`���l�C���Ť��O�b�����W�{�{�A�`�����H�������{ģ�ۭ��֪��]�O�C�C�@�������H�۱j�P���`���A�a���]�H���_�ʡC��_ù�����ɨ�O��ĵ���A�����o�쭵�֤����������C");
			monster.name = "�`�����H";
			monster.attack = 15;
			monster.defense = 7;
			monster.max_hp = monster.hp = 500;
			monster.exp = 60;
			break;
		case 1:
			printline(30, "�b�@�ӭ��ְj�Y���A��_ù�P����@�}�L���A���G������ֳt���ʪ����Ŧb�a��C��M���A�@�����߱ӱ������Ŭ��y�̥X�{�b�L���e�C�e���o�ۭ��֪��u���A�⤤�����Ť��b�R�ʦp�y���A�����ۦ�_ù�C��_ù�ݭn�B�Φۤv�����֤O�q�P����ܡA�w���e�������`���A�ç�X���������|�C");
			monster.name = "���Ŭ��y��";
			monster.attack = 20;
			monster.defense = 5;
			monster.max_hp = monster.hp = 200;
			monster.exp = 50;
			break;
		case 2:
			printline(30, "�b��ժ��ж��̡A��_ù�J��F�@�s�p������է���C�e�̦b�Ť������_�R�A�C�@�����R�����o�X�v¡�����ߡC�M�ӡA�o�]�O�@�ئM�I�A�]���e�̯������L�Ǫ����ѧ��@�C��_ù�ݭn�b������է��몺�P�ɡA�����|������L�����@���Ǫ��C");
			monster.name = "��է���";
			monster.attack = 5;
			monster.defense = 0;
			monster.max_hp = monster.hp = 100;
			monster.exp = 20;
			break;
		case 3:
			printline(30, "���_ù���i�@�ӫշt�����Y�A��M���@�D�H�H�����v�B�{�C�������F�H�}�B�����A�X�{�A���W�ϩ������D���v�A�C�@�D�N��ۤ��P�������C��԰��}�l�A�������F�N�o�X���������i�����A�Φ����������߰g�c�C��_ù���������a���ӭ��������ǰ{�ק����A�ç��w�����ϰ�C");
			monster.name = "�������F";
			monster.attack = 10;
			monster.defense = 4;
			monster.max_hp = monster.hp = 200;
			monster.exp = 30;
			break;
		case 4:
			printline(30, "�b�@�ӥR���j�ۭ����Ŷ����A��_ù�J��F�୵���~�A�~��������l���֦�������Ͱg�ۭ��i����O�C�o�ح��i����v�T��_ù���P���A�ϥL�ݨ��۪����H�C��_ù�����̾a��ı�M�զ��A��쥿�T���D���A�קK�Q�୵���~�����i�g�b�C");
			monster.name = "�୵���~";
			monster.attack = 17;
			monster.defense = 6;
			monster.max_hp = monster.hp = 160;
			monster.exp = 50;
			break;
		case 5:
			printline(30, "���_ù�i�J�@�ӯ�o�������U�ɡA��M�������`�F�X�{�A�L�̴��g�O�֤�A�{�b�o���쥢�����֪��A�G�C�o�Ǥ`�F����o�X�O�H�߸H�����ߡA�v�T��_ù���믫�C��_ù�ݭn�b�Y�������ߤ�����ܾ����`�F����k�C");
			monster.name = "�����`�F";
			monster.attack = 12;
			monster.defense = 3;
			monster.max_hp = monster.hp = 100;
			monster.exp = 20;
			break;
		case 6:
			printline(30, "�b�g�c���̲`�B�A��_ù�J��F�ĭ��~�A�o�O�@�د���l���P�򭵼֤������ͪ��C�ĭ��~���O�q�P�g�c�����ߺ�K�۳s�A�e������ܾԳ������֮槽�C��_ù�ݭn���_�A���A����ĭ��~�Q�έ��֤��������ܶi��N�Q���쪺�����C");
			monster.name = " �ĭ��~";
			monster.attack = 20;
			monster.defense = 6;
			monster.max_hp = monster.hp = 180;
			monster.exp = 60;
			break;
		case 7:
			printline(30, "���_ù�Ө�@�Ӽs�j�����֤j�U�ɡA�a����M�_�ʰ_�ӡC�@�����j���_�ٳD�]�X�{�A���W�����Ŧb���v���{�{�C�D�]����s�y�j�P�����i�A�O��_ù���Hí�w���ߡC�L���������׭��i���w���ϰ�A�æb���ɾ������o��_�ٳD�]�C");
			monster.name = "�_�ٳD�]";
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
	a = "�D��(y/n)";
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
		    printline(30, "�b�g�c���Y�өж��A��_ù�J��F�@�쯫�����q���j�v�C\n");
		    printline(30, "�o��j�v�֦���H���q���ޥ��M�w����O�A�L��b�q���C�����w����⪺�|�ʡA���A����D�Ԫ̡C\n");
		    printline(30, "�L���b�@�i�j�Ѫ��ۮ�e�A�إ��`��C�o��j�v��ۤ@�y�ï몺���T�A�⤤��ۤ@�Ӹ˦��T�ة_���Ÿ����j�Ѳ��l\n");
		    
			char gamer;
		    int computer;
		    int result;
		    int gamerWins = 0;
		    int computerWins = 0;
		    int round = 1;
			while (gamerWins < 2 && computerWins < 2)
		    {
		        printf("                                       ======= �� %d �^�X =======\n", round);
		        printf("                                           A:�ŤM\n                                           B:���Y\n                                           C:��\n                                           D:�ڴN��\n");
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
		
		        printf("                                           �q���j�v�X-->");
		        switch (computer)
		        {
		            case 0:
		                printf("�ŤM\n");
		                break;
		            case 1:
		                printf("���Y\n");
		                break;
		            case 2:
		                printf("��\n");
		                break;
		        }
		        printf("                                           �A�X-->");
		        switch (gamer)
		        {
		            case '4':
		                printf("�ŤM\n");
		                break;
		            case '7':
		                printf("���Y\n");
		                break;
		            case '10':
		                printf("��\n");
		                break;
		        }
		
		        if (result == 6 || result == 7 || result == 11)
		        {
		            printf("                                           �o�^�X�AĹ�F...\n");
		            gamerWins++;
		        }
		        else if (result == 5 || result == 9 || result == 10)
		        {
		            printf("                                           �o�^�X�A��F����\n");
		            computerWins++;
		        }
		        else
		        {
		            printf("                                           �o�^�X����!\n");
		        }
		
		        round++;
		        printf("                                     =============================\n");
		    }
		
		    if (gamerWins > computerWins){
		        printf("                                           �A���MĹ�F...");
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
		        printf("                                           �A�n��᫢��");
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
	printline(60, "�m���ߤ��g�n");
	printline(60,"�y�����]���`�O���R�Ӭ��n�A�P���p���Ť@�˽����_�R�A�ϩ��u���ۤ@���u�����ֳ��C��_ù�A�@�ӹﭵ�֥R�����R���~���H�A�ߺD�b�y���s�����B�A��ť�P���U�֦����۫ߡC");
	printline(60,"���@�ӱߤW�A��L���B�b�y���s���W�ɡA���M���Pı��@�}���P�M�`�����ߡC�y���s���������A�L�o�{�@�쯫�����Ѫ̡A���W���o�۬P���A����@���^���_�S�Ÿ��������C");
	printline(60,"�P����g�b�Ѫ̪��y�W�A�Φ��@����w�A�ϩ��L�O�o���P�Ū��@�����C�u�~���H�A�A�����F�ӾU�A����P���쭵�֪��l��C�v�Ѫ̪��n�����O���j�L��骺�n���A�X�M�ӴI���O�q�C");
	printline(60,"��_ù��Y�a�`���ۦѪ̡C�u�A�P����F�a�A�y�����{�ܭ����ɨ�C�u���A�A�Q�襤���_�I�̡A����Ѷ}�o�����������C�v�Ѫ̪������{ģ�۬P���A�ϩ�ĭ�õۦt�z�����K�C");
	printline(60,"�Ѫ̱N�@���_�ͻ�����_ù�A�_�ͤW�^�O�۰{�{�����šC�u�o�O�q�����֤������_�͡A�u���A����Ѷ}���������C�v�Ѫ̷L���ۡA�P���U�A�L�����v�����H�h�A�ϩ��ĤJ�F�]�šC");
	printline(60,"��_ù�⤤�������_�ʹ��o�X�L�z�����~�A�P���������Ť]�^���ۡA�n���b�u�۵ۤ@���j�Ѫ��G�y�C�]�ߡA�L���b�y���s���W�A�M�w�l�H�ۭ��֪��I��A��W�F�q���������_�I�����C?�O�@���H���֤��W���_�ۮȵ{�A�y�����R�B�N�b�o�֦~���⤤���s´���������ֳ��C");

}

void ending(){
	if(is_win && !is_die){
		printline(60,"��_ù��L�̫᪺�g�c�ϰ�A��F�F�g�c���֤ߡA�@�y���j�����֤��ߡC�o��Ŧ�몺���x���o�۷ŷx�����~�A�õo�X�_�������֡C");
		printline(60,"��_ù�P����@�}�_�����O�q�A�L�|�_�⤤�������_�͡A�N�仴�����J���֤��ߡC�����_�Ͷi�J��Ŧ�A��Ӱg�c���b�o�X�ʤH�����֡A�ϩ��j�a�M�Ů𳣿ĤJ�F�۫ߤ����C");
		printline(60,"�H�ۭ��֤��ߪ���ʡA�y����]�}�l�ܤơC�쥻���t���ѪųQ���ꪺ�P���ӫG�A�P�P�����ϩ����ʵۭ��šC�y���s�����OŢ���o�X��A�������~�A��Ӥp��R���F�]�k���^��C");
		printline(60,"���֤��ߵo�X�̫�@�}�y�����۫ߡA��_ù�P����@�ѷŷx���O�q�`�J�ߤ��C�y�����~���̶}�l�w�I�A�L�̷P����F�o�����֩_�ݱa�Ӫ��r���M�M�ӡC");
		printline(60,"��_ù��U�����_�͡A���b�y���s���W�A����۳o�ӭ��s�R���;����p��C�L���D�A�o���_�I���ȱa�ӤF��y�����@�ϡA�����L�`��z�ѤF���֪��O�q�C");
		printline(60,"�y�������F�@�Ӱg�H�����֥P�ҡA�Ӧ�_ù�h�����F�ǻ��������֫_�I�̡C�L���_�I�i�൲���F�A���y�������֩_�ݱN�û��y�ǤU�h�C");		
		
		system("cls");
		printf("\n\n\n\n\n\n                                             �P��");
		Sleep(2000); 
		system("cls");
		
		int i, j;
		for(i=29; i>-60; i--){
			for(j=0; j<i; j++) printf("\n");
			if(i<0) for(j=0; j<4-(i*(-1)+3)%4; j++) printf("\n");
			if(i<30 && i>=0)   printf("                                             �C���s�{: ���|�q\n\n\n\n");
			if(i<26 && i>=-4)  printf("                                             �C���s�{: ���{��\n\n\n\n");
			if(i<22 && i>=-8)  printf("                                             �M�׺޲z: ���Ӯ�\n\n\n\n");
			if(i<18 && i>=-12) printf("                                             �C������: chatGPT\n\n\n\n");
			if(i<14 && i>=-16) printf("                                             �C���]�p: chatGPT\n\n\n\n");
			if(i<10 && i>=-20) printf("                                             �C���U��: chatGPT\n\n\n\n");
			if(i<6 && i>=-24)  printf("                                             �C���s�@: chatGPT\n\n\n\n");
			if(i<2 && i>=-28)  printf("                                             �g�c�ͦ�: ���|�q\n\n\n\n");
			if(i<-2 && i>=-32) printf("                                             ���Ǿ���: ���|�q\n\n\n\n");
			if(i<-6 && i>=-36) printf("                                             ���ⱱ��: ���|�q\n\n\n\n");
			if(i<-10 && i>=-40)printf("                                             �ƥ����: ���Ӯ�\n\n\n\n");
			if(i<-14 && i>=-44)printf("                                             ���ֹC��: ���Ӯ�\n\n\n\n");
			if(i<-18 && i>=-48)printf("                                             �C���ʵe: ���Ӯ�\n\n\n\n");
			if(i<-22 && i>=-52)printf("                                             ��r�ʵe: ���|�q\n\n\n\n");
			Sleep(300); 
			system("cls");
		}
		printline(60,"Thank you");
		
		
	}else{
		printline(60,"���Ѥ����ֶ}�l�j���A���H�ۥD�����̫�@���ä�C�b�L�����u���A�g�c�����H�}�l�ᦱ�A���Ť]�v���ƥh��m�C����P����@�}�L�Ϊ��O�q�A�L�k��ܦa�ԧ�ۥL�i�J���֪��`�W�C");
		printline(60,"�y�����~���P����@�Ѥ��w���^��A�P���������Ť]�ܱo���I�C�P����F�Y�ؤ��M�`���ܤơC");
		printline(60,"��_ù�����ɳv���H�h�A�̫�u�ѤU�@���·t�C�L�P����ۤv���Q���֤��Y�]���A�@�}�������Pı���H�ۺ��­��{�C�b�o���t�v���A�L���_�I�ŧi�F���ѡC");
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

