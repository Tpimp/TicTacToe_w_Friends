#include "tictactoegame.h"

TicTacToeGame::TicTacToeGame(int board_width, int board_height, QObject *parent)
    : QObject(parent),
      //C++11 Brace initialization (must have C++11 enabled)
      mBoardData{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
      mPlayersPiece(X_PIECE), mCurrentPiece(X_PIECE), mTurnsLeft(9)
{

}


void TicTacToeGame::setupNewGame(bool  piece_isX)
{
    // for each cell, set it to empty
    for(quint8 index(0); index < CELL_COUNT; ++index)
    {
        mBoardData[index/3][index%3] = EMPTY; // calculate boar
    }
    mTurnsLeft = 9;
    mPlayersPiece =  piece_isX;
    mCurrentPiece = X_PIECE;
    emit boardReset(mCurrentPiece);
}

bool TicTacToeGame::isGameOver()
{
    return (mTurnsLeft == 0);
}

void TicTacToeGame::makeNetworkMove(bool piece, int cell)
{
    int row(cell/3);
    int col(cell%3);

    if(piece != mCurrentPiece) // check if current players turn
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
    mBoardData[row][col]  = (mCurrentPiece ? X_PIECE:O_PIECE);
    mTurnsLeft--; // update mTurnsLeft
    mCurrentPiece = !mCurrentPiece;

    emit validNetworkMoveMade(piece,cell); // notify the frontend

    // Check for win (or lose) game case
    bool is_over;
    bool opp_won;
    bool draw;
    findWinner(is_over,opp_won,draw);
    if(is_over)
    {
        if(draw)
        {
           emit gameOverDraw();
        }
        else if(opp_won)
        {
            emit gameOverLoss();
        }
        else
        {
            emit gameOverWon();
        }
    }
}


bool TicTacToeGame::makeMove(bool piece, int cell)
{
    int row(cell/3);
    int col(cell%3);

    if(piece != mCurrentPiece) // check if current players turn
    {
        emit invalidMoveNotYourTurn(); // notify invalid move not your turn
        return false;
    }
    if(row > ROW_COL_MAX || col > ROW_COL_MAX) // invalid data gaurd
    {
        emit invalidMoveOutOfBounds(); // notify invalid move (OOB)
        return false;
    }
    if(mBoardData[row][col] != EMPTY) // if move is taken
    {
        emit invalidMoveTaken(); // notify invalid move location already taken
        return false;
    }

    //  if made it here, valid move
    mBoardData[row][col]  = (mCurrentPiece ? X_PIECE:O_PIECE);
    mTurnsLeft--; // update mTurnsLeft
    mCurrentPiece = !mCurrentPiece;

    emit validMoveMade(piece,cell); // notify the frontend

    // Check for win (or lose) game case
    bool is_over;
    bool opp_won;
    bool draw;
    findWinner(is_over,opp_won,draw);
    if(is_over)
    {
        if(draw)
        {
           emit gameOverDraw();
        }
        else if(opp_won)
        {
            emit gameOverLoss();
        }
        else
        {
            emit gameOverWon();
        }
    }
    return true;

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

void TicTacToeGame::findWinner(bool &game_over, bool &opponent_won, bool &draw)
{
    game_over = false;
    opponent_won = false;
    quint8 opp_piece;
    quint8 my_piece;
    if(mPlayersPiece)
    {
        opp_piece = O_PIECE;
        my_piece  = X_PIECE;
    }
    else
    {
        opp_piece = X_PIECE;
        my_piece  = O_PIECE;
    }
    // USING SHORT CIRCUIT AND BOOLEAN OPERATORS
    // Any set of 3 Trues produces a win condition
    opponent_won = (((mBoardData[0][0] == opp_piece && // if top left
                      ( (mBoardData[0][1] == opp_piece &&
                         mBoardData[0][2] == opp_piece) || //if top row or
                        (mBoardData[1][0] == opp_piece &&
                         mBoardData[2][0] == opp_piece))   // if left col
                         )||
                       (mBoardData[2][2] == opp_piece && // if bottom right
                       ((mBoardData[2][1] == opp_piece &&
                         mBoardData[2][0] == opp_piece) || //if bottom row or
                        (mBoardData[1][2] == opp_piece &&
                         mBoardData[0][2] == opp_piece))   // if right col

                     ) ||
                    // not Left Column, Right Column, Top Row, or Bottom Row -
                    (mBoardData[1][1] == opp_piece  &&// SO CHECK CENTER
                    ((mBoardData[0][1] == opp_piece && //if center
                      mBoardData[2][1] == opp_piece) ||  // check center column or
                     (mBoardData[0][0] == opp_piece &&
                      mBoardData[2][2] == opp_piece) || // check left diag or
                     (mBoardData[0][2] == opp_piece &&
                      mBoardData[2][0] == opp_piece) || // check right diag or
                     (mBoardData[1][0] == opp_piece &&
                      mBoardData[1][2] == opp_piece))    // check center row
                      )));
    game_over = opponent_won;
    if(!game_over)
         game_over = (((mBoardData[0][0] == my_piece && // if top left
                      ( (mBoardData[0][1] == my_piece &&
                         mBoardData[0][2] == my_piece) || //if top row or
                        (mBoardData[1][0] == my_piece &&
                         mBoardData[2][0] == my_piece))   // if left col
                         )||
                       (mBoardData[2][2] == my_piece && // if bottom right
                       ((mBoardData[2][1] == my_piece &&
                         mBoardData[2][0] == my_piece) || //if bottom row or
                        (mBoardData[1][2] == my_piece &&
                         mBoardData[0][2] == my_piece))   // if right col

                     ) ||
                    // not Left Column, Right Column, Top Row, or Bottom Row -
                    (mBoardData[1][1] == my_piece  &&// SO CHECK CENTER
                    ((mBoardData[0][1] == my_piece && //if center
                      mBoardData[2][1] == my_piece) ||  // check center column or
                     (mBoardData[0][0] == my_piece &&
                      mBoardData[2][2] == my_piece) || // check left diag or
                     (mBoardData[0][2] == my_piece &&
                      mBoardData[2][0] == my_piece) || // check right diag or
                     (mBoardData[1][0] == my_piece &&
                      mBoardData[1][2] == my_piece))    // check center row
                      )));

    if(game_over)
        draw = false;
    else
    {
        game_over = draw = (mTurnsLeft <= 0);
    }

}

TicTacToeGame::~TicTacToeGame()
{

}



