#pragma once

// 纯游戏状态层：只负责棋盘、回合、胜负和 AI 搜索，不直接依赖界面。
class GameState
{
public:
	// 对局结果只描述棋盘状态，界面流程由 GameFlowController 决定。
	enum class RoundResult
	{
		Ongoing,
		XWin,
		OWin,
		Draw
	};

	GameState();

	void initGame();
	void restartRound();
	void stopGame();

	bool isRunning() const;
	char getCurrentPlayer() const;
	char getCell(int x, int y) const;

	bool inputPiece(int x, int y);
	void comPlay();
	RoundResult getRoundResult() const;

private:
	char currentPlayer;
	bool running;
	char board[3][3];

	void initBoardAndPlayer();
	void resetBoard();
	bool checkWin(char c) const;
	bool checkDraw() const;
	int winnerState() const;
	int eval() const;
	bool canAcceptMove() const;
	int minMaxSearch(int& index, int step, int a, int b);
};
