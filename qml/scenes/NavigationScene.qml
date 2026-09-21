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
        anchors.topMargin: Theme.sceneMarginTop
        anchors.bottomMargin: Theme.sceneMarginBottom
        anchors.leftMargin: Theme.sceneMarginLeft
        anchors.rightMargin: Theme.sceneMarginRight
        spacing: 25

        Rectangle {
            width: parent.width
            height: (parent.height - parent.spacing) * 0.65
            radius: Theme.cornerRadius
            color: "#80000000"
            border.color: Theme.light

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
            radius: Theme.cornerRadius
            color: Theme.dark
            border.color: Theme.light
            border.width: 2

            Row {
                anchors.centerIn: parent
                anchors.margins: 15
                spacing: 240

                Column {
                    spacing: 40

                    Text {
                        text: "CURRENT POSITION \n Earth"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }

                    Text {
                        text: "DESTINATION \n Mars"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }
                }

                Column {
                    spacing: 40

                    Text {
                        text: "DISTANCE \n 225,000,000 KM"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }

                    Text {
                        text: "VELOCITY \n 12 KM/S"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }
                }

                Column {
                    spacing: 40

                    Text {
                        text: "ESTIMATED TIME OF ARRIVAL \n 217 days"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }

                    Text {
                        text: "TRAJECTORY \n Nominal"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }
                }
            }
        }
    }
}
