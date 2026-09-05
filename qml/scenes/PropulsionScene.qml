import QtQuick
import QtQuick3D
import "../../models/EngineModel"

Item {

    Rectangle {
        width: parent.width
        height: parent.height
        anchors.centerIn: parent
        radius: 10
        color: "transparent"

        View3D {
            id: engineModel
            anchors.fill: parent

            PerspectiveCamera {
                id: engineCamera
                position: Qt.vector3d(0, 100, 300)
                Component.onCompleted: engineCamera.lookAt(engine.position)
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
                scale: Qt.vector3d(0.3, 0.3, 0.3)
                position: Qt.vector3d(0, -80, 0)

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
