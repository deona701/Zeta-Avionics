import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Zeta_Avionics

Item {
    id: communicationsSceneRoot

    required property SpacecraftSimulation sim
    readonly property CommunicationsSystem communicationsSystem: sim.state.communications

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: Theme.sceneMarginTop
        anchors.bottomMargin: Theme.sceneMarginBottom
        anchors.leftMargin: Theme.sceneMarginLeft
        anchors.rightMargin: Theme.sceneMarginRight
        spacing: 25

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 350
            color: Theme.dark
            border.color: Theme.light
            radius: Theme.cornerRadius

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
            Layout.preferredHeight: 100
            color: Theme.dark
            border.color: Theme.light
            radius: Theme.cornerRadius

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 15

                Rectangle {
                    id: transmitBar
                    Layout.fillWidth: true
                    height: 10
                    color: Theme.dark
                    border.color: Theme.light

                    Rectangle {
                        id: progressFill
                        width: 0
                        height: parent.height
                        color: Theme.light
                    }
                }

                NumberAnimation {
                    id: sendAnimation
                    target: progressFill
                    property: "width"
                    from: 0
                    to: transmitBar.width
                    duration: 2000

                    property string targetAction: "telemetry"

                    onFinished: {
                        if (targetAction === "telemetry") {
                            communicationsSceneRoot.communicationsSystem.sendTelemetryBurst();
                        }
                        else {
                            communicationsSceneRoot.communicationsSystem.triggerDistressBeacon();
                        }
                        progressFill.width = 0;
                    }
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 15

                    Button {
                        contentItem: Text {
                            text: parent.text
                            color: Theme.light
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            color: Theme.dark
                            border.color: Theme.light
                        }

                        text: "SEND TELEMETRY BURST"
                        onClicked: {
                            sendAnimation.targetAction = "telemetry"
                            sendAnimation.duration = 2000;
                            sendAnimation.start();
                        }
                    }

                    Button {
                        contentItem: Text {
                            text: parent.text
                            color: Theme.light
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            color: Theme.dark
                            border.color: Theme.light
                        }

                        text: "DISTRESS BEACON"
                        onClicked: {
                            sendAnimation.targetAction = "distress"
                            sendAnimation.duration = 500;
                            sendAnimation.start();
                        }
                    }
                }
            }
        }
    }
}