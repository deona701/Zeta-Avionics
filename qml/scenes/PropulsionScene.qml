import QtQuick
import QtQuick.Layouts
import QtQuick3D
import "../../models/EngineModel"

Item {
    id: propulsionSceneRoot
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        anchors.topMargin: 25
        anchors.bottomMargin: 95
        anchors.leftMargin: 95
        anchors.rightMargin: 25
        border.color: Theme.light
        radius: 10
        color: Theme.dark

        RowLayout {
            anchors.fill: parent
            anchors.margins: 30
            spacing: 40

            Item {
                Layout.preferredWidth: 500
                Layout.fillHeight: true

                View3D {
                    id: engineModel
                    anchors.fill: parent

                    environment: SceneEnvironment {
                        clearColor: "transparent"
                        backgroundMode: SceneEnvironment.Transparent
                    }

                    PerspectiveCamera {
                        id: engineCamera
                        position: Qt.vector3d(0, 0, 280)
                    }

                    DirectionalLight { eulerRotation: Qt.vector3d(-45, 45, 0); brightness: 1.2 }
                    DirectionalLight { eulerRotation: Qt.vector3d(30, -135, 0); brightness: 0.4 }
                    DirectionalLight { eulerRotation: Qt.vector3d(-10, 180, 0); brightness: 0.9 }
                    PointLight { position: Qt.vector3d(0, 100, 100); brightness: 0.2 }

                    Raptorengine {
                        id: engine
                        scale: Qt.vector3d(0.22, 0.22, 0.22)
                        position: Qt.vector3d(0, -50, 0)

                        NumberAnimation {
                            target: engine
                            property: "eulerRotation.y"
                            duration: 15000
                            from: 0
                            to: 360
                            loops: Animation.Infinite
                            running: true
                        }
                    }
                }
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
                        text: "Status: Standby"
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
                            text: "Thrust"
                            color: Theme.primaryText
                            font.bold: true
                            font.pixelSize: Theme.fontLarge
                            Layout.alignment: Qt.AlignHCenter
                        }
                        Text {
                            text: "0%"
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
                            text: "100%"
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
                            text: "...."
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
                            text: "Chamber Pressure"
                            color: Theme.primaryText
                            font.bold: true
                            font.pixelSize: Theme.fontLarge
                            Layout.alignment: Qt.AlignHCenter
                        }
                        Text {
                            text: "...."
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