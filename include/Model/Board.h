#ifndef SE_TICTACTOE_BOARD_H
#define SE_TICTACTOE_BOARD_H

#include <array>
#include <vector>
#include <utility>
#include <stdexcept>

namespace tictactoe {

enum class CellState : char {
    Empty = ' ',
    X = 'X',
    O = 'O'
};

inline CellState opponent(CellState piece) {
    if (piece == CellState::X) return CellState::O;
    if (piece == CellState::O) return CellState::X;
    return CellState::Empty;
}

class Board {
public:
    static constexpr int SIZE = 3;
    using Grid = std::array<std::array<CellState, SIZE>, SIZE>;

    Board();

    bool place(int row, int col, CellState piece);
    CellState getCell(int row, int col) const;

    bool checkWin(CellState piece) const;
    CellState getWinner() const;
    bool isFull() const;
    bool isGameOver() const;

    std::vector<std::pair<int, int>> getEmptyCells() const;

    void reset();

    const Grid& getGrid() const { return grid_; }

    bool operator==(const Board& other) const;

private:
    Grid grid_;

    static constexpr std::array<std::array<int, 3>, 8> WIN_LINES = {{
        {0,1,2}, {3,4,5}, {6,7,8},  // rows
        {0,3,6}, {1,4,7}, {2,5,8},  // cols
        {0,4,8}, {2,4,6}            // diagonals
    }};

    CellState& cellAt(int row, int col);
};

} // namespace tictactoe

#endif // SE_TICTACTOE_BOARD_H
