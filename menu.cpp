#include<iostream>
#include<fstream>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
//#include<windows.h>
#include<vector>
#include<algorithm>
#include"head.h"
#include <locale.h>
using namespace std;

int field[11][10]; //�洢���ڵ����״̬����(1,1)��ʼ
int record[300]; //���ף���1��ʼ
int chase_record[300];	//���ڼ�¼�Ƿ���׽�ţ��Ա��жϳ�׽
int turns = 0; //����
extern int deep_limit;	//ż������������
int view = 0;
int change_view = 0;
vector<int> valid_moves;

void delay(int del){
	for(int i = 0; i < del; ++i){
		for(int j = 0; j < del; ++j){
			for(int k = 0; k < del; ++k)
				int num = 54321 * 321;
		}
	}
	return;
}

int read_file() {	//����
	char filename[30];
	cout << "��������֮ǰ������ļ������ԡ�file����ʼ���ԡ�.dat��������������30���ַ���" << endl;
	scanf("%s", filename);
	ifstream fin(filename, ios::binary);
	for (int i = 0; i < 300; ++i)
		fin.read((char*)&record[i], sizeof(int));
	fin.close();
	return 0;
}

int write_file() {	//����
	char filename[30];
	cout << "��Ϊ���½����ļ��������ԡ�file����ʼ���ԡ�.dat����������Ҫ����30���ַ���" << endl;
	scanf("%s", filename);
	ofstream fout(filename, ios::binary);
	for (int i = 0; i < 300; ++i)
		fout.write((char*)&record[i], sizeof(int));
	fout.close();
	return 0;
}

int main(){
	cout << "��������linux������" << endl;
	setlocale(LC_ALL,"zh_CN.GBK"); 
	for (int i = 0; i < 300; ++i) {
		record[i] = 0;
		chase_record[i] = 0;
	}
	int mode = 0;
	system("cls");
	cout << endl << "            ��ӭ�����й�������Ϸ  w          " << endl << endl << endl;
	cout << "               0��˫����ս               " << endl << endl;            
	cout << "               1�����ִ��               " << endl << endl;
	cout << "               2�����ִ��               " << endl << endl;
	cout << "               3������浵               " << endl << endl;
	cout << "               4��д���ֿ�               " << endl << endl;
	cout << "               5���ԱȲ���               " << endl << endl;
	cout << "            6������ѵ������              " << endl << endl;
	cout << endl << "��ӭ��ϵ��zhaotianyang@pku.edu.cn" << endl;
	cout << "�������Ӧ���֣��Իس�������" << endl;
	cin >> mode;
	if (mode == 6) {
		system("cls");
		for (int i = 1; i <= 65; ++i) {
			char filename[30];
			sprintf(filename, "%d", i);
			learning_gen_data(filename);
			usleep(2000000);
		}
		return 0;
	}
	if (mode == 7) {
		//one_hot_trans();
		//count();
		split();
		return 0;
	}
	int test_mode = 0;
	if (mode == 5) {
		cout << "����1��procutִ�죬0������alpha-betaִ��" << endl;	
		cin >> test_mode;
	}
	ai_start_table_init();
	if (mode != 3) {
		initiate_field();
		print_field(field);
		print_field(field);
		int director = 0;
		bool start_table_hit_flag = 1;
		while (turns < 297) {
			//�췽
			++turns;
			if ((mode == 1) || (mode == 4))
				director = players_turn(1);
			else {
				if (mode == 5) {
					if(turns <= 2)	//������ָ������
						director = players_turn(1);
					else {
						if (test_mode) {
							AI(1, 0);
							director = AI(1, 1);
						}
						else
							director = AI(1, 0);
					}
				}
				else if (start_table_hit_flag) {
					director = ai_start_table_search(director);
					if (!director) {
						director = AI(1);
						start_table_hit_flag = 0;
					}
				}
				else
					director = AI(1);
			}
			if (!director)
				break;
			if (move_with_info(director) == 20)
				chase_record[turns] = 1;
			record[turns] = director;
			if((turns <= 2) || (mode != 5))
				print_field(field);
			//�ڷ�
			++turns;
			if ((mode == 2) || (mode == 4))
				director = players_turn(0);
			else {
				if (mode == 5) {
					if (turns <= 2)	//������ָ������
						director = players_turn(0);
					else {
						if (test_mode)
							director = AI(0, 0);
						else {
							AI(0, 0);
							director = AI(0, 1);
						}						
					}
				}
				else if (start_table_hit_flag) {
					director = ai_start_table_search(director);
					if (!director) {
						director = AI(0);
						start_table_hit_flag = 0;
					}
				}
				else
					director = AI(0);
			}
			if (!director)
				break;
			if (move_with_info(director) == 20)
				chase_record[turns] = 1;
			record[turns] = director;
			if ((turns <= 2) || (mode != 5))
				print_field(field);
		}
		cout << "���Ƿ���Ҫ���̣���������1����������0���Իس�������" << endl;
		int save_flag = 0;
		cin >> save_flag;
		if(save_flag)
			write_file();
	}
	else if (mode == 3)
		read_file();
	if (mode == 4)
		save_to_start_table();
	cout << "���Ƿ���Ҫ���̣���������1����������0���Իس�������" << endl;
	int replay_flag = 0;
	cin >> replay_flag;
	if (replay_flag)
		replay();
	cout << "���Ƿ���Ҫ���̣���������1����������0���Իس�������" << endl;
	int save_flag = 0;
	cin >> save_flag;
	if (save_flag)
		write_file();
	cout << "��ӭ���´�������" << endl;
	system("pause");
	return 0;
}
