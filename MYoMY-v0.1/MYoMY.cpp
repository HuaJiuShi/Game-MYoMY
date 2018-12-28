#include <iostream>
#include <Windows.h>
#include <process.h>
#define house "---房子---"
#define home  "---我们的家---"
using namespace std;
int loveDifficult, g_nKey, My_X, My_Y, Your_X, Your_Y, MyLove, YourLove, LoveTime, LoveFlag = 0;	//爱的难度、按键信息、控制台光标X、Y坐标
HANDLE hIn, hOut;	//输入/输出设备句柄
COORD  Outchar;
void KeyPressed(void* b);
void LovePositionSetting(int x, int y);
int main() {
	loveDifficult = 30;
	Outchar.X = 40; Outchar.Y = 0;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(hOut, &CursorInfo);//获取控制台光标信息
	CursorInfo.bVisible = false;			//隐藏控制台光标
	SetConsoleCursorInfo(hOut, &CursorInfo);//设置控制台光标状态
	system("mode con cols=90 lines=50");	//进入游戏界面
	//显示游戏画面
	LovePositionSetting(32, 0); cout << "前路漫漫，无法回头，眼前只有她" << endl;
	LovePositionSetting(30, 1); cout << "越过这条线，遇不到她，你将失去所有" << endl;
	LovePositionSetting(0, 49); cout << "操作说明：使用←↓→三个方向键，控制'我'移动，无法回头——步履不停，终将相遇";
	LovePositionSetting(0, loveDifficult + 3);	//绘制最后的线
	for (int i = 0; i < 90; i++) cout << '-';
	LovePositionSetting(41, 3);  cout << house << endl;
	LovePositionSetting(45, 4);			//设置“我”的初始出发位置
	Your_Y = loveDifficult + 4;			//她一直在终点，摇摆
	_beginthread(KeyPressed, 0, NULL);	//分出一个线程，让按键一直监控。
	while (1) {							//游戏每100ms刷新一次状态，500ms人物自动移动一次
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
		LovePositionSetting(My_X, My_Y); cout << "我";
		LoveTime = (My_Y == Your_Y - 1) ? LoveTime + 1 : LoveTime;
		//判断准则
		LoveFlag = (LoveTime == 50) ? 3 : LoveFlag;		//3:overtime,you have no chance
		LoveFlag = (My_Y == Your_Y + 5) ? 1 : LoveFlag;	//1:miss
		LoveFlag = (My_Y == Your_Y) ? (((My_X - Your_X >= -1) && (My_X - Your_X <= 1)) ? 2 : LoveFlag) : LoveFlag;	//2:meet

		//安排心中的 你 动，随机初始化位置运动
		YourLove == 5 ? YourLove = 0 : YourLove++;
		if (YourLove == 4 && !LoveFlag) {
			Outchar.X = Your_X; Outchar.Y = Your_Y; SetConsoleCursorPosition(hOut, Outchar); cout << "  ";
			Your_X = rand() % 50 + 20;
			Outchar.X = Your_X; Outchar.Y = Your_Y; SetConsoleCursorPosition(hOut, Outchar); cout << "她";
			LovePositionSetting(My_X, My_Y);
		}
		if (LoveFlag) break;
	}
	if (LoveFlag == 1) {
		LovePositionSetting(31, Your_Y - 20);
		cout << "爱不会重来，错过了，就让她去吧！";
	}
	else if (LoveFlag == 3) {
		LovePositionSetting(36, Your_Y - 20);
		cout << "畏惧不前，你没机会了！";
	}
	else if(LoveFlag == 2) {
		if (My_X > Your_X) {
			LovePositionSetting(My_X, Your_Y); cout << "我们";
		}else {
			LovePositionSetting(Your_X, Your_Y); cout << "我们";
		}
		LovePositionSetting(34, Your_Y - 20);
		cout << "遇见你，是我今生最大的幸福！";
		system("color 3D");
		LovePositionSetting(39, 3);  cout << home << endl;
	}
	LovePositionSetting(0, Your_Y+10); cout << "游戏结束，请截图后按q键退出！";
	while (g_nKey != 81) { Sleep(100); }
	return 0;
}
void KeyPressed(void* b) {	//按键事件处理方法
	hIn = GetStdHandle(STD_INPUT_HANDLE); // 获取标准输入设备句柄
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
