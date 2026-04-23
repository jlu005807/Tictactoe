#pragma once

#include <memory>

class GameState;
class IMAGE;

// 负责把 GameState 的只读棋盘数据绘制到 EasyX 窗口。
class GameRenderer
{
public:
	GameRenderer();
	~GameRenderer();

	void loadResources();
	void drawGame(const GameState& gameState);

private:
	std::unique_ptr<IMAGE> pieceO;
	std::unique_ptr<IMAGE> pieceX;
	std::unique_ptr<IMAGE> boardImage;
	bool initialized;

	void drawBoard();
	void drawPiece(char c, int x, int y);
	void drawAllPieces(const GameState& gameState);
	void drawTipText(char currentPlayer);
	static void transparentImage(int x, int y, const IMAGE& image);
};
