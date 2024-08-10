#pragma once

#include <graphics.h>
#include <functional>
#include <string>
#include <vector>
#include <iostream>

//�����



//��ǰ���
char player;
//����״̬
bool is_running;

//����״̬
char Piece_Board[3][3];

//ѡ����
int result;

//����
IMAGE *Piece_O=new IMAGE(190,190);
IMAGE* Piece_X = new IMAGE(190, 190);
//����
IMAGE* Board = new IMAGE(600, 600);

//ȥ���׵ױ���
void transparentimage(int x, int y, IMAGE img) {
	IMAGE img1;
	DWORD* d1;
	img1 = img;
	d1 = GetImageBuffer(&img1);
	float h, s, l;
	for (int i = 0; i < img1.getheight() * img1.getwidth(); i++) {
		RGBtoHSL(BGR(d1[i]), &h, &s, &l);
		/*if (l < 0.03) {
			d1[i] = BGR(WHITE);
		}
		if (d1[i] != BGR(WHITE)) {
			d1[i] = 0;
		}*/
		if (d1[i] == BGR(WHITE))
			d1[i] = 0;

	}
	putimage(x, y, &img1, SRCAND);
	/*putimage(x, y, &img, SRCPAINT);*/
	//	putimage(x+100,y,&img1);
	//	putimage(x+200,y,&img);
}


//��ʼ������
void init_Board_player()
{

	loadimage(Piece_O, _T(".\\piece_O.jpg"), 200, 200);
	loadimage(Piece_X, _T(".\\piece_X.jpg"), 200, 200);
	loadimage(Board, _T(".\\Board.png"), 600, 600);

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
	/*line(0, 200, 600, 200);
	line(0, 400, 600, 400);
	line(200, 0, 200, 600);
	line(400, 0, 400, 600);*/

	putimage(0, 0, Board);
}

//����"O"
void DrawCircle(int x, int y)
{
	transparentimage(200 * x , 200 * y ,*Piece_O);
}

//����"X"
void DrawX(int x, int y)
{
	/*line(200 * x, 200 * y, 200 * (x + 1), 200 * (y + 1));
	line(200 * (x + 1), 200 * y, 200 * x, 200 * (y + 1));*/
	transparentimage(200 * x, 200 * y, *Piece_X);
}

//��������
void DrawPiece(char c, int x, int y)
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
void Draw_game()
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
		result = MessageBox(GetHWnd(), _T("X ��һ�ʤ\n�Ƿ����¿�ʼ��Ϸ��"), _T("������Ϸ����"), MB_YESNO | MB_ICONQUESTION);
	}
	else if (CheckWin('O'))
	{
		result = MessageBox(GetHWnd(), _T("O ��һ�ʤ\n�Ƿ����¿�ʼ��Ϸ��"), _T("������Ϸ����"), MB_YESNO | MB_ICONQUESTION);
	}
	else if (CheckDraw())
	{
		result = MessageBox(GetHWnd(), _T("  ƽ��  \n�Ƿ����¿�ʼ��Ϸ��"), _T("������Ϸ����"), MB_YESNO | MB_ICONQUESTION);
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
		exit(0);
	}


}



class Button
{
private:
	//����
	int x;
	int y;
	int width;
	int height;
	float scale;//���ű���
	bool is_MouseOver;//����Ƿ���������
	std::wstring text;//�ı�
	std::function<void()>onClick;//�����ť��������
public:
	//��ʼ��
	Button(int x, int y, int width, int height, const std::wstring& text, const std::function<void()>& onClick) : x(x), y(y), width(width), height(height), text(text), onClick(onClick), scale(1.0f), is_MouseOver(false)
	{

	}

	// �������Ƿ��ڰ�ť�Ϸ�
	void checkMouseOver(int mouseX, int mouseY)
	{
		is_MouseOver = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);

		if (is_MouseOver) {
			scale = 0.9f; // �����ͣʱ���Ű�ť
		}
		else {
			scale = 1.0f; // �ָ���ťԭʼ��С
		}
	}

	bool checkClick(int mouseX, int mouseY)
	{
		is_MouseOver = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);

		if (is_MouseOver)
		{
			onClick();
			is_MouseOver = false;
			scale = 1.0f;
			return true;
		}

		return false;

	}

	void draw()
	{
		int scaledWidth = width * scale; // ���ź�İ�ť���
		int scaledHeight = height * scale; // ���ź�İ�ť�߶�
		int scaledX = x + (width - scaledWidth) / 2; // ���ź�İ�ťx����
		int scaledY = y + (height - scaledHeight) / 2; // ���ź�İ�ťy����

		if (is_MouseOver)
		{
			setlinecolor(RGB(0, 120, 215)); // �����ͣʱ��ť�߿���ɫ
			setfillcolor(RGB(229, 241, 251)); // �����ͣʱ��ť�����ɫ

		}
		else
		{
			setlinecolor(RGB(173, 173, 173)); // ��ť�߿���ɫ
			setfillcolor(RGB(225, 225, 225)); // ��ť�����ɫ
		}

		fillrectangle(scaledX, scaledY, scaledX + scaledWidth, scaledY + scaledHeight); // ���ư�ť
		settextcolor(BLACK); // �����ı���ɫΪ��ɫ
		setbkmode(TRANSPARENT); // �����ı�����͸��
		settextstyle(20 * scale, 0, _T("΢���ź�")); // �����ı���С������
		//������ʾ��ť�ı�
		int textX = scaledX + (scaledWidth - textwidth(text.c_str())) / 2; // �����ı��ڰ�ť�����x����
		int textY = scaledY + (scaledHeight - textheight(_T("΢���ź�"))) / 2; // �����ı��ڰ�ť�����y����
		outtextxy(textX, textY, text.c_str()); // �ڰ�ť�ϻ����ı�
	}

};


class Widget
{
private:
	int width; // ���
	int height; // �߶�
	int currentIndex; // ��ǰҳ������
	std::vector<IMAGE*> pages; // �洢����ҳ���ͼƬָ��
	std::vector<std::vector<Button*>> buttons; // �洢ÿ��ҳ���ϵİ�ť
	//���ҳ��
	void addPage(IMAGE* page)
	{
		pages.push_back(page);
		buttons.push_back({});
	}

	// ��ָ��ҳ�������һ����ť
	void addButton(int index, Button* button)
	{
		if (index >= 0 && index < buttons.size())
		{
			buttons[index].push_back(button);
		}
	}

	// ���õ�ǰ��ʾ��ҳ������
	void setCurrentIndex(int index)
	{
		if (index >= 0 && index < pages.size())
		{
			currentIndex = index;
		}
	}

	// ����������¼�
	void mouseClick(int mouseX, int mouseY)
	{
		if (currentIndex >= 0 && currentIndex < buttons.size())
		{
			for (Button* button : buttons[currentIndex])
			{
				if (button->checkClick(mouseX, mouseY))
				{
					break;
				}
			}
		}
	}

	// ��������ƶ��¼�
	void mouseMove(int mouseX, int mouseY)
	{
		if (currentIndex >= 0 && currentIndex < buttons.size())
		{
			for (Button* button : buttons[currentIndex])
			{
				button->checkMouseOver(mouseX, mouseY);
			}
		}
	}

	// ���Ƶ�ǰҳ�������
	void draw()
	{
		if (currentIndex >= 0 && currentIndex < pages.size())
		{
			putimage(0, 0, pages[currentIndex]); // �ڴ����л��Ƶ�ǰҳ���ͼƬ

			if (currentIndex >= 0 && currentIndex < buttons.size())
			{
				for (Button* button : buttons[currentIndex])
				{
					button->draw(); // ���Ƶ�ǰҳ���ϵ����а�ť
				}
			}

			//��Ҷ�ս
			if (currentIndex == 3)
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
					Draw_game();

					//�ж�
					Check();

					DWORD end_time = GetTickCount();
					DWORD gap_time = end_time - start_time;

					if (gap_time < 1000 / 60)//����60֡
					{
						Sleep(1000 / 60 - gap_time);
					}

				}



			}
			//�˻���ս֮��
			else if (currentIndex == 4)
			{
				//��ʼ��(Ĭ�����Ϊ'O')
				init_game();

				//sleep
				DWORD start_time = GetTickCount();


				while (is_running)
				{
					ExMessage msg;
					//��ȡ����
					if(player =='O')
					{
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
					}
					else if (player == 'X')
					{
						Sleep(256);
						int empty_index[9][2];
						int empty_num = 0;
						for(int y=0;y<3;y++)
							for (int x = 0; x < 3; x++)
							{
								if (Piece_Board[x][y] == '-')
								{
									empty_index[empty_num][0] = x;
									empty_index[empty_num][1] = y;
									empty_num++;
								}
							}
						//�������
						int key = rand()%empty_num;
						Input_Piece(empty_index[key][0], empty_index[key][1]);
					}

					//��ͼ
					Draw_game();

					//�ж�
					Check();

					DWORD end_time = GetTickCount();
					DWORD gap_time = end_time - start_time;

					if (gap_time < 1000 / 60)//����60֡
					{
						Sleep(1000 / 60 - gap_time);
					}

				}





			}

		}
	}

public:

	Widget(int width, int height) :width(width), height(height), currentIndex(-1)
	{
	}

	~Widget() {}

	void init()
	{
		initgraph(width, height);

		IMAGE *page1= new IMAGE(width, height);
		loadimage(page1, _T(".\\backgroud.jpg"), 600, 600);
		addPage(page1);//��ӽ���1

		//�ڽ���һ������ťһ
		Button* button1_1 = new Button(250, 500, 100, 50, L"��ʼ��Ϸ", [&]() {setCurrentIndex(1); });
		addButton(0, button1_1);
		
		IMAGE *page2 = new IMAGE(width, height);
		loadimage(page2, _T(".\\backgroud.jpg"), 600, 600);
		addPage(page2);//��ӽ���2

		Button* button2_1 = new Button(150, 500, 100, 50, L"��Ҷ�ս", [&]() {setCurrentIndex(3); });
		addButton(1,button2_1);

		Button* button2_2 = new Button(350, 500, 100, 50, L"�˻���ս", [&]() {setCurrentIndex(2); });
		addButton(1, button2_2);

		IMAGE* page3 = new IMAGE(width, height);//����ֱ����loadimage()��������ͼƬ
		loadimage(page3, _T(".\\page3.jpg"), 600, 600);
		addPage(page3); // ���ҳ��3

		Button* button3_1 = new Button(150, 500, 100, 50, L"��", [&]() {setCurrentIndex(4); });
		addButton(2, button3_1);

		Button* button3_2 = new Button(400, 500, 100, 50, L"����", [&]() {setCurrentIndex(5); });
		addButton(2, button3_2);

		IMAGE* page4 = new IMAGE(width, height);//����ֱ����loadimage()��������ͼƬ
		setfillcolor(RGB(0, 0, 0)); // ����ҳ��1�ı�����ɫΪ
		solidrectangle(0, 0, width, height); // ����ҳ��1�ı�������
		getimage(page4, 0, 0, width, height); // ��ҳ��1�����ݱ��浽ͼƬ��
		addPage(page4); // ���ҳ��4

		IMAGE* page5 = new IMAGE(width, height);//����ֱ����loadimage()��������ͼƬ
		setfillcolor(RGB(0, 0, 0)); // ����ҳ��1�ı�����ɫΪ
		solidrectangle(0, 0, width, height); // ����ҳ��1�ı�������
		getimage(page5, 0, 0, width, height); // ��ҳ��1�����ݱ��浽ͼƬ��

		addPage(page5); // ���ҳ��5

		IMAGE* page6 = new IMAGE(width, height);//����ֱ����loadimage()��������ͼƬ
		setfillcolor(RGB(0, 0, 0)); // ����ҳ��1�ı�����ɫΪ
		solidrectangle(0, 0, width, height); // ����ҳ��1�ı�������
		getimage(page6, 0, 0, width, height); // ��ҳ��1�����ݱ��浽ͼƬ��

		addPage(page6); // ���ҳ��6
		setCurrentIndex(0);
	}

	void run()
	{
		ExMessage msg;

		DWORD start_time = GetTickCount();

		BeginBatchDraw();

		while (1)
		{
			if (peekmessage(&msg))
			{
				int mouseX = msg.x;
				int mouseY = msg.y;

				switch (msg.message)
				{
				case WM_LBUTTONDOWN: // �����������¼�
					mouseClick(mouseX, mouseY); // ����������¼�
					break;
				case WM_MOUSEMOVE: // ����ƶ��¼�
					mouseMove(mouseX, mouseY); // ��������ƶ��¼�
					break;
				}

			}

			draw();
			FlushBatchDraw();

			DWORD end_time = GetTickCount();
			DWORD gap_time = end_time - start_time;

			if (gap_time < 1000 / 60)//����60֡
			{
				Sleep(1000 / 60 - gap_time);
			}
		}

		EndBatchDraw();
	}

	void close()
	{
		closegraph();
	}

};