import QtQuick
import QtQuick.Layouts

Flickable {
    id: root

    required property string text

    Layout.fillWidth: true
    Layout.preferredHeight: valText.height

    contentWidth: valText.width
    contentHeight: valText.height
    clip: true // Hides the text that spills over the left edge

    boundsBehavior: Flickable.StopAtBounds
    flickableDirection: Flickable.HorizontalFlick

    // These triggers ensure the math recalculates if the screen is resized
    Component.onCompleted: snapToRight()
    onWidthChanged: snapToRight()
    onContentWidthChanged: snapToRight()

    // Logic to push the "camera" to the far right if the number is too long
    function snapToRight() {
        if (contentWidth > width) {
            contentX = contentWidth - width;
        } else {
            contentX = 0;
        }
    }

    Text {
        id: valText
        text: root.text
        color: "#d4cfc5"
        font.pixelSize: 30

        // Logic to push the text to the far right if the number is short
        x: Math.max(0, root.width - width)
    }
}
