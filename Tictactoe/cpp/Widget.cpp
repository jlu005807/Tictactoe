#include "..\\include\\Widget.h"

#include <cstdlib>
#include <graphics.h>
#include <utility>

namespace
{
constexpr int kTargetFps = 60;
constexpr int kCellPixelSize = 200;
constexpr int kStartPage = 0;
constexpr int kModeSelectionPage = 1;
constexpr int kAiDifficultyPage = 2;
constexpr int kPvpGamePage = 3;
constexpr int kRandomAiGamePage = 4;
constexpr int kMinimaxAiGamePage = 5;
}

Widget::Widget(int width, int height) : width(width), height(height), currentIndex(-1)
{
}

Widget::~Widget() = default;

void Widget::handlePostGameAction()
{
	const GameFlowController::PostGameAction action = gameFlowController.handleRoundEnd(gameState);
	if (action == GameFlowController::PostGameAction::ReturnToModeSelection)
	{
		setCurrentIndex(kModeSelectionPage);
	}
}

void Widget::addPage(std::unique_ptr<IMAGE> page)
{
	pages.push_back(std::move(page));
	buttons.push_back({});
}

void Widget::addButton(int index, std::unique_ptr<Button> button)
{
	if (index >= 0 && index < static_cast<int>(buttons.size()))
	{
		buttons[index].push_back(std::move(button));
	}
}

void Widget::setCurrentIndex(int index)
{
	if (index >= 0 && index < static_cast<int>(pages.size()))
	{
		currentIndex = index;
	}
}

void Widget::mouseClick(int mouseX, int mouseY)
{
	if (currentIndex < 0 || currentIndex >= static_cast<int>(buttons.size()))
	{
		return;
	}

	for (auto& button : buttons[currentIndex])
	{
		if (button->checkClick(mouseX, mouseY))
		{
			break;
		}
	}
}

void Widget::mouseMove(int mouseX, int mouseY)
{
	if (currentIndex < 0 || currentIndex >= static_cast<int>(buttons.size()))
	{
		return;
	}

	for (auto& button : buttons[currentIndex])
	{
		button->checkMouseOver(mouseX, mouseY);
	}
}

void Widget::limitFrame(DWORD frameStart) const
{
	const DWORD frameTime = GetTickCount() - frameStart;
	if (frameTime < 1000 / kTargetFps)
	{
		Sleep(1000 / kTargetFps - frameTime);
	}
}

void Widget::runPvpGameLoop()
{
	gameState.initGame();
	while (gameState.isRunning())
	{
		// 对局循环会临时接管主循环，直到玩家选择返回模式页。
		const DWORD frameStart = GetTickCount();
		ExMessage msg;
		while (peekmessage(&msg))
		{
			if (msg.message == WM_LBUTTONDOWN)
			{
				gameState.inputPiece(msg.x / kCellPixelSize, msg.y / kCellPixelSize);
			}
		}

		gameRenderer.drawGame(gameState);
		handlePostGameAction();

		limitFrame(frameStart);
	}
}

void Widget::runRandomAiGameLoop()
{
	gameState.initGame();
	while (gameState.isRunning())
	{
		const DWORD frameStart = GetTickCount();
		ExMessage msg;

		if (gameState.getCurrentPlayer() == 'O')
		{
			while (peekmessage(&msg))
			{
				if (msg.message == WM_LBUTTONDOWN)
				{
					gameState.inputPiece(msg.x / kCellPixelSize, msg.y / kCellPixelSize);
				}
			}
		}
		else
		{
			Sleep(256);
			std::vector<std::pair<int, int>> emptyCells;
			for (int y = 0; y < 3; ++y)
			{
				for (int x = 0; x < 3; ++x)
				{
					if (gameState.getCell(x, y) == '-')
					{
						emptyCells.push_back({ x, y });
					}
				}
			}

			if (!emptyCells.empty())
			{
				const int key = rand() % static_cast<int>(emptyCells.size());
				gameState.inputPiece(emptyCells[key].first, emptyCells[key].second);
			}
		}

		gameRenderer.drawGame(gameState);
		handlePostGameAction();

		limitFrame(frameStart);
	}
}

void Widget::runMinimaxAiGameLoop()
{
	gameState.initGame();
	int first = 1;

	while (gameState.isRunning())
	{
		const DWORD frameStart = GetTickCount();
		ExMessage msg;

		if (gameState.getCurrentPlayer() == 'O')
		{
			while (peekmessage(&msg))
			{
				if (msg.message == WM_LBUTTONDOWN)
				{
					if (gameState.inputPiece(msg.x / kCellPixelSize, msg.y / kCellPixelSize))
					{
						first = 0;
					}
				}
			}
		}
		else
		{
			Sleep(384);
			if (first && gameState.getCell(1, 1) == '-')
			{
				gameState.inputPiece(1, 1);
			}
			else
			{
				gameState.comPlay();
			}
		}

		gameRenderer.drawGame(gameState);
		handlePostGameAction();

		limitFrame(frameStart);
	}
}

void Widget::draw()
{
	if (currentIndex < 0 || currentIndex >= static_cast<int>(pages.size()))
	{
		return;
	}

	putimage(0, 0, pages[currentIndex].get());
	if (currentIndex >= 0 && currentIndex < static_cast<int>(buttons.size()))
	{
		for (auto& button : buttons[currentIndex])
		{
			button->draw();
		}
	}

	if (currentIndex == kPvpGamePage)
	{
		runPvpGameLoop();
	}
	else if (currentIndex == kRandomAiGamePage)
	{
		runRandomAiGameLoop();
	}
	else if (currentIndex == kMinimaxAiGamePage)
	{
		runMinimaxAiGameLoop();
	}
}

void Widget::init()
{
	initgraph(width, height);
	gameRenderer.loadResources();

	auto page1 = std::make_unique<IMAGE>(width, height);
	loadimage(page1.get(), _T(".\\src\\backgroud.jpg"), 600, 600);
	addPage(std::move(page1));
	addButton(kStartPage, std::make_unique<Button>(250, 500, 100, 50, L"Start", [&]() { setCurrentIndex(kModeSelectionPage); }));

	auto page2 = std::make_unique<IMAGE>(width, height);
	loadimage(page2.get(), _T(".\\src\\backgroud.jpg"), 600, 600);
	addPage(std::move(page2));
	addButton(kModeSelectionPage, std::make_unique<Button>(150, 500, 100, 50, L"PvP", [&]() { setCurrentIndex(kPvpGamePage); }));
	addButton(kModeSelectionPage, std::make_unique<Button>(350, 500, 100, 50, L"PvE", [&]() { setCurrentIndex(kAiDifficultyPage); }));

	auto page3 = std::make_unique<IMAGE>(width, height);
	loadimage(page3.get(), _T(".\\src\\page3.jpg"), 600, 600);
	addPage(std::move(page3));
	addButton(kAiDifficultyPage, std::make_unique<Button>(150, 500, 100, 50, L"Easy", [&]() { setCurrentIndex(kRandomAiGamePage); }));
	addButton(kAiDifficultyPage, std::make_unique<Button>(400, 500, 100, 50, L"Hard", [&]() { setCurrentIndex(kMinimaxAiGamePage); }));

	auto page4 = std::make_unique<IMAGE>(width, height);
	setfillcolor(RGB(0, 0, 0));
	solidrectangle(0, 0, width, height);
	getimage(page4.get(), 0, 0, width, height);
	addPage(std::move(page4));

	auto page5 = std::make_unique<IMAGE>(width, height);
	setfillcolor(RGB(0, 0, 0));
	solidrectangle(0, 0, width, height);
	getimage(page5.get(), 0, 0, width, height);
	addPage(std::move(page5));

	auto page6 = std::make_unique<IMAGE>(width, height);
	setfillcolor(RGB(0, 0, 0));
	solidrectangle(0, 0, width, height);
	getimage(page6.get(), 0, 0, width, height);
	addPage(std::move(page6));

	setCurrentIndex(kStartPage);
}

void Widget::run()
{
	ExMessage msg;
	BeginBatchDraw();

	while (true)
	{
		const DWORD frameStart = GetTickCount();
		while (peekmessage(&msg))
		{
			const int mouseX = msg.x;
			const int mouseY = msg.y;

			switch (msg.message)
			{
			case WM_LBUTTONDOWN:
				mouseClick(mouseX, mouseY);
				break;
			case WM_MOUSEMOVE:
				mouseMove(mouseX, mouseY);
				break;
			}
		}

		draw();
		FlushBatchDraw();
		limitFrame(frameStart);
	}

	EndBatchDraw();
}

void Widget::close()
{
	closegraph();
}
