#include "..\\include\\GameRenderer.h"

#include <graphics.h>
#include <tchar.h>

#include "..\\include\\Game.h"

namespace
{
constexpr int kBoardPixelSize = 600;
constexpr int kCellPixelSize = 200;
constexpr int kPiecePixelSize = 200;
}

GameRenderer::GameRenderer()
	: pieceO(std::make_unique<IMAGE>(kPiecePixelSize, kPiecePixelSize)),
	pieceX(std::make_unique<IMAGE>(kPiecePixelSize, kPiecePixelSize)),
	boardImage(std::make_unique<IMAGE>(kBoardPixelSize, kBoardPixelSize)),
	initialized(false)
{
}

GameRenderer::~GameRenderer() = default;

void GameRenderer::loadResources()
{
	if (initialized)
	{
		return;
	}

	loadimage(pieceO.get(), _T(".\\src\\piece_O.jpg"), kPiecePixelSize, kPiecePixelSize);
	loadimage(pieceX.get(), _T(".\\src\\piece_X.jpg"), kPiecePixelSize, kPiecePixelSize);
	loadimage(boardImage.get(), _T(".\\src\\Board.png"), kBoardPixelSize, kBoardPixelSize);
	initialized = true;
}

void GameRenderer::drawGame(const GameState& gameState)
{
	if (!initialized)
	{
		loadResources();
	}

	cleardevice();
	drawBoard();
	drawAllPieces(gameState);
	drawTipText(gameState.getCurrentPlayer());
	FlushBatchDraw();
}

void GameRenderer::drawBoard()
{
	putimage(0, 0, boardImage.get());
}

void GameRenderer::drawPiece(char c, int x, int y)
{
	if (c == 'O')
	{
		transparentImage(kCellPixelSize * x, kCellPixelSize * y, *pieceO);
	}
	else if (c == 'X')
	{
		transparentImage(kCellPixelSize * x, kCellPixelSize * y, *pieceX);
	}
}

void GameRenderer::drawAllPieces(const GameState& gameState)
{
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			drawPiece(gameState.getCell(x, y), x, y);
		}
	}
}

void GameRenderer::drawTipText(char currentPlayer)
{
	TCHAR text[64];
	_stprintf_s(text, _T("Current Player: %c"), currentPlayer);
	settextcolor(RGB(225, 175, 45));
	outtextxy(0, 0, text);
}

void GameRenderer::transparentImage(int x, int y, const IMAGE& image)
{
	// 复制一份图片再处理白色背景，避免修改缓存的原始棋子资源。
	IMAGE imageCopy = image;
	DWORD* buffer = GetImageBuffer(&imageCopy);
	for (int i = 0; i < imageCopy.getheight() * imageCopy.getwidth(); ++i)
	{
		if (buffer[i] == BGR(WHITE))
		{
			buffer[i] = 0;
		}
	}
	putimage(x, y, &imageCopy, SRCAND);
}
