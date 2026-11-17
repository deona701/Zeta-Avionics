pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Zeta_Avionics

Item {
    id: zetaSceneRoot

    required property SpacecraftSimulation sim
    readonly property ZetaAssistantSystem zeta: sim.state.zeta

    anchors.topMargin: Theme.sceneMarginTop
    anchors.bottomMargin: Theme.sceneMarginBottom
    anchors.leftMargin: Theme.sceneMarginLeft
    anchors.rightMargin: Theme.sceneMarginRight

    ColumnLayout {
        anchors.fill: parent
        spacing: 15

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 90
            color: Theme.dark
            border.color: Theme.light
            border.width: 1
            radius: Theme.cornerRadius

            RowLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 20

                ColumnLayout {
                    spacing: 4
                    Text { text: "AVIONICS AI SUBSYSTEM"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall }
                    Text { text: "ZETA FLIGHT ASSISTANT // NEURAL VOICE CORE"; color: Theme.primaryText; font.bold: true; font.pixelSize: Theme.fontNormal }
                }

                Item { Layout.fillWidth: true }

                ColumnLayout {
                    spacing: 4
                    Text { text: "AUDIO STATUS"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall; Layout.alignment: Qt.AlignRight }
                    Text { 
                        text: zeta.audioStatus
                        color: zeta.isListening ? "#ff3333" : (zeta.isSpeaking ? "#00e5ff" : "#33ff33")
                        font.bold: true
                        font.pixelSize: 12
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: Theme.dark
            border.color: Theme.light
            border.width: 1
            radius: Theme.cornerRadius

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 30
                spacing: 20

                ColumnLayout {
                    spacing: 6
                    visible: zeta.lastHeardText.length > 0
                    Layout.fillWidth: true

                    Text {
                        text: "CAPTAIN VOICE TRANSCRIPT"
                        color: Theme.secondaryText
                        font.pixelSize: Theme.fontSmall
                    }

                    Text {
                        text: "\"" + zeta.lastHeardText + "\""
                        color: "#00e5ff"
                        font.italic: true
                        font.pixelSize: 16
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }
                }

                ColumnLayout {
                    spacing: 6
                    Layout.fillWidth: true

                    Text {
                        text: "ZETA RESPONSE FEED"
                        color: Theme.secondaryText
                        font.pixelSize: Theme.fontSmall
                    }

                    Text {
                        text: zeta.lastResponse
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: 18
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }
                }

                Item { Layout.fillHeight: true }

                Row {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 6
                    visible: zeta.isListening || zeta.isSpeaking

                    Repeater {
                        model: 12
                        Rectangle {
                            id: waveBar
                            width: 6
                            height: 12
                            color: zeta.isListening ? "#ff3333" : "#00e5ff"
                            radius: 3

                            Timer {
                                interval: 110
                                running: zeta.isListening || zeta.isSpeaking
                                repeat: true
                                onTriggered: waveBar.height = Math.random() * 48 + 10
                            }

                            Behavior on height {
                                NumberAnimation { duration: 100 }
                            }
                        }
                    }
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 15

            Button {
                Layout.preferredWidth: 240
                Layout.preferredHeight: 50

                background: Rectangle {
                    color: zeta.isListening ? "#551111" : Theme.dark
                    border.color: zeta.isListening ? "#ff3333" : Theme.light
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: zeta.isListening ? "RELEASE TO TRANSMIT" : "HOLD TO SPEAK (MIC)"
                    color: zeta.isListening ? "#ff6666" : Theme.primaryText
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onPressed: zeta.startListening()
                onReleased: zeta.stopListening()
            }

            TextField {
                id: fallbackInput
                Layout.fillWidth: true
                Layout.preferredHeight: 50
                placeholderText: "Or type command (e.g. 'status report', 'mars distance', 'fuel', 'cabin vitals')..."
                color: Theme.primaryText
                font.pixelSize: 14
                background: Rectangle {
                    color: Theme.dark
                    border.color: Theme.light
                    border.width: 1
                    radius: 4
                }
                onAccepted: {
                    zeta.processCommand(text)
                    text = ""
                }
            }

            Button {
                text: "Send"
                Layout.preferredWidth: 100
                Layout.preferredHeight: 50

                background: Rectangle {
                    color: Theme.dark
                    border.color: Theme.light
                    border.width: 1
                    radius: 4
                }

                contentItem: Text {
                    text: parent.text
                    color: Theme.primaryText
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    zeta.processCommand(fallbackInput.text)
                    fallbackInput.text = ""
                }
            }
        }
    }
}