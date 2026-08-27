import QtQuick

Item {
    id: startupSceneRoot
    signal sequenceFinished()

    Text {
        anchors.centerIn: parent
        font.pixelSize: 24
        font.family: Theme.plexRegular
        text: "Zeta Avionics"
        color: "#008000"
    }

}
