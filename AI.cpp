//����������ȫ�ֱ���
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<vector>
#include<map>
#include<queue>
#include<set>
#include<algorithm>

#include"head.h"
using namespace std;

int cnn_score(int color);

const char punctuation = ',';

extern int field[11][10];
extern vector<int> valid_moves;
int deep_limit = 4;
int cuttab_index = 0;
extern int turns;

int deep_limit_cnt = 0;
int eval = 0;

const bool debug = 0;
const bool debug2 = 1;
//const bool debug2 = 0;
const bool cache = 0;	//�Ƿ�ʹ��rectab_cache

const int time_limit_k = 3900000;
const int time_limit_t = 5;
float eat_k = 0.3;

const int cuttabR = 17;
const int cuttabJ = 10;
const float cuttab[cuttabR][13] = { 
 1, 1, 1.00, 1.00, 1.00, 0.04, 0.05, 0.05, 0.05, 0.35, 0.35, 0.45, 0.45,
 1, 1, 1.00, 1.00, 1.00, 0.06, 0.06, 0.06, 0.06, 0.35, 0.35, 0.45, 0.45,
 1, 1, 1.00, 1.00, 1.00, 0.08, 0.08, 0.08, 0.08, 0.35, 0.35, 0.45, 0.45,
 1, 1, 1.00, 1.00, 1.00, 0.10, 0.10, 0.10, 0.12, 0.35, 0.35, 0.45, 0.45,
 1, 1, 1.00, 1.00, 1.00, 0.12, 0.12, 0.15, 0.20, 0.35, 0.35, 0.45, 0.45,
 1, 1, 1.00, 1.00, 1.00, 0.17, 0.17, 0.20, 0.30, 0.35, 0.35, 0.45, 0.45,
 1, 1, 1.00, 1.00, 1.00, 0.22, 0.22, 0.25, 0.40, 0.50, 0.50, 0.70, 0.70,
 1, 1, 1.00, 1.00, 1.00, 0.30, 0.30, 0.30, 0.50, 0.55, 0.55, 0.75, 0.75,
 1, 1, 1.00, 1.00, 1.00, 0.40, 0.40, 0.40, 0.60, 0.60, 0.60, 0.80, 0.80,
 1, 1, 1.00, 1.00, 1.00, 0.50, 0.50, 0.50, 0.65, 0.65, 0.65, 0.82, 0.82,
 1, 1, 1.00, 1.00, 1.00, 0.60, 0.60, 0.60, 0.70, 0.70, 0.70, 0.85, 0.85,
 1, 1, 1.00, 1.00, 1.00, 0.70, 0.70, 0.70, 0.70, 0.70, 0.70, 0.85, 0.85,
 1, 1, 1.00, 1.00, 1.00, 0.75, 0.75, 0.75, 0.75, 0.75, 0.75, 0.85, 0.85,
 1, 1, 1.00, 1.00, 1.00, 0.80, 0.80, 0.80, 0.80, 0.80, 0.80, 0.85, 0.85,
 1, 1, 1.00, 1.00, 1.00, 0.85, 0.85, 0.85, 0.85, 0.85, 0.85, 0.85, 0.85,
 1, 1, 1.00, 1.00, 1.00, 0.90, 0.90, 0.90, 0.90, 0.90, 0.90, 0.90, 0.90,
 1, 1, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00
};

int decision = 0;
int deep_now = 0;
const int large_num = 1000000;
int maxmin[15];
int count_cal = 0;
int abcut = 0;	//��֦ϵ������ȷʱȡ0
float cut_k = 0.2;	//��֦ϵ������ȷʱȡ1
int hit_count = 0;

int score_king[11][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0, 1, 0, 0, 0,
		0, 0, 0, 0, 5, 5, 5, 0, 0, 0,
	};

int score_bishop[11][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 1, 0, 0, 0, 1, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,-3, 0, 0, 0, 6, 0, 0, 0,-3,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 2, 0, 0, 0, 2, 0, 0,
	};

int score_knight[11][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 4, 7, 8, 7, 6, 7, 8, 7, 4,
		0, 5, 9,15, 7, 7, 7,15, 9, 5,
		0, 7,10,12,14,12,14,12,10, 7,
		0, 4, 8, 6, 7, 5, 7, 6, 8, 4,
		0, 2, 8, 8, 8, 8, 8, 8, 8, 2,
		0, 2, 8, 9, 8, 8, 8, 9, 8, 2,
		0, 0, 7, 7, 6, 8, 6, 7, 7, 0,
		0, 0, 4, 3, 4, 4, 4, 3, 4, 0,
		0,-2, 2, 2, 2,-7, 2, 2, 2,-2,
		0,-3,-3, 0, 0, 0, 0, 0,-3,-3,
	};

int score_cannon[11][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,12, 9, 3, 0, 0, 0, 3, 9,12,
		0, 3, 2, 2, 3, 1, 3, 2, 2, 3,
		0, 3, 2, 1, 1, 2, 1, 1, 2, 3,
		0, 2, 2, 1, 1,12, 1, 1, 2, 2,
		0, 1, 1, 1, 1,11, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 7, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 4, 1, 1, 1, 1,
		0, 5, 5, 5, 6, 7, 6, 5, 5, 5,
		0, 0, 1, 1, 0, 1, 0, 1, 1, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

int score_rook[11][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,11,15, 6,15, 6,15, 6,15,11,
		0,10,15,12,16,10,16,12,15,10,
		0,10,11,11,12,10,12,11,11,10,
		0, 2,10, 2,10, 4,10, 2,10, 2,
		0, 2,10, 2,10, 2,10, 2,10, 2,
		0, 2,12, 2,15, 2,15, 2,12, 2,
		0, 2, 6, 2, 6, 2, 6, 2, 6, 2,
		0, 3, 7, 3, 7, 3, 7, 3, 7, 3,
		0, 4, 8, 4, 2, 2, 2, 4, 8, 4,
		0,-5, 6, 2, 8, 2, 8, 2, 6,-5,
	};

int score_pawn[11][10] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 1, 3, 6,11, 6,11, 6, 3, 1,
		0,18,22,27,30,30,30,27,22,18,
		0,18,22,27,32,32,32,27,22,18,
		0,17,20,22,27,27,27,22,20,17,
		0,16,17,21,25,25,25,21,17,16,
		0, 1, 0, 7, 0,12, 0, 7, 0, 1,
		0, 0, 0,-2, 0,12, 0,-2, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

//���º���ԭ�ͺ����ݽṹ����
int prejudge(int color, int deep, bool from_max);

/*����V2.0�����ĺ���������ݽṹ
	�ð汾����ּ��ͨ����¼��ķ�ʽ�洢�Ѿ�������Ľ���ֵ��
	�Ӷ����������ظ�����ͽ���ظ�������*/

class packed_board {	//ѹ���������棬����������״����˭����
private:
	int info[6];	/*	��6 * 32 = 192λ��ǰ96λ��¼���ӵ�λ�ã���96λ��¼���ӵ�λ��
			int[0]�Ӹ�λ����λ�ֱ�Ϊ�ڳ�1����2����1����2������7+7+7+7+4��
			int[1]�Ӹ�λ����λ�ֱ�Ϊ����1����2����1����2��ʿ1���������λ��1Ϊ�죩��7+7+7+7+3+1��
			int[2]�Ӹ�λ����λ�ֱ�Ϊ�ڱ�3����4����5��ʿ2����1����2����ȱλ��7+7+7+3+3+3+2��
			int[3]�Ӹ�λ����λ�ֱ�Ϊ�쳵1����2����1����2������7+7+7+7+4��
			int[4]�Ӹ�λ����λ�ֱ�Ϊ����1����2����1����2��ʿ1����ȱλ��7+7+7+7+3+1��
			int[5]�Ӹ�λ����λ�ֱ�Ϊ���3����4����5��ʿ2����1����2����ȱλ��7+7+7+3+3+3+2��
			λ�õı�ʾ����
			�κ�������0��ʾ��������
			�����ڱ�������i������j����λ�ñ���ʾΪ(10 * i + j)��
			�콫��(8,4),(8,5),(8,6),(9,4),(9,5),(9,6),(10,4),(10,5),(10,6)�ֱ𱻱�ʾΪ1-9��
			�ڽ���(1,4),(1,5),(1,6),(2,4),(2,5),(2,6),(3,4),(3,5),(3,6)�ֱ𱻱�ʾΪ1-9��
			��ʿ��(8,4),(8,6),(9,5),(10,4),(10,6)�ֱ𱻱�ʾΪ1-5��
			��ʿ��(1,4),(1,6),(2,5),(3,4),(3,6)�ֱ𱻱�ʾΪ1-5��
			���ࣺ(6,3),(6,7),(8,1),(8,5),(8,9),(10,3),(10,7)�ֱ𱻱�ʾΪ1-7��
			���ࣺ(1,3),(1,7),(3,1),(3,5),(3,9),(5,3),(5,7)�ֱ𱻱�ʾΪ1-7��
			*/
private:
	inline int shift_right(int val, int dgt) {	//��������λ
		if (dgt >= 0)
			return val >> dgt;
		return val << (-dgt);
	};
	inline void put(int val, int len, int arr_idx, int dgt_idx) {	//��val�ĺ�lenλ�Ž�info[arr_idx]����dgt_idxΪ���λ��λ������
		int tmp = ((int)0x80000000 >> (31 - dgt_idx)) ^ (shift_right((int)0x80000000, 31 - dgt_idx - len));
		info[arr_idx] |= tmp;
		info[arr_idx] &= ((~tmp) | (val << dgt_idx));
		return;
	};
	inline int get(int len, int arr_idx, int dgt_idx) {	//��info[arr_idx]����dgt_idxΪ���λ�ĳ�Ϊlen��ֵ����
		int tmp = ((int)0x80000000 >> (31 - dgt_idx)) ^ (shift_right((int)0x80000000, 31 - dgt_idx - len));
		tmp &= info[arr_idx];
		return ((unsigned)tmp) >> dgt_idx;
	};
	inline int get_value(int type, int i, int j) {	//����(i,j)��type�������ӵ�λ�ñ��뷵��
		switch (type) {
		case 1: return 3 * i + j - 27;	//�콫
		case 2: return (3 * i + j - 26) / 2;	//��ʿ
		case 3: return (4 * i + j) / 3 - 8;	//���࣬ȡ��������
		case 11: return 3 * i + j - 6;	//�ڽ�
		case 12: return (3 * i + j - 5) / 2;	//��ʿ
		case 13: return (4 * i + j - 1) / 3 - 1;	//����
		default: return 10 * i + j;	//�������ڣ���
		}
	};
	inline int get_len(int type) {	//����type��������pack��ռ��λ��
		int len[18] = {0, 4, 3, 3, 7, 7, 7, 7, 0, 0, 0, 4, 3, 3, 7, 7, 7, 7};
		return len[type];
	};
	inline int get_arridx(int type, int id) {	//����type���͵�id��(��1��ʼ)����pack���arr_idx
		int arridx[18] = { 0, 3, 4, 5, 3, 3, 4, 4, 0, 0, 0, 0, 1, 2, 0, 0, 1, 1 };
		int tmp = arridx[type];
		if (((type == 2) || (type == 12)) && (id == 2))	//��2��ʿ
			++tmp;
		else if (((type == 7) || (type == 17)) && (id >= 3))	//��3��4��5����
			++tmp;
		return tmp;
	};
	inline int get_dgtidx(int type, int id) {	//����type���͵�id��(��1��ʼ)����pack���dgt_idx
		int dgtidx[17] = {0, 0, 1, 8, 5, 2, 25, 18, 11, 4, 25, 18, 11, 4, 25, 18, 11};
		return dgtidx[2 * (type % 10) + id - 3];
	};
public:
	void print() {
		for (int i = 0; i < 6; ++i)
			printf("%x ", info[i]);
		cout << endl;
		return;
	};
	void pack(const int board[][10], const int red_black) {	//��boardѹ�������1Ϊ��
		for (int i = 0; i < 6; ++i)
			info[i] = 0;
		int cnt[18] = {0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1 };
		for (int i = 1; i <= 10; ++i) {
			for (int j = 1; j <= 9; ++j) {
				int tmp = board[i][j];
				if (tmp) {	//������
					put(get_value(tmp, i, j), get_len(tmp), get_arridx(tmp, cnt[tmp]), get_dgtidx(tmp, cnt[tmp]));
					++cnt[tmp];
				}
			}
		}
		//�������λ��1Ϊ�죩
		put(red_black, 1, 1, 0);
		return;
	};
	void checked_pack(const int board[][10], const int red_black) {	//��unpack���飬����debug��Ч�ʺܵͣ���������ν
		this->pack(board, red_black);
		//unpack
		int unpacked[11][10];
		for (int i = 0; i < 11; ++i) {
			for (int j = 0; j < 10; ++j)
				unpacked[i][j] = 0;
		}
		int cnt[18] = { 0, 1, 2, 2, 2, 2, 2, 5, 0, 0, 0, 1, 2, 2, 2, 2, 2, 5 };
		for (int type = 0; type < 18; ++type) {
			if (cnt[type] > 0) {
				int val = get(get_len(type), get_arridx(type, cnt[type]), get_dgtidx(type, cnt[type]));
				if (val) {	//δ����
					int i, j;
					switch (type) {
					case 1:
						switch (val) {
						case 1:	i = 8; j = 4; break;
						case 2:	i = 8; j = 5; break;
						case 3:	i = 8; j = 6; break;
						case 4:	i = 9; j = 4; break;
						case 5:	i = 9; j = 5; break;
						case 6:	i = 9; j = 6; break;
						case 7:	i = 10; j = 4; break;
						case 8:	i = 10; j = 5; break;
						case 9:	i = 10; j = 6; break;
						default: cout << "pack err1!" << endl; system("pause");
						}
						break;
					case 2:
						switch (val) {
						case 1:	i = 8; j = 4; break;
						case 2:	i = 8; j = 6; break;
						case 3:	i = 9; j = 5; break;
						case 4:	i = 10; j = 4; break;
						case 5:	i = 10; j = 6; break;
						default: cout << "pack err1!" << endl; system("pause");
						}
						break;
					case 3:
						switch(val) {
						case 1:	i = 6; j = 3; break;
						case 2:	i = 6; j = 7; break;
						case 3:	i = 8; j = 1; break;
						case 4:	i = 8; j = 5; break;
						case 5:	i = 8; j = 9; break;
						case 6:	i = 10; j = 3; break;
						case 7:	i = 10; j = 7; break;
						default: cout << "pack err1!" << endl; system("pause");
						}
						break;
					case 11:
						switch (val) {
						case 1:	i = 1; j = 4; break;
						case 2:	i = 1; j = 5; break;
						case 3:	i = 1; j = 6; break;
						case 4:	i = 2; j = 4; break;
						case 5:	i = 2; j = 5; break;
						case 6:	i = 2; j = 6; break;
						case 7:	i = 3; j = 4; break;
						case 8:	i = 3; j = 5; break;
						case 9:	i = 3; j = 6; break;
						default: cout << "pack err1!" << endl; system("pause");
						}
						break;
					case 12:
						switch (val) {
						case 1:	i = 1; j = 4; break;
						case 2:	i = 1; j = 6; break;
						case 3:	i = 2; j = 5; break;
						case 4:	i = 3; j = 4; break;
						case 5:	i = 3; j = 6; break;
						default: cout << "pack err1!" << endl; system("pause");
						}
						break;
					case 13:
						switch (val) {
						case 1:	i = 1; j = 3; break;
						case 2:	i = 1; j = 7; break;
						case 3:	i = 3; j = 1; break;
						case 4:	i = 3; j = 5; break;
						case 5:	i = 3; j = 9; break;
						case 6:	i = 5; j = 3; break;
						case 7:	i = 5; j = 7; break;
						default: cout << "pack err1!" << endl; system("pause");
						}
						break;
					default:
						i = val / 10;
						j = val % 10;
					}
					unpacked[i][j] = type;
				}
				--cnt[type];
				if (cnt[type])
					--type;
			}
		}
		//compare
		for (int i = 0; i < 11; ++i) {
			for (int j = 0; j < 10; ++j) {
				if (board[i][j] != unpacked[i][j]) {
					cout << "pack err2!" << endl;
					system("pause");
					this->print();
					cout << "ori: " << endl;
					print_field(field);
					system("pause");
					cout << "pack: " << endl;
					print_field(unpacked);
					system("pause");
				}
			}
		}
		return;
	};
	friend const bool operator < (const packed_board& b1, const packed_board& b2) {	//map����С�ں�
		for (int i = 0; i < 6; ++i) {
			if (b1.info[i] < b2.info[i])
				return 1;
			if (b1.info[i] > b2.info[i])	//!!!!!!!!!
				return 0;
		}
		return 0;	//1�Ļ����κζ�����ȣ�Ӧ����0
	};
};

struct child_node {	//�ӽڵ������㣬�ݲ����á�
public:
	short value;
	child_node* next;	//NULL��ʾ����β��ע���ͷſռ䣡
};

class board_info {	//������Ϣ���ݲ������ӽڵ�����
private:
	int info[3];	/*ÿ��int��������ȣ�ֱ����score�����Ϊ1�������8λ����������24λ��
					val[0]Ϊ���Ϊ1��ֵ��val[1]Ϊm��val[2]ΪM*/
public:
	board_info(){
		for(int i = 0; i < 3; ++i)
			info[i] = 0;
	};
	int read_depth(int mode) {	//ֱ����score�����Ϊ1��
		int tmp = ((int)0x80000000) >> 7;
		return ((unsigned)(tmp & info[mode])) >> 24;
	};
	void write_depth(int mode, int val) {	//ֱ����score�����Ϊ1��
		int tmp = ((int)0x80000000) >> 7;
		info[mode] &= (~tmp);
		info[mode] |= (val << 24);
		return;
	};
	int read_score(int mode) {
		int tmp = ((int)0x80000000) >> 7;
		int res = info[mode] & (~tmp);
		if (res & 0x800000)	//�����Ĵ���������������������
			res |= 0xff000000;
		return res;
	};
	void write_score(int mode, int val) {
		int tmp = ((int)0x80000000) >> 7;
		info[mode] &= tmp;
		info[mode] |= val;
		return;
	};
	void make_blank(int mode) {
		info[mode] = 0;
		return;
	};
	bool is_blank(int mode) {
		if (info[mode] == 0)
			return 1;
		return 0;
	};
};

class record_table {	//��¼��
	//���ڼ�֦�ò�����ȷֵ�������Դ洢���½磡��
	//�Ż���������ֵķ����ж���Ҫ���ģ���֦�����趨���Ƽ�֦ϵ����Ӧ�ñ����ظ�����score��Ԥ����ͼ��㣩��2����3��֮��share�����ݿ�����߼�֦Ч�ʣ�push��find�ϲ����⽵��Ч�ʣ����ⳤ׽����debug........
private:
	map<packed_board, board_info> table;	//map���ú����ʵ�ֵģ�ʱ�临�Ӷ�O(lgn)
	queue<packed_board> table_seq;	//��ʱ��queue��ʵ���滻���ԣ��Ժ���Կ�����set
public:
	unsigned max_volume;	//�������
private:
	void traversemap() {
		map<packed_board, board_info>::iterator iter;
		cout << "traverse map: " << endl;
		for (iter = table.begin(); iter != table.end(); iter++) {
			packed_board tmp = iter->first;
			tmp.print();
		}
		cout << endl;
		return;
	};
	int power(int expo) {
		int res = 1;
		while (expo--)
			res *= 3;
		return res;
	};
public:
	record_table() {
		max_volume = cut_k * 250 * power(deep_limit);	//ʵ�������6000000��������޶ȴ�������ڴ�800Mb
		table.clear();
		while (!table_seq.empty())
			table_seq.pop();
		return;
	};
	board_info find(const int board[][10], const int red_black) {	//�ҵ�����board_info�����򷵻ؿյ�board_info���Ժ��Ż��Ļ����Է����ֱ�Ӳ��븳ֵ��1Ϊ������
		packed_board obj;
		obj.pack(board, red_black);
		map<packed_board, board_info>::iterator it = table.find(obj);
		if (it != table.end()) {	//�ҵ�
			board_info tmp = it->second;
			return tmp;
		}
		//û�ҵ�
		board_info blank;
		return blank;
	};
	void push(const int board[][10], const int red_black, const int depth, const int score, int mode ) {	//ѹ���¼��Ҳ�����ڸ������е�ֵ��red-blackͳһΪ1
		//�滻����
		if (table.size() >= max_volume) {	//��
			map<packed_board, board_info>::iterator it;
			do {
				packed_board pb = table_seq.front();
				table_seq.pop();
				it = table.find(pb);
			} while (it == table.end());	//do-while���ǵ���map��ɾ����seqδɾ����������޸����н��ֵ����
			table.erase(it);
		}
		//���²���
		board_info info;
		info.write_depth(mode, depth);
		info.write_score(mode, score);
		packed_board pboard;
		pboard.pack(board, red_black);
		map<packed_board, board_info>::iterator it = table.find(pboard);
		if (it == table.end()) {	//����û��
			table_seq.push(pboard);
			table[pboard] = info;
		}
		else {	//�����У��ж�һ����ȴ�С�Ƚϣ������Ƿ����
			if (it->second.read_depth(mode) < depth) {
				it->second.write_depth(mode, depth);
				it->second.write_score(mode, score);
			}
		}
		return;
	};
	int get_size() {
		return table.size();
	};
};

record_table rectab;

//����V1�еĺ����壬��V2.0�����˸���
//V2.1�н��е������Ԥ�м�֦(probcut)

//const int power_score[8] = {0, 100000, 21, 20, 110, 45, 45, 3};
//const int power_score[8] = { 0, 100000, 32, 30, 165, 69, 69, 8 };
const int power_score[9] = { 0, 100000, 22, 20, 165, 69, 69, 8, 0 };	//���һ����׽�ӷ�

int single_score(int color) {	//����
	++count_cal;	//for trial
	int score = 0;
	
	for (int i = 1; i <= 10; ++i) {
		for (int j = 1; j <= 9; ++j) {
			if (get_color(field[i][j]) == color) {
				int type = field[i][j] % 10;
				int ii = i;
				if (color == 0)
					ii = 11 - i;
				score += power_score[type];
				switch (type) {
				case 1: score += score_king[ii][j]; break;
				case 2: break;
				case 3: score += score_bishop[ii][j]; break;
				case 4: score += score_rook[ii][j]; break;
				case 5: score += score_knight[ii][j]; break;
				case 6: score += score_cannon[ii][j]; break;
				case 7: int plus = (turns >> 3); if (plus > 16) plus = 16; score += plus; score += score_pawn[ii][j]; break;	//��turns�йأ�����rectab��������
				}
			}
		}
	}
	return score;
	
	//V4.1 Update
	//return cnn_score(color);
}

int score(int color) {	//˫�ߣ���-��
	//board_info info = rectab.find(field, 1);
	//if (info.is_blank(0)) {	//û��������¼
		int red_score = single_score(1) - single_score(0);
	//	rectab.push(field, 1, 1, red_score, 0);
		if (color)	//��
			return red_score;
		else	//��
			return -red_score;
	//}
	//��������¼
	//int red_score = info.read_score(0);
	//if (color)	//��
	//	return red_score;
	//else	//��
	//	return -red_score;
}

int random(int color) {
	get_valid_moves(color);
	int size = valid_moves.size();
	//reference: http://zhidao.baidu.com/link?url=Vmc3IgT1G_Houa7--AGtE_16BPl8d5SbzGOspv9B-lSAcL8PY_QTjh5pthya1aE-EkZN9XKjG828AW0E9smwg6mhX5rQJr-npYGOlb_SKsO
	srand((unsigned)time(NULL)); //��ʱ�����֣�ÿ�β����������һ�� 
	int num = rand() % (size - 1);  //��������� 
	move(valid_moves[num]);
	return valid_moves[num];
}

int greedy(int color) {
	get_valid_moves(color);
	int size = valid_moves.size();
	int num = 0;
	int save[11][10];
	int max_score = -1000000;
	int max_num = size - 1;
	for (num = size - 1; num >= 0; --num) {
		for (int i = 0; i <= 10; ++i) {
			for (int j = 0; j <= 9; ++j)
				save[i][j] = field[i][j];
		}
		move(valid_moves[num]);
		int tmp = score(color);
		if (tmp > max_score) {
			max_score = tmp;
			max_num = num;
		}
		for (int i = 0; i <= 10; ++i) {
			for (int j = 0; j <= 9; ++j)
				field[i][j] = save[i][j];
		}
	}
	move(valid_moves[max_num]);
	return valid_moves[max_num];
}

struct move_info {
public:
	int director;
	int score;
	friend bool operator < (const move_info& x, const move_info& y) {
		return x.score < y.score;
	};
	move_info(int dir, int sco) : director(dir), score(sco) {};
};

int search_min(int color)	//a-b��֦DFS�����㷨
{
	int search_max(int color);
	//�Ƿ��������
	int king = 0;
	for (int i = 1; i <= 10; ++i) {
		for (int j = 1; j <= 9; ++j) {
			if (field[i][j] % 10 == 1)
				++king;
		}
	}
	if (king < 2 || deep_now >= deep_limit)
		return score(color);
	//�������������
	int min = large_num; //��������������߷��Ĺ�ֵ
	//����rectab
	if (cache) {
		board_info info = rectab.find(field, 1);
		if (color) {	//��
			if ((!info.is_blank(2)) && (info.read_depth(2) == 10 + deep_limit - deep_now)) {	//Ϊʲô>=���ԡ�����
				min = info.read_score(2);
				++hit_count;
			}
		}
		else {	//��
			if ((!info.is_blank(1)) && (info.read_depth(1) == 10 + deep_limit - deep_now)) {
				min = -info.read_score(1);
				++hit_count;
			}
		}
	}
	//�洢��ʼ״̬
	int save[11][10]; //���ڴ洢�µ���û������ʱ�ľ���
	for (int i = 1; i <= 10; ++i) {
		for (int j = 1; j <= 9; ++j)
			save[i][j] = field[i][j];
	}
	//���½���Ԥ���۾��棬���Ż�����˳�򣬴Ӷ�������֦Ч�ʣ��Ժ����һ��Ӧ���������ݣ���
	vector<move_info> list;
	get_valid_moves(1 - color);
	int size = valid_moves.size(); //һ�����е���������
	for (int p = 0; p < size; ++p)
		list.push_back(move_info(valid_moves[p], 0));
	valid_moves.clear();
	for (int p = 0; p < size; ++p) {
		int eat = move(list[p].director); //��������
		list[p].score = prejudge(color, deep_limit - deep_now, 0) - eat_k * power_score[eat % 10];	//���ڱ���һЩbug����������+ !!!!!
		for (int i = 1; i <= 10; ++i) { //����
			for (int j = 1; j <= 9; ++j)
				field[i][j] = save[i][j];
		}
	}
	//����
	sort(list.begin(), list.end());
	//����Ѱ��������ӵ�
	float cut = cuttab[cuttab_index][deep_limit - deep_now];
	for (int p = 0; p < cut * (float)size; ++p)
	{
		move(list[p].director);
		maxmin[deep_now] = min; //�洢�ڵ����ݣ�׼����֦
		++deep_now;
		int temp = search_max(color); //���ü���к���
		--deep_now; //����
		if (temp < min) //���ŵ����ӣ�С�ڵ��ں��ǲ��Եģ���ʹ��֦����
			min = temp;
		for (int i = 1; i <= 10; ++i) { //����
			for (int j = 1; j <= 9; ++j)
				field[i][j] = save[i][j];
		}
		for (int step = deep_now - 1; step >= 0; step = step - 2) { //��0�Ļ������Ľ��Ǿ�ȷ�ģ�����ÿ��������Ĺ�ֵ��׼ȷ	
			if (min <= maxmin[step] + abcut) { //alpha��֦
				//����rectab
				if (cache) {
					if (color)	//��
						rectab.push(field, 1, 10 + deep_limit - deep_now, min, 2);	//���Ǵ�1��ʼ��depth
					else	//��
						rectab.push(field, 1, 10 + deep_limit - deep_now, -min, 1);
				}
				return min;
			}
		}
	}
	//����rectab
	if (cache) {
		if (color) {	//��
			rectab.push(field, 1, 10 + deep_limit - deep_now, min, 2);	//���Ǵ�1��ʼ��depth
			rectab.push(field, 1, 10 + deep_limit - deep_now, min, 1);
		}
		else {	//��
			rectab.push(field, 1, 10 + deep_limit - deep_now, -min, 1);
			rectab.push(field, 1, 10 + deep_limit - deep_now, -min, 2);
		}
	}
	return min;
}

int search_max(int color)
{
	//�Ƿ��������
	int king = 0;
	for (int i = 1; i <= 10; ++i) {
		for (int j = 1; j <= 9; ++j) {
			if (field[i][j] % 10 == 1)
				++king;
		}
	}
	if (king < 2 || deep_now >= deep_limit)
		return score(color);
	//�������������
	int max = -large_num; //��������������߷��Ĺ�ֵ
	//����rectab
	if (cache) {
		board_info info = rectab.find(field, 1);
		if (color) {	//��
			if ((!info.is_blank(1)) && (info.read_depth(1) == 10 + deep_limit - deep_now)) {
				max = info.read_score(1);
				++hit_count;
			}
		}
		else {	//��
			if ((!info.is_blank(2)) && (info.read_depth(2) == 10 + deep_limit - deep_now)) {
				max = -info.read_score(2);
				++hit_count;
			}
		}
	}
	//�洢��ʼ״̬
	int save[11][10]; //���ڴ洢�µ���û������ʱ�ľ���
	for (int i = 1; i <= 10; ++i) {
		for (int j = 1; j <= 9; ++j)
			save[i][j] = field[i][j];
	}
	//���½���Ԥ���۾��棬���Ż�����˳�򣬴Ӷ�������֦Ч��
	vector<move_info> list;
	get_valid_moves(color);
	int size = valid_moves.size(); //һ�����е���������
	for (int p = 0; p < size; ++p)
		list.push_back(move_info(valid_moves[p], 0));
	valid_moves.clear();
	for (int p = 0; p < size; ++p) {
		int eat = move(list[p].director); //��������
		list[p].score = prejudge(color, deep_limit - deep_now, 1) + eat_k * power_score[eat % 10];
		for (int i = 1; i <= 10; ++i) { //����
			for (int j = 1; j <= 9; ++j)
				field[i][j] = save[i][j];
		}
	}
	sort(list.begin(), list.end()); //����
									//����Ѱ��������ӵ�
	float cut = cuttab[cuttab_index][deep_limit - deep_now];	//������ȫ��cut���ݹ鷵��ʱ�ᱻ�޸ģ�����
	int best_move = 0;
	for (int p = size - 1; p >= ((float)1 - cut) * ((float)size - (float)1); --p)	//����cut * (size - 1)!!!!!!!!!
	{
		move(list[p].director);
		maxmin[deep_now] = max; //�洢�ڵ����ݣ�׼����֦
		++deep_now;
		int temp = search_min(color); //���ü���к���
		--deep_now; //����
		if (temp > max) { //���ŵ����ӣ����ڵ��ں��ǲ��Եģ���ʹ��֦����
			max = temp;
			best_move = p;
		}
		for (int i = 1; i <= 10; ++i) { //����
			for (int j = 1; j <= 9; ++j)
				field[i][j] = save[i][j];
		}
		for (int step = deep_now - 1; step >= 0; step = step - 2) { //��0�Ļ������Ľ��Ǿ�ȷ�ģ�����ÿ��������Ĺ�ֵ��׼ȷ
			if (max >= maxmin[step] - abcut) { //beta��֦
				//����rectab
				if (cache) {
					if (color)	//��
						rectab.push(field, 1, 10 + deep_limit - deep_now, max, 1);	//���Ǵ�1��ʼ��depth
					else	//��
						rectab.push(field, 1, 10 + deep_limit - deep_now, -max, 2);
				}
				decision = list[best_move].director;
				return max;
			}
		}
	}
	//����rectab
	if (cache) {
		if (color) {	//��
			rectab.push(field, 1, 10 + deep_limit - deep_now, max, 1);	//���Ǵ�1��ʼ��depth
			rectab.push(field, 1, 10 + deep_limit - deep_now, max, 2);
		}
		else {	//��
			rectab.push(field, 1, 10 + deep_limit - deep_now, -max, 2);
			rectab.push(field, 1, 10 + deep_limit - deep_now, -max, 1);
		}
	}
	decision = list[best_move].director;
	return max;
}

int AI_probcut(int color) {
	//���������Ϣ
	if (debug2) {
		cout << "count_cal = " << count_cal << ";    ";
		cout << "rectab = " << rectab.get_size() << ";    ";
		cout << "hit_count = " << hit_count << ";    ";
		cout << "deep_limit = " << deep_limit << ";    ";
		cout << "cuttab_index = " << cuttab_index << endl;
		cout << "eval = " << eval << ";    " << endl;
	}
	hit_count = 0;
	count_cal = 0;
	//cout << "���Ķ���probcut����˼������ȴ� 5 ~ 15 ��..." << endl;
	deep_now = 0;
	eval = search_max(color);
	if (debug) {
		//freopen("out.csv", "a+", stdout);
		cout << "turns = " << turns << punctuation;
		cout << "eval = " << eval << punctuation;
		cout << "count_cal = " << count_cal << punctuation;
		cout << "rectab = " << rectab.get_size() << punctuation;
		cout << "hit_count = " << hit_count << punctuation;
		cout << "deep_limit = " << deep_limit << punctuation;
		cout << "cuttab_index = " << cuttab_index << endl;
		cout << turns << punctuation;
		cout << eval << punctuation;
		cout << count_cal << punctuation;
		cout << rectab.get_size() << punctuation;
		cout << hit_count << punctuation;
		cout << deep_limit << punctuation;
		cout << cuttab_index << endl;
		system("pause");
	}
	/*����Ӧ������ȡ����
	if (turns <= 6)
		deep_limit = 8;	//����ʱҲ����Ϊ6
	else if (turns <= 8)
		deep_limit = 8;
	else if ((count_cal < time_limit_k) && (turns > 1))
		++deep_limit_cnt;
	if (deep_limit_cnt >= time_limit_t) {
		if (cuttab_index == cuttabR - 1) {
			cuttab_index = cuttabJ;
			deep_limit += 1;
			rectab.max_volume *= 3;
			eat_k = 0;
		}
		else {
			++cuttab_index;
		}
		deep_limit_cnt = 0;
	}*/
	return decision;
}

int AI_alphabeta(int color) {
	//save and reset
	int save_deep_limit = deep_limit;
	deep_limit = 6;
	int save_cuttab_index = cuttab_index;
	cuttab_index = cuttabR - 1;
	//���������Ϣ
	if (debug2) {
		cout << "count_cal = " << count_cal << ";    ";
		cout << "rectab = " << rectab.get_size() << ";    ";
		cout << "hit_count = " << hit_count << ";    ";
		cout << "deep_limit = " << deep_limit << ";    ";
		cout << "cuttab_index = " << cuttab_index << endl;
		cout << "eval = " << eval << ";    ";
	}
	hit_count = 0;
	count_cal = 0;
	//cout << "���Ķ���alpha-beta����˼������ȴ� 5 ~ 15 ��..." << endl;
	deep_now = 0;
	eval = search_max(color);
	if (debug) {
		//freopen("out.csv", "a+", stdout);
		/*cout << "turns = " << turns << punctuation;
		cout << "eval = " << eval << punctuation;
		cout << "count_cal = " << count_cal << punctuation;
		cout << "rectab = " << rectab.get_size() << punctuation;
		cout << "hit_count = " << hit_count << punctuation;
		cout << "deep_limit = " << deep_limit << punctuation;
		cout << "cuttab_index = " << cuttab_index << endl;*/
		cout << turns << punctuation;
		cout << eval << punctuation;
		cout << count_cal << punctuation;
		cout << rectab.get_size() << punctuation;
		cout << hit_count << punctuation;
		cout << deep_limit << punctuation;
		cout << cuttab_index << endl;
		system("pause");
	}
	//restore
	deep_limit = save_deep_limit;
	cuttab_index = save_cuttab_index;
	return decision;
}

int AI(int color, int test_mode) {	//test_modeΪ1��ִ��procut����ִ������alpha-beta 
	if (test_mode)
		return AI_probcut(color);
	else
		return AI_alphabeta(color);
}

//����V2.1�������ݽṹ�ͺ�����
int prejudge(int color, int deep, bool from_max) {	//Ԥ�к���,deep��ʾ��չ������,deepΪ1ֱ�Ӽ�ֱ����score
	if (deep == 1)	//����ֵ�������ظ�����!!!!!
		return 0;
	if (deep <= 4)	//4.ֱ����ֵ,������ȫ��Ӧ��Ϊ3����Ϊ100?
		return score(color);
	//Ԥ����ȴ���0
	//����ȫ�����ݽṹ
	int save_field[11][10];
	for (int i = 0; i < 11; ++i) {
		for (int j = 0; j < 10; ++j)
			save_field[i][j] = field[i][j];
	}
	int save_deep_limit = deep_limit;
	int save_turns = turns;
	int save_deep_now = deep_now;
	int save_maxmin[15];
	for (int i = 0; i < 15; ++i)
		save_maxmin[i] = maxmin[i];
	int save_abcut = abcut;
	//���е���Ԥ�е�alpha-beta��֦����
	int res = 0;

	deep_limit = deep - 3;	//���Գ���deep - 1��2��3��4��������Ӧ����2
	deep_now = 0;
	for (int i = 0; i < 15; ++i)
		maxmin[i] = 0;
	//abcut = 0;
	//cut = 1;
	if (from_max)
		res = search_min(color);
	else
		res = search_max(color);

	//�ָ�ȫ�����ݽṹ
	for (int i = 0; i < 11; ++i) {
		for (int j = 0; j < 10; ++j)
			field[i][j] = save_field[i][j];
	}
	deep_limit = save_deep_limit;
	turns = save_turns;
	deep_now = save_deep_now;
	for (int i = 0; i < 15; ++i)
		maxmin[i] = save_maxmin[i];
	abcut = save_abcut;
	//����������
	return res;
}




