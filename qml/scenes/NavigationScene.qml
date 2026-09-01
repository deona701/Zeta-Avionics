import QtQuick

Item {
    id: navigationSceneRoot
    anchors.fill: parent

    Column {
    anchors.fill: parent
    anchors.topMargin: 25
    anchors.bottomMargin: 95
    anchors.leftMargin: 95
    anchors.rightMargin: 25
    spacing: 25

        Rectangle {
            width: parent.width
            height: (parent.height - parent.spacing) * 0.65
            radius: 10
            color: "#BDBDBD"
            border.color: "black"
            border.width: 2
        }

        Rectangle {
            width: parent.width
            height: (parent.height - parent.spacing) * 0.30
            radius: 10
            color: "#BDBDBD"
            border.color: "black"
            border.width: 2
        }
    }
}
