# TicTacToe_w_Friends
A port of SDL Tic Tac Toe (I wrote in 2008) to Qt5/QML.

To compile only requires Qt5 and linux development headers. (build-essentials)

First compile the Server:
navigate to the TicTacToeServer folder and run
  
       qmake TicTacToeServer.pro
       make
  
then start the server (./tictactoeserver)

Then compile the client:
Navigate up a directory (cd ..)
  
      qmake TicTacToe_W_Friends.pro
      make
  
then start the server (probably want two clients) - ./tictactoe_w_friends

