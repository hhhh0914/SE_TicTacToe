#include "../../include/Controller/GameController.h"
#include "../../include/Controller/AIPlayer.h"
#include <iostream>
#include <thread>
#include <chrono>

namespace tictactoe {

GameController::GameController()
    : gameState_(nullptr) {}

void GameController::run() {
    while (running_) {
        view_.displayMenu();

        std::string input;
        std::getline(std::cin, input);
        if (input.empty()) continue;

        int choice = 0;
        try {
            choice = std::stoi(input);
        } catch (...) {
            view_.displayMessage("Please enter a number (0-5).");
            view_.displayMessage("Press Enter to continue...");
            std::cin.get();
            continue;
        }

        if (choice == 0) {
            view_.clearScreen();
            view_.displayMessage("Thanks for playing! Goodbye.");
            running_ = false;
            return;
        }

        if (choice < 1 || choice > 5) {
            view_.displayMessage("Please select a valid option (0-5).");
            view_.displayMessage("Press Enter to continue...");
            std::cin.get();
            continue;
        }

        startGame(choice);

        if (!playAgain()) {
            running_ = false;
        }
    }
}

void GameController::startGame(int mode) {
    view_.clearScreen();

    std::string modeName;
    bool p1IsAI = false, p2IsAI = false;

    switch (mode) {
        case 1:
            modeName = "Player vs Player";
            break;
        case 2:
            modeName = "Player vs AI (Easy)";
            p2IsAI = true;
            aiDifficulty_ = AIPlayer::Difficulty::Easy;
            break;
        case 3:
            modeName = "Player vs AI (Medium)";
            p2IsAI = true;
            aiDifficulty_ = AIPlayer::Difficulty::Medium;
            break;
        case 4:
            modeName = "Player vs AI (Hard - Minimax)";
            p2IsAI = true;
            aiDifficulty_ = AIPlayer::Difficulty::Hard;
            break;
        case 5:
            modeName = "AI vs AI (Watch Mode)";
            p1IsAI = true;
            p2IsAI = true;
            aiDifficulty_ = AIPlayer::Difficulty::Hard;
            break;
    }

    view_.displayGameStart(modeName);

    Player player1 = (p1IsAI)
        ? Player("AI-X", CellState::X, Player::Type::AI)
        : Player(getPlayerName(1), CellState::X, Player::Type::Human);

    Player player2 = (p2IsAI)
        ? Player("AI-O", CellState::O, Player::Type::AI)
        : Player(getPlayerName(2), CellState::O, Player::Type::Human);

    gameState_ = std::make_unique<GameState>(std::move(player1), std::move(player2));
    gameLoop();
}

void GameController::gameLoop() {
    while (true) {
        view_.displayBoard(gameState_->getBoard());
        view_.displayTurn(gameState_->getCurrentPlayer());

        const auto& currentPlayer = gameState_->getCurrentPlayer();
        int move;

        if (currentPlayer.isAI()) {
            view_.displayMessage("AI thinking...");
            std::this_thread::sleep_for(std::chrono::milliseconds(600));
            move = AIPlayer::getBestMove(
                gameState_->getBoard(),
                currentPlayer.getSymbol(),
                aiDifficulty_
            );
        } else {
            move = getPlayerInput(gameState_->getBoard());
            if (move == -1) {
                view_.displayMessage("Game abandoned. Back to menu...");
                std::this_thread::sleep_for(std::chrono::milliseconds(800));
                return;
            }
        }

        int row = move / Board::SIZE;
        int col = move % Board::SIZE;
        gameState_->getBoard().place(row, col, currentPlayer.getSymbol());

        gameState_->updateStatus();

        if (gameState_->getStatus() != GameState::Status::Playing) {
            view_.displayBoard(gameState_->getBoard());
            view_.displayResult(*gameState_);
            return;
        }

        gameState_->switchTurn();
    }
}

int GameController::getPlayerInput(const Board& board) {
    while (true) {
        std::cout << "  Enter your move (row col, 0 0 to quit): ";
        std::cout.flush();

        std::string line;
        std::getline(std::cin, line);
        if (line.empty()) continue;

        int row = 0, col = 0;
        if (sscanf_s(line.c_str(), "%d %d", &row, &col) < 2) {
            view_.displayMessage("Enter two numbers separated by space (e.g., 1 2).");
            continue;
        }

        if (row == 0 && col == 0) return -1;

        int r = row - 1;
        int c = col - 1;

        if (r < 0 || r >= Board::SIZE || c < 0 || c >= Board::SIZE) {
            view_.displayMessage("Position out of range. Use 1-3 for row and column.");
            continue;
        }

        if (board.getCell(r, c) != CellState::Empty) {
            view_.displayMessage("That cell is already taken. Choose another.");
            continue;
        }

        return r * Board::SIZE + c;
    }
}

bool GameController::playAgain() {
    std::cout << "\n  Play again? (y/n): ";
    std::cout.flush();
    std::string input;
    std::getline(std::cin, input);
    return !input.empty() && (input[0] == 'y' || input[0] == 'Y');
}

std::string GameController::getPlayerName(int playerNum) {
    std::cout << "  Enter name for Player " << playerNum << ": ";
    std::cout.flush();
    std::string name;
    std::getline(std::cin, name);
    return name.empty() ? ("Player " + std::to_string(playerNum)) : name;
}

} // namespace tictactoe
