import QtQuick
import QtQuick3D
import "../components/"
import Zeta_Avionics

Item {
    id: navigationSceneRoot

    required property SpacecraftSimulation sim
    readonly property PropulsionSystem propulsionSystem: sim.state.propulsion
    readonly property NavigationSystem navigationSystem: sim.state.navigation

    Column {
        id: navMainColumn
        anchors.fill: parent
        anchors.topMargin: Theme.sceneMarginTop
        anchors.bottomMargin: Theme.sceneMarginBottom
        anchors.leftMargin: Theme.sceneMarginLeft
        anchors.rightMargin: Theme.sceneMarginRight
        spacing: 25

        Rectangle {
            width: parent.width
            height: (parent.height - parent.spacing) * 0.65
            radius: Theme.cornerRadius
            color: "#80000000"
            border.color: Theme.light

            PlanetsView {}
        }

        Rectangle {
            width: parent.width
            height: (parent.height - parent.spacing) * 0.35
            radius: Theme.cornerRadius
            color: Theme.dark
            border.color: Theme.light
            border.width: 2

            Row {
                anchors.centerIn: parent
                anchors.margins: 15
                spacing: 240

                Column {
                    spacing: 40

                    Text {
                        text: "CURRENT POSITION \n " + navigationSystem.currentPos
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }

                    Text {
                        text: "DESTINATION \n " + navigationSystem.destination
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }
                }

                Column {
                    spacing: 40

                    Text {
                        text: "DISTANCE \n " + navigationSystem.distance + " KM"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }

                    Text {
                        text: "VELOCITY \n " + navigationSystem.velocity.toFixed(1) + " KM/S"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }
                }

                Column {
                    spacing: 40

                    Text {
                        text: "ESTIMATED TIME OF ARRIVAL \n" + navigationSystem.eta + " days"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }

                    Text {
                        text: "TRAJECTORY \n Nominal"
                        color: Theme.primaryText
                        font.bold: true
                        font.pixelSize: Theme.fontLarge
                    }
                }
            }
        }
    }
}
