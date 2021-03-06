#ifndef TICTACTOEGAME_H
#define TICTACTOEGAME_H

#include <QObject>

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
    bool   playersTurn()
    {
        return mCurrentPiece;
    }

    bool   isGameOver();
    QString       boardToString();
signals:
    void   invalidMoveTaken();
    void   invalidMoveNotYourTurn();
    void   invalidMoveOutOfBounds();
    void   validMoveMade(bool piece, int cell);
    void   validNetworkMoveMade(bool piece, int cell);
    void   gameOverDraw();
    void   gameOverWon();
    void   gameOverLoss();
    void   boardReset(bool players_piece);
    void   boardUpdated(QString board_data);

public slots:
    bool   makeMove(bool piece,int cell);
    void   makeNetworkMove(bool piece, int cell);
    void   setupNewGame(bool  piece_isX = true);

private:
    void              findWinner(bool & game_over, bool & opponent_won, bool & draw);
    quint8         mBoardData[3][3];
    quint8         mTurnsLeft;
    bool             mCurrentPiece;
    bool             mPlayersPiece;

};

#endif // TICTACTOEGAME_H
