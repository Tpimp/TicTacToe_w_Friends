import QtQuick 2.4

Rectangle {
    width: 300
    height:150
    border.color: "white"
    border.width: 4
    radius: parent.width/64
    color:"black"
    property alias image:dialogIcon.source
    property alias text:dialogText.text
    property alias titleText:dialogTitle.text
    property alias textPixelSize: dialogText.font.pixelSize

    // The idea here is to provide base styling
    Rectangle{
        id:titleContainer
        anchors.top: parent.top
        anchors.topMargin: parent.width/30
        anchors.leftMargin: parent.width/10
        anchors.rightMargin: parent.width/10
        anchors.horizontalCenter: parent.horizontalCenter
        width:parent.width*.4
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
            font.pixelSize: height * .4
        }
    }
    Flow
    {
        anchors.top: titleContainer.bottom
        anchors.left: parent.left
        anchors.leftMargin: parent.width/8
        anchors.right: parent.right
        anchors.rightMargin: anchors.leftMargin
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parent.height/8
        Text{
            id:dialogText
            height: parent.height
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment:   Text.AlignVCenter
            color: "yellow"
            text: ""
            font.pixelSize: height * .2
        }
    }

}

