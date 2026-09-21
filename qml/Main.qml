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

    readonly property var sceneMap: ({
        "MAIN": Qt.resolvedUrl("scenes/OverviewScene.qml"),
        "PROP": Qt.resolvedUrl("scenes/PropulsionScene.qml"),
        "POWER": Qt.resolvedUrl("scenes/PowerScene.qml"),
        "LIFESUPPORT": Qt.resolvedUrl("scenes/LifeSupportScene.qml"),
        "NAV": Qt.resolvedUrl("scenes/NavigationScene.qml"),
        "COMMS": Qt.resolvedUrl("scenes/CommunicationsScene.qml")
    })

    onCurrentSceneChanged: {
        if (!uiActive) return;
        var sceneUrl = sceneMap[currentScene];
        if (sceneUrl) {
            stackView.replace(sceneUrl);
        }
    }

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

    StackView {
        id: stackView
        anchors.fill: parent
        visible: window.uiActive

        replaceEnter: Transition {
            NumberAnimation {
                property: "opacity"
                from: 0.0
                to: 1.0
                duration: 400
                easing.type: Easing.InOutQuad
            }
        }

        replaceExit: Transition {
            NumberAnimation {
                property: "opacity"
                from: 1.0
                to: 0.0
                duration: 400
                easing.type: Easing.InOutQuad
            }
        }
    }

    SideNavBar {}
    BottomNavBar {}

    StartupScene {
        id: startupScene
        anchors.fill: parent
        z: 1

        onSequenceFinished: {
            startupScene.visible = false
            window.uiActive = true
            stackView.replace(window.sceneMap[window.currentScene])
        }
    }
}
