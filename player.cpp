#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<vector>
#include<string>
#include<algorithm>
#include"head.h"
using namespace std;

extern int field[11][10];
extern vector<int> valid_moves;
extern int record[300];
extern int chase_record[300];
extern int turns;
extern int change_view;

extern int deep_limit;
extern int cuttab_index;

int initiate_field(){
	/*�������͵Ĵ��ţ�
	�콫��1��        �ڽ���11��
	��ʿ��2��        ��ʿ��12��
	���ࡡ3��        ���ࡡ13��
	�쳵��4��        �ڳ���14��
	����5��        ����15��
	���ڡ�6��        ���ڡ�16��
	�����7��        �ڱ���17�� */
	int tmp[11][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,14,15,13,12,11,12,13,15,14,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0,16, 0, 0, 0, 0, 0,16, 0,
		0,17, 0,17, 0,17, 0,17, 0,17,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 7, 0, 7, 0, 7, 0, 7, 0, 7,
		0, 0, 6, 0, 0, 0, 0, 0, 6, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 4, 5, 3, 2, 1, 2, 3, 5, 4,
	};
	for(int i = 0; i <= 10; ++i){
		for(int j = 0; j <= 9; ++j)
			field[i][j] = tmp[i][j];
	}
	turns = 0;
	return 0;
}

bool undo(){
	int temp = turns;
	initiate_field();
	while(turns < temp - 3){
		++turns;
		move(record[turns]);
	}
	turns++;
	return 0;
}

int players_turn(int color){ //��һغ�
	if(turns < 5){
		cout << "�������ӣ�����������һ��4λ�������λ�����������ң��ֱ��ʾ" << endl;
		cout <<	"Ҫ�ƶ������������ڵ���������������Ѿ��������������������Ϸ���������" << endl;
		cout <<	"Ҫ�ƶ�����λ�õ�������������" << endl;
		cout <<	"���磬�콫���һ��0595���Իس��������������ڰ�ƽ�壺8885���Իس���������" << endl;
		cout <<	"�����ϣ�����壬�벻Ҫ����������λ����ֱ�����롰undo���Իس��������������ֽ׶β�Ҫ���塱����" << endl;
		cout <<	"���岻���Ʋ�����������һֱ�ڵ���һ����" << endl;
		cout <<	"�����ϣ�������ӽǣ���ڷ��ӽǶԻ�������ֱ�����롰view���Իس�����������" << endl;
		cout << "�����ϣ��������Ϸ����ֱ�����롰quit���Իس�����������" << endl;
		cout << "�����ϣ��������Ϸ�����̣���ֱ�����롰save���Իس�������������";
		cout <<	"���̺󣬿��Խ��븴��ģʽ" << endl;
	}
	else
		cout << "�������룺";
	string inpu;
	cin >> inpu;
	if ((inpu.length() == 4) && (inpu[0] >= '0') && (inpu[0] <= '9') && (inpu[1] >= '1') && (inpu[1] <= '9') && (inpu[2] >= '0') && (inpu[2] <= '9') && (inpu[3] >= '1') && (inpu[3] <= '9')) {
		int cmd = 1000 * (inpu[0] - '0') + 100 * (inpu[1] - '0') + 10 * (inpu[2] - '0') + (inpu[3] - '0');
		int director = player_move(cmd);
		get_valid_moves(color, 0);
		vector<int>::iterator it = find(valid_moves.begin(), valid_moves.end(), director);
		if (it != valid_moves.end()) { //�ҵ�
			//cout << "director = " << director;	//for debug
			//system("pause");
			return director;
		}
		else {
			cout << "�Ƿ����ӣ�����������" << endl;
			return players_turn(color);
		}
	}
	else if ((inpu.length() == 4) && (inpu[0] == 'u') && (inpu[1] == 'n') && (inpu[2] == 'd') && (inpu[3] == 'o')) {
		undo();
		system("cls");
		print_field(field);
		return players_turn(color);
	}
	else if ((inpu.length() == 4) && (inpu[0] == 'v') && (inpu[1] == 'i') && (inpu[2] == 'e') && (inpu[3] == 'w')) {
		change_view = 1;
		system("cls");
		print_field(field);
		return players_turn(color);
	}
	else if ((inpu.length() == 4) && (inpu[0] == 'f') && (inpu[1] == 'a') && (inpu[2] == 's') && (inpu[3] == 't')) {
		--cuttab_index;
		return players_turn(color);
	}
	else if ((inpu.length() == 4) && (inpu[0] == 's') && (inpu[1] == 'l') && (inpu[2] == 'o') && (inpu[3] == 'w')) {
		++cuttab_index;
		return players_turn(color);
	}
	else if ((inpu.length() == 4) && (inpu[0] == 's') && (inpu[1] == 'a') && (inpu[2] == 'v') && (inpu[3] == 'e')) {
		return 0;
	}
	else if ((inpu.length() == 4) && (inpu[0] == 'q') && (inpu[1] == 'u') && (inpu[2] == 'i') && (inpu[3] == 't'))
		exit(0);
	else{
		cout << "�Ƿ����룡����������" << endl;
		return players_turn(color);
	}
}

void replay(){
	cout << "�Ƿ�������ģʽ��1Ϊ�ǣ�0Ϊ��" << endl;
	int mode = 0;
	cin >> mode;
	int ai_turn = 500;
	if (mode) {
		cout << "�ڵڼ����л�ΪAI��" << endl;
		cin >> ai_turn;
		cout << "deep_limit = ��" << endl;
		cin >> deep_limit;
		cout << "cuttab_index = ��" << endl;
		cin >> cuttab_index;
	}
	initiate_field();
	print_field(field);
	cout << "���̿�ʼ" << endl;
	while(turns < 297){
		++turns;
		if (turns >= ai_turn) {
			//system("pause");
			int director = AI(turns % 2);
			if (move_with_info(director) == 20)
				chase_record[turns] = 1;
			record[turns] = director;
			print_field(field);
		}
		else {
			if (!record[turns])
				break;
			if (!mode)
				system("pause");
			move(record[turns]);
			print_field(field);
		}
	}
	cout << "���̵��˽���" << endl;
	system("pause");
	return;
}