import QtQuick
import QtQuick.Layouts
import Calculator

Rectangle {
    readonly property int __spacing: 40

    // implicitWidth: txt.implicitWidth
    // implicitHeight: txt.implicitHeight
    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.leftMargin: __spacing
    Layout.rightMargin: __spacing

    color: "transparent"

    GridLayout {
        anchors {
            fill: parent
        }
        columns: 5
        rows: 4
        columnSpacing: 10
        rowSpacing: 10

        BackspaceButton {}
        DigitButton {
            text: "7"
        }
        DigitButton {
            text: "8"
        }
        DigitButton {
            text: "9"
        }
        OperatorButton {
            keySequences: ["/"]
            text: "÷"
        }
        ACButton {}
        DigitButton {
            text: "4"
        }
        DigitButton {
            text: "5"
        }
        DigitButton {
            text: "6"
        }
        OperatorButton {
            keySequences: ["*"]
            text: "×"
        }
        OperatorButton {
            text: "="
            keySequences: ["=", "Return", "Enter"]
            Layout.rowSpan: 2
            autoProcess: false
            onClicked: CalculatorEngine.assignNewValue()
        }
        DigitButton {
            text: "1"
        }
        DigitButton {
            text: "2"
        }
        DigitButton {
            text: "3"
        }
        OperatorButton {
            text: "-"
        }
        FlipSignButton {}
        DigitButton {
            text: "0"
        }
        DotButton {}
        OperatorButton {
            text: "+"
        }
    }
}
