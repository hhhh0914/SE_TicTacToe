#ifndef SE_TICTACTOE_CONSOLEVIEW_H
#define SE_TICTACTOE_CONSOLEVIEW_H

#include <string>
#include "../Model/Board.h"
#include "../Model/GameState.h"
#include "../Model/Player.h"

namespace tictactoe {

class ConsoleView {
public:
    ConsoleView();
    ~ConsoleView();

    void displayBoard(const Board& board) const;
    void displayTurn(const Player& player) const;
    void displayResult(const GameState& state) const;
    void displayMenu() const;
    void displayMessage(const std::string& msg) const;
    void displayError(const std::string& msg) const;
    void displayInfo(const std::string& msg) const;
    void displayInstructions() const;
    void displayGameStart(const std::string& mode) const;
    void clearScreen() const;
    void displaySeparator() const;
    void displayTitle() const;

private:
    static std::string cellToString(CellState cell);

    enum Color : int {
        COLOR_DEFAULT = 7,
        COLOR_CYAN    = 11,
        COLOR_WHITE   = 15,
        COLOR_RED     = 12,
        COLOR_BLUE    = 9,
        COLOR_YELLOW  = 14,
        COLOR_GREEN   = 10,
        COLOR_GRAY    = 8,
        COLOR_MAGENTA = 13,
    };

    void setColor(int color) const;
    void resetColor() const;
    void print(const std::string& text) const;
    void printLine() const;

#ifdef _WIN32
    void* hConsole_;
#endif
};

} // namespace tictactoe

#endif // SE_TICTACTOE_CONSOLEVIEW_H
