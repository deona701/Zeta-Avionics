import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: communicationsSceneRoot
    anchors.fill: parent

    property string pendingTitle: ""
    property string pendingMessage: ""

    function sendNtfyNotification(title, message) {
        var xhr = new XMLHttpRequest();
        var url = "https://ntfy.sh/zeta_avionics";

        xhr.open("POST", url, true);
        xhr.setRequestHeader("Title", title);

        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                console.log("ntfy response status:", xhr.status);
            }
        }

        xhr.send(message);
    }

    function transmit(title, message, durationMs) {
        if (sendAnimation.running) return;

        pendingTitle = title;
        pendingMessage = message;
        sendAnimation.duration = durationMs || 2000;
        sendAnimation.to = transmitBar.width;
        sendAnimation.start();
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 25
        anchors.bottomMargin: 95
        anchors.leftMargin: 95
        anchors.rightMargin: 25
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

                GridLayout {
                    columns: 3
                    rowSpacing: 30
                    columnSpacing: 40
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignCenter

                    ColumnLayout {
                        spacing: 4
                        Text { text: "ANTENNA TARGET LOCK"; color: Theme.secondaryText; font.pixelSize: 12 }
                        Text { text: "LOCKED"; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                    }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "SIGNAL STRENGTH"; color: Theme.secondaryText; font.pixelSize: 12 }
                        Text { text: "91% STRONG"; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                    }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "DOWNLINK STATUS"; color: Theme.secondaryText; font.pixelSize: 12 }
                        Text { text: "ACTIVE"; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                    }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "DATA TRANSFER RATE"; color: Theme.secondaryText; font.pixelSize: 12 }
                        Text { text: "--- kbps"; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                    }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "TELEMETRY PACKETS"; color: Theme.secondaryText; font.pixelSize: 12 }
                        Text { text: "-------"; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
                    }

                    ColumnLayout {
                        spacing: 4
                        Text { text: "ACTIVE LINK"; color: Theme.secondaryText; font.pixelSize: 12 }
                        Text { text: "EARTH RELAY"; color: Theme.primaryText; font.pixelSize: Theme.fontLarge; font.bold: true }
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
                    height: 10
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

                    onFinished: {
                        sendNtfyNotification(pendingTitle, pendingMessage);
                        progressFill.width = 0;
                    }
                }

                RowLayout {
                    Layout.alignment: Qt.AlignHCenter
                    spacing: 15

                    Button {
                        contentItem: Text {
                            text: parent.text
                            color: Theme.light
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            color: Theme.dark
                            border.color: Theme.light
                        }

                        text: "SEND TELEMETRY BURST"
                        onClicked: {
                            transmit(
                                "ZETA AVIONICS: Telemetry Burst",
                                "Outbound telemetry package transmitted to Earth Relay successfully.",
                                2000
                            )
                        }
                    }

                    Button {
                        contentItem: Text {
                            text: parent.text
                            color: Theme.light
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                        }

                        background: Rectangle {
                            color: Theme.dark
                            border.color: Theme.light
                        }

                        text: "DISTRESS BEACON"
                        onClicked: {
                            transmit(
                                "CRITICAL ALERT: Zeta Avionics",
                                "Master Caution triggered. Emergency broadcast sequence active.",
                                500
                            )
                        }
                    }
                }
            }
        }
    }
}