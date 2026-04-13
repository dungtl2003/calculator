import QtQuick.Layouts
import Calculator

CalculatorButton {

    readonly property int __minSide: Math.max(implicitWidth, implicitHeight)

    Layout.preferredWidth: __minSide
    Layout.preferredHeight: __minSide

    onClicked: CalculatorEngine.processInput(text, false)
}
