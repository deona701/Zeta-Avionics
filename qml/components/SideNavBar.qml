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

        readonly property var navItems: [
            { label: "Propulsion", scene: "PROP" },
            { label: "Power", scene: "POWER" },
            { label: "Life\nSupport", scene: "LIFESUPPORT" },
            { label: "Navigation", scene: "NAV" },
            { label: "Comms", scene: "COMMS" }
        ]

        readonly property real buttonHeight: (sidePanel.height - 20 - (4 * 10)) / 5

        Repeater {
            model: sidePanelColumn.navItems

            Rectangle {
                required property var modelData

                width: parent.width
                height: sidePanelColumn.buttonHeight
                radius: 6
                color: Theme.dark

                Text {
                    text: parent.modelData.label
                    color: Theme.primaryText
                    anchors.centerIn: parent
                    font.bold: true
                    font.pixelSize: Theme.fontSmall
                    horizontalAlignment: Text.AlignHCenter
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: window.currentScene = parent.modelData.scene
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
}
