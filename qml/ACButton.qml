import QtQuick.Layouts
import Calculator

CalculatorButton {
    readonly property int __minSide: Math.max(implicitWidth, implicitHeight)

    Layout.preferredWidth: __minSide + 5

    text: "AC"
    palette.buttonText: "#b22524"
    font {
        bold: true
        pointSize: 22
    }

    onClicked: CalculatorEngine.clear()
}
