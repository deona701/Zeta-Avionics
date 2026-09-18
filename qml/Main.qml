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
        color: "black"
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
        Behavior on opacity { NumberAnimation { duration: 800 } }
    }

    SpacecraftView {
        id: spacecraftView
        anchors.fill: parent
        isSpinning: uiActive
        opacity: window.currentScene === "MAIN" ? 1.0 : 0.0
        visible: opacity > 0
        Behavior on opacity { NumberAnimation { duration: 400 } }
    }

    SideNavBar {}
    BottomNavBar {}

    OverviewScene {
        id: overviewScene
        anchors.fill: parent
        visible: window.currentScene === "MAIN" && window.uiActive
    }

    PropulsionScene {
        id: propulsionScene
        anchors.fill: parent

        visible: opacity > 0
        opacity: (window.currentScene === "PROP" && window.uiActive) ? 1.0 : 0.0

        Behavior on opacity {
            NumberAnimation {
                duration: 400
                easing.type: Easing.InOutQuad
            }
        }
    }

    PowerScene {
        id: powerScene
        anchors.fill: parent

        visible: opacity > 0
        opacity: (window.currentScene === "POWER" && window.uiActive) ? 1.0 : 0.0

        Behavior on opacity {
            NumberAnimation {
                duration: 400
                easing.type: Easing.InOutQuad
            }
        }
    }

    LifeSupportScene {
        id: lifeSupportScene
        anchors.fill: parent

        visible: opacity > 0
        opacity: (window.currentScene === "LIFESUPPORT" && window.uiActive) ? 1.0 : 0.0

        Behavior on opacity {
            NumberAnimation {
                duration: 400
                easing.type: Easing.InOutQuad
            }
        }
    }

    NavigationScene {
        id: navigationScene
        anchors.fill: parent

        visible: opacity > 0
        opacity: (window.currentScene === "NAV" && window.uiActive) ? 1.0 : 0.0

        Behavior on opacity {
            NumberAnimation {
                duration: 400
                easing.type: Easing.InOutQuad
            }
        }
    }

    CommunicationsScene {
        id: communicationsScene
        anchors.fill: parent

        visible: opacity > 0
        opacity: (window.currentScene === "COMMS" && window.uiActive) ? 1.0 : 0.0

        Behavior on opacity {
            NumberAnimation {
                duration: 400
                easing.type: Easing.InOutQuad
            }
        }
    }

    StartupScene {
        id: startupScene
        anchors.fill: parent
        z: 1

        onSequenceFinished: {
            startupScene.visible = false
            window.uiActive = true
        }
    }
}
