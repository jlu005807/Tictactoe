#include "..\\include\\Widget.h"

int main()
{
	// EasyX 窗口固定为 600x600，对应 3x3 棋盘的三格布局。
	Widget widget(600, 600);
	widget.init();
	widget.run();
	widget.close();

	return 0;
}
