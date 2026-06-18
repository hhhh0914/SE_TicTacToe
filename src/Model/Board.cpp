#include "../../include/Model/Board.h"

namespace tictactoe {

Board::Board() {
    reset();
}

void Board::reset() {
    for (auto& row : grid_) {
        for (auto& cell : row) {
            cell = CellState::Empty;
        }
    }
}

CellState& Board::cellAt(int row, int col) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        throw std::out_of_range("Board position out of range");
    }
    return grid_[row][col];
}

bool Board::place(int row, int col, CellState piece) {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        return false;
    }
    if (grid_[row][col] != CellState::Empty) {
        return false;
    }
    if (piece == CellState::Empty) {
        return false;
    }
    grid_[row][col] = piece;
    return true;
}

CellState Board::getCell(int row, int col) const {
    if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
        throw std::out_of_range("Board position out of range");
    }
    return grid_[row][col];
}

bool Board::checkWin(CellState piece) const {
    for (const auto& line : WIN_LINES) {
        int r0 = line[0] / SIZE, c0 = line[0] % SIZE;
        int r1 = line[1] / SIZE, c1 = line[1] % SIZE;
        int r2 = line[2] / SIZE, c2 = line[2] % SIZE;
        if (grid_[r0][c0] == piece &&
            grid_[r1][c1] == piece &&
            grid_[r2][c2] == piece) {
            return true;
        }
    }
    return false;
}

CellState Board::getWinner() const {
    for (const auto& line : WIN_LINES) {
        int r0 = line[0] / SIZE, c0 = line[0] % SIZE;
        int r1 = line[1] / SIZE, c1 = line[1] % SIZE;
        int r2 = line[2] / SIZE, c2 = line[2] % SIZE;
        if (grid_[r0][c0] != CellState::Empty &&
            grid_[r0][c0] == grid_[r1][c1] &&
            grid_[r1][c1] == grid_[r2][c2]) {
            return grid_[r0][c0];
        }
    }
    return CellState::Empty;
}

bool Board::isFull() const {
    for (const auto& row : grid_) {
        for (const auto& cell : row) {
            if (cell == CellState::Empty) {
                return false;
            }
        }
    }
    return true;
}

bool Board::isGameOver() const {
    return getWinner() != CellState::Empty || isFull();
}

std::vector<std::pair<int, int>> Board::getEmptyCells() const {
    std::vector<std::pair<int, int>> cells;
    cells.reserve(SIZE * SIZE);
    for (int r = 0; r < SIZE; ++r) {
        for (int c = 0; c < SIZE; ++c) {
            if (grid_[r][c] == CellState::Empty) {
                cells.emplace_back(r, c);
            }
        }
    }
    return cells;
}

bool Board::operator==(const Board& other) const {
    return grid_ == other.grid_;
}

} // namespace tictactoe
