#include<graphics.h>
#include<TCHAR.H>
#include<time.h>
#include<easyx.h>
#include<iostream>


//��ǰ���
char player;
//����״̬
bool is_running;

//����״̬
char Piece_Board[3][3];

//ѡ����
int result;

//��ʼ������
void init_Board_player()
{

	//�������
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

//�ж�ʤ��
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

//�ж�ƽ��
bool CheckDraw()
{
	for (int x = 0; x < 3; x++)
		for (int y = 0; y < 3; y++)
			if (Piece_Board[x][y] == '-')
				return false;
	return true;
}

//������������
void DrawBoard()
{
	line(0, 200, 600, 200);
	line(0, 400, 600, 400);
	line(200, 0, 200, 600);
	line(400, 0, 400, 600);
}

//����"O"
void DrawCircle(int x,int y)
{
	circle(200 * x + 100, 200 * y + 100, 100);
}

//����"X"
void DrawX(int x,int y)
{
	line(200 * x, 200 * y, 200 * (x + 1), 200 * (y + 1));
	line(200 * (x + 1), 200 * y, 200 * x, 200 * (y + 1));
}

//��������
void DrawPiece(char c,int x,int y)
{
	if (c == 'O')DrawCircle(x, y);
	else if (c == 'X')DrawX(x, y);
}

//������������
void DrawAllPiece()
{
	for (int y = 0; y < 3; y++)
		for (int x = 0; x < 3; x++)
			DrawPiece(Piece_Board[x][y], x, y);
}

//������ʾ�ı�
void DrawTipTxt()
{
	static TCHAR str[64];
	_stprintf_s(str, _T("��ǰ��������Ϊ��%c"), player);

	settextcolor(RGB(225, 175, 45));
	outtextxy(0, 0, str);
}

//����
void Input_Piece(int x, int y)
{
	if (Piece_Board[x][y] == '-')
	{
		Piece_Board[x][y] = player;

		//�ı����
		if (player == 'O')player = 'X';
		else player = 'O';
	}

}

//����Ⱦ
void Draw()
{
	cleardevice();

	DrawBoard();
	DrawAllPiece();
	DrawTipTxt();

	FlushBatchDraw();
}

//���ж�
void Check()
{
	/*static TCHAR message[64];
	_stprintf_s(message, sizeof(message)/sizeof(TCHAR), _T("%c ��һ�ʤ\n�Ƿ����¿�ʼ��Ϸ��"), player);*/

	if (CheckWin('X'))
	{
		result=MessageBox(GetHWnd(), _T("X ��һ�ʤ\n�Ƿ����¿�ʼ��Ϸ��"), _T("������Ϸ����"),  MB_YESNO | MB_ICONQUESTION);
	}
	else if (CheckWin('O'))
	{
		result = MessageBox(GetHWnd(), _T("O ��һ�ʤ\n�Ƿ����¿�ʼ��Ϸ��"), _T("������Ϸ����"), MB_YESNO | MB_ICONQUESTION);
	}
	else if (CheckDraw())
	{
		result=MessageBox(GetHWnd(), _T("  ƽ��  \n�Ƿ����¿�ʼ��Ϸ��"), _T("������Ϸ����"), MB_YESNO | MB_ICONQUESTION);
	}


	/*std::cout << result << std::endl;
	system("Pause");*/
	//�Ƿ������Ϸ
	if (result == IDYES)
	{
		init_Board_player();
	}
	else if (result == IDNO)
	{
		is_running = false;
		MessageBox(GetHWnd(), _T("�ɹ��˳���Ϸ"), _T("��ʾ"), MB_OK | MB_ICONSTOP);
	}


}

int main()
{
	//��ʼ��
	init_game();

	//sleep
	DWORD start_time = GetTickCount();


	while (is_running)
	{
		ExMessage msg;
		//��ȡ����

		while (peekmessage(&msg))
		{
			//��������������Ϣ
			if (msg.message == WM_LBUTTONDOWN)
			{
				//��������
				//��������λ��
				int x = msg.x / 200;
				int y = msg.y / 200;

				Input_Piece(x, y);
			}

		}

		//��ͼ
		Draw();

		//�ж�
		Check();

		DWORD end_time = GetTickCount();
		DWORD gap_time = end_time - start_time;

		if (gap_time < 1000 / 60)//����60֡
		{
			Sleep(1000 / 60 - gap_time);
		}

	}

	EndBatchDraw();

	return 0;
}