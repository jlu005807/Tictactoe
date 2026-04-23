#include "..\\include\\GameFlow.h"

#include <graphics.h>
#include <tchar.h>

#include "..\\include\\Game.h"

GameFlowController::PostGameAction GameFlowController::handleRoundEnd(GameState& gameState)
{
	// Ongoing 时不要打断渲染循环，只有终局才弹窗询问下一步。
	const GameState::RoundResult result = gameState.getRoundResult();
	if (result == GameState::RoundResult::Ongoing)
	{
		return PostGameAction::Continue;
	}

	const TCHAR* message = _T("Draw!\nPlay again?");
	if (result == GameState::RoundResult::XWin)
	{
		message = _T("X wins!\nPlay again?");
	}
	else if (result == GameState::RoundResult::OWin)
	{
		message = _T("O wins!\nPlay again?");
	}

	const int choice = MessageBox(GetHWnd(), message, _T("Game Over"), MB_YESNO | MB_ICONQUESTION);
	if (choice == IDYES)
	{
		gameState.restartRound();
		return PostGameAction::Restarted;
	}

	gameState.stopGame();
	return PostGameAction::ReturnToModeSelection;
}
