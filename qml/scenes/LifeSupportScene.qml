import QtQuick

Item {
    Rectangle {
        anchors.fill: parent
        radius: 10
        anchors.topMargin: 25
        anchors.bottomMargin: 95
        anchors.leftMargin: 95
        anchors.rightMargin: 25
        color: "transparent"

        Row {
            spacing: 15

            Column {

                Rectangle {
                    width: 380
                    height: 300
                    radius: 6
                    color: "black"
                    border.color: "white"

                    Text {
                        text: "Life Support"
                        color: "white"
                        anchors.centerIn: parent
                        font.bold: true
                        font.pixelSize: Theme.fontSmall
                    }
                }

                Rectangle {
                    width: 380
                    height: 300
                    radius: 6
                    color: "black"
                    border.color: "white"

                    Text {
                        text: "Life Support"
                        color: "white"
                        anchors.centerIn: parent
                        font.bold: true
                        font.pixelSize: Theme.fontSmall
                    }
                }
            }

            Column {
                Rectangle {
                    width: 380
                    height: 300
                    radius: 6
                    color: "black"
                    border.color: "white"

                    Text {
                        text: "Life Support"
                        color: "white"
                        anchors.centerIn: parent
                        font.bold: true
                        font.pixelSize: Theme.fontSmall
                    }
                }
                Rectangle {
                    width: 380
                    height: 300
                    radius: 6
                    color: "black"
                    border.color: "white"

                    Text {
                        text: "Life Support"
                        color: "white"
                        anchors.centerIn: parent
                        font.bold: true
                        font.pixelSize: Theme.fontSmall
                    }
                }
            }

            Column {
                Rectangle {
                    width: 380
                    height: 600
                    radius: 6
                    color: "black"
                    border.color: "white"


                    Text {
                        text: "Life Support"
                        color: "white"
                        anchors.centerIn: parent
                        font.bold: true
                        font.pixelSize: Theme.fontSmall
                    }
                }
            }
        }
    }
}
