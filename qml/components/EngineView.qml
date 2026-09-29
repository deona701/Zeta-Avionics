import QtQuick
import QtQuick3D
import "../../models/EngineModel"

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