import QtQuick 2.4

Rectangle {
    id: board
    width: 360
    height: 240
    property real spacerWidth: (board.width * .0125)
    property real pieceW: (board.width - (spacerWidth*2))/3
    property real pieceH: (board.height - (spacerWidth*2))/3
    property bool playersPiece: true
    property real cellInQuestion: -1
    function setCellGraphic(cell_index, cell_data)
    {
        switch(cell_index)
        {
            case 0:
                cell0.source = (cell_data > 0 ? (cell_data == 1 ? "/images/X.bmp":"/images/O.Bmp"):"")
            break;
            case 1:
                cell1.source = (cell_data > 0 ? (cell_data == 1 ? "/images/X.bmp":"/images/O.Bmp"):"")
            break;
            case 2:
                cell2.source = (cell_data > 0 ? (cell_data == 1 ? "/images/X.bmp":"/images/O.Bmp"):"")
            break;
            case 3:
                cell3.source = (cell_data > 0 ? (cell_data == 1 ? "/images/X.bmp":"/images/O.Bmp"):"")
            break;
            case 4:
                cell4.source = (cell_data > 0 ? (cell_data == 1 ? "/images/X.bmp":"/images/O.Bmp"):"")
            break;
            case 5:
                cell5.source = (cell_data > 0 ? (cell_data == 1 ? "/images/X.bmp":"/images/O.Bmp"):"")
            break;
            case 6:
                cell6.source = (cell_data > 0 ? (cell_data == 1 ? "/images/X.bmp":"/images/O.Bmp"):"")
            break;
            case 7:
                 cell7.source = (cell_data > 0 ? (cell_data == 1 ? "/images/X.bmp":"/images/O.Bmp"):"")
            break;
            case 8:
                cell8.source = (cell_data > 0 ? (cell_data == 1 ? "/images/X.bmp":"/images/O.Bmp"):"")
            break;
            default:break;
        }
    }

    Connections{
        target: TicTacToeGame
        onGameOverWon:
        {
            tttDialog.openYouWonDialog();
        }
        onGameOverLoss:
        {
            tttDialog.openYouLostDialog();
        }
        onGameOverDraw:
        {
            tttDialog.openCatsGameDialog();
        }

        onBoardUpdated:
        {
            var index;
            for(index = 0; index < board_data.length; ++index)
            {
                setCellGraphic(index,board_data[index]);
            }
        }
        onBoardReset:
        {
            cell0.source = ""
            cell1.source = ""
            cell2.source = ""
            cell3.source = ""
            cell4.source = ""
            cell5.source = ""
            cell6.source = ""
            cell7.source = ""
            cell8.source = ""
            playersPiece = players_piece;
            tttDialog.visible = false;
        }
        onValidMoveMade:
        {
            switch(cellInQuestion)
            {
                case 0:
                    cell0.source = (playersPiece ? "/images/X.bmp":"/images/O.Bmp")
                break;
                case 1:
                    cell1.source = (playersPiece ? "/images/X.bmp":"/images/O.Bmp")
                break;
                case 2:
                    cell2.source = (playersPiece ? "/images/X.bmp":"/images/O.Bmp")
                break;
                case 3:
                    cell3.source = (playersPiece ? "/images/X.bmp":"/images/O.Bmp")
                break;
                case 4:
                    cell4.source = (playersPiece ? "/images/X.bmp":"/images/O.Bmp")
                break;
                case 5:
                    cell5.source = (playersPiece ? "/images/X.bmp":"/images/O.Bmp")
                break;
                case 6:
                    cell6.source = (playersPiece ? "/images/X.bmp":"/images/O.Bmp")
                break;
                case 7:
                     cell7.source = (playersPiece ? "/images/X.bmp":"/images/O.Bmp")
                break;
                case 8:
                    cell8.source = (playersPiece ? "/images/X.bmp":"/images/O.Bmp")
                break;
                default:break;
            }

            playersPiece = !playersPiece;
        }
    }

    Grid{ // define the Board Grid
        id:boardGrid
        // space for the amount calculated above (dynamic)
        spacing: spacerWidth
        anchors.fill: parent
        // align everything to the center
        horizontalItemAlignment: Grid.AlignHCenter
        verticalItemAlignment: Grid.AlignVCenter
        columns: 3
        rows: 3 //  3 x 3
        Rectangle{
            id:cell0
            width: pieceW
            height: pieceH
            color: "black"
            property alias source: image0.source
            Image{ // an image to display
                id:image0
                anchors.fill: parent
                source: ""
            }
            MouseArea{ // a mouse area for clicking
                anchors.fill: parent
                onClicked: {
                    cellInQuestion = 0;
                    TicTacToeGame.makeMove(playersPiece,0,0);
                }
            } // End CELL 0
        }
        Rectangle{
            id:cell1
            width: pieceW
            height: pieceH
            color: "black"
            property alias source: image1.source
            Image{
                id:image1
                anchors.fill: parent
                source: ""
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    cellInQuestion = 1;
                    TicTacToeGame.makeMove(playersPiece,0,1);
                }
            }
        }
        Rectangle{
            id:cell2
            width: pieceW
            height: pieceH
            color: "black"
            property alias source: image2.source
            Image{
                id:image2
                anchors.fill: parent
                source: ""
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    cellInQuestion = 2;
                    TicTacToeGame.makeMove(playersPiece,0,2);
                }
            }
        }

        Rectangle{
            id:cell3
            width: pieceW
            height: pieceH
            color: "black"
            property alias source: image3.source
            Image{
                id:image3
                anchors.fill: parent
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    cellInQuestion = 3;
                    TicTacToeGame.makeMove(playersPiece,1,0);
                }
            }
        }
        Rectangle{
            id:cell4
            width: pieceW
            height: pieceH
            color: "black"
            property alias source: image4.source
            Image{
                id:image4
                anchors.fill: parent
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    cellInQuestion = 4;
                    TicTacToeGame.makeMove(playersPiece,1,1);
                }
            }
        }
        Rectangle{
            id:cell5
            width: pieceW
            height: pieceH
            color: "black"
            property alias source: image5.source
            Image{
                id:image5
                anchors.fill: parent
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    cellInQuestion = 5;
                    TicTacToeGame.makeMove(playersPiece,1,2);
                }
            }
        }
        Rectangle{
            id:cell6
            width: pieceW
            height: pieceH
            color: "black"
            property alias source: image6.source
            Image{
                id:image6
                anchors.fill: parent
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    cellInQuestion = 6;
                    TicTacToeGame.makeMove(playersPiece,2,0);
                }
            }
        }
        Rectangle{
            id:cell7
            width: pieceW
            height: pieceH
            color: "black"
            property alias source: image7.source
            Image{
                id:image7
                anchors.fill: parent
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    cellInQuestion = 7;
                    TicTacToeGame.makeMove(playersPiece,2,1);
                }
            }
        }
        Rectangle{
            id:cell8
            width: pieceW
            height: pieceH
            color: "black"
            property alias source: image8.source
            Image{
                id:image8
                anchors.fill: parent
            }
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    cellInQuestion = 8;
                    TicTacToeGame.makeMove(playersPiece,2,2);
                }
            }
        }
    }
    // Define the # bars
    Rectangle{
        id:topBar
        height:spacerWidth
        width: parent.width
        border.color: "yellow"
        border.width: 1
        color: "yellow"
        anchors.verticalCenter: boardGrid.verticalCenter
        anchors.verticalCenterOffset: -((pieceH+spacerWidth) /2)
        anchors.left: boardGrid.left
        anchors.right:boardGrid.right
    }
    Rectangle{
        id:bottomBar
        height:spacerWidth
        width: parent.width
        border.color: "yellow"
        border.width: 1
        color: "yellow"
        anchors.verticalCenter: boardGrid.verticalCenter
        anchors.verticalCenterOffset: ((pieceH-spacerWidth)/2) + spacerWidth
        anchors.left: boardGrid.left
        anchors.right:boardGrid.right
    }
    Rectangle{
        id:leftBar
        width:spacerWidth
        height: parent.height
        border.color: "yellow"
        border.width: 1
        color: "yellow"
        anchors.horizontalCenter: boardGrid.horizontalCenter
        anchors.horizontalCenterOffset: -((pieceW+spacerWidth) /2)
        anchors.top: boardGrid.top
        anchors.bottom:boardGrid.bottom
    }
    Rectangle{
        id:rightBar
        width:spacerWidth
        height: parent.height
        border.color: "yellow"
        border.width: 1
        color: "yellow"
        anchors.horizontalCenter: boardGrid.horizontalCenter
        anchors.horizontalCenterOffset: ((pieceW-spacerWidth)/2) + spacerWidth
        anchors.top: boardGrid.top
        anchors.bottom:boardGrid.bottom
    }
}

