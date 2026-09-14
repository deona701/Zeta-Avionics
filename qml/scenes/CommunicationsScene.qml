import QtQuick
import QtQuick.Layouts

Item {
    id: communicationsSceneRoot
    anchors.fill: parent

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 25
        anchors.bottomMargin: 95
        anchors.leftMargin: 95
        anchors.rightMargin: 25
        spacing: 25

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 3
            color: "black"
            border.color: "white"
            radius: 10

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 15

                GridLayout {
                    columns: 3
                    rowSpacing: 30
                    columnSpacing: 40
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignCenter

                    ColumnLayout {
                        spacing: 4
                        Text { text: "ANTENNA TARGET LOCK"; color: Theme.secondaryText; font.pixelSize: 12 }
                        Text { text: "LOCKED"; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                    }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "SIGNAL STRENGTH"; color: Theme.secondaryText; font.pixelSize: 12 }
                        Text { text: "91% STRONG"; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                    }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "DOWNLINK STATUS"; color: Theme.secondaryText; font.pixelSize: 12 }
                        Text { text: "ACTIVE"; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                    }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "DATA TRANSFER RATE"; color: Theme.secondaryText; font.pixelSize: 12 }
                        Text { text: "--- kbps"; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                    }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "TELEMETRY PACKETS"; color: Theme.secondaryText; font.pixelSize: 12 }
                        Text { text: "-------"; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                    }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "ACTIVE LINK"; color: Theme.secondaryText; font.pixelSize: 12 }
                        Text { text: "EARTH RELAY"; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 0.5
            color: "black"
            border.color: "white"
            radius: 10

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 15

            Text {
                text: "SCROLLING TELEMETRY FEED"; color: Theme.primaryText; font.pixelSize: Theme.fontSmall; font.bold: true }
            }
        }
    }
}
