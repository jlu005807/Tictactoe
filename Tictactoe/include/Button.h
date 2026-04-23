#pragma once

#include <functional>
#include <string>

// 简单按钮组件：负责命中检测、悬停反馈和点击回调。
class Button
{
private:
	int x;
	int y;
	int width;
	int height;
	float scale;
	bool isMouseOver;
	std::wstring text;
	std::function<void()> onClick;

	bool contains(int mouseX, int mouseY) const;

public:
	Button(int x, int y, int width, int height, const std::wstring& text, const std::function<void()>& onClick);

	void checkMouseOver(int mouseX, int mouseY);
	bool checkClick(int mouseX, int mouseY);
	void draw();
};
