import QtQuick 2.4

Rectangle {
    width: parent.width
    height: parent.height
    gradient: Gradient {
        GradientStop {
            position: 0.04;
            color: "#515151";
        }

        GradientStop {
            position: 1.00;
            color: "#00000000";
        }
    }
    property string loginName: ""
    property string opponentName:""
    function disconnect()
    {
        Server.disconnectFromServer()
        lobbyModel.clear()
    }

    Connections{
        target: Server
        onClientFound:{
            if(name != loginName)
            {
                lobbyModel.append({"text":name, "state":"Challenge"});
            }
            else
            {
                lobbyModel.append({"text":name, "state":""});
            }
            //console.log(name);
        }
        onClientLeftLobby:
        {
            var index =0;
            var found = false;
            for(index; index < lobbyModel.count; ++index)
            {
                var current_item = lobbyModel.get(index);
                if(current_item.text == name)
                {
                    found = true;
                    break;
                }
                else
                {
                    continue;
                }

            }
            if(found)
                lobbyModel.remove(index);
        }
        onClearLobby:
        {
            lobbyModel.clear()
        }
        onBoardUpdated:
        {

        }

        onReceivedChallenge:
        {
            opponentName = from;
            tttDialog.openReceivedChallenge(from);
        }
        onReceivedChallengeReply:
        {
            if(accepted)
                tttDialog.openWaitingToJoin()
            else
            {
                // find the player and change state of model
                var index =0;
                var found = false;
                tttDialog.visible = false;
                for(index; index < lobbyModel.count; ++index)
                {
                    var current_item = lobbyModel.get(index);
                    if(current_item.text == from)
                    {
                        current_item.state = "Denied"
                        break;
                    }
                    else
                    {
                        continue;
                    }

                }

            }
        }
        onServerNameChanged:
        {
            console.log("Received name changed " + name)
            loginName = name
        }

        onNetworkGameStarted:
        {
            console.log("Recieved start new game " );
            if(playersPiece)
                console.log("As X")
            else
                console.log("As O")
            TicTacToeGame.setupNewGame(playersPiece);
            moveToGameScreen()
        }

        onNetworkGameEnded:
        {
            moveToLobbyScreen()
        }
    }
    ListModel{
        id:lobbyModel
    }

    Rectangle{
        id:titleBar
        anchors.top:parent.top
        anchors.right: lobbyListView.right
        anchors.left:lobbyListView.left
        width:parent.width * .8
        height: parent.height * .25
        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#06d9f9";
            }
            GradientStop {
                position: 1.00;
                color: "#00d1ff";
            }
        }
        Image{
            anchors.fill: parent
            anchors.margins: 4
            source: "/images/TicTacToeWFriends.png"
        }
    }
    Rectangle{
        id: lobbyListView
        anchors.top: titleBar.bottom
        anchors.left: parent.left
        clip:true
        anchors.right: parent.right
        anchors.leftMargin: parent.width/20
        anchors.rightMargin: parent.width/20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/10
        border.color: "lightsteelblue"
        border.width: 2
        color:"black"
        ListView{
            id: listView
            anchors.fill: parent
            model:lobbyModel
            delegate:
                Rectangle{
                border.width: 2
                color:"white"
                anchors.left: parent.left
                anchors.right: parent.right
                height: 60
                Rectangle{
                    color:"white"
                    border.color: "black"
                    border.width: 2
                    anchors.left: parent.left
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width:parent.width/2
                    Text{
                        anchors.fill: parent
                        font.pixelSize: height*.45
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:  Text.AlignVCenter
                        text:model.text
                    }
                }
                Rectangle{
                    color:"blue"
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    width:parent.width/2
                    Rectangle{
                        color:"lightgrey"
                        border.width: 1
                        border.color:"black"
                        anchors.centerIn: parent
                        width:parent.width *.88
                        height:parent.height *.88
                        visible:  model.state != ""
                        Text{
                            anchors.fill: parent
                            color:"black"
                            text: model.state
                            font.pixelSize: height*.45
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment:  Text.AlignVCenter
                        }
                        MouseArea{
                            anchors.fill: parent
                            onClicked:{
                                Server.requestChallenge(model.text);
                                tttDialog.openWaitingForReply()
                                model.state = "Waiting for Reply..."
                            }
                        }
                    }
                }
            }
        }
    }
    Rectangle{
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        border.width:1
        border.color: "black"
        color:"blue"
        width: parent.width/6
        height:parent.height/12
        anchors.margins: 8
        Text{
            color:"white"
            text:"Logout"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: height*.45
            anchors.fill: parent
        }
        MouseArea{
            anchors.fill: parent
            onClicked:{
                tictactoeGame.disconnectFromLobby()
            }
        }
    }
}

