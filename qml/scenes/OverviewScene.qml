pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Layouts
import Zeta_Avionics

Item {
    id: overviewSceneRoot

    required property SpacecraftSimulation sim

    readonly property PropulsionSystem propulsion: sim.state.propulsion
    readonly property NavigationSystem navigation: sim.state.navigation
    readonly property PowerSystem power: sim.state.power
    readonly property LifeSupportSystem lifeSupport: sim.state.lifeSupport
    readonly property CommunicationsSystem comms: sim.state.communications
    readonly property DiagnosticsSystem diagnostics: sim.state.diagnostics
    readonly property MissionsSystem missions: sim.state.missions
    readonly property ZetaAssistantSystem zeta: sim.state.zeta

    anchors.topMargin: Theme.sceneMarginTop
    anchors.bottomMargin: Theme.sceneMarginBottom
    anchors.leftMargin: Theme.sceneMarginLeft
    anchors.rightMargin: Theme.sceneMarginRight

    component StatBlock : ColumnLayout {
        property string label: ""
        property string value: ""
        property string detail: ""
        property string targetScene: ""
        property bool rightAligned: false

        spacing: 1

        Text {
            text: parent.label
            color: Theme.secondaryText
            font.family: Theme.plexRegular
            font.pixelSize: Theme.fontSmall - 1
            Layout.alignment: parent.rightAligned ? Qt.AlignRight : Qt.AlignLeft
        }

        Text {
            text: parent.value
            color: Theme.primaryText
            font.family: Theme.plexRegular
            font.bold: true
            font.pixelSize: Theme.fontLarge
            Layout.alignment: parent.rightAligned ? Qt.AlignRight : Qt.AlignLeft
        }

        Text {
            text: parent.detail
            color: Theme.secondaryText
            font.family: Theme.plexRegular
            font.pixelSize: Theme.fontSmall
            visible: text !== ""
            Layout.alignment: parent.rightAligned ? Qt.AlignRight : Qt.AlignLeft
        }

        MouseArea {
            anchors.fill: parent
            enabled: parent.targetScene !== ""
            cursorShape: enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
            onClicked: window.currentScene = parent.targetScene
        }
    }

    component ThinRule : Rectangle {
        property real ratio: -1.0

        Layout.preferredHeight: 1
        color: Theme.grey

        Rectangle {
            visible: parent.ratio >= 0.0
            width: parent.width * Math.max(0.0, Math.min(1.0, parent.ratio))
            height: parent.height
            color: Theme.light
            Behavior on width { NumberAnimation { duration: 120 } }
        }
    }

    RowLayout {
        id: topBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        spacing: 16

        Text {
            text: overviewSceneRoot.missions.currentMission.toUpperCase() + "  " + overviewSceneRoot.missions.objectiveProgress.toFixed(0) + "%"
            color: Theme.primaryText
            font.family: Theme.plexRegular
            font.bold: true
            font.pixelSize: Theme.fontSmall + 1

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: window.currentScene = "MISSIONS"
            }
        }

        Item { Layout.fillWidth: true }

        Text {
            text: "ZETA  " + overviewSceneRoot.zeta.audioStatus
            color: Theme.secondaryText
            font.family: Theme.plexRegular
            font.pixelSize: Theme.fontSmall

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: window.currentScene = "ZETA"
            }
        }

        Rectangle { Layout.preferredWidth: 1; Layout.preferredHeight: 16; color: Theme.grey }

        Text {
            text: overviewSceneRoot.diagnostics.systemHealthStatus + " (" + overviewSceneRoot.diagnostics.activeFaultCount + ")"
            color: Theme.primaryText
            font.family: Theme.plexRegular
            font.bold: true
            font.pixelSize: Theme.fontSmall + 1

            MouseArea {
                anchors.fill: parent
                cursorShape: Qt.PointingHandCursor
                onClicked: window.currentScene = "DIAG"
            }
        }
    }

    ThinRule {
        id: topLine
        anchors.top: topBar.bottom
        anchors.topMargin: 6
        anchors.left: parent.left
        anchors.right: parent.right
    }

    ColumnLayout {
        anchors.top: topLine.bottom
        anchors.topMargin: 12
        anchors.left: parent.left
        width: 185
        spacing: 10

        StatBlock {
            label: "THRUST OUTPUT"
            value: overviewSceneRoot.propulsion.thrustOutput.toFixed(1) + " kN"
            detail: "FUEL " + overviewSceneRoot.propulsion.propellantPercentage.toFixed(0) + "%  ·  " + overviewSceneRoot.propulsion.engineTemp.toFixed(0) + "°C"
            targetScene: "PROP"
        }

        ThinRule {
            Layout.fillWidth: true
            ratio: overviewSceneRoot.propulsion.propellantPercentage / 100.0
        }

        StatBlock {
            label: "VELOCITY"
            value: overviewSceneRoot.navigation.velocity.toFixed(2) + " km/s"
            detail: overviewSceneRoot.navigation.destination.toUpperCase() + "  ·  ETA " + overviewSceneRoot.navigation.eta + "d"
            targetScene: "NAV"
        }
    }

    ColumnLayout {
        anchors.top: topLine.bottom
        anchors.topMargin: 12
        anchors.right: parent.right
        width: 185
        spacing: 10

        StatBlock {
            label: "MAIN BATTERY"
            value: overviewSceneRoot.power.mainBatteryBus.toFixed(1) + "%"
            detail: (overviewSceneRoot.power.inEclipse ? "ECLIPSE" : "SOLAR") + "  ·  " + (overviewSceneRoot.power.netBusBalance >= 0 ? "+" : "") + overviewSceneRoot.power.netBusBalance.toFixed(1) + " kW"
            targetScene: "POWER"
            rightAligned: true
        }

        ThinRule {
            Layout.fillWidth: true
            ratio: overviewSceneRoot.power.mainBatteryBus / 100.0
        }

        StatBlock {
            label: "CABIN PRESS"
            value: overviewSceneRoot.lifeSupport.cabinPressure.toFixed(1) + " kPa"
            detail: "CO2 " + overviewSceneRoot.lifeSupport.co2Concentration.toFixed(2) + "%  ·  " + overviewSceneRoot.lifeSupport.astronaut1BPM + " BPM"
            targetScene: "LIFESUPPORT"
            rightAligned: true
        }
    }

    RowLayout {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        spacing: 20

        StatBlock {
            label: "DELTA-V"
            value: overviewSceneRoot.propulsion.deltaV.toFixed(0) + " m/s"
            detail: overviewSceneRoot.navigation.trajectory.toUpperCase()
            targetScene: "PROP"
        }

        Item { Layout.fillWidth: true }

        StatBlock {
            label: "COMMS SIGNAL"
            value: overviewSceneRoot.comms.signalStrength.toFixed(0) + "%"
            detail: overviewSceneRoot.comms.antennaTargetLock
            targetScene: "COMMS"
            rightAligned: true
        }
    }
}