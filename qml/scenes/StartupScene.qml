import QtQuick

Item {
    id: startupSceneRoot
    signal sequenceFinished()

    property int dotCount: 1
    property var systemLines: [
        "POWER SYSTEM ............ ONLINE",
        "NAVIGATION .............. ONLINE",
        "PROPULSION .............. ONLINE",
        "LIFE SUPPORT ............ ONLINE",
        "COMMUNICATIONS .......... ONLINE",
        "SENSORS ................. ONLINE",
        "VOICE INTERFACE ......... ONLINE"
    ]
    property int revealedLineCount: 0

    Text {
        id: startupZetaAvionicsText
        anchors.centerIn: parent
        text: "Zeta Avionics"
        font.pixelSize: Theme.fontTitle
        font.family: Theme.plexRegular
        color: Theme.primaryText
        opacity: 0.0
    }

    Text {
        id: startupInitialization
        anchors.centerIn: parent
        text: "Initializing onboard computer" + ".".repeat(dotCount)
        font.pixelSize: Theme.fontLarge
        font.family: Theme.plexRegular
        color: Theme.primaryText
        opacity: 0.0

        Timer {
            id: dotTimer
            interval: 200
            repeat: true
            onTriggered: dotCount = (dotCount % 3) + 1
        }
    }

    Column {
        id: startupSystemLines
        anchors.centerIn: parent
        spacing: 8

        Repeater {
            model: systemLines

            Text {
                text: modelData
                font.pixelSize: Theme.fontNormal
                font.family: Theme.plexRegular
                color: Theme.primaryText
                visible: index < startupSceneRoot.revealedLineCount
            }
        }
        Timer {
            id: startupSystemLinesTimer
            interval: 150
            repeat: true
            onTriggered: {
                if (revealedLineCount < systemLines.length) {
                    revealedLineCount++
                }
                else {
                    startupSystemLinesTimer.stop()
                }
            }
        }
    }

    Text {
        id: startupComplete
        anchors.centerIn: parent
        text: "ALL PRIMARY SYSTEMS NOMINAL"
        font.pixelSize: Theme.fontLarge
        font.family: Theme.plexRegular
        color: Theme.primaryText
        opacity: 0.0
    }

    SequentialAnimation {
        running: true

        NumberAnimation {
            target: startupZetaAvionicsText
            property: "opacity"
            duration: 600
            to: 1.0
        }
        PauseAnimation { duration: 2200 }
        NumberAnimation {
            target: startupZetaAvionicsText
            property: "opacity"
            duration: 600
            to: 0.0
        }

        PauseAnimation { duration: 2200 }

        ScriptAction {
            script: {
                dotTimer.start()
                startupInitialization.opacity = 1.0
            }
        }
        PauseAnimation { duration: 4200 }
        ScriptAction {
            script: {
                dotTimer.stop()
                startupInitialization.opacity = 0.0
            }
        }

        PauseAnimation { duration: 4200 }

        ScriptAction {
            script: {
                startupSystemLinesTimer.start()
                startupSystemLines.opacity = 1.0
            }
        }
        PauseAnimation { duration: 3200 }
        ScriptAction {
            script: {
                startupSystemLinesTimer.stop()
                startupSystemLines.opacity = 0.0
            }
        }

        PauseAnimation { duration: 1200 }

        NumberAnimation {
            target: startupComplete
            property: "opacity"
            duration: 600
            to: 1.0
        }
        PauseAnimation { duration: 2200 }
        NumberAnimation {
            target: startupComplete
            property: "opacity"
            duration: 600
            to: 0.0
        }

        ScriptAction {
            script: startupSceneRoot.sequenceFinished()
        }
    }
}
