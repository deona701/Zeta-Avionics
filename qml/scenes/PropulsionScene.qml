import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick3D
import "../components/"
import Zeta_Avionics

Item {
    id: propulsionSceneRoot

    PropulsionSystem {
        id: propulsionSystem
    }

    Timer {
        interval: 16
        running: true
        repeat: true
        onTriggered: propulsionSystem.updateSimulation(0.016)
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
                spacing: 30

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

                    Button {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.topMargin: 10

                        contentItem: Text {
                            text: propulsionSystem.engineStatus ? "IGNITION OFF" : "START ENGINE"
                            color: Theme.light
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            color: Theme.dark
                            border.color: Theme.light
                        }

                        onClicked: {
                            propulsionSystem.engineStatus = !propulsionSystem.engineStatus
                        }
                    }
                }

                ColumnLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 8

                    Text {
                        text: "Throttle: " + propulsionSystem.throttle.toFixed(1) + "%"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                        Layout.alignment: Qt.AlignHCenter
                    }

                    Slider {
                        id: throttleSlider
                        Layout.preferredWidth: 300
                        from: 0
                        to: 100
                        value: propulsionSystem.throttle
                        onMoved: {
                            propulsionSystem.throttle = value
                        }

                        background: Rectangle {
                            x: throttleSlider.leftPadding
                            y: throttleSlider.topPadding + throttleSlider.availableHeight / 2 - height / 2
                            implicitWidth: 200
                            implicitHeight: 12
                            width: throttleSlider.availableWidth
                            height: implicitHeight
                            radius: Theme.cornerRadius
                            color: Theme.dark
                            border.color: Theme.light

                            Rectangle {
                                width: throttleSlider.visualPosition * parent.width
                                height: parent.height
                                color: Theme.light
                                radius: Theme.cornerRadius
                            }
                        }

                        handle: Rectangle {
                            x: throttleSlider.leftPadding + throttleSlider.visualPosition * (throttleSlider.availableWidth - width)
                            y: throttleSlider.topPadding + throttleSlider.availableHeight / 2 - height / 2
                            implicitWidth: 24
                            implicitHeight: 28
                            radius: Theme.cornerRadius
                            color: Theme.dark
                            border.color: Theme.light
                        }
                    }
                }

                GridLayout {
                    columns: 2
                    rowSpacing: 25
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