import QtQuick

Item {
    id: startupSceneRoot
    signal sequenceFinished()

    Text {
        id: startupZetaAvionicsText
        anchors.centerIn: parent
        font.pixelSize: 24
        font.family: Theme.plexRegular
        text: "Zeta Avionics"
        color: Theme.primaryText
        opacity: 0.0

        SequentialAnimation {
            running: true

            NumberAnimation {
                target: startupZetaAvionicsText
                property: "opacity"
                duration: 600
                to: 1.0
            }


            PauseAnimation {
                duration: 2200
            }

            NumberAnimation {
                target: startupZetaAvionicsText
                property: "opacity"
                duration: 600
                to: 0.0
            }
        }
    }
}
