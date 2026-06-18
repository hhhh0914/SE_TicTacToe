#ifndef SE_TICTACTOE_AIPLAYER_H
#define SE_TICTACTOE_AIPLAYER_H

#include "../Model/Board.h"

namespace tictactoe {

class AIPlayer {
public:
    enum class Difficulty { Easy, Medium, Hard };

    static int getBestMove(const Board& board, CellState aiSymbol, Difficulty difficulty = Difficulty::Hard);

private:
    static int minimax(Board& board, CellState currentPlayer, bool isMaximizing, int depth, CellState aiSymbol);
    static int evaluateBoard(const Board& board, CellState aiSymbol);
    static int getRandomMove(const Board& board);
    static int getHeuristicMove(const Board& board, CellState aiSymbol);
};

} // namespace tictactoe

#endif // SE_TICTACTOE_AIPLAYER_H
