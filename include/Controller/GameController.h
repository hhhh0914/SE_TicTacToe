#ifndef SE_TICTACTOE_GAMECONTROLLER_H
#define SE_TICTACTOE_GAMECONTROLLER_H

#include <memory>
#include "../Model/GameState.h"
#include "../View/ConsoleView.h"
#include "AIPlayer.h"

namespace tictactoe {

class GameController {
public:
    GameController();
    void run();

private:
    std::unique_ptr<GameState> gameState_;
    ConsoleView view_;
    AIPlayer::Difficulty aiDifficulty_ = AIPlayer::Difficulty::Hard;
    bool running_ = true;

    void startGame(int mode);
    void gameLoop();
    int getPlayerInput(const Board& board);
    bool playAgain();
    std::string getPlayerName(int playerNum);
};

} // namespace tictactoe

#endif // SE_TICTACTOE_GAMECONTROLLER_H
