#pragma once

class GameState;

// 将“棋局结束后怎么办”从 Widget 主循环中拆出来，便于单独维护。
class GameFlowController
{
public:
	enum class PostGameAction
	{
		Continue,
		Restarted,
		ReturnToModeSelection
	};

	PostGameAction handleRoundEnd(GameState& gameState);
};
