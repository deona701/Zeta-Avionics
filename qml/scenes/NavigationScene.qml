import QtQuick

Item {
    id: navigationSceneRoot
    anchors.fill: parent

    Column {
    anchors.fill: parent
    anchors.topMargin: 25
    anchors.bottomMargin: 95
    anchors.leftMargin: 95
    anchors.rightMargin: 25
    spacing: 25

        Rectangle {
            width: parent.width
            height: (parent.height - parent.spacing) * 0.65
            radius: 10
            color: "transparent"
            border.color: "white"
            border.width: 2
        }

        Rectangle {
            width: parent.width
            height: (parent.height - parent.spacing) * 0.35
            radius: 10
            color: "transparent"
            border.color: "white"
            border.width: 2

            Row {
                anchors.centerIn: parent
                anchors.margins: 15
                spacing: 240

                Column {
                    spacing: 40

                    Text {
                        text: "CURRENT POSITION"
                        color: "white"
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }

                    Text {
                        text: "DESTINATION"
                        color: "white"
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }
                }

                Column {
                    spacing: 40

                    Text {
                        text: "DISTANCE"
                        color: "white"
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }

                    Text {
                        text: "VELOCITY"
                        color: "white"
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }
                }

                Column {
                    spacing: 40

                    Text {
                        text: "ESTIMATED TIME OF ARRIVAL"
                        color: "white"
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }

                    Text {
                        text: "TRAJECTORY"
                        color: "white"
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }
                }
            }
        }
    }
}
