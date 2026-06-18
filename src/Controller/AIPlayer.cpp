#include "../../include/Controller/AIPlayer.h"
#include <algorithm>
#include <random>
#include <limits>
#include <chrono>

namespace tictactoe {

int AIPlayer::getBestMove(const Board& board, CellState aiSymbol, Difficulty difficulty) {
    switch (difficulty) {
        case Difficulty::Easy:
            return getRandomMove(board);
        case Difficulty::Medium:
            return getHeuristicMove(board, aiSymbol);
        case Difficulty::Hard: {
            // Minimax: find the best move by simulating all possibilities
            Board simBoard = board;
            int bestScore = std::numeric_limits<int>::min();
            int bestMove = -1;

            auto emptyCells = simBoard.getEmptyCells();
            for (const auto& [r, c] : emptyCells) {
                int move = r * Board::SIZE + c;
                simBoard.place(r, c, aiSymbol);
                int score = minimax(simBoard, opponent(aiSymbol), false, 0, aiSymbol);
                simBoard.place(r, c, CellState::Empty); // undo

                if (score > bestScore) {
                    bestScore = score;
                    bestMove = move;
                }
            }
            return bestMove;
        }
    }
    return getRandomMove(board);
}

int AIPlayer::minimax(Board& board, CellState currentPlayer, bool isMaximizing, int depth, CellState aiSymbol) {
    CellState winner = board.getWinner();
    if (winner != CellState::Empty) {
        return (winner == aiSymbol) ? (10 - depth) : (depth - 10);
    }
    if (board.isFull()) {
        return 0; // draw
    }

    auto emptyCells = board.getEmptyCells();
    if (emptyCells.empty()) return 0;

    if (isMaximizing) {
        int best = std::numeric_limits<int>::min();
        for (const auto& [r, c] : emptyCells) {
            board.place(r, c, currentPlayer);
            int score = minimax(board, opponent(currentPlayer), false, depth + 1, aiSymbol);
            board.place(r, c, CellState::Empty);
            best = std::max(best, score);
        }
        return best;
    } else {
        int best = std::numeric_limits<int>::max();
        for (const auto& [r, c] : emptyCells) {
            board.place(r, c, currentPlayer);
            int score = minimax(board, opponent(currentPlayer), true, depth + 1, aiSymbol);
            board.place(r, c, CellState::Empty);
            best = std::min(best, score);
        }
        return best;
    }
}

int AIPlayer::getRandomMove(const Board& board) {
    auto emptyCells = board.getEmptyCells();
    if (emptyCells.empty()) return -1;

    auto seed = static_cast<unsigned int>(
        std::chrono::steady_clock::now().time_since_epoch().count()
    );
    std::mt19937 rng(seed);
    std::uniform_int_distribution<size_t> dist(0, emptyCells.size() - 1);

    const auto& [r, c] = emptyCells[dist(rng)];
    return r * Board::SIZE + c;
}

int AIPlayer::getHeuristicMove(const Board& board, CellState aiSymbol) {
    CellState oppSymbol = opponent(aiSymbol);
    Board simBoard = board;
    auto emptyCells = simBoard.getEmptyCells();

    // 1. Win if possible
    for (const auto& [r, c] : emptyCells) {
        simBoard.place(r, c, aiSymbol);
        if (simBoard.checkWin(aiSymbol)) {
            return r * Board::SIZE + c;
        }
        simBoard.place(r, c, CellState::Empty);
    }

    // 2. Block opponent if about to win
    for (const auto& [r, c] : emptyCells) {
        simBoard.place(r, c, oppSymbol);
        if (simBoard.checkWin(oppSymbol)) {
            return r * Board::SIZE + c;
        }
        simBoard.place(r, c, CellState::Empty);
    }

    // 3. Take center if available
    if (board.getCell(1, 1) == CellState::Empty) {
        return 1 * Board::SIZE + 1;
    }

    // 4. Take corners
    std::vector<std::pair<int, int>> corners = {{0,0}, {0,2}, {2,0}, {2,2}};
    for (const auto& [r, c] : corners) {
        if (board.getCell(r, c) == CellState::Empty) {
            return r * Board::SIZE + c;
        }
    }

    // 5. Fallback to random
    return getRandomMove(board);
}

} // namespace tictactoe
