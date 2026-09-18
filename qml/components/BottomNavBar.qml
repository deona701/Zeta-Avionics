import QtQuick

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

        // SPACECRAFT - MAIN SCREEN
        Rectangle {
            width: bottomPanelRow.itemWidth
            height: parent.height
            color: "black"
            radius: 6

            Text {
                text: "Spacecraft"
                color: "white"
                font.pixelSize: Theme.fontNormal
                font.bold: true
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: window.currentScene = "MAIN"
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
                font.pixelSize: Theme.fontNormal
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
                font.pixelSize: Theme.fontNormal
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
                font.pixelSize: Theme.fontNormal
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
            when: uiActive
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
            duration: 1000
            easing.type: Easing.OutCubic
        }
    }
]
}