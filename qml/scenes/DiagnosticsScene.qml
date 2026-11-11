pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Zeta_Avionics

Item {
    id: diagnosticsSceneRoot

    required property SpacecraftSimulation sim
    readonly property DiagnosticsSystem diagnostics: sim.state.diagnostics

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
                spacing: 30

                ColumnLayout {
                    spacing: 4
                    Text { text: "SYSTEM HEALTH STATUS"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall }
                    Text { 
                        text: diagnosticsSceneRoot.diagnostics.systemHealthStatus 
                        color: diagnosticsSceneRoot.diagnostics.activeFaultCount === 0 ? "#33ff33" : "#ff3333"
                        font.bold: true; font.pixelSize: Theme.fontNormal 
                    }
                }

                Item { Layout.fillWidth: true }

                ColumnLayout {
                    spacing: 4
                    Text { text: "ACTIVE FAULTS"; color: Theme.secondaryText; font.pixelSize: Theme.fontSmall }
                    Text { text: diagnosticsSceneRoot.diagnostics.activeFaultCount; color: Theme.primaryText; font.bold: true; font.pixelSize: Theme.fontNormal }
                }

                Button {
                    text: "Run Scan"

                    background: Rectangle {
                        color: Theme.dark
                        border.color: Theme.light
                    }

                    onClicked: diagnosticsSceneRoot.diagnostics.runDiagnosticsScan()
                }

                Button {
                    text: "Clear Logs"

                    background: Rectangle {
                        color: Theme.dark
                        border.color: Theme.light
                    }

                    onClicked: diagnosticsSceneRoot.diagnostics.clearFaults()
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
                anchors.margins: 15
                spacing: 10

                Text {
                    text: "AVIONICS ERROR & EVENT LOG"
                    color: Theme.primaryText
                    font.bold: true
                    font.pixelSize: Theme.fontSmall
                }

                Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: Theme.grey }

                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: diagnosticsSceneRoot.diagnostics.errorLogs
                    spacing: 6
                    clip: true

                    delegate: Text {
                        required property string modelData
                        text: modelData
                        color: modelData.includes("ERR") || modelData.includes("CRIT") ? "#ff3333" : (modelData.includes("WARN") ? "#ffaa33" : Theme.primaryText)
                        font.family: "Monospace"
                        font.pixelSize: 11
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
    }
}