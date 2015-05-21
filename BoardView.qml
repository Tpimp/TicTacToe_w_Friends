import QtQuick 2.4

Rectangle {
    width: parent.width
    height: parent.height
    gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#0011ff";
            }
            GradientStop {
                position: 1.00;
                color: "#6678ff";
        }
    }

    property alias ticTacToeBoard: ticTacToeBoard
    TicTacToeBoard{
        id: ticTacToeBoard
        height: parent.height
        width: parent.width*.75
        anchors.leftMargin: parent.width/30
        anchors.topMargin: parent.height/30
        anchors.bottomMargin: parent.height/30
        anchors.rightMargin: parent.width/20
        anchors.left:  parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: buttonPanel.left
    }
    Rectangle{// Button Container
        // size and id (dynamic sizing with binding)
        height: parent.height
        width: parent.width*.25
        id: buttonPanel
        color: "black"
        // positioning on the screen (parent is "Window" in this case)
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.topMargin: parent.height/30
        anchors.bottomMargin: parent.height/30
        Rectangle{
            id: currentMove
            anchors.top:parent.top
            anchors.left:parent.left
            anchors.right:parent.right
            color:"black"
            height:parent.height/2
            Text{
                id:currentPiece
                anchors.top: parent.top
                height: parent.height/4
                anchors.margins: 2
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter
                text:"Current Player"
                color:"yellow"
                font.pixelSize: height*.25
            }
            Image{
                anchors.verticalCenter: parent.verticalCenter
                height: parent.height/2
                anchors.margins: 2
                source: (ticTacToeBoard.currentTurn ? "/images/X.bmp":"/images/O.Bmp")
                anchors.left:parent.left
                anchors.right:parent.right
            }
            Text{
                id:currentTurn
                height: parent.height/4
                anchors.margins: 2
                anchors.left: parent.left
                color:"yellow"
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: height*.25
                text: (isNetworkedGame ?
                           (ticTacToeBoard.currentTurn == ticTacToeBoard.playersPiece )?
                            "Is Your Turn":"Not Your Turn":"")
            }
         }

        // New Game Button
        Rectangle
        {
            color: "black"
            height: parent.height/6
            visible: !isNetworkedGame
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: leaveButton.top
            anchors.bottomMargin: parent.height/10
            border.color: "white"
            border.width: 4
            Text
            {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment:   Text.AlignVCenter
                color: "yellow"
                text: "New Game"
                font.pixelSize: height * .35 < width/text.length
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    TicTacToeGame.setupNewGame();
                }
            }
        }
        Rectangle
        {
            id: leaveButton
            color: "black"
            height: parent.height/6
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            border.color: "white"
            border.width: 4
            Text
            {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment:   Text.AlignVCenter
                color: "yellow"
                text: "Leave Game"
                font.pixelSize: height * .4
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    if(isNetworkedGame)
                    {
                        Server.sendLeavingGame();
                        moveToLobbyScreen()
                    }
                    else
                    {
                        moveToHomeScreen()
                    }
                }
            }
        }
    }
}

