import QtQuick 2.4

Rectangle {
    width: parent.width
    height: parent.height
    property alias nameBox:nameBox
    gradient: Gradient {
        GradientStop {
            position: 0.00;
            color: "#4f4f4f";
        }
        GradientStop {
            position: 0.46;
            color: "#777777";
        }
        GradientStop {
            position: 1.00;
            color: "#b9b9b9";
        }
    }

    Connections{
        target:  Server
       onConnectedToServer:{
           lobbyView.loginName = nameBox.text;
           tictactoeGame.moveToLobbyScreen()
           tictactoeGame.isNetworkedGame = true;
        }
       onDisconnectedFromServer:{
           tictactoeGame.moveToHomeScreen()
           nameBox.text = ""
           lobbyView.loginName = ""
           tictactoeGame.isNetworkedGame = false
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
        id:name
        anchors.bottom:  connectButton.top
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
        TextInput{
            id:nameBox
            anchors.fill: parent
            maximumLength: 30
            text:""
            horizontalAlignment: TextEdit.AlignHCenter
            verticalAlignment: TextEdit.AlignVCenter
            font.pixelSize: ((parent.height * .8 < (width/text.length)) ? (parent.height * .8):(width/text.length))
            onAccepted:
            {
                if(nameBox.text.length > 0)
                    Server.connectToServer(nameBox.text)
            }
        }
    }
    Rectangle{
        id:connectButton
        anchors.bottom:  parent.bottom
        anchors.bottomMargin: parent.height/30
        anchors.horizontalCenter: parent.horizontalCenter
        width:parent.width/2
        height: parent.height/8
        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#1905ff";
            }
            GradientStop {
                position: 0.38;
                color: "#080562";
            }
            GradientStop {
                position: 0.62;
                color: "#03044b";
            }
            GradientStop {
                position: 1.00;
                color: "#052eff";
            }
        }
        Text{
            text: "Connect To Server"
            color: "white"
            anchors.fill: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: parent.height *.45
        }
        MouseArea{
            anchors.fill: parent
            onClicked:{
                if(nameBox.text.length > 0)
                    Server.connectToServer(nameBox.text)
            }
        }
    }

}

