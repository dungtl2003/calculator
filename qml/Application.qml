import QtQuick
import QtQuick.Layouts

Window {
    id: appWindow

    visible: true
    minimumWidth: 450
    minimumHeight: 630

    title: "Calculator Pro"

    color: "#1d1d1d"

    readonly property int __spacing: 20

    ColumnLayout {
        anchors {
            fill: parent
            margins: appWindow.__spacing
        }
        spacing: appWindow.__spacing

        Display {}

        NumPad {}
    }
}
