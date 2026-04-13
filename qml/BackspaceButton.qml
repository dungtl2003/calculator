import QtQuick
import Calculator

CalculatorButton {
    icon {
        width: 40
        height: 40
        source: "assets/backspace.svg"
        // By default, Qt automatically applies a color overlay so the icon matches the current application theme (which often defaults to white text in many Linux desktop environments).
        color: "transparent"
    }

    onClicked: CalculatorEngine.deleteLastDigit()
}
