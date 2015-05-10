#ifndef TICTACTOEGAME_H
#define TICTACTOEGAME_H

#include <QObject>
#include <QPixmap>

#define  X_PIECE     1
#define  O_PIECE     2
#define  EMPTY       0
#define  ROW_COL_MAX 2
#define  CELL_COUNT  9

class TicTacToeGame : public QObject
{
    Q_OBJECT
public:
    explicit TicTacToeGame(int board_width, int board_height, QObject *parent = 0);
    ~TicTacToeGame();

    bool isGameOver();
signals:
    void   invalidMoveTaken();
    void   invalidMoveNotYourTurn();
    void   invalidMoveOutOfBounds();
    void   validMoveMade(int turns_left);
    void   gameOverDraw();
    void   gameOverWon();
    void   gameOverLoss();
    void   boardReset(bool players_piece);
    void   boardUpdated(QString board_data);

public slots:
    void   makeMove(bool piece, int row, int col);
    void   setupNewGame();

private:
    QString          boardToString();
    void             findWinner(bool & game_over, bool & opponent_won, bool & draw);
    quint8           mBoardData[3][3];
    quint8           mTurnsLeft;
    bool             mCurrentPiece;
    bool             mPlayersPiece;

};

#endif // TICTACTOEGAME_H
