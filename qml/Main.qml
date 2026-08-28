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

    OverviewScene {
        id: overviewScene
        anchors.fill: parent
        visible: true
    }

    /*StartupScene {
        id: startupScene
        anchors.fill: parent
        z: 1

        onSequenceFinished: {
            startupScene.visible = false
            overviewScene.visible = true
        }
    }*/
}
