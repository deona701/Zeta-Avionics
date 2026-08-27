import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic
import Zeta_Avionics
import "scenes"

ApplicationWindow {
    id: window
    width: 1280
    height: 720
    minimumWidth: 1280
    minimumHeight: 720
    visible: true
    title: "Zeta Avionics"

    StartupScene {
        id: startupScene
        anchors.fill: parent

        onSequenceFinished: {
            startupScene.visible = false
        }
    }
}
