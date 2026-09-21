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

    // Font colors
    readonly property color primaryText: "#FFFFFF" // White
    readonly property color secondaryText: "#E3E3E3" // Grey

    // Colors
    readonly property color dark: "#000000" // Black
    readonly property color light: "#FFFFFF" // White
    readonly property color grey: "#333333" // Grey

    // Common dimensions
    readonly property int cornerRadius: 6

}
