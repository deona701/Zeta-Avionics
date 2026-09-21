import QtQuick
import QtQuick.Layouts

Item {
    id: lifeSupportSceneRoot
    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        anchors.topMargin: 25
        anchors.bottomMargin: 95
        anchors.leftMargin: 95
        anchors.rightMargin: 25
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
                    bpm: "71"
                    spo2: "98%"
                    resp: "13/min"
                    temp: "36.6°C"
                }

                CrewCard {
                    name: "Astronaut #2"
                    bpm: "71"
                    spo2: "98%"
                    resp: "13/min"
                    temp: "36.6°C"
                }
            }

            ColumnLayout {
                spacing: 15
                Layout.fillWidth: true
                Layout.fillHeight: true

                CrewCard {
                    name: "Astronaut #3"
                    bpm: "71"
                    spo2: "98%"
                    resp: "13/min"
                    temp: "36.6°C"
                }

                CrewCard {
                    name: "Astronaut #4"
                    bpm: "71"
                    spo2: "98%"
                    resp: "13/min"
                    temp: "36.6°C"
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
                                text: "101.3 kPa"
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
                                text: "21.1 kPa"
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
                                text: "0.31 %"
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
                                text: "45 %"
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
                                text: "21.5°C"
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
                                text: "88.4 %"
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
                                text: "32.1 %"
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
                                text: "94.8 %"
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