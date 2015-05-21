import QtQuick 2.4
import QtQuick.Window 2.2

Window {
    id: tictactoeGame
    visible: true
    color:"black"
    height:720
    width:1280
    property bool isNetworkedGame: false
    function moveToGameScreen(){
        homeView.visible = false
        homeView.enabled = false
        connectView.enabled = false
        connectView.visible = false
        lobbyView.visible = false
        lobbyView.enabled = false
        boardView.visible = true
        boardView.enabled = true
        boardView.ticTacToeBoard.enabled = true
        tttDialog.visible = false;
    }
    function moveToLobbyScreen(){
        homeView.visible = false
        homeView.enabled = false
        connectView.enabled = false
        connectView.visible = false
        lobbyView.visible = true
        lobbyView.enabled = true
        boardView.visible = false
        boardView.enabled = false
        tttDialog.visible = false;
    }
    function moveToHomeScreen(){
        homeView.visible = true
        homeView.enabled = true
        connectView.enabled = false
        connectView.visible = false
        lobbyView.visible = false
        lobbyView.enabled = false
        boardView.visible = false
        boardView.enabled = false
        tttDialog.visible = false;
    }
    function moveToConnectScreen(){
        homeView.visible = false
        homeView.enabled = false
        connectView.enabled = true
        connectView.visible = true
        lobbyView.visible = false
        lobbyView.enabled = false
        boardView.visible = false
        boardView.enabled = false
        tttDialog.visible = false;
        connectView.nameBox.focus = true
    }
    function disconnectFromLobby()
    {
        lobbyView.disconnect();
        moveToHomeScreen();
    }


    HomeView{
        id:homeView
        //visible: false
        anchors.centerIn: parent
    }
    ConnectView{
        id:connectView
        visible: false
        anchors.centerIn: parent
    }
    LobbyView{
        id:lobbyView
        visible: false
        anchors.centerIn:  parent
    }

    Dialog{
        id: tttDialog
        visible: false
        width:parent.width* .8
        height: parent.height * .66
        titleText:"You Lost!"
        text: "Too bad. play again?\n Click one below."
        anchors.centerIn: parent
        z: parent.z +20
        image: "/images/lose.png"
        function openYouWonDialog()
        {
            tttDialog.titleText = lobbyView.loginName + "  Won!  "
            tttDialog.image =  "/images/win.png"
            tttDialog.text  =  "Congragulations. Play Again?\n Click one below."
            tttDialog.visible = true
            boardView.ticTacToeBoard.enabled = false
            tttDialog.b1Text.text = "Play Again"
            tttDialog.b2Text.text = "Leave"
            tttDialog.isGameEndMenu = true
            tttDialog.button1.visible = (isNetworkedGame? false:true)
            tttDialog.button2.visible = true

        }
        function openYouLostDialog()
        {
            tttDialog.titleText = lobbyView.opponentName + "  Won!  "
            tttDialog.image =  "/images/lose.png"
            tttDialog.text  =  "Too bad. Play Again?\n Click one below."
            boardView.ticTacToeBoard.enabled = false
            tttDialog.b1Text.text = "Play Again"
            tttDialog.b2Text.text = "Leave"
            tttDialog.isGameEndMenu = true
            tttDialog.button1.visible = (isNetworkedGame? false:true)
            tttDialog.button2.visible = true
            tttDialog.visible = true
        }
        function openCatsGameDialog()
        {
            tttDialog.titleText = "      You Tie!      "
            tttDialog.image =  "/images/info.png"
            tttDialog.text  =  "Play Again?\n Click one below."
            boardView.ticTacToeBoard.enabled = false
            tttDialog.b1Text.text = "Play Again"
            tttDialog.b2Text.text = "Leave"
            tttDialog.isGameEndMenu = true
            tttDialog.button1.visible = (isNetworkedGame? false:true)
            tttDialog.button2.visible = true
            tttDialog.visible = true

        }

        function openReceivedChallenge( from)
        {
            tttDialog.titleText =  from;
            tttDialog.image = "/images/info.png"
            tttDialog.text =   "Has challenged you!\nWould like to play them?"
            tttDialog.visible = true
            tttDialog.button1.visible = true
            tttDialog.button2.visible = true
            tttDialog.b1Text.text = "Accept"
            tttDialog.b2Text.text = "Deny"
            tttDialog.isGameEndMenu = false
            lobbyView.enabled =false

        }
        function openWaitingToJoin()
        {
            tttDialog.titleText = "Starting Game"
            tttDialog.image = "/images/win.png"
            tttDialog.text = "Waiting for available Game Lobby..."
            tttDialog.visible = true
            tttDialog.button1.visible = false
            tttDialog.button2.visible = false
            tttDialog.isGameEndMenu = false
            lobbyView.enabled = false
        }
        function openWaitingForReply()
        {
            tttDialog.titleText = "Waiting on Reply"
            tttDialog.image = "/images/info.png"
            tttDialog.text = "Waiting for Reply from " + lobbyView.opponentName + "..."
            tttDialog.visible = true
            tttDialog.button1.visible = false
            tttDialog.button2.visible = false
            tttDialog.isGameEndMenu = false
            lobbyView.enabled = false
        }
    }
    BoardView{
           id:boardView
           anchors.centerIn: parent
    }
    Component.onCompleted: {
        moveToHomeScreen()
    }
}
