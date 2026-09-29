import QtQuick
import QtQuick.Layouts
import QtQuick3D
import "../components/"
import Zeta_Avionics

Item {
    id: propulsionSceneRoot

    PropulsionSystem {
        id: propulsionSystem
    }

    Rectangle {
        anchors.fill: parent
        anchors.topMargin: Theme.sceneMarginTop
        anchors.bottomMargin: Theme.sceneMarginBottom
        anchors.leftMargin: Theme.sceneMarginLeft
        anchors.rightMargin: Theme.sceneMarginRight
        border.color: Theme.light
        radius: Theme.cornerRadius
        color: Theme.dark

        RowLayout {
            anchors.fill: parent
            anchors.margins: 30
            spacing: 40

            Item {
                Layout.preferredWidth: 500
                Layout.fillHeight: true

                EngineView {}
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter
                spacing: 40

                ColumnLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 8

                    Text {
                        text: "Main Engine"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: 48
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Text {
                        text: "Status: " + (propulsionSystem && propulsionSystem.engineStatus ? "ACTIVE" : "STANDBY")
                        color: Theme.secondaryText
                        font.bold: true
                        font.pixelSize: Theme.fontTitle
                        Layout.alignment: Qt.AlignHCenter
                    }
                }

                GridLayout {
                    columns: 2
                    rowSpacing: 30
                    columnSpacing: 60
                    Layout.alignment: Qt.AlignHCenter

                    ColumnLayout {
                        spacing: 4
                        Layout.alignment: Qt.AlignHCenter

                        Text {
                            text: "Thrust Output"
                            color: Theme.primaryText
                            font.bold: true
                            font.pixelSize: Theme.fontLarge
                            Layout.alignment: Qt.AlignHCenter
                        }
                        Text {
                            text: propulsionSystem.thrustOutput.toFixed(1) + " kN"
                            color: Theme.primaryText
                            font.bold: true
                            font.pixelSize: Theme.fontLarge
                            Layout.alignment: Qt.AlignHCenter
                        }
                    }

                    ColumnLayout {
                        spacing: 4
                        Layout.alignment: Qt.AlignHCenter

                        Text {
                            text: "Fuel"
                            color: Theme.primaryText
                            font.bold: true
                            font.pixelSize: Theme.fontLarge
                            Layout.alignment: Qt.AlignHCenter
                        }
                        Text {
                            text: propulsionSystem.propellantPercentage.toFixed(1) + " %"
                            color: Theme.primaryText
                            font.bold: true
                            font.pixelSize: Theme.fontLarge
                            Layout.alignment: Qt.AlignHCenter
                        }
                    }

                    ColumnLayout {
                        spacing: 4
                        Layout.alignment: Qt.AlignHCenter

                        Text {
                            text: "Engine Temp"
                            color: Theme.primaryText
                            font.bold: true
                            font.pixelSize: Theme.fontLarge
                            Layout.alignment: Qt.AlignHCenter
                        }
                        Text {
                            text: propulsionSystem.engineTemp.toFixed(1) + " °C"
                            color: Theme.primaryText
                            font.bold: true
                            font.pixelSize: Theme.fontLarge
                            Layout.alignment: Qt.AlignHCenter
                        }
                    }

                    ColumnLayout {
                        spacing: 4
                        Layout.alignment: Qt.AlignHCenter

                        Text {
                            text: "Delta-V"
                            color: Theme.primaryText
                            font.bold: true
                            font.pixelSize: Theme.fontLarge
                            Layout.alignment: Qt.AlignHCenter
                        }
                        Text {
                            text: propulsionSystem.deltaV.toFixed(1) + " m/s"
                            color: Theme.primaryText
                            font.bold: true
                            font.pixelSize: Theme.fontLarge
                            Layout.alignment: Qt.AlignHCenter
                        }
                    }
                }
            }

            Item { Layout.fillWidth: true }
        }
    }
}