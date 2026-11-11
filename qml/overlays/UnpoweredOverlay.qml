import QtQuick
import "../"

Item {
    id: overlayRoot

    required property bool isPowered

    z: 100

    Rectangle {
        anchors.fill: parent
        color: "#000000"
        border.color: Theme.light
        border.width: 2
        opacity: overlayRoot.isPowered ? 0.0 : 1.0
        visible: opacity > 0.01

        MouseArea {
            anchors.fill: parent
            enabled: !overlayRoot.isPowered

            acceptedButtons: Qt.AllButtons
            onWheel: (wheel) => wheel.accepted = true
        }

        Column {
            anchors.centerIn: parent
            spacing: 6

            Text {
                text: "SYSTEM OFFLINE"
                color: Theme.primaryText
                font.pixelSize: Theme.fontLarge
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                text: "0 kW ALLOCATED"
                color: Theme.secondaryText
                font.pixelSize: Theme.fontSmall
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }
}