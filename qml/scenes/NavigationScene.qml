import QtQuick
import QtQuick3D
import "../../models/EarthModel"
import "../../models/MarsModel"

Item {
    id: navigationSceneRoot
    anchors.fill: parent

    Column {
        id: navMainColumn
        anchors.fill: parent
        anchors.topMargin: 25
        anchors.bottomMargin: 95
        anchors.leftMargin: 95
        anchors.rightMargin: 25
        spacing: 25

        Rectangle {
            width: parent.width
            height: (parent.height - parent.spacing) * 0.65
            radius: 10
            color: "transparent"
            border.color: "white"
            border.width: 2

            Row {
                anchors.centerIn: parent
                anchors.margins: 15
                spacing: 100

                View3D {
                    id: earthModel
                    width: 300
                    height: 300

                    PerspectiveCamera {
                        id: earthCamera
                        position: Qt.vector3d(80, 60, 180)
                        Component.onCompleted: earthCamera.lookAt(earth.position)
                    }

                    DirectionalLight {
                        brightness: 1.5
                        eulerRotation: Qt.vector3d(30, 35, 0)
                    }

                    PointLight {
                        position: Qt.vector3d(0, 100, 100)
                        brightness: 0.8
                    }

                    Timeworx_world_4045 {
                        id: earth
                        scale: Qt.vector3d(6, 6, 6)

                        NumberAnimation {
                            target: earth
                            property: "eulerRotation.y"
                            duration: 15000
                            from: earth.eulerRotation.y
                            to: earth.eulerRotation.y + 360
                            loops: Animation.Infinite
                            running: true
                        }
                    }
                }

                View3D {
                    id: marsModel
                    width: 300
                    height: 300

                    PerspectiveCamera {
                        id: camera
                        position: Qt.vector3d(80, 60, 180)
                        Component.onCompleted: camera.lookAt(mars.position)
                    }

                    DirectionalLight {
                        brightness: 1.5
                        eulerRotation: Qt.vector3d(30, 35, 0)
                    }

                    PointLight {
                        position: Qt.vector3d(0, 100, 100)
                        brightness: 0.8
                    }

                    Timeworx_mars_4071 {
                        id: mars
                        scale: Qt.vector3d(70, 70, 70)

                        NumberAnimation {
                            target: mars
                            property: "eulerRotation.y"
                            duration: 15000
                            from: mars.eulerRotation.y
                            to: mars.eulerRotation.y + 360
                            loops: Animation.Infinite
                            running: true
                        }
                    }
                }
            }
        }

        Rectangle {
            width: parent.width
            height: (parent.height - parent.spacing) * 0.35
            radius: 10
            color: "transparent"
            border.color: "white"
            border.width: 2

            Row {
                anchors.centerIn: parent
                anchors.margins: 15
                spacing: 240

                Column {
                    spacing: 40

                    Text {
                        text: "CURRENT POSITION \n Earth"
                        color: "white"
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }

                    Text {
                        text: "DESTINATION \n Mars"
                        color: "white"
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }
                }

                Column {
                    spacing: 40

                    Text {
                        text: "DISTANCE \n 225,000,000 KM"
                        color: "white"
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }

                    Text {
                        text: "VELOCITY \n 12 KM/S"
                        color: "white"
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }
                }

                Column {
                    spacing: 40

                    Text {
                        text: "ESTIMATED TIME OF ARRIVAL \n 217 days"
                        color: "white"
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }

                    Text {
                        text: "TRAJECTORY \n Nominal"
                        color: "white"
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }
                }
            }
        }
    }
}
