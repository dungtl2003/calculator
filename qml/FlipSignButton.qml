import QtQuick.Layouts
import Calculator

CalculatorButton {

    readonly property int __minSide: Math.max(implicitWidth, implicitHeight)

    Layout.preferredWidth: __minSide
    Layout.preferredHeight: __minSide

    text: "±"
    palette.buttonText: "#25bd71"
    keySequences: ["~"]

    onClicked: CalculatorEngine.flipValueSign()
}
