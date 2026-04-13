import QtQuick.Layouts
import Calculator

CalculatorButton {
    readonly property int __minSide: Math.max(implicitWidth, implicitHeight)

    property bool autoProcess: true

    Layout.preferredWidth: __minSide - 5
    Layout.preferredHeight: __minSide - 5

    palette.buttonText: "#25bd71"
    font.pointSize: 26

    onClicked: {
        if (autoProcess) {
            CalculatorEngine.processInput(text, true);
        }
    }
}
