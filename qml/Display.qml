pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Layouts
import QtQml
import Calculator

Rectangle {
    id: root

    enum Op {
        Plus = 0,
        Minus = 1,
        Multiply = 2,
        Divide = 3,
        Equal = 4
    }

    readonly property int __spacing: 20

    Layout.fillWidth: true
    Layout.fillHeight: true

    color: "#202020"
    radius: 20

    ListView {
        id: historyList

        anchors {
            fill: parent
            leftMargin: root.__spacing
            rightMargin: root.__spacing
            topMargin: root.__spacing
        }
        clip: true

        Behavior on contentY {
            NumberAnimation {
                duration: 150
                easing.type: Easing.OutQuad
            }
        }

        model: CalculatorEngine.historyModel
        delegate: delegate
        footer: footerComponent
    }

    Component {
        id: delegate

        RowLayout {
            id: delegateRoot

            width: ListView.view.width

            required property string value
            required property string prefix

            property ListView __lv: ListView.view

            Item {
                // By giving this a fixed width, all operators form a perfectly straight column
                Layout.preferredWidth: 40
                Layout.fillHeight: true

                Text {
                    text: delegateRoot.prefix
                    color: "#25bd71"
                    font.pointSize: 26
                    anchors.centerIn: parent
                }
            }

            ScrollableText {
                text: delegateRoot.value
            }
        }
    }

    Component {
        id: footerComponent

        RowLayout {
            id: footerRoot

            width: ListView.view.width
            visible: ListView.view.count > 0

            Item {
                Layout.preferredWidth: 40
                Layout.fillHeight: true

                Text {
                    text: "="
                    color: "#25bd71"
                    font.pointSize: 26
                    anchors.centerIn: parent
                }
            }

            ScrollableText {
                text: CalculatorEngine.resultStep.value
            }
        }
    }

    Shortcut {
        sequences: ["Up", "k"]
        onActivated: {
            historyList.contentY = Math.max(0, historyList.contentY - 60);
        }
    }

    Shortcut {
        sequences: ["Down", "j"]
        onActivated: {
            let maxScroll = Math.max(0, historyList.contentHeight - historyList.height);

            historyList.contentY = Math.min(maxScroll, historyList.contentY + 60);
        }
    }
}
