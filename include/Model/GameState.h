#ifndef SE_TICTACTOE_GAMESTATE_H
#define SE_TICTACTOE_GAMESTATE_H

#include "Board.h"
#include "Player.h"

namespace tictactoe {

class GameState {
public:
    enum class Status { Playing, XWon, OWon, Draw };

    GameState() = default;
    GameState(Player p1, Player p2);

    Status getStatus() const { return status_; }
    Board& getBoard() { return board_; }
    const Board& getBoard() const { return board_; }
    Player& getCurrentPlayer() { return (currentPlayerIndex_ == 0) ? player1_ : player2_; }
    const Player& getCurrentPlayer() const { return (currentPlayerIndex_ == 0) ? player1_ : player2_; }
    const Player& getPlayer1() const { return player1_; }
    const Player& getPlayer2() const { return player2_; }

    void switchTurn();
    void updateStatus();
    void reset();

    int getCurrentPlayerIndex() const { return currentPlayerIndex_; }

private:
    Board board_;
    Player player1_;
    Player player2_;
    int currentPlayerIndex_ = 0;
    Status status_ = Status::Playing;
};

} // namespace tictactoe

#endif // SE_TICTACTOE_GAMESTATE_H
