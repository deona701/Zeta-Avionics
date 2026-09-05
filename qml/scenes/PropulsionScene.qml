import QtQuick
import QtQuick3D
import "../../models/EngineModel"

Item {

    View3D {
        id: engineModel
        width: 300
        height: 300

        PerspectiveCamera {
            id: engineCamera
            position: Qt.vector3d(80, 60, 180)
            Component.onCompleted: engineCamera.lookAt(earth.position)
        }

        DirectionalLight {
            brightness: 1.5
            eulerRotation: Qt.vector3d(30, 35, 0)
        }

        PointLight {
            position: Qt.vector3d(0, 100, 100)
            brightness: 0.8
        }

        Raptorengine {
            id: engine
            scale: Qt.vector3d(6, 6, 6)

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
