#include "..\\include\\Button.h"

#include <graphics.h>
#include <tchar.h>

Button::Button(int x, int y, int width, int height, const std::wstring& text, const std::function<void()>& onClick)
	: x(x), y(y), width(width), height(height), scale(1.0f), isMouseOver(false), text(text), onClick(onClick)
{
}

bool Button::contains(int mouseX, int mouseY) const
{
	return mouseX >= x && mouseX <= x + width && mouseY >= y && mouseY <= y + height;
}

void Button::checkMouseOver(int mouseX, int mouseY)
{
	isMouseOver = contains(mouseX, mouseY);
	scale = isMouseOver ? 0.9f : 1.0f;
}

bool Button::checkClick(int mouseX, int mouseY)
{
	isMouseOver = contains(mouseX, mouseY);
	if (!isMouseOver)
	{
		return false;
	}

	onClick();
	isMouseOver = false;
	scale = 1.0f;
	return true;
}

void Button::draw()
{
	// 鼠标悬停时缩小按钮，以提供轻量的视觉反馈。
	const int scaledWidth = static_cast<int>(width * scale);
	const int scaledHeight = static_cast<int>(height * scale);
	const int scaledX = x + (width - scaledWidth) / 2;
	const int scaledY = y + (height - scaledHeight) / 2;

	if (isMouseOver)
	{
		setlinecolor(RGB(0, 120, 215));
		setfillcolor(RGB(229, 241, 251));
	}
	else
	{
		setlinecolor(RGB(173, 173, 173));
		setfillcolor(RGB(225, 225, 225));
	}

	fillrectangle(scaledX, scaledY, scaledX + scaledWidth, scaledY + scaledHeight);
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(static_cast<int>(20 * scale), 0, _T("Segoe UI"));

	const int textX = scaledX + (scaledWidth - textwidth(text.c_str())) / 2;
	const int textY = scaledY + (scaledHeight - textheight(_T("Hg"))) / 2;
	outtextxy(textX, textY, text.c_str());
}
