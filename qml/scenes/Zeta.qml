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

    RowLayout {
        anchors.fill: parent
        spacing: 15

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
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

                    Button {
                        Layout.preferredWidth: 150
                        Layout.preferredHeight: 40
                        background: Rectangle {
                            color: commandDrawer.visible ? "#00e5ff" : Theme.dark
                            border.color: Theme.light
                            border.width: 1
                            radius: 4
                        }
                        contentItem: Text {
                            text: commandDrawer.visible ? "HIDE COMMANDS" : "COMMANDS"
                            color: commandDrawer.visible ? Theme.dark : Theme.primaryText
                            font.bold: true
                            font.pixelSize: 11
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }
                        onClicked: commandDrawer.visible = !commandDrawer.visible
                    }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "AUDIO STATUS"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall; Layout.alignment: Qt.AlignRight }
                        Text { 
                            text: zetaSceneRoot.zeta.audioStatus
                            color: zetaSceneRoot.zeta.isListening ? "#ff3333" : (zetaSceneRoot.zeta.isSpeaking ? "#00e5ff" : "#33ff33")
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
                        visible: zetaSceneRoot.zeta.lastHeardText.length > 0
                        Layout.fillWidth: true

                        Text {
                            text: "CAPTAIN VOICE TRANSCRIPT"
                            color: Theme.secondaryText
                            font.pixelSize: Theme.fontSmall
                        }

                        Text {
                            text: "\"" + zetaSceneRoot.zeta.lastHeardText + "\""
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
                            text: zetaSceneRoot.zeta.lastResponse
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
                        visible: zetaSceneRoot.zeta.isListening || zetaSceneRoot.zeta.isSpeaking

                        Repeater {
                            model: 12
                            Rectangle {
                                id: waveBar
                                width: 6
                                height: 12
                                color: zetaSceneRoot.zeta.isListening ? "#ff3333" : "#00e5ff"
                                radius: 3

                                Timer {
                                    interval: 110
                                    running: zetaSceneRoot.zeta.isListening || zetaSceneRoot.zeta.isSpeaking
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
                    Layout.preferredWidth: 220
                    Layout.preferredHeight: 50

                    background: Rectangle {
                        color: zetaSceneRoot.zeta.isListening ? "#551111" : Theme.dark
                        border.color: zetaSceneRoot.zeta.isListening ? "#ff3333" : Theme.light
                        border.width: 1
                        radius: 4
                    }

                    contentItem: Text {
                        text: zetaSceneRoot.zeta.isListening ? "RELEASE TO TRANSMIT" : "HOLD TO SPEAK (MIC)"
                        color: zetaSceneRoot.zeta.isListening ? "#ff6666" : Theme.primaryText
                        font.bold: true
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onPressed: zetaSceneRoot.zeta.startListening()
                    onReleased: zetaSceneRoot.zeta.stopListening()
                }

                TextField {
                    id: fallbackInput
                    Layout.fillWidth: true
                    Layout.preferredHeight: 50
                    placeholderText: "Type command (e.g. 'status', 'mars distance', 'set throttle 75', 'scrubbers on')..."
                    color: Theme.primaryText
                    font.pixelSize: 14
                    background: Rectangle {
                        color: Theme.dark
                        border.color: Theme.light
                        border.width: 1
                        radius: 4
                    }
                    onAccepted: {
                        zetaSceneRoot.zeta.processCommand(text)
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
                        zetaSceneRoot.zeta.processCommand(fallbackInput.text)
                        fallbackInput.text = ""
                    }
                }
            }
        }

        Rectangle {
            id: commandDrawer
            visible: false
            Layout.preferredWidth: 320
            Layout.fillHeight: true
            color: Theme.dark
            border.color: Theme.light
            border.width: 1
            radius: Theme.cornerRadius

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 10

                Text {
                    text: "QUICK COMMAND CHEAT SHEET"
                    color: "#00e5ff"
                    font.bold: true
                    font.pixelSize: 13
                }

                ScrollView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true

                    ColumnLayout {
                        width: commandDrawer.width - 30
                        spacing: 8

                        Text { text: "Propulsion & Engines"; color: Theme.secondaryText; font.bold: true; font.pixelSize: 11; topPadding: 5 }
                        Button {
                            Layout.fillWidth: true; Layout.preferredHeight: 36
                            background: Rectangle { color: Theme.dark; border.color: Theme.light; radius: 3 }
                            contentItem: Text { text: "Status"; color: Theme.primaryText; font.pixelSize: 12; leftPadding: 10; verticalAlignment: Text.AlignVCenter }
                            onClicked: zetaSceneRoot.zeta.processCommand("status")
                        }
                        Button {
                            Layout.fillWidth: true; Layout.preferredHeight: 36
                            background: Rectangle { color: Theme.dark; border.color: Theme.light; radius: 3 }
                            contentItem: Text { text: "Set Throttle to 75%"; color: Theme.primaryText; font.pixelSize: 12; leftPadding: 10; verticalAlignment: Text.AlignVCenter }
                            onClicked: zetaSceneRoot.zeta.processCommand("set throttle 75")
                        }
                        Button {
                            Layout.fillWidth: true; Layout.preferredHeight: 36
                            background: Rectangle { color: Theme.dark; border.color: Theme.light; radius: 3 }
                            contentItem: Text { text: "Ignite Main Engines"; color: Theme.primaryText; font.pixelSize: 12; leftPadding: 10; verticalAlignment: Text.AlignVCenter }
                            onClicked: zetaSceneRoot.zeta.processCommand("ignite engines")
                        }
                        Button {
                            Layout.fillWidth: true; Layout.preferredHeight: 36
                            background: Rectangle { color: Theme.dark; border.color: Theme.light; radius: 3 }
                            contentItem: Text { text: "Cut Engines / Shutdown"; color: Theme.primaryText; font.pixelSize: 12; leftPadding: 10; verticalAlignment: Text.AlignVCenter }
                            onClicked: zetaSceneRoot.zeta.processCommand("shutdown engines")
                        }

                        Text { text: "Navigation"; color: Theme.secondaryText; font.bold: true; font.pixelSize: 11; topPadding: 5 }
                        Button {
                            Layout.fillWidth: true; Layout.preferredHeight: 36
                            background: Rectangle { color: Theme.dark; border.color: Theme.light; radius: 3 }
                            contentItem: Text { text: "Distance to Mars & ETA"; color: Theme.primaryText; font.pixelSize: 12; leftPadding: 10; verticalAlignment: Text.AlignVCenter }
                            onClicked: zetaSceneRoot.zeta.processCommand("mars distance")
                        }
                        Button {
                            Layout.fillWidth: true; Layout.preferredHeight: 36
                            background: Rectangle { color: Theme.dark; border.color: Theme.light; radius: 3 }
                            contentItem: Text { text: "Velocity & Trajectory"; color: Theme.primaryText; font.pixelSize: 12; leftPadding: 10; verticalAlignment: Text.AlignVCenter }
                            onClicked: zetaSceneRoot.zeta.processCommand("velocity vector")
                        }

                        Text { text: "Power & Life Support"; color: Theme.secondaryText; font.bold: true; font.pixelSize: 11; topPadding: 5 }
                        Button {
                            Layout.fillWidth: true; Layout.preferredHeight: 36
                            background: Rectangle { color: Theme.dark; border.color: Theme.light; radius: 3 }
                            contentItem: Text { text: "Power Grid Status"; color: Theme.primaryText; font.pixelSize: 12; leftPadding: 10; verticalAlignment: Text.AlignVCenter }
                            onClicked: zetaSceneRoot.zeta.processCommand("power grid status")
                        }
                        Button {
                            Layout.fillWidth: true; Layout.preferredHeight: 36
                            background: Rectangle { color: Theme.dark; border.color: Theme.light; radius: 3 }
                            contentItem: Text { text: "Toggle CO2 Scrubbers On"; color: Theme.primaryText; font.pixelSize: 12; leftPadding: 10; verticalAlignment: Text.AlignVCenter }
                            onClicked: zetaSceneRoot.zeta.processCommand("scrubbers on")
                        }
                        Button {
                            Layout.fillWidth: true; Layout.preferredHeight: 36
                            background: Rectangle { color: Theme.dark; border.color: Theme.light; radius: 3 }
                            contentItem: Text { text: "Scan Crew Vitals"; color: Theme.primaryText; font.pixelSize: 12; leftPadding: 10; verticalAlignment: Text.AlignVCenter }
                            onClicked: zetaSceneRoot.zeta.processCommand("astronaut vitals")
                        }

                        Text { text: "Communications & Comms"; color: Theme.secondaryText; font.bold: true; font.pixelSize: 11; topPadding: 5 }
                        Button {
                            Layout.fillWidth: true; Layout.preferredHeight: 36
                            background: Rectangle { color: Theme.dark; border.color: Theme.light; radius: 3 }
                            contentItem: Text { text: "Switch to Mars Orbiter Relay"; color: Theme.primaryText; font.pixelSize: 12; leftPadding: 10; verticalAlignment: Text.AlignVCenter }
                            onClicked: zetaSceneRoot.zeta.processCommand("mars orbiter relay")
                        }
                        Button {
                            Layout.fillWidth: true; Layout.preferredHeight: 36
                            background: Rectangle { color: Theme.dark; border.color: Theme.light; radius: 3 }
                            contentItem: Text { text: "Send Telemetry Burst (ntfy)"; color: Theme.primaryText; font.pixelSize: 12; leftPadding: 10; verticalAlignment: Text.AlignVCenter }
                            onClicked: zetaSceneRoot.zeta.processCommand("telemetry burst")
                        }

                        Text { text: "Easter Eggs"; color: Theme.secondaryText; font.bold: true; font.pixelSize: 11; topPadding: 5 }
                        Button {
                            Layout.fillWidth: true; Layout.preferredHeight: 36
                            background: Rectangle { color: "#221111"; border.color: "#ff3333"; radius: 3 }
                            contentItem: Text { text: "Open Pod Bay Doors"; color: "#ff6666"; font.pixelSize: 12; leftPadding: 10; verticalAlignment: Text.AlignVCenter }
                            onClicked: zetaSceneRoot.zeta.processCommand("open the pod bay doors")
                        }
                        Button {
                            Layout.fillWidth: true; Layout.preferredHeight: 36
                            background: Rectangle { color: Theme.dark; border.color: Theme.light; radius: 3 }
                            contentItem: Text { text: "Punch It / Engage"; color: Theme.primaryText; font.pixelSize: 12; leftPadding: 10; verticalAlignment: Text.AlignVCenter }
                            onClicked: zetaSceneRoot.zeta.processCommand("punch it")
                        }
                        Button {
                            Layout.fillWidth: true; Layout.preferredHeight: 36
                            background: Rectangle { color: Theme.dark; border.color: Theme.light; radius: 3 }
                            contentItem: Text { text: "Tell Me a Joke"; color: Theme.primaryText; font.pixelSize: 12; leftPadding: 10; verticalAlignment: Text.AlignVCenter }
                            onClicked: zetaSceneRoot.zeta.processCommand("tell me a joke")
                        }
                    }
                }
            }
        }
    }
}