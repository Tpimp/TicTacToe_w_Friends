import QtQuick 2.4

Rectangle {
    id: dialog
    border.color: "white"
    border.width: 4
    radius: parent.width/64
    color:"black"
    property alias image:dialogIcon.source
    property alias text:dialogText.text
    property alias titleText:dialogTitle.text
    property alias textPixelSize: dialogText.font.pixelSize
    property alias b1Text: button1Text
    property alias b2Text: button2Text
    property alias b1Mouse: button1MouseArea
    property alias b2Mouse: button2MouseArea
    property alias button1:button1
    property alias button2:button2
    property bool isGameEndMenu: false
    // The idea here is to provide base styling
    Rectangle{
        id:titleContainer
        anchors.top: parent.top
        anchors.topMargin: parent.width/30
        anchors.leftMargin: parent.width/10
        anchors.rightMargin: parent.width/10
        anchors.horizontalCenter: parent.horizontalCenter
        width: (dialogTitle.text.length * dialogTitle.font.pixelSize ) > parent.width ?
                        (parent.width *.9):(dialogTitle.text.length * dialogTitle.font.pixelSize )
        color:"black"
        border.color: "yellow"
        border.width: 1
        radius: width/3
        height:parent.height/3
        Rectangle{
            color:"black"
            id:iconContainer
            anchors.left:parent.left
            anchors.top:parent.top
            anchors.bottom: parent.bottom
            anchors.margins: 10
            anchors.rightMargin: 4
            width:parent.width/3
            height: width
            radius: width
            clip:true
            Image{
                id:dialogIcon
                anchors.fill: parent
                anchors.margins:1
                fillMode: Image.PreserveAspectFit
            }
        }
        Text{
            id:dialogTitle
            anchors.leftMargin: 8
            anchors.left:iconContainer.right
            anchors.top: parent.top
            anchors.bottom:parent.bottom
            anchors.right:parent.right
            horizontalAlignment: Text.AlignLeft
            verticalAlignment:   Text.AlignVCenter
            color: "yellow"
            text: ""
            font.pixelSize: height * .3
        }
    }
    Flow
    {
        id: flow
        anchors.top: titleContainer.bottom
        anchors.left: parent.left
        anchors.leftMargin: parent.width/10
        anchors.right: parent.right
        anchors.rightMargin: anchors.leftMargin
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/4
        Text{
            id:dialogText
            height: parent.height*.75
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment:   Text.AlignVCenter
            color: "yellow"
            text: ""
            font.pixelSize: height * .3
        }
            Rectangle{
                width:dialog.width * .8
                color: "black"
                height: iconContainer.height
               // anchors.bottom: parent.bottom
              //  anchors.left: parent.left
              //  anchors.right: parent.right
             //   anchors.margins:  parent.width/30
             //   anchors.top:flow.bottom
                Rectangle{
                    id: button1
                    anchors.left: parent.left
                    border.color: "white"
                    border.width: 2
                    anchors.bottom: parent.bottom
                    anchors.top: parent.top
                    width:parent.width * .44
                    color:"blue"
                    radius: width/2
                    visible: button1Text.text != ""
                    Text{
                        id:button1Text
                        text: ""
                        visible: text != ""
                        color: "white"
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: parent.height *.45
                    }
                    MouseArea{
                        id:button1MouseArea
                        anchors.fill: parent
                        onClicked: {
                            if(isGameEndMenu)
                            {
                                if(isNetworkedGame)
                                {
                                    Server.requestChallenge(lobbyView.opponentName)
                                    tttDialog.openWaitingForReply()
                                }
                                else
                                {
                                    TicTacToeGame.setupNewGame()
                                }
                            }
                            else
                            {
                                Server.sendAcceptChallenge(lobbyView.opponentName,true);
                                tttDialog.openWaitingToJoin()
                            }

                        }

                    }
                }
                Rectangle{
                    id:button2
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.top: parent.top
                    width:parent.width * .44
                    color:"blue"
                    border.width: 2
                    border.color: "white"
                    visible: button2Text.text != ""
                    radius:width/2
                    Text{
                        id:button2Text
                        text: ""
                        color:  "white"
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        font.pixelSize: parent.height *.45
                    }
                    MouseArea{
                        id:button2MouseArea
                        anchors.fill: parent
                        onClicked: {
                            if(isGameEndMenu)
                            {
                                if(isNetworkedGame)
                                {
                                    Server.sendLeavingGame()
                                    tictactoeGame.moveToLobbyScreen()
                                }
                                else
                                    tictactoeGame.moveToHomeScreen()
                            }
                            else
                            {
                                Server.sendAcceptChallenge(lobbyView.opponentName,false);
                                moveToLobbyScreen()
                            }
                            dialog.visible = false
                        }
                    }
                }
        }
    }
}

