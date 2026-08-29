import QtQuick
import QtQuick3D
import "SpacecraftModel"

View3D {
    id: spacecraftModelView3D
    anchors.fill: parent

    camera: camera

    environment: SceneEnvironment {
        clearColor: "#05070a"
        backgroundMode: SceneEnvironment.Transparent
        antialiasingMode: SceneEnvironment.MSAA
        antialiasingQuality: SceneEnvironment.High
        probeExposure: 1.5
    }

    property real pitchAngle: 0
    property real yawAngle: 0
    property real rollAngle: 0

    PerspectiveCamera {
        id: camera
        position: Qt.vector3d(80, 60, 180)
        Component.onCompleted: camera.lookAt(spacecraft.position)
    }

    DirectionalLight {
        brightness: 1.5
        eulerRotation: Qt.vector3d(30, 35, 0)
    }

    PointLight {
        position: Qt.vector3d(0, 100, 100)
        brightness: 0.8
    }

    Futuristic_spacecraft_3dmodel {
        id: spacecraft
        scale: Qt.vector3d(280, 280, 280)

        eulerRotation.x: spacecraftModelView3D.pitchAngle + 10
        eulerRotation.y: spacecraftModelView3D.yawAngle - 35
        eulerRotation.z: spacecraftModelView3D.rollAngle

        NumberAnimation {
            id: spinAnimation
            target: spacecraft
            property: "eulerRotation.y"
            duration: 35000
            from: spacecraft.eulerRotation.y
            to: spacecraft.eulerRotation.y + 360
            loops: Animation.Infinite
            running: true
        }
    }

    MouseArea {
        anchors.fill: parent
        property real lastX: 0

        onPressed: (mouse) => {
            spinAnimation.stop()
            lastX = mouse.x
        }
        onPositionChanged: (mouse) => {
            var delta = mouse.x - lastX
            spacecraft.eulerRotation.y += delta * 0.5
            lastX = mouse.x
        }
        onReleased: { spinAnimation.start() }
    }
}
