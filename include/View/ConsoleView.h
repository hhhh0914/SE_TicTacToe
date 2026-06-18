#ifndef SE_TICTACTOE_CONSOLEVIEW_H
#define SE_TICTACTOE_CONSOLEVIEW_H

#include <string>
#include "../Model/Board.h"
#include "../Model/GameState.h"
#include "../Model/Player.h"

namespace tictactoe {

class ConsoleView {
public:
    void displayBoard(const Board& board) const;
    void displayTurn(const Player& player) const;
    void displayResult(const GameState& state) const;
    void displayMenu() const;
    void displayMessage(const std::string& msg) const;
    void displayInstructions() const;
    void displayGameStart(const std::string& mode) const;
    void clearScreen() const;
    void displaySeparator() const;

private:
    void printHorizontalLine() const;
    static std::string cellToString(CellState cell);
};

} // namespace tictactoe

#endif // SE_TICTACTOE_CONSOLEVIEW_H
