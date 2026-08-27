import QtQuick
import QtQuick3D
import "SpacecraftModel"

View3D {
    id: spacecraftModelView3D
    anchors.fill: parent

    camera: camera

    //environment: SceneEnvironment {
    //    clearColor: "#0a0d14"
    //    backgroundMode: SceneEnvironment.Color
    //}

    property real pitchAngle: 0
    property real yawAngle: 0
    property real rollAngle: 0

    PerspectiveCamera {
        id: camera
        position: Qt.vector3d(0, 20, 350)
        Component.onCompleted: camera.lookAt(spacecraft.position)
    }

    DirectionalLight {
        brightness: 1.5
        eulerRotation: Qt.vector3d(-30, -30, 0)
    }

    PointLight {
        position: Qt.vector3d(0, 100, 100)
        brightness: 0.8
    }

    Futuristic_spacecraft_3dmodel {
        id: spacecraft
        scale: Qt.vector3d(350, 350, 350)

        eulerRotation.x: spacecraftModelView3D.pitchAngle
        eulerRotation.y: spacecraftModelView3D.yawAngle
        eulerRotation.z: spacecraftModelView3D.rollAngle
    }
}