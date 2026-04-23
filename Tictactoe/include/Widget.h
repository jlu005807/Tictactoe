#pragma once

#include <graphics.h>
#include <memory>
#include <vector>

#include "Button.h"
#include "Game.h"
#include "GameFlow.h"
#include "GameRenderer.h"

class IMAGE;

// Widget 负责页面切换、鼠标事件分发，并串联游戏逻辑/渲染/结束流程。
class Widget
{
private:
	int width;
	int height;
	int currentIndex;
	GameState gameState;
	GameFlowController gameFlowController;
	GameRenderer gameRenderer;
	std::vector<std::unique_ptr<IMAGE>> pages;
	std::vector<std::vector<std::unique_ptr<Button>>> buttons;

	void handlePostGameAction();
	void addPage(std::unique_ptr<IMAGE> page);
	void addButton(int index, std::unique_ptr<Button> button);
	void setCurrentIndex(int index);
	void mouseClick(int mouseX, int mouseY);
	void mouseMove(int mouseX, int mouseY);
	void limitFrame(DWORD frameStart) const;

	void runPvpGameLoop();
	void runRandomAiGameLoop();
	void runMinimaxAiGameLoop();
	void draw();

public:
	Widget(int width, int height);
	~Widget();
	void init();
	void run();
	void close();
};
