import QtQuick 2.4
import QtQuick.Window 2.2

Window {
    visible: true
    color:"black"
    TicTacToeBoard{
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

        // New Game Button
        Rectangle
        {
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
                text: "New Game"
                font.pixelSize: height * .4
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    TicTacToeGame.setupNewGame();
                }
            }
        }

    }
    Dialog{
        id: tttDialog
        visible: false
        width:parent.width/2
        height: parent.height/3
        titleText:"You Lost!"
        text: "Too bad. play again?\n Click one below."
        anchors.centerIn: parent
        image: "/images/lose.png"
        function openYouWonDialog()
        {
            tttDialog.titleText = "You Won!"
            tttDialog.image =  "/images/win.png"
            tttDialog.text  =  "Congragulations. Play Again?\n Click one below."
            tttDialog.visible = true
        }
        function openYouLostDialog()
        {
            tttDialog.titleText = "You Lost!"
            tttDialog.image =  "/images/lose.png"
            tttDialog.text  =  "Too bad. Play Again?\n Click one below."
            tttDialog.visible = true
        }
        function openCatsGameDialog()
        {
            tttDialog.titleText = "You Tie!"
            tttDialog.image =  "/images/info.png"
            tttDialog.text  =  "Play Again?\n Click one below."
            tttDialog.visible = true
        }
        function openConnectionLostDialog()
        {

        }

    }
}
