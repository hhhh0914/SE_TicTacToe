#include "../../include/View/ConsoleView.h"
#include <iostream>
#include <cstdlib>

namespace tictactoe {

std::string ConsoleView::cellToString(CellState cell) {
    switch (cell) {
        case CellState::X:  return " X ";
        case CellState::O:  return " O ";
        default:            return "   ";
    }
}

void ConsoleView::printHorizontalLine() const {
    std::cout << "---+---+---\n";
}

void ConsoleView::displayBoard(const Board& board) const {
    clearScreen();
    std::cout << "\n";
    std::cout << "       1   2   3\n";
    std::cout << "     +---+---+---+\n";
    for (int r = 0; r < Board::SIZE; ++r) {
        std::cout << "   " << (r + 1) << " |";
        for (int c = 0; c < Board::SIZE; ++c) {
            std::cout << cellToString(board.getCell(r, c));
            if (c < Board::SIZE - 1) std::cout << "|";
        }
        std::cout << "|\n";
        if (r < Board::SIZE - 1) {
            std::cout << "     +---+---+---+\n";
        }
    }
    std::cout << "     +---+---+---+\n\n";
}

void ConsoleView::displayTurn(const Player& player) const {
    std::cout << "--- Current Turn ---\n";
    std::cout << "  Player: " << player.getName()
              << " (" << ((player.getSymbol() == CellState::X) ? "X" : "O") << ")\n\n";
}

void ConsoleView::displayResult(const GameState& state) const {
    displaySeparator();
    switch (state.getStatus()) {
        case GameState::Status::XWon: {
            const Player& winner = (state.getPlayer1().getSymbol() == CellState::X)
                ? state.getPlayer1() : state.getPlayer2();
            std::cout << "  🎉  Player " << winner.getName() << " (X) WINS! 🎉\n";
            break;
        }
        case GameState::Status::OWon: {
            const Player& winner = (state.getPlayer1().getSymbol() == CellState::O)
                ? state.getPlayer1() : state.getPlayer2();
            std::cout << "  🎉  Player " << winner.getName() << " (O) WINS! 🎉\n";
            break;
        }
        case GameState::Status::Draw:
            std::cout << "  🤝  It's a DRAW!  🤝\n";
            break;
        default:
            break;
    }
    displaySeparator();
}

void ConsoleView::displayMenu() const {
    clearScreen();
    std::cout << "\n";
    displaySeparator();
    std::cout << "     TIC-TAC-TOE (井字棋)\n";
    displaySeparator();
    std::cout << "  1. Player vs Player\n";
    std::cout << "  2. Player vs AI (Easy)\n";
    std::cout << "  3. Player vs AI (Medium)\n";
    std::cout << "  4. Player vs AI (Hard - Minimax)\n";
    std::cout << "  5. AI vs AI (Watch Mode)\n";
    std::cout << "  0. Exit\n";
    displaySeparator();
    std::cout << "  Select mode (0-5): ";
    std::cout.flush();
}

void ConsoleView::displayMessage(const std::string& msg) const {
    std::cout << "  " << msg << "\n";
    std::cout.flush();
}

void ConsoleView::displayInstructions() const {
    std::cout << "  Enter row and column numbers (1-3) separated by space.\n";
    std::cout << "  Example: 1 2  means row 1, column 2\n";
    std::cout << "  Enter 0 0 to quit the current game.\n\n";
}

void ConsoleView::displayGameStart(const std::string& mode) const {
    std::cout << "\n  Starting: " << mode << "\n";
    displayInstructions();
}

void ConsoleView::clearScreen() const {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void ConsoleView::displaySeparator() const {
    std::cout << "  =================================\n";
}

} // namespace tictactoe
