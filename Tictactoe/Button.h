#pragma once

#include <graphics.h>
#include <functional>
#include <string>
#include <vector>
#include <iostream>

//杂项函数



//当前玩家
char player;
//运行状态
bool is_running;

//棋盘状态
char Piece_Board[3][3];

//选择结果
int result;

//棋子
IMAGE *Piece_O=new IMAGE(190,190);
IMAGE* Piece_X = new IMAGE(190, 190);
//棋盘
IMAGE* Board = new IMAGE(600, 600);

//去除白底背景
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


//初始化棋盘
void init_Board_player()
{

	loadimage(Piece_O, _T(".\\piece_O.jpg"), 200, 200);
	loadimage(Piece_X, _T(".\\piece_X.jpg"), 200, 200);
	loadimage(Board, _T(".\\Board.png"), 600, 600);

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
	/*line(0, 200, 600, 200);
	line(0, 400, 600, 400);
	line(200, 0, 200, 600);
	line(400, 0, 400, 600);*/

	putimage(0, 0, Board);
}

//绘制"O"
void DrawCircle(int x, int y)
{
	transparentimage(200 * x , 200 * y ,*Piece_O);
}

//绘制"X"
void DrawX(int x, int y)
{
	/*line(200 * x, 200 * y, 200 * (x + 1), 200 * (y + 1));
	line(200 * (x + 1), 200 * y, 200 * x, 200 * (y + 1));*/
	transparentimage(200 * x, 200 * y, *Piece_X);
}

//绘制棋子
void DrawPiece(char c, int x, int y)
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
void Draw_game()
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
		result = MessageBox(GetHWnd(), _T("X 玩家获胜\n是否重新开始游戏？"), _T("本局游戏结束"), MB_YESNO | MB_ICONQUESTION);
	}
	else if (CheckWin('O'))
	{
		result = MessageBox(GetHWnd(), _T("O 玩家获胜\n是否重新开始游戏？"), _T("本局游戏结束"), MB_YESNO | MB_ICONQUESTION);
	}
	else if (CheckDraw())
	{
		result = MessageBox(GetHWnd(), _T("  平局  \n是否重新开始游戏？"), _T("本局游戏结束"), MB_YESNO | MB_ICONQUESTION);
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
		exit(0);
	}


}



class Button
{
private:
	//属性
	int x;
	int y;
	int width;
	int height;
	float scale;//缩放比例
	bool is_MouseOver;//鼠标是否在区域内
	std::wstring text;//文本
	std::function<void()>onClick;//点击按钮触发函数
public:
	//初始化
	Button(int x, int y, int width, int height, const std::wstring& text, const std::function<void()>& onClick) : x(x), y(y), width(width), height(height), text(text), onClick(onClick), scale(1.0f), is_MouseOver(false)
	{

	}

	// 检查鼠标是否在按钮上方
	void checkMouseOver(int mouseX, int mouseY)
	{
		is_MouseOver = (mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height);

		if (is_MouseOver) {
			scale = 0.9f; // 鼠标悬停时缩放按钮
		}
		else {
			scale = 1.0f; // 恢复按钮原始大小
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
		int scaledWidth = width * scale; // 缩放后的按钮宽度
		int scaledHeight = height * scale; // 缩放后的按钮高度
		int scaledX = x + (width - scaledWidth) / 2; // 缩放后的按钮x坐标
		int scaledY = y + (height - scaledHeight) / 2; // 缩放后的按钮y坐标

		if (is_MouseOver)
		{
			setlinecolor(RGB(0, 120, 215)); // 鼠标悬停时按钮边框颜色
			setfillcolor(RGB(229, 241, 251)); // 鼠标悬停时按钮填充颜色

		}
		else
		{
			setlinecolor(RGB(173, 173, 173)); // 按钮边框颜色
			setfillcolor(RGB(225, 225, 225)); // 按钮填充颜色
		}

		fillrectangle(scaledX, scaledY, scaledX + scaledWidth, scaledY + scaledHeight); // 绘制按钮
		settextcolor(BLACK); // 设置文本颜色为黑色
		setbkmode(TRANSPARENT); // 设置文本背景透明
		settextstyle(20 * scale, 0, _T("微软雅黑")); // 设置文本大小和字体
		//居中显示按钮文本
		int textX = scaledX + (scaledWidth - textwidth(text.c_str())) / 2; // 计算文本在按钮中央的x坐标
		int textY = scaledY + (scaledHeight - textheight(_T("微软雅黑"))) / 2; // 计算文本在按钮中央的y坐标
		outtextxy(textX, textY, text.c_str()); // 在按钮上绘制文本
	}

};


class Widget
{
private:
	int width; // 宽度
	int height; // 高度
	int currentIndex; // 当前页面索引
	std::vector<IMAGE*> pages; // 存储所有页面的图片指针
	std::vector<std::vector<Button*>> buttons; // 存储每个页面上的按钮
	//添加页面
	void addPage(IMAGE* page)
	{
		pages.push_back(page);
		buttons.push_back({});
	}

	// 在指定页面上添加一个按钮
	void addButton(int index, Button* button)
	{
		if (index >= 0 && index < buttons.size())
		{
			buttons[index].push_back(button);
		}
	}

	// 设置当前显示的页面索引
	void setCurrentIndex(int index)
	{
		if (index >= 0 && index < pages.size())
		{
			currentIndex = index;
		}
	}

	// 处理鼠标点击事件
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

	// 处理鼠标移动事件
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

	// 绘制当前页面的内容
	void draw()
	{
		if (currentIndex >= 0 && currentIndex < pages.size())
		{
			putimage(0, 0, pages[currentIndex]); // 在窗口中绘制当前页面的图片

			if (currentIndex >= 0 && currentIndex < buttons.size())
			{
				for (Button* button : buttons[currentIndex])
				{
					button->draw(); // 绘制当前页面上的所有按钮
				}
			}

			//玩家对战
			if (currentIndex == 3)
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
					Draw_game();

					//判定
					Check();

					DWORD end_time = GetTickCount();
					DWORD gap_time = end_time - start_time;

					if (gap_time < 1000 / 60)//保持60帧
					{
						Sleep(1000 / 60 - gap_time);
					}

				}



			}
			//人机对战之简单
			else if (currentIndex == 4)
			{
				//初始化(默认玩家为'O')
				init_game();

				//sleep
				DWORD start_time = GetTickCount();


				while (is_running)
				{
					ExMessage msg;
					//获取数据
					if(player =='O')
					{
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
						//随机洛子
						int key = rand()%empty_num;
						Input_Piece(empty_index[key][0], empty_index[key][1]);
					}

					//绘图
					Draw_game();

					//判定
					Check();

					DWORD end_time = GetTickCount();
					DWORD gap_time = end_time - start_time;

					if (gap_time < 1000 / 60)//保持60帧
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
		addPage(page1);//添加界面1

		//在界面一创建按钮一
		Button* button1_1 = new Button(250, 500, 100, 50, L"开始游戏", [&]() {setCurrentIndex(1); });
		addButton(0, button1_1);
		
		IMAGE *page2 = new IMAGE(width, height);
		loadimage(page2, _T(".\\backgroud.jpg"), 600, 600);
		addPage(page2);//添加界面2

		Button* button2_1 = new Button(150, 500, 100, 50, L"玩家对战", [&]() {setCurrentIndex(3); });
		addButton(1,button2_1);

		Button* button2_2 = new Button(350, 500, 100, 50, L"人机对战", [&]() {setCurrentIndex(2); });
		addButton(1, button2_2);

		IMAGE* page3 = new IMAGE(width, height);//可以直接用loadimage()函数加载图片
		loadimage(page3, _T(".\\page3.jpg"), 600, 600);
		addPage(page3); // 添加页面3

		Button* button3_1 = new Button(150, 500, 100, 50, L"简单", [&]() {setCurrentIndex(4); });
		addButton(2, button3_1);

		Button* button3_2 = new Button(400, 500, 100, 50, L"困难", [&]() {setCurrentIndex(5); });
		addButton(2, button3_2);

		IMAGE* page4 = new IMAGE(width, height);//可以直接用loadimage()函数加载图片
		setfillcolor(RGB(0, 0, 0)); // 设置页面1的背景颜色为
		solidrectangle(0, 0, width, height); // 绘制页面1的背景矩形
		getimage(page4, 0, 0, width, height); // 将页面1的内容保存到图片中
		addPage(page4); // 添加页面4

		IMAGE* page5 = new IMAGE(width, height);//可以直接用loadimage()函数加载图片
		setfillcolor(RGB(0, 0, 0)); // 设置页面1的背景颜色为
		solidrectangle(0, 0, width, height); // 绘制页面1的背景矩形
		getimage(page5, 0, 0, width, height); // 将页面1的内容保存到图片中

		addPage(page5); // 添加页面5

		IMAGE* page6 = new IMAGE(width, height);//可以直接用loadimage()函数加载图片
		setfillcolor(RGB(0, 0, 0)); // 设置页面1的背景颜色为
		solidrectangle(0, 0, width, height); // 绘制页面1的背景矩形
		getimage(page6, 0, 0, width, height); // 将页面1的内容保存到图片中

		addPage(page6); // 添加页面6
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
				case WM_LBUTTONDOWN: // 鼠标左键按下事件
					mouseClick(mouseX, mouseY); // 处理鼠标点击事件
					break;
				case WM_MOUSEMOVE: // 鼠标移动事件
					mouseMove(mouseX, mouseY); // 处理鼠标移动事件
					break;
				}

			}

			draw();
			FlushBatchDraw();

			DWORD end_time = GetTickCount();
			DWORD gap_time = end_time - start_time;

			if (gap_time < 1000 / 60)//保持60帧
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