#include "..\\include\\Game.h"

#include <climits>
#include <cstdlib>
#include <ctime>
#include <vector>

namespace
{
constexpr int kBoardSize = 3;
constexpr char kPlayerO = 'O';
constexpr char kPlayerX = 'X';
constexpr char kEmptyCell = '-';
constexpr char kInvalidCell = '?';

bool isInsideBoard(int x, int y)
{
	return x >= 0 && x < kBoardSize && y >= 0 && y < kBoardSize;
}

char nextPlayer(char player)
{
	return player == kPlayerO ? kPlayerX : kPlayerO;
}
}

GameState::GameState() : currentPlayer(kPlayerO), running(false)
{
	resetBoard();
}

void GameState::initGame()
{
	running = true;
	initBoardAndPlayer();
}

void GameState::restartRound()
{
	running = true;
	initBoardAndPlayer();
}

void GameState::stopGame()
{
	running = false;
}

bool GameState::isRunning() const
{
	return running;
}

char GameState::getCurrentPlayer() const
{
	return currentPlayer;
}

char GameState::getCell(int x, int y) const
{
	if (!isInsideBoard(x, y))
	{
		return kInvalidCell;
	}
	return board[x][y];
}

bool GameState::inputPiece(int x, int y)
{
	if (!canAcceptMove())
	{
		return false;
	}

	if (!isInsideBoard(x, y))
	{
		return false;
	}

	if (board[x][y] != kEmptyCell)
	{
		return false;
	}

	board[x][y] = currentPlayer;
	currentPlayer = nextPlayer(currentPlayer);
	return true;
}

void GameState::comPlay()
{
	if (!canAcceptMove())
	{
		return;
	}

	int index = -1;
	minMaxSearch(index, 1, INT_MIN, INT_MAX);
	if (index >= 0)
	{
		inputPiece(index / kBoardSize, index % kBoardSize);
	}
}

GameState::RoundResult GameState::getRoundResult() const
{
	if (checkWin(kPlayerX))
	{
		return RoundResult::XWin;
	}
	if (checkWin(kPlayerO))
	{
		return RoundResult::OWin;
	}
	if (checkDraw())
	{
		return RoundResult::Draw;
	}
	return RoundResult::Ongoing;
}

void GameState::initBoardAndPlayer()
{
	static bool seeded = false;
	if (!seeded)
	{
		srand(static_cast<unsigned int>(time(nullptr)));
		seeded = true;
	}

	currentPlayer = (rand() % 2 == 0) ? kPlayerO : kPlayerX;
	resetBoard();
}

void GameState::resetBoard()
{
	for (int x = 0; x < kBoardSize; ++x)
	{
		for (int y = 0; y < kBoardSize; ++y)
		{
			board[x][y] = kEmptyCell;
		}
	}
}

bool GameState::checkWin(char c) const
{
	for (int i = 0; i < kBoardSize; ++i)
	{
		if (board[i][0] == c && board[i][1] == c && board[i][2] == c)
		{
			return true;
		}
		if (board[0][i] == c && board[1][i] == c && board[2][i] == c)
		{
			return true;
		}
	}

	if (board[0][0] == c && board[1][1] == c && board[2][2] == c)
	{
		return true;
	}
	if (board[2][0] == c && board[1][1] == c && board[0][2] == c)
	{
		return true;
	}

	return false;
}

bool GameState::checkDraw() const
{
	for (int x = 0; x < kBoardSize; ++x)
	{
		for (int y = 0; y < kBoardSize; ++y)
		{
			if (board[x][y] == kEmptyCell)
			{
				return false;
			}
		}
	}
	return true;
}

int GameState::winnerState() const
{
	if (checkWin(kPlayerX))
	{
		return 1;
	}
	if (checkWin(kPlayerO))
	{
		return 0;
	}
	return -1;
}

int GameState::eval() const
{
	int emptyNum = 0;
	for (int i = 0; i < kBoardSize; ++i)
	{
		for (int j = 0; j < kBoardSize; ++j)
		{
			if (board[i][j] == kEmptyCell)
			{
				++emptyNum;
			}
		}
	}

	const int winFlag = winnerState();
	if (winFlag == 1)
	{
		return emptyNum + 1;
	}
	if (winFlag == 0)
	{
		return -(emptyNum + 1);
	}
	return 0;
}

bool GameState::canAcceptMove() const
{
	// 先判断 running，再看棋盘结果，避免胜负已定后继续消费鼠标点击。
	return running && getRoundResult() == RoundResult::Ongoing;
}

int GameState::minMaxSearch(int& index, int step, int a, int b)
{
	if (winnerState() >= 0 || checkDraw())
	{
		return eval();
	}

	std::vector<int> positions;
	for (int i = 0; i < kBoardSize; ++i)
	{
		for (int j = 0; j < kBoardSize; ++j)
		{
			if (board[i][j] == kEmptyCell)
			{
				positions.push_back(i * kBoardSize + j);
			}
		}
	}

	const bool isXTurn = (step & 1) != 0;
	int bestScore = isXTurn ? INT_MIN : INT_MAX;

	for (int i = 0; i < static_cast<int>(positions.size()); ++i)
	{
		const int position = positions[i];
		board[position / kBoardSize][position % kBoardSize] = isXTurn ? kPlayerX : kPlayerO;
		int child = position;
		const int childValue = minMaxSearch(child, step + 1, a, b);
		board[position / kBoardSize][position % kBoardSize] = kEmptyCell;

		if (isXTurn)
		{
			if (childValue > bestScore)
			{
				bestScore = childValue;
				if (step == 1)
				{
					index = position;
				}
			}
			a = (a > bestScore) ? a : bestScore;
		}
		else
		{
			if (childValue < bestScore)
			{
				bestScore = childValue;
			}
			b = (b < bestScore) ? b : bestScore;
		}

		if (a >= b)
		{
			break;
		}
	}

	return bestScore;
}
