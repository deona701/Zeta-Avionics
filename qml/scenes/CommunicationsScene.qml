import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import Zeta_Avionics

Item {
    id: communicationsSceneRoot

    required property SpacecraftSimulation sim
    readonly property CommunicationsSystem communicationsSystem: sim.state.communications

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: Theme.sceneMarginTop
        anchors.bottomMargin: Theme.sceneMarginBottom
        anchors.leftMargin: Theme.sceneMarginLeft
        anchors.rightMargin: Theme.sceneMarginRight
        spacing: 25

        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 350
            color: Theme.dark
            border.color: Theme.light
            radius: Theme.cornerRadius

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 15

                RowLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.margins: 200
                    spacing: 120

                    GridLayout {
                        columns: 3
                        rowSpacing: 30
                        columnSpacing: 40
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignCenter

                        ColumnLayout {
                            spacing: 4
                            Text { text: "ANTENNA TARGET LOCK"; color: Theme.secondaryText; font.pixelSize: 12 }
                            Text { text: communicationsSceneRoot.communicationsSystem.antennaTargetLock; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                        }

                        ColumnLayout {
                            spacing: 4
                            Text { text: "SIGNAL STRENGTH"; color: Theme.secondaryText; font.pixelSize: 12 }
                            Text { text: communicationsSceneRoot.communicationsSystem.signalStrength.toFixed(1); color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                        }

                        ColumnLayout {
                            spacing: 4
                            Text { text: "DOWNLINK STATUS"; color: Theme.secondaryText; font.pixelSize: 12 }
                            Text { text: communicationsSceneRoot.communicationsSystem.downlinkStatus ? "ACTIVE" : "OFFLINE"; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                        }

                        ColumnLayout {
                            spacing: 4
                            Text { text: "DATA TRANSFER RATE"; color: Theme.secondaryText; font.pixelSize: 12 }
                            Text { text: communicationsSceneRoot.communicationsSystem.dataTransferRate.toFixed(1) + " kbps"; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                        }

                        ColumnLayout {
                            spacing: 4
                            Text { text: "TELEMETRY PACKETS"; color: Theme.secondaryText; font.pixelSize: 12 }
                            Text { text: communicationsSceneRoot.communicationsSystem.telemetryPackets; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                        }

                        ColumnLayout {
                            spacing: 4
                            Text { text: "ACTIVE LINK"; color: Theme.secondaryText; font.pixelSize: 12 }
                            Text { text: communicationsSceneRoot.communicationsSystem.activeLink; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                        }
                    }

                    ColumnLayout {
                        Layout.alignment: Qt.AlignVCenter
                        spacing: 15

                        Button {
                            Layout.alignment: Qt.AlignCenter
                            Layout.topMargin: 10

                            contentItem: Text {
                                text: communicationsSceneRoot.communicationsSystem.downlinkStatus ? "ACTIVE" : "OFFLINE"
                                color: Theme.light
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }

                            background: Rectangle {
                                color: Theme.dark
                                border.color: Theme.light
                            }

                            onClicked: {
                                communicationsSceneRoot.communicationsSystem.downlinkStatus = !communicationsSceneRoot.communicationsSystem.downlinkStatus;
                            }
                        }

                        Slider {
                            id: activeLinkSlider
                            Layout.preferredWidth: 200
                            from: 0
                            to: 2
                            stepSize: 1
                            snapMode: Slider.SnapOnRelease
                            value: {
                                let link = communicationsSceneRoot.communicationsSystem.activeLink;
                                if (link.includes("MARS"))
                                    return 1;
                                if (link.includes("GATEWAY") || link.includes("LAGRANGE"))
                                    return 2;
                                return 0;
                            }
                            onMoved: {
                                if (value === 0)
                                    communicationsSceneRoot.communicationsSystem.activeLink = "EARTH RELAY / DSN";
                                else if (value === 1)
                                    communicationsSceneRoot.communicationsSystem.activeLink = "MARS ORBITER RELAY";
                                else if (value === 2)
                                    communicationsSceneRoot.communicationsSystem.activeLink = "DEEP SPACE GATEWAY";
                            }

                            background: Rectangle {
                                x: activeLinkSlider.leftPadding
                                y: activeLinkSlider.topPadding + activeLinkSlider.availableHeight / 2 - height / 2
                                implicitWidth: 200
                                implicitHeight: 12
                                width: activeLinkSlider.availableWidth
                                height: implicitHeight
                                radius: Theme.cornerRadius
                                color: Theme.dark
                                border.color: Theme.light

                                Rectangle {
                                    width: activeLinkSlider.visualPosition * parent.width
                                    height: parent.height
                                    color: Theme.light
                                    radius: Theme.cornerRadius
                                }
                            }

                            handle: Rectangle {
                                x: activeLinkSlider.leftPadding + activeLinkSlider.visualPosition * (activeLinkSlider.availableWidth - width)
                                y: activeLinkSlider.topPadding + activeLinkSlider.availableHeight / 2 - height / 2
                                implicitWidth: 24
                                implicitHeight: 28
                                radius: Theme.cornerRadius
                                color: Theme.dark
                                border.color: Theme.light
                            }
                        }

                        Slider {
                            id: antennaAlignmentSlider
                            Layout.preferredWidth: 200
                            from: 0
                            to: 100
                            value: communicationsSceneRoot.communicationsSystem.antennaAlignment
                            onMoved: {
                                communicationsSceneRoot.communicationsSystem.antennaAlignment = value;
                            }

                            background: Rectangle {
                                x: antennaAlignmentSlider.leftPadding
                                y: antennaAlignmentSlider.topPadding + antennaAlignmentSlider.availableHeight / 2 - height / 2
                                implicitWidth: 200
                                implicitHeight: 12
                                width: antennaAlignmentSlider.availableWidth
                                height: implicitHeight
                                radius: Theme.cornerRadius
                                color: Theme.dark
                                border.color: Theme.light

                                Rectangle {
                                    width: antennaAlignmentSlider.visualPosition * parent.width
                                    height: parent.height
                                    color: Theme.light
                                    radius: Theme.cornerRadius
                                }
                            }

                            handle: Rectangle {
                                x: antennaAlignmentSlider.leftPadding + antennaAlignmentSlider.visualPosition * (antennaAlignmentSlider.availableWidth - width)
                                y: antennaAlignmentSlider.topPadding + antennaAlignmentSlider.availableHeight / 2 - height / 2
                                implicitWidth: 24
                                implicitHeight: 28
                                radius: Theme.cornerRadius
                                color: Theme.dark
                                border.color: Theme.light
                            }
                        }
                    }
                }
            }
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 100
            color: Theme.dark
            border.color: Theme.light
            radius: Theme.cornerRadius

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 15

                Rectangle {
                    id: transmitBar
                    Layout.fillWidth: true
                    Layout.preferredHeight: 10
                    color: Theme.dark
                    border.color: Theme.light

                    Rectangle {
                        id: progressFill
                        width: 0
                        height: parent.height
                        color: Theme.light
                    }
                }

                NumberAnimation {
                    id: sendAnimation
                    target: progressFill
                    property: "width"
                    from: 0
                    to: transmitBar.width
                    duration: 2000

                    property string targetAction: "telemetry"

                    onFinished: {
                        if (targetAction === "telemetry") {
                            communicationsSceneRoot.communicationsSystem.sendTelemetryBurst();
                        } else {
                            communicationsSceneRoot.communicationsSystem.triggerDistressBeacon();
                        }
                        progressFill.width = 0;
                    }
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 15

                    Button {
                        id: burstButton
                        text: "SEND TELEMETRY BURST"

                        contentItem: Text {
                            text: burstButton.text
                            color: Theme.light
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            color: Theme.dark
                            border.color: Theme.light
                        }

                        onClicked: {
                            sendAnimation.targetAction = "telemetry";
                            sendAnimation.duration = 2000;
                            sendAnimation.start();
                        }
                    }

                    Button {
                        id: distressButton
                        text: "DISTRESS BEACON"

                        contentItem: Text {
                            text: distressButton.text
                            color: Theme.light
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            color: Theme.dark
                            border.color: Theme.light
                        }

                        onClicked: {
                            sendAnimation.targetAction = "distress";
                            sendAnimation.duration = 500;
                            sendAnimation.start();
                        }
                    }
                }
            }
        }
    }
}