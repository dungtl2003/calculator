import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Button {
    id: control

    property var keySequences: [text]

    Layout.fillWidth: true
    Layout.fillHeight: true
    // Because all buttons will equal "1", they will all scale at a 1:1:1 ratio
    Layout.preferredWidth: 1
    Layout.preferredHeight: 1

    text: ""
    font.pointSize: 24
    palette.buttonText: "#d4cfc5"

    Shortcut {
        sequences: control.keySequences

        onActivated: control.clicked()
    }

    background: Rectangle {
        border {
            width: 2
            color: "#646464"
        }
        radius: 10
        color: "transparent"
    }
}
