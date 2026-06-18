#include "../../include/Model/GameState.h"

namespace tictactoe {

GameState::GameState(Player p1, Player p2)
    : player1_(std::move(p1))
    , player2_(std::move(p2))
    , currentPlayerIndex_(0)
    , status_(Status::Playing) {}

void GameState::switchTurn() {
    currentPlayerIndex_ = 1 - currentPlayerIndex_;
}

void GameState::updateStatus() {
    if (board_.checkWin(CellState::X)) {
        status_ = Status::XWon;
    } else if (board_.checkWin(CellState::O)) {
        status_ = Status::OWon;
    } else if (board_.isFull()) {
        status_ = Status::Draw;
    } else {
        status_ = Status::Playing;
    }
}

void GameState::reset() {
    board_.reset();
    currentPlayerIndex_ = 0;
    status_ = Status::Playing;
}

} // namespace tictactoe
