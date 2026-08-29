import QtQuick
import QtMultimedia
import "../components/"

Item {
    id: overviewSceneRoot
    anchors.fill: parent

    property bool sceneActive: false

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
        width: 80
        height: 450
        anchors.left: parent.left
        anchors.leftMargin: -100
        anchors.verticalCenter: parent.verticalCenter
        radius: 6
        opacity: 1

        Column {
            id: sidePanelColumn
            spacing: 10
            width: parent.width - 15
            anchors.centerIn: parent

            readonly property real buttonHeight: (sidePanel.height - 20 - (4 * 10)) / 5

            // Button
            Rectangle {
                width: parent.width
                height: sidePanelColumn.buttonHeight
                radius: 6
                color: "black"

                Text {
                    text: "Propulsion"
                    color: "white"
                    anchors.centerIn: parent
                    font.bold: true
                    font.pixelSize: 12
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("Propulsion clicked")
                }
            }

            // Button
            Rectangle {
                width: parent.width
                height: sidePanelColumn.buttonHeight
                radius: 6
                color: "black"

                Text {
                    text: "Power"
                    color: "white"
                    anchors.centerIn: parent
                    font.bold: true
                    font.pixelSize: 12
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("Power clicked")
                }
            }

            // Button
            Rectangle {
                width: parent.width
                height: sidePanelColumn.buttonHeight
                radius: 6
                color: "black"

                Text {
                    text: "    Life \nSupport"
                    color: "white"
                    anchors.centerIn: parent
                    font.bold: true
                    font.pixelSize: 12
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("Life Support clicked")
                }
            }

            // Button
            Rectangle {
                width: parent.width
                height: sidePanelColumn.buttonHeight
                radius: 6
                color: "black"

                Text {
                    text: "Navigation"
                    color: "white"
                    anchors.centerIn: parent
                    font.bold: true
                    font.pixelSize: 12
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("Nav clicked")
                }
            }

            // Button
            Rectangle {
                width: parent.width
                height: sidePanelColumn.buttonHeight
                radius: 6
                color: "black"

                Text {
                    text: "Comms"
                    color: "white"
                    anchors.centerIn: parent
                    font.bold: true
                    font.pixelSize: 12
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("Comms clicked")
                }
            }
        }

        states: [
            State {
                name: "VISIBLE"
                when: overviewSceneRoot.sceneActive
                PropertyChanges {
                    target: sidePanel
                    anchors.leftMargin: 8
                }
            }
        ]
        transitions: [
            Transition {
                from: "*"
                to: "VISIBLE"

                NumberAnimation {
                    target: sidePanel
                    property: "anchors.leftMargin"
                    duration: 600
                    easing.type: Easing.OutCubic
                }
            }
        ]
    }

    Rectangle {
        id: bottomPanel
        width: 800
        height: 80
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -100
        anchors.horizontalCenter: parent.horizontalCenter
        radius: 6
        opacity: 1
        color: "white"

        Row {
            id: bottomPanelRow
            anchors.fill: parent
            anchors.margins: 6
            spacing: 8

            readonly property real itemWidth: (width - (spacing * 4)) / 5

            // SPACECRAFT _ MAIN SCREEN
            Rectangle {
                width: bottomPanelRow.itemWidth
                height: parent.height
                color: "black"
                radius: 6

                Text {
                    text: "Spacecraft"
                    color: "white"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.centerIn: parent
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("Spacecraft clicked")
                }
            }

            // MISSIONS CENTER
            Rectangle {
                width: bottomPanelRow.itemWidth
                height: parent.height
                color: "black"
                radius: 6

                Text {
                    text: "Missions"
                    color: "white"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.centerIn: parent
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("Missions clicked")
                }
            }

            // EMPTY SPACE
            Rectangle {
                width: bottomPanelRow.itemWidth
                height: parent.height
                color: "black"
                radius: 2
            }

            // DIAGNOSTICS SYSTEM
            Rectangle {
                width: bottomPanelRow.itemWidth
                height: parent.height
                color: "black"
                radius: 6

                Text {
                    text: "Diagnostics"
                    color: "white"
                    font.pixelSize: 18
                    font.bold: true
                    anchors.centerIn: parent
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: console.log("Diagnostics clicked")
                }
            }

            // ZETA CONTROLS
            Rectangle {
                width: bottomPanelRow.itemWidth
                height: parent.height
                color: "black"
                radius: 6

                Text {
                    text: "Zeta"
                    color: "white"
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

        states: [
            State {
                name: "VISIBLE"
                when: overviewSceneRoot.sceneActive
                PropertyChanges {
                    target: bottomPanel
                    anchors.bottomMargin: 8
                }
            }
        ]
        transitions: [
            Transition {
                from: "*"
                to: "VISIBLE"

                NumberAnimation {
                    target: bottomPanel
                    property: "anchors.bottomMargin"
                    duration: 600
                    easing.type: Easing.OutCubic
                }
            }
        ]
    }
}