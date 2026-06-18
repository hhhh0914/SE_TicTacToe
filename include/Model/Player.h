#ifndef SE_TICTACTOE_PLAYER_H
#define SE_TICTACTOE_PLAYER_H

#include <string>
#include "Board.h"

namespace tictactoe {

class Player {
public:
    enum class Type { Human, AI };

    Player() = default;
    Player(std::string name, CellState symbol, Type type);

    const std::string& getName() const { return name_; }
    CellState getSymbol() const { return symbol_; }
    Type getType() const { return type_; }
    bool isAI() const { return type_ == Type::AI; }

    void setName(const std::string& name) { name_ = name; }

private:
    std::string name_;
    CellState symbol_ = CellState::Empty;
    Type type_ = Type::Human;
};

} // namespace tictactoe

#endif // SE_TICTACTOE_PLAYER_H
