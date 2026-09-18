import QtQuick

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
                font.pixelSize: Theme.fontSmall
            }

            MouseArea {
                anchors.fill: parent
                onClicked: window.currentScene = "PROP"
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
                font.pixelSize: Theme.fontSmall
            }

            MouseArea {
                anchors.fill: parent
                onClicked: window.currentScene = "POWER"
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
                font.pixelSize: Theme.fontSmall
            }

            MouseArea {
                anchors.fill: parent
                onClicked: window.currentScene = "LIFESUPPORT"
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
                font.pixelSize: Theme.fontSmall
            }

            MouseArea {
                anchors.fill: parent
                onClicked: window.currentScene = "NAV"
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
                font.pixelSize: Theme.fontSmall
            }

            MouseArea {
                anchors.fill: parent
                onClicked: window.currentScene = "COMMS"
            }
        }
    }

    states: [
        State {
            name: "VISIBLE"
            when: uiActive
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
                duration: 1000
                easing.type: Easing.OutCubic
            }
        }
    ]
}