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
}