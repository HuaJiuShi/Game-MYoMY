#include <iostream>
#include <Windows.h>
#include <process.h>
#define house "---����---"
#define home  "---���ǵļ�---"
using namespace std;
int loveDifficult, g_nKey, My_X, My_Y, Your_X, Your_Y, MyLove, YourLove, LoveTime, LoveFlag = 0;	//�����Ѷȡ�������Ϣ������̨���X��Y����
HANDLE hIn, hOut;	//����/����豸���
COORD  Outchar;
void KeyPressed(void* b);
void LovePositionSetting(int x, int y);
int main() {
	loveDifficult = 30;
	Outchar.X = 40; Outchar.Y = 0;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hOut, &CursorInfo);//��ȡ����̨�����Ϣ
	CursorInfo.bVisible = false;			//���ؿ���̨���
	SetConsoleCursorInfo(hOut, &CursorInfo);//���ÿ���̨���״̬
	system("mode con cols=90 lines=50");	//������Ϸ����
	//��ʾ��Ϸ����
	LovePositionSetting(32, 0); cout << "ǰ·�������޷���ͷ����ǰֻ����" << endl;
	LovePositionSetting(30, 1); cout << "Խ�������ߣ������������㽫ʧȥ����" << endl;
	LovePositionSetting(0, 49); cout << "����˵����ʹ�á��������������������'��'�ƶ����޷���ͷ�������Ĳ�ͣ���ս�����";
	LovePositionSetting(0, loveDifficult + 3);	//����������
	for (int i = 0; i < 90; i++) cout << '-';
	LovePositionSetting(41, 3);  cout << house << endl;
	LovePositionSetting(45, 4);			//���á��ҡ��ĳ�ʼ����λ��
	Your_Y = loveDifficult + 4;			//��һֱ���յ㣬ҡ��
	_beginthread(KeyPressed, 0, NULL);	//�ֳ�һ���̣߳��ð���һֱ��ء�
	while (1) {							//��Ϸÿ100msˢ��һ��״̬��500ms�����Զ��ƶ�һ��
		Sleep(100);
		(g_nKey == 37 || g_nKey == 39 || g_nKey == 40) ? MyLove = 0 : (MyLove == 5 ? MyLove = 0 : MyLove++);
		Outchar.X = My_X; Outchar.Y = My_Y; SetConsoleCursorPosition(hOut, Outchar); cout << "  ";
		if (My_Y < Your_Y) {
			My_X = (g_nKey == 37) ? ((My_X > 20) ? My_X - 1 : 20) : My_X;
			My_X = (g_nKey == 39) ? ((My_X < 70) ? My_X + 1 : My_X) : My_X;
			My_Y = (g_nKey == 40 || MyLove == 4) ? ((My_Y < loveDifficult + 4) ? My_Y + 1 : loveDifficult + 4) : My_Y;
			g_nKey = 0;
		}else
			My_Y++;
		LovePositionSetting(My_X, My_Y); cout << "��";
		LoveTime = (My_Y == Your_Y - 1) ? LoveTime + 1 : LoveTime;
		//�ж�׼��
		LoveFlag = (LoveTime == 50) ? 3 : LoveFlag;		//3:overtime,you have no chance
		LoveFlag = (My_Y == Your_Y + 5) ? 1 : LoveFlag;	//1:miss
		LoveFlag = (My_Y == Your_Y) ? (((My_X - Your_X >= -1) && (My_X - Your_X <= 1)) ? 2 : LoveFlag) : LoveFlag;	//2:meet

		//�������е� �� ���������ʼ��λ���˶�
		YourLove == 5 ? YourLove = 0 : YourLove++;
		if (YourLove == 4 && !LoveFlag) {
			Outchar.X = Your_X; Outchar.Y = Your_Y; SetConsoleCursorPosition(hOut, Outchar); cout << "  ";
			Your_X = rand() % 50 + 20;
			Outchar.X = Your_X; Outchar.Y = Your_Y; SetConsoleCursorPosition(hOut, Outchar); cout << "��";
			LovePositionSetting(My_X, My_Y);
		}
		if (LoveFlag) break;
	}
	if (LoveFlag == 1) {
		LovePositionSetting(31, Your_Y - 20);
		cout << "����������������ˣ�������ȥ�ɣ�";
	}
	else if (LoveFlag == 3) {
		LovePositionSetting(36, Your_Y - 20);
		cout << "η�岻ǰ����û�����ˣ�";
	}
	else if(LoveFlag == 2) {
		if (My_X > Your_X) {
			LovePositionSetting(My_X, Your_Y); cout << "����";
		}else {
			LovePositionSetting(Your_X, Your_Y); cout << "����";
		}
		LovePositionSetting(34, Your_Y - 20);
		cout << "�����㣬���ҽ��������Ҹ���";
		system("color 3D");
		LovePositionSetting(39, 3);  cout << home << endl;
	}
	LovePositionSetting(0, Your_Y+10); cout << "��Ϸ���������ͼ��q���˳���";
	while (g_nKey != 81) { Sleep(100); }
	return 0;
}
void KeyPressed(void* b) {	//�����¼�������
	hIn = GetStdHandle(STD_INPUT_HANDLE); // ��ȡ��׼�����豸���
	INPUT_RECORD keyRec;
	DWORD state = 0, res;
	for (;;){
		ReadConsoleInput(hIn, &keyRec, 1, &res);
		if (keyRec.EventType == KEY_EVENT)
			g_nKey = keyRec.Event.KeyEvent.wVirtualKeyCode;
	}
}
void LovePositionSetting(int x, int y) {
	My_X = x; My_Y = y;
	Outchar.X = My_X; Outchar.Y = My_Y;
	SetConsoleCursorPosition(hOut, Outchar);
}
