pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Zeta_Avionics

Item {
    id: missionsSceneRoot

    required property SpacecraftSimulation sim
    readonly property MissionsSystem missions: sim.state.missions

    anchors.topMargin: Theme.sceneMarginTop
    anchors.bottomMargin: Theme.sceneMarginBottom
    anchors.leftMargin: Theme.sceneMarginLeft
    anchors.rightMargin: Theme.sceneMarginRight

    RowLayout {
        anchors.fill: parent
        spacing: 15

        Rectangle {
            Layout.preferredWidth: 320
            Layout.fillHeight: true
            color: Theme.dark
            border.color: Theme.light
            border.width: 1
            radius: Theme.cornerRadius

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 20
                spacing: 15

                Text {
                    text: "FLIGHT PROFILE SELECTOR"
                    color: Theme.primaryText
                    font.bold: true
                    font.pixelSize: Theme.fontSmall
                }

                Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: Theme.grey }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: missionsSceneRoot.missions.availableMissions
                    spacing: 8
                    clip: true

                    delegate: Button {
                        required property string modelData
                        width: parent.width
                        Layout.preferredHeight: 45

                        background: Rectangle {
                            color: missionsSceneRoot.missions.currentMission === modelData ? Theme.light : Theme.dark
                            border.color: Theme.light
                            radius: 4
                        }

                        contentItem: Text {
                            text: modelData
                            color: missionsSceneRoot.missions.currentMission === modelData ? Theme.dark : Theme.primaryText
                            font.bold: true
                            font.pixelSize: 12
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        onClicked: missionsSceneRoot.missions.selectMission(modelData)
                    }
                }
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 15

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 110
                color: Theme.dark
                border.color: Theme.light
                border.width: 1
                radius: Theme.cornerRadius

                RowLayout {
                    anchors.fill: parent
                    anchors.margins: 20
                    spacing: 30

                    ColumnLayout {
                        spacing: 4
                        Text { text: "ACTIVE FLIGHT PHASE"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall }
                        Text { text: missionsSceneRoot.missions.currentMission; color: Theme.primaryText; font.bold: true; font.pixelSize: Theme.fontNormal }
                    }

                    Item { Layout.fillWidth: true }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "STATUS"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall }
                        Text { 
                            text: missionsSceneRoot.missions.missionStatus
                            color: missionsSceneRoot.missions.missionStatus === "COMPLETED" ? "#33ff33" : "#33ccff"
                            font.bold: true; font.pixelSize: Theme.fontNormal 
                        }
                    }

                    Button {
                        text: "Restart Phase"
                        background: Rectangle { color: Theme.dark; border.color: Theme.light }
                        onClicked: missionsSceneRoot.missions.resetMission()
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
                    anchors.margins: 25
                    spacing: 20

                    Text {
                        text: "CURRENT OBJECTIVE DIRECTIVE"
                        color: Theme.secondaryText
                        font.pixelSize: Theme.fontSmall
                    }

                    Text {
                        text: missionsSceneRoot.missions.currentObjective
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: 16
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                    }

                    Item { Layout.preferredHeight: 10 }

                    RowLayout {
                        Layout.fillWidth: true
                        Text { text: "Phase Completion Progress:"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall }
                        Item { Layout.fillWidth: true }
                        Text { text: missionsSceneRoot.missions.objectiveProgress.toFixed(0) + " %"; color: Theme.primaryText; font.bold: true; font.pixelSize: Theme.fontSmall }
                    }

                    ProgressBar {
                        Layout.fillWidth: true
                        value: missionsSceneRoot.missions.objectiveProgress / 100.0
                        from: 0.0
                        to: 1.0

                        background: Rectangle {
                            implicitHeight: 8
                            color: Theme.grey
                            border.color: Theme.grey
                            radius: 4
                        }
                        contentItem: Item {
                            implicitHeight: 8
                            Rectangle {
                                width: parent.width * parent.visualPosition
                                height: parent.height
                                color: Theme.light
                                radius: 4
                            }
                        }
                    }

                    Item { Layout.fillHeight: true }
                }
            }
        }
    }
}