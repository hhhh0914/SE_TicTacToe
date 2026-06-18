#include "../../include/Model/Player.h"

namespace tictactoe {

Player::Player(std::string name, CellState symbol, Type type)
    : name_(std::move(name))
    , symbol_(symbol)
    , type_(type) {
    if (symbol == CellState::Empty) {
        throw std::invalid_argument("Player symbol cannot be Empty");
    }
}

} // namespace tictactoe
