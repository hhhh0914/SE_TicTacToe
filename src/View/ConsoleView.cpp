#include "../../include/View/ConsoleView.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <algorithm>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

namespace tictactoe {

// UTF-8 box-drawing characters
namespace Box {
    const char* H  = "\xe2\x95\x90";  // ═
    const char* V  = "\xe2\x95\x91";  // ║
    const char* TL = "\xe2\x95\x94";  // ╔
    const char* TR = "\xe2\x95\x97";  // ╗
    const char* BL = "\xe2\x95\x9a";  // ╚
    const char* BR = "\xe2\x95\x9d";  // ╝
    const char* T  = "\xe2\x95\xa0";  // ╠
    const char* B  = "\xe2\x95\xa3";  // ╣
    const char* LH = "\xe2\x94\x8c";  // ┌
    const char* RH = "\xe2\x94\x90";  // ┐
    const char* LL = "\xe2\x94\x94";  // └
    const char* RL = "\xe2\x94\x98";  // ┘
    const char* DH = "\xe2\x94\x80";
}

ConsoleView::ConsoleView() {
#ifdef _WIN32
    hConsole_ = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleOutputCP(CP_UTF8);
#endif
}

ConsoleView::~ConsoleView() {
    resetColor();
}

void ConsoleView::setColor(int color) const {
#ifdef _WIN32
    SetConsoleTextAttribute(static_cast<HANDLE>(hConsole_), static_cast<WORD>(color));
#endif
}

void ConsoleView::resetColor() const {
    setColor(COLOR_DEFAULT);
}

void ConsoleView::print(const std::string& text) const {
    std::cout << text;
    std::cout.flush();
}

void ConsoleView::printLine() const {
    std::cout << "\n";
}

std::string ConsoleView::cellToString(CellState cell) {
    switch (cell) {
        case CellState::X:  return " X ";
        case CellState::O:  return " O ";
        default:            return "   ";
    }
}

void ConsoleView::clearScreen() const {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void ConsoleView::displaySeparator() const {
    setColor(COLOR_GRAY);
    std::cout << "  " << Box::T;
    for (int i = 0; i < 38; ++i) std::cout << Box::H;
    std::cout << Box::B << "\n";
    resetColor();
}

void ConsoleView::displayTitle() const {
    setColor(COLOR_CYAN);
    std::cout << "  " << Box::TL;
    for (int i = 0; i < 38; ++i) std::cout << Box::H;
    std::cout << Box::TR << "\n";

    setColor(COLOR_WHITE);
    std::cout << "  " << Box::V
              << "            TIC-TAC-TOE              "
              << Box::V << "\n";
    resetColor();
}

void ConsoleView::displayBoard(const Board& board) const {
    clearScreen();
    displayTitle();
    printLine();

    setColor(COLOR_GRAY);
    std::cout << "     " << Box::V << "   1   " << Box::V
              << "   2   " << Box::V << "   3   " << Box::V << "\n";
    std::cout << "     " << Box::LH;
    for (int i = 0; i < 3; ++i) {
        std::cout << Box::DH << Box::DH << Box::DH << Box::DH << Box::DH
                  << Box::DH << Box::DH;
        if (i < 2) std::cout << Box::DH << Box::DH;
    }
    std::cout << Box::RH << "\n";
    resetColor();

    for (int r = 0; r < 3; ++r) {
        setColor(COLOR_YELLOW);
        std::cout << "  " << (r + 1) << "  ";
        resetColor();
        setColor(COLOR_GRAY);
        std::cout << Box::V;
        resetColor();

        for (int c = 0; c < 3; ++c) {
            CellState cell = board.getCell(r, c);
            if (cell == CellState::X) {
                setColor(COLOR_RED);
                std::cout << " X ";
                resetColor();
            } else if (cell == CellState::O) {
                setColor(COLOR_BLUE);
                std::cout << " O ";
                resetColor();
            } else {
                std::cout << "   ";
            }
            if (c < 2) {
                setColor(COLOR_GRAY);
                std::cout << Box::V;
                resetColor();
            }
        }
        setColor(COLOR_GRAY);
        std::cout << Box::V;
        resetColor();
        printLine();
    }

    setColor(COLOR_GRAY);
    std::cout << "     " << Box::LL;
    for (int i = 0; i < 3; ++i) {
        std::cout << Box::DH << Box::DH << Box::DH << Box::DH << Box::DH
                  << Box::DH << Box::DH;
        if (i < 2) std::cout << Box::DH << Box::DH;
    }
    std::cout << Box::RL << "\n";
    resetColor();
    printLine();
}

void ConsoleView::displayTurn(const Player& player) const {
    setColor(COLOR_CYAN);
    std::cout << "  " << Box::V << " Turn: ";
    resetColor();

    if (player.getSymbol() == CellState::X) {
        setColor(COLOR_RED);
    } else {
        setColor(COLOR_BLUE);
    }
    std::cout << player.getName() << " ("
              << ((player.getSymbol() == CellState::X) ? "X" : "O") << ")";
    resetColor();
    printLine();
    printLine();
}

void ConsoleView::displayResult(const GameState& state) const {
    std::string message;
    Color playerColor = COLOR_DEFAULT;

    switch (state.getStatus()) {
        case GameState::Status::XWon: {
            const Player& winner = (state.getPlayer1().getSymbol() == CellState::X)
                ? state.getPlayer1() : state.getPlayer2();
            playerColor = COLOR_RED;
            message = "Player " + winner.getName() + " (X) WINS!";
            break;
        }
        case GameState::Status::OWon: {
            const Player& winner = (state.getPlayer1().getSymbol() == CellState::O)
                ? state.getPlayer1() : state.getPlayer2();
            playerColor = COLOR_BLUE;
            message = "Player " + winner.getName() + " (O) WINS!";
            break;
        }
        case GameState::Status::Draw:
            playerColor = COLOR_YELLOW;
            message = "It's a DRAW!";
            break;
        default:
            return;
    }

    printLine();
    int fw = (std::max)(26, static_cast<int>(message.length()) + 8);

    setColor(playerColor);
    std::cout << "  " << Box::TL;
    for (int i = 0; i < fw; ++i) std::cout << Box::H;
    std::cout << Box::TR << "\n";

    for (int row = 0; row < 3; ++row) {
        std::cout << "  " << Box::V;
        for (int i = 0; i < fw; ++i) std::cout << " ";
        std::cout << Box::V << "\n";
        if (row == 1) {
            // Back up and print message in the middle row
            std::cout << "\r  " << Box::V << "    ";
            setColor(COLOR_WHITE);
            std::cout << message;
            resetColor();
            setColor(playerColor);
            for (int i = 0; i < fw + 4 - static_cast<int>(message.length()) - 4; ++i) std::cout << " ";
            std::cout << Box::V << "\n";
        }
    }

    std::cout << "  " << Box::BL;
    for (int i = 0; i < fw; ++i) std::cout << Box::H;
    std::cout << Box::BR << "\n";
    resetColor();
    printLine();
}

void ConsoleView::displayMenu() const {
    clearScreen();
    printLine();

    setColor(COLOR_CYAN);
    std::cout << "  " << Box::TL;
    for (int i = 0; i < 38; ++i) std::cout << Box::H;
    std::cout << Box::TR << "\n";

    setColor(COLOR_WHITE);
    std::cout << "  " << Box::V
              << "              TIC-TAC-TOE              "
              << Box::V << "\n";

    setColor(COLOR_CYAN);
    std::cout << "  " << Box::T;
    for (int i = 0; i < 38; ++i) std::cout << Box::H;
    std::cout << Box::B << "\n";

    struct MenuItem { const char* label; int color; };
    MenuItem items[] = {
        {"[1] Player vs Player", COLOR_GREEN},
        {"[2] Player vs AI (Easy)", COLOR_YELLOW},
        {"[3] Player vs AI (Medium)", COLOR_YELLOW},
        {"[4] Player vs AI (Hard - Minimax)", COLOR_MAGENTA},
        {"[5] AI vs AI (Watch Mode)", COLOR_CYAN},
        {"[0] Exit", COLOR_GRAY},
    };

    for (const auto& item : items) {
        std::cout << "  " << Box::V << "  ";
        setColor(item.color);
        std::cout << item.label;
        resetColor();
        int pad = 38 - static_cast<int>(std::strlen(item.label)) - 2;
        for (int i = 0; i < pad; ++i) std::cout << " ";
        setColor(COLOR_CYAN);
        std::cout << Box::V << "\n";
        resetColor();
    }

    setColor(COLOR_CYAN);
    std::cout << "  " << Box::BL;
    for (int i = 0; i < 38; ++i) std::cout << Box::H;
    std::cout << Box::BR << "\n";

    resetColor();
    std::cout << "  Your choice [0-5]: ";
    std::cout.flush();
}

void ConsoleView::displayMessage(const std::string& msg) const {
    setColor(COLOR_CYAN);
    std::cout << "  " << Box::V << " ";
    resetColor();
    std::cout << msg << "\n";
    std::cout.flush();
}

void ConsoleView::displayError(const std::string& msg) const {
    setColor(COLOR_RED);
    std::cout << "  >> " << msg << " <<\n";
    resetColor();
    std::cout.flush();
}

void ConsoleView::displayInfo(const std::string& msg) const {
    setColor(COLOR_GREEN);
    std::cout << "  " << msg << "\n";
    resetColor();
    std::cout.flush();
}

void ConsoleView::displayInstructions() const {
    setColor(COLOR_GRAY);
    std::cout << "  Enter: row col (e.g. 1 2)  |  0 0 to quit\n";
    resetColor();
    printLine();
}

void ConsoleView::displayGameStart(const std::string& mode) const {
    displayInfo("Starting: " + mode);
    printLine();
    displayInstructions();
}

} // namespace tictactoe