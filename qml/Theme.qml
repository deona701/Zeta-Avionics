pragma Singleton

import QtQuick

Item {
    // Font family names
    FontLoader {
        id: regularFontLoader
        source: "qrc:/qt/qml/Zeta_Avionics/assets/fonts/IBMPlexMono-Regular.ttf"
    }
    property string plexRegular: regularFontLoader.name
}
