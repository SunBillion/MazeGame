#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<windows.h>
#include<conio.h >

#include"maze.h"

int main(){
	srand(time(NULL));
	
	printline(60, "�m���ߤ��g�n");
	
	printline(60, "�b�@�ӳQ�٬��u�M�n���a�v���_�ۤ��ꤤ�A�s�b�@�y�������g�c�A �Q�H�٬��u���ߤ��g�v�C�o�y�g�c�֦��ۯ��������֤O�q�A����v�T��Ӥ��ꪺ�M�ӡC�ǻ����A�u������q�L���ָ������_�I�̡A�~���F�g�c�֤ߡA�Ѷ}���ꪺ�R�B�����C");
	
	printline(60, "��_ù�]Ekiro�^�O�@��~�����_�I�̡A�L�Ӧ۳Q�s����¶���p��u�y����v�C�]�ߡA�y�����s���W�|��ۭ��ָ`�A�H�̻E�b�@�_�@���w�֮ɥ��C��_ù���b�s�������ȤW�A��ť�۬��������֡A�ߤ��R���۹ﭵ�֪����R�C");	
	
	printline(60, "�P�P�I�󪺩]�Ť��A�y���s���Q�O���I�G�A���R���OŢ�b�L�����n���A��U�A�������v�C�p��~����۵��R���A�ˡA�C�ӤH�y�W���v�����w���A�R�O�W���֤�t���X�������֦��C");
	
	printline(60, "�s�������������J��A�O�y�����H�x�A���o�۷L�z�����֤����C");
	
	printline(60, "��_ù���إ��Q�@�쯫�����Ѫ̧l�ޡA�b�y���s���@���A�@�쨭��j�Ѫ��T���Ѫ̧��b�@�i���Ȥl�W�A�����`��C�L�⤤���ۤ@���^���_�S�Ÿ��������C");
	
	printline(60, "��_ù�a��Ѫ̡A�o�{�L���������{�{�ۯ��������~�C�Ѫ̪`���ۦ�_ù�A�ϩ���ݳz�L���F��C");
	
	printline(60, "�Ѫ̰_���A���X�@����A�⤤�B�{�@�ӷL���������_�͡C�L���n���G�u�i�����_�I�̡A�o�O�q�����ְg�c���_�͡A�u���A������ܭ��֤��O�������C�v�H�ۦѪ̪��ܻy�A�����_�ʹ��o�X�ŷx�����~�A�ĤJ��_ù���⤤�C");
	
	printline(60, "�Ѫ̫��V�p����t���@���O�L�A���D�G�u���ְg�c���J�f�N�b���̡A���o���ȬO�_�I�A��O�@�����֪��ҥܡC�v��_ù�`�����O�L�A�ߤ��R���F�n�_�M�M�ߡC�L�M�w�ҵ{�A�h�Ѷ}���ְg�c�����������C");
	
	gotoxy(0, 0);
	
	Maze map1;
	//creatMap(&map1, 111, 111);
	creatMap(&map1, 100, 100);
	
	printCamera(&map1, 0, 0, 25, 25);
	
	char ch;
	bool win;
	while (1){
		if (_kbhit()){ //�p�G��������U�A�h_kbhit()��ƶǦ^�u   
            ch = _getch(); //�ϥ�_getch()��ƨ��o���U�����
            playerMove(&map1, ch);
            if(ch == 27) break;//����UESC�ɴ`���AESC�䪺��Ȯ�27.
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
