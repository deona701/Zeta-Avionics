import QtQuick
import QtQuick3D
import "../../models/EngineModel"

Item {

    Rectangle {
        width: parent.width
        height: parent.height
        radius: 10
        color: "transparent"
        border.color: "white"
        border.width: 2

        View3D {
            id: engineModel
            anchors.fill: parent

            PerspectiveCamera {
                id: engineCamera
                position: Qt.vector3d(160, -20, 300)
            }

            DirectionalLight {
                eulerRotation: Qt.vector3d(-45, 45, 0)
                brightness: 1.2
            }
            DirectionalLight {
                eulerRotation: Qt.vector3d(30, -135, 0)
                brightness: 0.4
            }
            DirectionalLight {
                eulerRotation: Qt.vector3d(-10, 180, 0)
                brightness: 0.9
            }

            PointLight {
                position: Qt.vector3d(0, 100, 100)
                brightness: 0.2
            }

            Raptorengine {
                id: engine
                scale: Qt.vector3d(0.22, 0.22, 0.22)

                NumberAnimation {
                    target: engine
                    property: "eulerRotation.y"
                    duration: 15000
                    from: engine.eulerRotation.y
                    to: engine.eulerRotation.y + 360
                    loops: Animation.Infinite
                    running: true
                }
            }
        }
    }
}
