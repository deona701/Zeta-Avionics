pragma Singleton

import QtQuick

Item {
    // Font family names
    FontLoader {
        id: regularFontLoader
        source: "qrc:/qt/qml/Zeta_Avionics/assets/fonts/IBMPlexMono-Regular.ttf"
    }
    property string plexRegular: regularFontLoader.name

    // Font sizes
    readonly property int fontSmall: 12
    readonly property int fontNormal: 18
    readonly property int fontLarge: 20
    readonly property int fontTitle: 28

    // Font colors (only placeholders before picking the background for the main scene)
    readonly property color primaryText: "#FFFFFF"
    readonly property color secondaryText: "#E3E3E3"

    // Common dimensions
    readonly property int cornerRadius: 6

}
