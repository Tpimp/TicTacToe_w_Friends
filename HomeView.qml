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

    Rectangle{
        id: homeScreen
        clip: true
        color:"transparent"
        height: parent.height * .60
        radius: width
        border.color: "black"
        border.width: 6
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 20
        Image{
            width:parent.width
            height:parent.height
            anchors.centerIn: parent
            anchors.margins: 4
            source: "/images/TicTacToeWFriends.png"
        }
    }

    Rectangle{
        id:localGame
        anchors.bottom:  networkGame.top
        anchors.bottomMargin: parent.height/30
        anchors.horizontalCenter: parent.horizontalCenter
        width:parent.width/2
        height: parent.height/8
        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#fffb00";
            }
            GradientStop {
                position: 0.80;
                color: "#e4d504";
            }
        }
        Text{
            text: "Play Local Game"
            color: "black"
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: parent.height *.45
        }
        MouseArea{
            anchors.fill: parent
            onClicked:
            {
                tictactoeGame.isNetworkedGame = false;
                lobbyView.loginName = "    X  "
                lobbyView.opponentName = "   O    "
                TicTacToeGame.setupNewGame()
                tictactoeGame.moveToGameScreen()
            }
        }
    }
    Rectangle{
        id:networkGame
        anchors.bottom:  parent.bottom
        anchors.bottomMargin: parent.height/30
        anchors.horizontalCenter: parent.horizontalCenter
        width:parent.width/2
        height: parent.height/8
        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#fffb00";
            }
            GradientStop {
                position: 0.80;
                color: "#e4d504";
            }
        }
        Text{
            text: "Play with Friends"
            color: "black"
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: parent.height *.45
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                tictactoeGame.moveToConnectScreen()
            }
        }
    }
}

