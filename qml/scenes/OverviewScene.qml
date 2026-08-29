import QtQuick
import QtMultimedia
import "../components/"

Item {
    id: dashboardRoot
    anchors.fill: parent

    MediaPlayer {
        id: bgPlayer
        source: Qt.resolvedUrl("../../assets/background/Photoman61.mp4")
        loops: MediaPlayer.Infinite
        videoOutput: bgVideoOutput
        Component.onCompleted: bgPlayer.play()
    }

    VideoOutput {
        id: bgVideoOutput
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectCrop
    }

    SpacecraftView {
        anchors.fill: parent
    }

    Rectangle {
        id: sidePanel
        width: 69
        height: 500
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        radius: 15
        opacity: 0.7
    }

    Rectangle {
        width: 800
        height: 70
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        radius: 15
        opacity: 0.7
    }
}