import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import Zeta_Avionics
import QtMultimedia
import "scenes"
import "components"

ApplicationWindow {
    id: window
    width: 1280
    height: 720
    minimumWidth: 1280
    minimumHeight: 720
    visible: true
    title: "Zeta Avionics"

    property bool uiActive: false
    property string currentScene: "MAIN"

    Rectangle {
        anchors.fill: parent
        color: Theme.dark
    }

    MediaPlayer {
        id: bgPlayer
        source: Qt.resolvedUrl("../assets/background/Photoman61.mp4")
        loops: MediaPlayer.Infinite
        videoOutput: bgVideoOutput
        Component.onCompleted: bgPlayer.play()
    }

    VideoOutput {
        id: bgVideoOutput
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectCrop
        visible: window.uiActive

        opacity: window.uiActive ? 1.0 : 0.0
        Behavior on opacity {
            NumberAnimation { duration: 800 }
        }
    }

    SpacecraftView {
        id: spacecraftView
        anchors.fill: parent
        isSpinning: uiActive
        opacity: window.currentScene === "MAIN" ? 1.0 : 0.0
        visible: opacity > 0
        Behavior on opacity {
            NumberAnimation { duration: 400 }
        }
    }

    Item {
        id: sceneContainer
        anchors.fill: parent
        visible: window.uiActive

        SceneWrapper { active: window.currentScene === "MAIN"; OverviewScene { anchors.fill: parent } }
        SceneWrapper { active: window.currentScene === "PROP"; PropulsionScene { anchors.fill: parent } }
        SceneWrapper { active: window.currentScene === "POWER"; PowerScene { anchors.fill: parent } }
        SceneWrapper { active: window.currentScene === "LIFESUPPORT"; LifeSupportScene { anchors.fill: parent } }
        SceneWrapper { active: window.currentScene === "NAV"; NavigationScene { anchors.fill: parent } }
        SceneWrapper { active: window.currentScene === "COMMS"; CommunicationsScene { anchors.fill: parent } }
    }

    SideNavBar {}
    BottomNavBar {}

    StartupScene {
        id: startupScene
        anchors.fill: parent
        z: 1

        onSequenceFinished: {
            startupScene.visible = false;
            window.uiActive = true;
        }
    }

    component SceneWrapper : Item {
        property bool active: false
        anchors.fill: parent
        opacity: active ? 1.0 : 0.0
        visible: opacity > 0
        Behavior on opacity { NumberAnimation { duration: 400; easing.type: Easing.InOutQuad } }
    }
}