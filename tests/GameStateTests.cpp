#include "../Tictactoe/include/Game.h"

#include <cstdlib>
#include <initializer_list>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

namespace
{
void require(bool condition, const std::string& message)
{
	if (!condition)
	{
		std::cerr << "FAIL: " << message << '\n';
		std::exit(1);
	}
}

std::string boardToString(const GameState& gameState)
{
	std::ostringstream stream;
	for (int y = 0; y < 3; ++y)
	{
		for (int x = 0; x < 3; ++x)
		{
			stream << gameState.getCell(x, y);
		}
		if (y < 2)
		{
			stream << '/';
		}
	}
	return stream.str();
}

char nextPlayer(char player)
{
	return player == 'O' ? 'X' : 'O';
}

GameState::RoundResult winResultFor(char player)
{
	return player == 'X' ? GameState::RoundResult::XWin : GameState::RoundResult::OWin;
}

void playMoves(GameState& gameState, std::initializer_list<std::pair<int, int>> moves)
{
	for (const auto& move : moves)
	{
		require(gameState.inputPiece(move.first, move.second), "expected scripted move to be accepted");
	}
}

char playWinningTopRow(GameState& gameState)
{
	// 根据随机先手组局，让先手玩家在顶部横线获胜。
	const char firstPlayer = gameState.getCurrentPlayer();
	playMoves(gameState, { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 }, { 2, 0 } });
	return firstPlayer;
}

char playWinningMiddleRowForSecondPlayer(GameState& gameState)
{
	// 前五手避免先手提前成线，第六手让后手完成中间横线。
	const char secondPlayer = nextPlayer(gameState.getCurrentPlayer());
	playMoves(gameState, { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 }, { 2, 2 }, { 2, 1 } });
	return secondPlayer;
}

void setupXImmediateWin(GameState& gameState)
{
	gameState.initGame();
	if (gameState.getCurrentPlayer() == 'X')
	{
		playMoves(gameState, { { 0, 0 }, { 0, 1 }, { 1, 0 }, { 1, 1 } });
	}
	else
	{
		playMoves(gameState, { { 0, 1 }, { 0, 0 }, { 1, 1 }, { 1, 0 }, { 2, 2 } });
	}

	require(gameState.getCurrentPlayer() == 'X', "X should be the computer player before minimax search");
}

void setupXMustBlockO(GameState& gameState)
{
	gameState.initGame();
	if (gameState.getCurrentPlayer() == 'X')
	{
		playMoves(gameState, { { 1, 1 }, { 0, 0 }, { 2, 2 }, { 1, 0 } });
	}
	else
	{
		playMoves(gameState, { { 0, 0 }, { 1, 1 }, { 1, 0 }, { 2, 2 }, { 2, 1 } });
	}

	require(gameState.getCurrentPlayer() == 'X', "X should be the computer player before blocking");
}

void rejectsInvalidMoves()
{
	GameState gameState;
	gameState.initGame();

	const char firstPlayer = gameState.getCurrentPlayer();
	require(!gameState.inputPiece(-1, 0), "negative x coordinate should be rejected");
	require(!gameState.inputPiece(0, 3), "out-of-range y coordinate should be rejected");
	require(gameState.getCurrentPlayer() == firstPlayer, "invalid moves should not switch player");

	require(gameState.inputPiece(0, 0), "first valid move should be accepted");
	const char next = gameState.getCurrentPlayer();
	require(!gameState.inputPiece(0, 0), "occupied cell should be rejected");
	require(gameState.getCurrentPlayer() == next, "occupied move should not switch player");
}

void detectsFirstPlayerWin()
{
	GameState gameState;
	gameState.initGame();

	const char winner = playWinningTopRow(gameState);
	require(gameState.getRoundResult() == winResultFor(winner), "top row should report the first player as winner");
}

void detectsSecondPlayerWin()
{
	GameState gameState;
	gameState.initGame();

	const char winner = playWinningMiddleRowForSecondPlayer(gameState);
	require(gameState.getRoundResult() == winResultFor(winner), "middle row should report the second player as winner");
}

void detectsDraw()
{
	GameState gameState;
	gameState.initGame();

	playMoves(gameState, { { 0, 0 }, { 1, 0 }, { 2, 0 }, { 1, 1 }, { 0, 1 }, { 2, 1 }, { 1, 2 }, { 0, 2 }, { 2, 2 } });
	require(gameState.getRoundResult() == GameState::RoundResult::Draw, "full board without three-in-a-row should be a draw");
}

void rejectsMovesAfterRoundEnds()
{
	GameState gameState;
	gameState.initGame();
	playWinningTopRow(gameState);

	require(gameState.getRoundResult() != GameState::RoundResult::Ongoing,
		"round should have a winner before checking extra input");

	const char before = gameState.getCell(2, 2);
	require(!gameState.inputPiece(2, 2), "extra move after a win should be rejected");
	require(gameState.getCell(2, 2) == before, "extra move should not change the board");
}

void minimaxTakesImmediateWin()
{
	GameState gameState;
	setupXImmediateWin(gameState);

	gameState.comPlay();

	require(gameState.getCell(2, 0) == 'X', "minimax should complete X's top-row win");
	require(gameState.getRoundResult() == GameState::RoundResult::XWin, "minimax winning move should end with XWin");
}

void minimaxBlocksImmediateLoss()
{
	GameState gameState;
	setupXMustBlockO(gameState);

	gameState.comPlay();

	require(gameState.getCell(2, 0) == 'X',
		"minimax should block O's top-row threat, board=" + boardToString(gameState));
	require(gameState.getRoundResult() == GameState::RoundResult::Ongoing, "blocking move should keep the round ongoing");
}
}

int main()
{
	rejectsInvalidMoves();
	detectsFirstPlayerWin();
	detectsSecondPlayerWin();
	detectsDraw();
	rejectsMovesAfterRoundEnds();
	minimaxTakesImmediateWin();
	minimaxBlocksImmediateLoss();

	std::cout << "All GameState tests passed.\n";
	return 0;
}
