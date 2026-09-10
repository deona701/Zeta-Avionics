import QtQuick
import QtQuick.Layouts

Item {
    id: root
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
                    radius: 6
                    color: "black"
                    border.color: "white"

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 20
                        spacing: 14

                        Text {
                            text: "CABIN ECLSS OVERVIEW"
                            color: "white"
                            font.bold: true
                            font.pixelSize: Theme.fontNormal
                            Layout.alignment: Qt.AlignHCenter
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            height: 1
                            color: "#444444"
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
                                color: "white"
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
                                color: "white"
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
                                color: "white"
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
                                color: "white"
                                font.bold: true
                                font.pixelSize: Theme.fontSmall
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            height: 1
                            color: "#222222"
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
                                color: "white"
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
                                color: "white"
                                font.bold: true
                                font.pixelSize: Theme.fontSmall
                            }
                        }

                        Rectangle {
                            Layout.fillWidth: true
                            height: 1
                            color: "#222222"
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
                                color: "white"
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
                                color: "white"
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
                                color: "white"
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
        radius: 6
        color: "black"
        border.color: "white"

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 10

            Text {
                text: name
                color: "white"
                font.bold: true
                font.pixelSize: 16
                Layout.alignment: Qt.AlignHCenter
            }

            Rectangle {
                Layout.fillWidth: true
                height: 1
                color: "#333333"
            }

            GridLayout {
                columns: 2
                rowSpacing: 12
                columnSpacing: 20
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignCenter

                Text {
                    text: "Heart Rate:"
                    color: "white"
                    font.pixelSize: Theme.fontSmall
                }
                Text {
                    text: bpm + " BPM"
                    color: "white"
                    font.pixelSize: Theme.fontSmall
                }

                Text {
                    text: "SpO2:"
                    color: "white"
                    font.pixelSize: Theme.fontSmall
                }
                Text {
                    text: spo2
                    color: "white"
                    font.pixelSize: Theme.fontSmall
                }

                Text {
                    text: "Resp. Rate:"
                    color: "white"
                    font.pixelSize: Theme.fontSmall
                }
                Text {
                    text: resp
                    color: "white"
                    font.pixelSize: Theme.fontSmall
                }

                Text {
                    text: "Core Temp:"
                    color: "white"
                    font.pixelSize: Theme.fontSmall
                }
                Text {
                    text: temp
                    color: "white"
                    font.pixelSize: Theme.fontSmall
                }
            }
        }
    }
}