import QtQuick
import QtMultimedia
import "../components/"

Item {
    id: dashboardRoot
    anchors.fill: parent

    MediaPlayer {
        id: bgPlayer
        source: Qt.resolvedUrl("../../assets/background/Photoman61.mp4")
        loops: MediaPlayer.Infinite
        videoOutput: bgVideoOutput
        Component.onCompleted: bgPlayer.play()
    }

    VideoOutput {
        id: bgVideoOutput
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectCrop
    }

    SpacecraftView {
        anchors.fill: parent
    }

    Rectangle {
        id: sidePanel
        width: 69
        height: 500
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        radius: 15
        opacity: 0.7
    }

    Rectangle {
        width: 800
        height: 70
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        radius: 15
        opacity: 0.7

        Row {
            width: parent.width
            height: parent.height

            // SPACECRAFT _ MAIN SCREEN
            Rectangle {
                width: (parent.width - 4) / 5
                height: parent.height
                color: "transparent"

                Text {
                    text: "Spacecraft"
                    color: "black"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.centerIn: parent
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("Spacecraft clicked")
                }
            }
            // DIVIDER
            Rectangle {
                width: 2
                height: parent.height
                color: "black"
            }

            // MISSIONS CENTER
            Rectangle {
                width: (parent.width - 4) / 5
                height: parent.height
                color: "transparent"

                Text {
                    text: "Missions"
                    color: "black"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.centerIn: parent
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("Missions clicked")
                }
            }

            // DIVIDER
            Rectangle {
                width: 2
                height: parent.height
                color: "black"
            }

            // EMPTY SPACE
            Rectangle {
                width: (parent.width - 4) / 5
                height: parent.height
                color: "black"
            }

            // DIVIDER
            Rectangle {
                width: 2
                height: parent.height
                color: "black"
            }

            // DIAGNOSTICS SYSTEM
            Rectangle {
                width: (parent.width - 4) / 5
                height: parent.height
                color: "transparent"

                Text {
                    text: "Diagnostics"
                    color: "black"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.centerIn: parent
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("Diagnostics clicked")
                }
            }

            // DIVIDER
            Rectangle {
                width: 1.6
                height: parent.height
                color: "black"
            }

            // ZETA CONTROLS
            Rectangle {
                width: (parent.width - 4) / 5
                height: parent.height
                color: "transparent"

                Text {
                    text: "Zeta"
                    color: "black"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.centerIn: parent
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("Zeta clicked")
                }
            }
        }
    }
}