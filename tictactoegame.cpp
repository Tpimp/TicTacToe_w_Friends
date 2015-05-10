#include "tictactoegame.h"

TicTacToeGame::TicTacToeGame(int board_width, int board_height, QObject *parent)
    : QObject(parent),
      //C++11 Brace initialization (must have C++11 enabled)
      mBoardData{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
      mPlayersPiece(X_PIECE), mTurnsLeft(9)
{

}


void TicTacToeGame::setupNewGame()
{
    // for each cell, set it to empty
    for(quint8 index(0); index < CELL_COUNT; ++index)
    {
        mBoardData[index/3][index%3] = EMPTY; // calculate boar
    }
    mTurnsLeft = 9;
    mPlayersPiece = X_PIECE;
    emit boardUpdated(QString(' ',9)); // send 9 spaces
}

bool TicTacToeGame::isGameOver()
{
    return (mTurnsLeft == 0);
}

void TicTacToeGame::makeMove(bool piece, int row, int col)
{
    if(piece != mPlayersPiece) // check if current players turn
    {
        emit invalidMoveNotYourTurn(); // notify invalid move not your turn
        return;
    }
    if(row > ROW_COL_MAX || col > ROW_COL_MAX) // invalid data gaurd
    {
        emit invalidMoveOutOfBounds(); // notify invalid move (OOB)
        return;
    }
    if(mBoardData[row][col] != EMPTY) // if move is taken
    {
        emit invalidMoveTaken(); // notify invalid move location already taken
        return;
    }

    //  if made it here, valid move
    mBoardData[row][col]  = (mPlayersPiece ? O_PIECE:X_PIECE);
    mTurnsLeft--; // update mTurnsLeft
    mPlayersPiece = !mPlayersPiece;

    emit validMoveMade(mTurnsLeft); // notify the fronte

}

QString TicTacToeGame::boardToString()
{
    // build a string representing the board
    QString board;
    for(int i(0); i < CELL_COUNT; ++i)
    {
        switch(mBoardData[i/3][i%3])
        {
            case EMPTY: board.append(' '); break;
            case X_PIECE: board.append('x'); break;
            case O_PIECE: board.append('o'); break;
            default: board.append(' '); break;
        }
    }
    return board;
}



TicTacToeGame::~TicTacToeGame()
{

}



