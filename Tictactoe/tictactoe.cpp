#include<graphics.h>
#include<TCHAR.H>
#include<time.h>
#include<easyx.h>
#include<iostream>
#include"Button.h"




int main()
{
	Widget widget(600, 600);
	widget.init();
	widget.run();
	widget.close();

	return 0;
}