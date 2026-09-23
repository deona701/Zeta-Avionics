import QtQuick
import QtQuick3D
import "../../models/EarthModel"
import "../../models/MarsModel"

View3D {
    id: planetsView3D
    anchors.fill: parent

    environment: SceneEnvironment {
        clearColor: "transparent"
        backgroundMode: SceneEnvironment.Transparent
        antialiasingMode: SceneEnvironment.MSAA
        antialiasingQuality: SceneEnvironment.High
    }

    PerspectiveCamera {
        id: camera
        position: Qt.vector3d(0, 0, 360)
        clipNear: 1.0
        clipFar: 1000.0
    }

    DirectionalLight {
        brightness: 1.8
        eulerRotation: Qt.vector3d(-15, 30, 0)
    }

    PointLight {
        position: Qt.vector3d(0, 50, 250)
        brightness: 0.6
        linearFade: 0.0
    }

    Node {
        position: Qt.vector3d(-130, 0, 0)

        Timeworx_world_4045 {
            id: earth
            scale: Qt.vector3d(5.5, 5.5, 5.5)

            NumberAnimation on eulerRotation.y {
                from: 0
                to: 360
                duration: 20000
                loops: Animation.Infinite
                running: true
            }
        }
    }

    Node {
        position: Qt.vector3d(130, 0, 0)

        Timeworx_mars_4071 {
            id: mars
            scale: Qt.vector3d(60, 60, 60)

            NumberAnimation on eulerRotation.y {
                from: 0
                to: 360
                duration: 20000
                loops: Animation.Infinite
                running: true
            }
        }
    }
}