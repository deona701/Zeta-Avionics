import QtQuick
import QtQuick.Layouts
import Zeta_Avionics

Item {
    id: lifeSupportSceneRoot

    required property SpacecraftSimulation sim
    readonly property LifeSupportSystem lifeSupportSystem: sim.state.lifeSupport

    Rectangle {
        anchors.fill: parent
        anchors.topMargin: Theme.sceneMarginTop
        anchors.bottomMargin: Theme.sceneMarginBottom
        anchors.leftMargin: Theme.sceneMarginLeft
        anchors.rightMargin: Theme.sceneMarginRight
        color: "transparent"

        RowLayout {
            anchors.fill: parent
            spacing: 15

            ColumnLayout {
                spacing: 15
                Layout.fillWidth: true
                Layout.fillHeight: true

                CrewCard {
                    name: "Astronaut #1"
                    bpm: lifeSupportSceneRoot.lifeSupportSystem.astronaut1BPM
                    spo2: lifeSupportSceneRoot.lifeSupportSystem.astronaut1SPO2 + "%"
                    resp: lifeSupportSceneRoot.lifeSupportSystem.astronaut1Resp + "/min"
                    temp: lifeSupportSceneRoot.lifeSupportSystem.astronaut1Temp.toFixed(1) + "°C"
                }

                CrewCard {
                    name: "Astronaut #2"
                    bpm: lifeSupportSceneRoot.lifeSupportSystem.astronaut2BPM
                    spo2: lifeSupportSceneRoot.lifeSupportSystem.astronaut2SPO2 + "%"
                    resp: lifeSupportSceneRoot.lifeSupportSystem.astronaut2Resp + "/min"
                    temp: lifeSupportSceneRoot.lifeSupportSystem.astronaut2Temp.toFixed(1) + "°C"
                }
            }

            ColumnLayout {
                spacing: 15
                Layout.fillWidth: true
                Layout.fillHeight: true

                CrewCard {
                    name: "Astronaut #3"
                    bpm: lifeSupportSceneRoot.lifeSupportSystem.astronaut3BPM
                    spo2: lifeSupportSceneRoot.lifeSupportSystem.astronaut3SPO2 + "%"
                    resp: lifeSupportSceneRoot.lifeSupportSystem.astronaut3Resp + "/min"
                    temp: lifeSupportSceneRoot.lifeSupportSystem.astronaut3Temp.toFixed(1) + "°C"
                }

                CrewCard {
                    name: "Astronaut #4"
                    bpm: lifeSupportSceneRoot.lifeSupportSystem.astronaut4BPM
                    spo2: lifeSupportSceneRoot.lifeSupportSystem.astronaut4SPO2 + "%"
                    resp: lifeSupportSceneRoot.lifeSupportSystem.astronaut4Resp + "/min"
                    temp: lifeSupportSceneRoot.lifeSupportSystem.astronaut4Temp.toFixed(1) + "°C"
                }
            }

            ColumnLayout {
                spacing: 15
                Layout.fillWidth: true
                Layout.fillHeight: true

                Rectangle {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    radius: Theme.cornerRadius
                    color: Theme.dark
                    border.color: Theme.light

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 20
                        spacing: 14

                        Text {
                            text: "CABIN ECLSS OVERVIEW"
                            color: Theme.primaryText
                            font.bold: true
                            font.pixelSize: Theme.fontNormal
                            Layout.alignment: Qt.AlignHCenter
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            height: 1
                            color: Theme.grey
                        }

                        Text {
                            text: "ATMOSPHERE & AIR"
                            color: Theme.secondaryText
                            font.bold: true
                            font.pixelSize: Theme.fontSmall
                        }

                        GridLayout {
                            columns: 2
                            rowSpacing: 6
                            columnSpacing: 15
                            Layout.fillWidth: true

                            Text {
                                text: "Cabin Pressure:"
                                color: Theme.secondaryText
                                font.pixelSize: Theme.fontSmall
                            }
                            Text {
                                text: lifeSupportSceneRoot.lifeSupportSystem.cabinPressure.toFixed(1) + " kPa"
                                color: Theme.primaryText
                                font.bold: true
                                font.pixelSize: Theme.fontSmall
                            }

                            Text {
                                text: "O2 Partial Press:"
                                color: Theme.secondaryText
                                font.pixelSize: Theme.fontSmall
                            }
                            Text {
                                text: lifeSupportSceneRoot.lifeSupportSystem.o2PartialPress.toFixed(1) + " kPa"
                                color: Theme.primaryText
                                font.bold: true
                                font.pixelSize: Theme.fontSmall
                            }

                            Text {
                                text: "CO2 Concentration:"
                                color: Theme.secondaryText
                                font.pixelSize: Theme.fontSmall
                            }
                            Text {
                                text: lifeSupportSceneRoot.lifeSupportSystem.co2Concentration.toFixed(1) + " %"
                                color: Theme.primaryText
                                font.bold: true
                                font.pixelSize: Theme.fontSmall
                            }

                            Text {
                                text: "Relative Humidity:"
                                color: Theme.secondaryText
                                font.pixelSize: Theme.fontSmall
                            }
                            Text {
                                text: lifeSupportSceneRoot.lifeSupportSystem.relativeHumidity.toFixed(1) + " %"
                                color: Theme.primaryText
                                font.bold: true
                                font.pixelSize: Theme.fontSmall
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            height: 1
                            color: Theme.grey
                        }

                        Text {
                            text: "ENVIRONMENTAL CONTROL"
                            color: Theme.secondaryText
                            font.bold: true
                            font.pixelSize: Theme.fontSmall
                        }

                        GridLayout {
                            columns: 2
                            rowSpacing: 6
                            columnSpacing: 15
                            Layout.fillWidth: true

                            Text {
                                text: "Cabin Temp:"
                                color: Theme.secondaryText
                                font.pixelSize: Theme.fontSmall
                            }
                            Text {
                                text: lifeSupportSceneRoot.lifeSupportSystem.cabinTemp.toFixed(1) + "°C"
                                color: Theme.primaryText
                                font.bold: true
                                font.pixelSize: Theme.fontSmall
                            }

                            Text {
                                text: "LiOH Scrubbers:"
                                color: Theme.secondaryText
                                font.pixelSize: Theme.fontSmall
                            }
                            Text {
                                text: "ACTIVE (92%)"
                                color: "green"
                                font.bold: true
                                font.pixelSize: Theme.fontSmall
                            }

                            Text {
                                text: "Coolant Loop A:"
                                color: Theme.secondaryText
                                font.pixelSize: Theme.fontSmall
                            }
                            Text {
                                text: "4.2°C (NOMINAL)"
                                color: Theme.primaryText
                                font.bold: true
                                font.pixelSize: Theme.fontSmall
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            height: 1
                            color: Theme.grey
                        }

                        Text {
                            text: "CONSUMABLES & RECYCLING"
                            color: Theme.secondaryText
                            font.bold: true
                            font.pixelSize: Theme.fontSmall
                        }

                        GridLayout {
                            columns: 2
                            rowSpacing: 6
                            columnSpacing: 15
                            Layout.fillWidth: true

                            Text {
                                text: "Potable Water:"
                                color: Theme.secondaryText
                                font.pixelSize: Theme.fontSmall
                            }
                            Text {
                                text: lifeSupportSceneRoot.lifeSupportSystem.portableWater.toFixed(1) + " %"
                                color: Theme.primaryText
                                font.bold: true
                                font.pixelSize: Theme.fontSmall
                            }

                            Text {
                                text: "Greywater Tank:"
                                color: Theme.secondaryText
                                font.pixelSize: Theme.fontSmall
                            }
                            Text {
                                text: lifeSupportSceneRoot.lifeSupportSystem.greywaterTank.toFixed(1) + " %"
                                color: Theme.primaryText
                                font.bold: true
                                font.pixelSize: Theme.fontSmall
                            }

                            Text {
                                text: "WPA Efficiency:"
                                color: Theme.secondaryText
                                font.pixelSize: Theme.fontSmall
                            }
                            Text {
                                text: lifeSupportSceneRoot.lifeSupportSystem.wpaEfficiency.toFixed(1) + " %"
                                color: Theme.primaryText
                                font.bold: true
                                font.pixelSize: Theme.fontSmall
                            }
                        }

                        Item {
                            Layout.fillHeight: true
                        }
                    }
                }
            }
        }
    }

    component CrewCard: Rectangle {
        property string name: "CREW MEMBER"
        property string bpm: "--"
        property string spo2: "--%"
        property string resp: "--/min"
        property string temp: "--°C"

        Layout.fillWidth: true
        Layout.fillHeight: true
        radius: Theme.cornerRadius
        color: Theme.dark
        border.color: Theme.light

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 10

            Text {
                text: name
                color: Theme.primaryText
                font.bold: true
                font.pixelSize: 16
                Layout.alignment: Qt.AlignHCenter
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: Theme.grey
            }

            GridLayout {
                columns: 2
                rowSpacing: 12
                columnSpacing: 20
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignCenter

                Text {
                    text: "Heart Rate:"
                    color: Theme.primaryText
                    font.pixelSize: Theme.fontSmall
                }
                Text {
                    text: bpm + " BPM"
                    color: Theme.primaryText
                    font.pixelSize: Theme.fontSmall
                }

                Text {
                    text: "SpO2:"
                    color: Theme.primaryText
                    font.pixelSize: Theme.fontSmall
                }
                Text {
                    text: spo2
                    color: Theme.primaryText
                    font.pixelSize: Theme.fontSmall
                }

                Text {
                    text: "Resp. Rate:"
                    color: Theme.primaryText
                    font.pixelSize: Theme.fontSmall
                }
                Text {
                    text: resp
                    color: Theme.primaryText
                    font.pixelSize: Theme.fontSmall
                }

                Text {
                    text: "Core Temp:"
                    color: Theme.primaryText
                    font.pixelSize: Theme.fontSmall
                }
                Text {
                    text: temp
                    color: Theme.primaryText
                    font.pixelSize: Theme.fontSmall
                }
            }
        }
    }
}