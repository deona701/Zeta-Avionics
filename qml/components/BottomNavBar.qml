import QtQuick

Rectangle {
    id: bottomPanel
    width: 800
    height: 80
    anchors.bottom: parent.bottom
    anchors.bottomMargin: -100
    anchors.horizontalCenter: parent.horizontalCenter
    radius: Theme.cornerRadius
    opacity: 1
    color: Theme.light

    readonly property var navItems: [
        { label: "Spacecraft", scene: "MAIN" },
        { label: "Missions", scene: "MISSIONS" },
        { isSpacer: true },
        { label: "Diagnostics", scene: "DIAG" },
        { label: "Zeta", scene: "ZETA" }
    ]

    Row {
        id: bottomPanelRow
        anchors.fill: parent
        anchors.margins: 6
        spacing: 8

        readonly property real itemWidth: (width - ((bottomPanel.navItems.length - 1) * spacing)) / bottomPanel.navItems.length

        Repeater {
            model: bottomPanel.navItems

            Rectangle {
                required property var modelData

                width: bottomPanelRow.itemWidth
                height: parent.height
                color: Theme.dark
                radius: modelData.isSpacer ? 2 : 6

                Text {
                    text: modelData.label || ""
                    color: Theme.primaryText
                    font.pixelSize: Theme.fontNormal
                    font.bold: true
                    anchors.centerIn: parent
                    visible: !modelData.isSpacer
                }

                MouseArea {
                    anchors.fill: parent
                    enabled: !modelData.isSpacer
                    onClicked: {
                        if (modelData.scene) {
                            window.currentScene = modelData.scene
                        }
                    }
                }
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
