import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: powerSceneRoot
    anchors.fill: parent

    signal requestPowerReroute(string systemName, real deltaKw)

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 25
        anchors.bottomMargin: 95
        anchors.leftMargin: 95
        anchors.rightMargin: 25
        spacing: 15

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 160
            color: Theme.dark
            border.color: Theme.light
            border.width: 1
            radius: 10

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 8

                Text {
                    text: "MAIN POWER GENERATION OVERVIEW"
                    color: Theme.primaryText
                    font.bold: true
                    font.pixelSize: Theme.fontSmall
                    Layout.alignment: Qt.AlignHCenter
                }

                Rectangle { Layout.fillWidth: true; height: 1; color: "#222222" }

                GridLayout {
                    columns: 3
                    rowSpacing: 20
                    columnSpacing: 20
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignCenter

                    ColumnLayout {
                        spacing: 4
                        Text { text: "SOLAR ARRAY OUTPUT"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall }
                        Text { text: "----"; color: Theme.primaryText; font.pixelSize: Theme.fontNormal; font.bold: true }
                    }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "REACTOR CORE STATUS"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall }
                        Text { text: "----"; color: Theme.primaryText; font.pixelSize: Theme.fontNormal; font.bold: true }
                    }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "MAIN BATTERY BUS"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall }
                        Text { text: "----"; color: Theme.primaryText; font.pixelSize: Theme.fontNormal; font.bold: true }
                    }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "TOTAL GENERATION"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall }
                        Text { text: "----"; color: Theme.primaryText; font.pixelSize: Theme.fontNormal; font.bold: true }
                    }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "TOTAL CONSUMPTION"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall }
                        Text { text: "----"; color: Theme.primaryText; font.pixelSize: Theme.fontNormal; font.bold: true }
                    }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "NET BUS BALANCE"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall }
                        Text { text: "----"; color: Theme.primaryText; font.pixelSize: Theme.fontNormal; font.bold: true }
                    }
                }
            }
        }

        GridLayout {
            columns: 2
            rowSpacing: 15
            columnSpacing: 15
            Layout.fillWidth: true
            Layout.fillHeight: true

            component PowerSubsystemCard: Rectangle {
                property string systemId: ""
                property string systemName: "SUBSYSTEM"
                property string allocatedPower: "----"
                property string loadPercent: "----"
                property string statusText: "----"
                property real fillRatio: 0.0

                Layout.fillWidth: true
                Layout.fillHeight: true
                color: Theme.dark
                border.color: Theme.light
                border.width: 1
                radius: 10

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 18
                    spacing: 10

                    Text {
                        text: systemName
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontNormal
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Rectangle { Layout.fillWidth: true; height: 1; color: "#222222" }

                    RowLayout {
                        Layout.fillWidth: true
                        Text { text: "Current Draw:"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall }
                        Item { Layout.fillWidth: true }
                        Text { text: allocatedPower; color: Theme.primaryText; font.bold: true; font.pixelSize: Theme.fontSmall }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Text { text: "Bus Load:"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall }
                        Item { Layout.fillWidth: true }
                        Text { text: loadPercent; color: Theme.primaryText; font.bold: true; font.pixelSize: Theme.fontSmall }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Text { text: "Status:"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall }
                        Item { Layout.fillWidth: true }
                        Text { text: statusText; color: Theme.primaryText; font.bold: true; font.pixelSize: Theme.fontSmall }
                    }

                    ProgressBar {
                        Layout.fillWidth: true
                        value: fillRatio
                        from: 0.0
                        to: 1.0

                        background: Rectangle {
                            implicitHeight: 6
                            color: "#111111"
                            border.color: "#333333"
                        }
                        contentItem: Item {
                            implicitHeight: 6
                            Rectangle {
                                width: parent.width * parent.visualPosition
                                height: parent.height
                                color: Theme.light
                            }
                        }
                    }

                    Item { Layout.fillHeight: true }

                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        spacing: 15

                        Button {
                            implicitWidth: 80
                            implicitHeight: 32
                            text: "- 1.0 kW"
                            onClicked: requestPowerReroute(systemId, -1.0)

                            contentItem: Text {
                                text: parent.text
                                color: Theme.primaryText
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            background: Rectangle {
                                color: Theme.dark
                                border.color: Theme.light
                            }
                        }

                        Button {
                            implicitWidth: 80
                            implicitHeight: 32
                            text: "+ 1.0 kW"
                            onClicked: requestPowerReroute(systemId, 1.0)

                            contentItem: Text {
                                text: parent.text
                                color: Theme.primaryText
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            background: Rectangle {
                                color: Theme.dark
                                border.color: Theme.light
                            }
                        }
                    }
                }
            }

            PowerSubsystemCard {
                systemId: "propulsion"
                systemName: "PROPULSION POWER"
            }

            PowerSubsystemCard {
                systemId: "life_support"
                systemName: "LIFE SUPPORT POWER"
            }

            PowerSubsystemCard {
                systemId: "navigation"
                systemName: "NAVIGATION POWER"
            }

            PowerSubsystemCard {
                systemId: "comms"
                systemName: "COMMUNICATIONS POWER"
            }
        }
    }
}