#include<graphics.h>
#include<TCHAR.H>
#include<time.h>
#include<easyx.h>
#include<iostream>


//当前玩家
char player;
//运行状态
bool is_running;

//棋盘状态
char Piece_Board[3][3];

//选择结果
int result;

//初始化棋盘
void init_Board_player()
{

	//随机先手
	srand((unsigned int)time(0));
	int i = rand() % 2;

	if (i == 0)player = 'O';
	else player = 'X';


	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			Piece_Board[x][y] = '-';

	result = -1;
}

void init_game()
{
	initgraph(600, 600);
	
	is_running = true;

	init_Board_player();

	BeginBatchDraw();


}

//判定胜利
bool CheckWin(char c)
{
	for (int i = 0; i < 3; i++)
	{
		if (Piece_Board[i][0] == c && Piece_Board[i][1] == c && Piece_Board[i][2] == c)
			return true;
		if (Piece_Board[0][i] == c && Piece_Board[1][i] == c && Piece_Board[2][i] == c)
			return true;
	}
	if (Piece_Board[0][0] == c && Piece_Board[1][1] == c && Piece_Board[2][2] == c)
		return true;
	if (Piece_Board[2][0] == c && Piece_Board[1][1] == c && Piece_Board[0][2] == c)
		return true;

	return false;
}

//判定平局
bool CheckDraw()
{
	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			if (Piece_Board[x][y] == '-')
				return false;
	return true;
}

//绘制棋盘网格
void DrawBoard()
{
	line(0, 200, 600, 200);
	line(0, 400, 600, 400);
	line(200, 0, 200, 600);
	line(400, 0, 400, 600);
}

//绘制"O"
void DrawCircle(int x,int y)
{
	circle(200 * x + 100, 200 * y + 100, 100);
}

//绘制"X"
void DrawX(int x,int y)
{
	line(200 * x, 200 * y, 200 * (x + 1), 200 * (y + 1));
	line(200 * (x + 1), 200 * y, 200 * x, 200 * (y + 1));
}

//绘制棋子
void DrawPiece(char c,int x,int y)
{
	if (c == 'O')DrawCircle(x, y);
	else if (c == 'X')DrawX(x, y);
}

//绘制所有棋子
void DrawAllPiece()
{
	for (int y = 0; y < 3; y++)
		for (int x = 0; x < 3; x++)
			DrawPiece(Piece_Board[x][y], x, y);
}

//绘制提示文本
void DrawTipTxt()
{
	static TCHAR str[64];
	_stprintf_s(str, _T("当前棋子类型为：%c"), player);

	settextcolor(RGB(225, 175, 45));
	outtextxy(0, 0, str);
}

//落子
void Input_Piece(int x, int y)
{
	if (Piece_Board[x][y] == '-')
	{
		Piece_Board[x][y] = player;

		//改变玩家
		if (player == 'O')player = 'X';
		else player = 'O';
	}

}

//总渲染
void Draw()
{
	cleardevice();

	DrawBoard();
	DrawAllPiece();
	DrawTipTxt();

	FlushBatchDraw();
}

//总判定
void Check()
{
	/*static TCHAR message[64];
	_stprintf_s(message, sizeof(message)/sizeof(TCHAR), _T("%c 玩家获胜\n是否重新开始游戏？"), player);*/

	if (CheckWin('X'))
	{
		result=MessageBox(GetHWnd(), _T("X 玩家获胜\n是否重新开始游戏？"), _T("本局游戏结束"),  MB_YESNO | MB_ICONQUESTION);
	}
	else if (CheckWin('O'))
	{
		result = MessageBox(GetHWnd(), _T("O 玩家获胜\n是否重新开始游戏？"), _T("本局游戏结束"), MB_YESNO | MB_ICONQUESTION);
	}
	else if (CheckDraw())
	{
		result=MessageBox(GetHWnd(), _T("  平局  \n是否重新开始游戏？"), _T("本局游戏结束"), MB_YESNO | MB_ICONQUESTION);
	}


	/*std::cout << result << std::endl;
	system("Pause");*/
	//是否继续游戏
	if (result == IDYES)
	{
		init_Board_player();
	}
	else if (result == IDNO)
	{
		is_running = false;
		MessageBox(GetHWnd(), _T("成功退出游戏"), _T("提示"), MB_OK | MB_ICONSTOP);
	}


}

int main()
{
	//初始化
	init_game();

	//sleep
	DWORD start_time = GetTickCount();


	while (is_running)
	{
		ExMessage msg;
		//获取数据

		while (peekmessage(&msg))
		{
			//检查鼠标左键点击消息
			if (msg.message == WM_LBUTTONDOWN)
			{
				//处理数据
				//计算落子位置
				int x = msg.x / 200;
				int y = msg.y / 200;

				Input_Piece(x, y);
			}

		}

		//绘图
		Draw();

		//判定
		Check();

		DWORD end_time = GetTickCount();
		DWORD gap_time = end_time - start_time;

		if (gap_time < 1000 / 60)//保持60帧
		{
			Sleep(1000 / 60 - gap_time);
		}

	}

	EndBatchDraw();

	return 0;
}