import QtQuick
import QtQuick3D
import "../components/"

Item {
    id: navigationSceneRoot

    Column {
        id: navMainColumn
        anchors.fill: parent
        anchors.topMargin: Theme.sceneMarginTop
        anchors.bottomMargin: Theme.sceneMarginBottom
        anchors.leftMargin: Theme.sceneMarginLeft
        anchors.rightMargin: Theme.sceneMarginRight
        spacing: 25

        Rectangle {
            width: parent.width
            height: (parent.height - parent.spacing) * 0.65
            radius: Theme.cornerRadius
            color: "#80000000"
            border.color: Theme.light

            PlanetsView {}
        }

        Rectangle {
            width: parent.width
            height: (parent.height - parent.spacing) * 0.35
            radius: Theme.cornerRadius
            color: Theme.dark
            border.color: Theme.light
            border.width: 2

            Row {
                anchors.centerIn: parent
                anchors.margins: 15
                spacing: 240

                Column {
                    spacing: 40

                    Text {
                        text: "CURRENT POSITION \n Earth"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }

                    Text {
                        text: "DESTINATION \n Mars"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }
                }

                Column {
                    spacing: 40

                    Text {
                        text: "DISTANCE \n 225,000,000 KM"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }

                    Text {
                        text: "VELOCITY \n 12 KM/S"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }
                }

                Column {
                    spacing: 40

                    Text {
                        text: "ESTIMATED TIME OF ARRIVAL \n 217 days"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }

                    Text {
                        text: "TRAJECTORY \n Nominal"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }
                }
            }
        }
    }
}
