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
        anchors {
            fill: parent
            leftMargin: root.__spacing
            rightMargin: root.__spacing
            topMargin: root.__spacing
        }
        clip: true

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

            Text {
                id: pref

                text: delegateRoot.prefix
                color: "#25bd71"
                font.pointSize: 26
            }

            Item {
                Layout.fillWidth: true
            }

            Text {
                id: val
                text: delegateRoot.value
                color: "#d4cfc5"
                font.pointSize: 24
            }
        }
    }

    Component {
        id: footerComponent

        RowLayout {
            id: footerRoot
            width: ListView.view.width

            visible: ListView.view.count > 0

            Text {
                text: "="
                color: "#25bd71"
                font.pointSize: 26
            }

            Item {
                Layout.fillWidth: true
            }

            Text {
                text: CalculatorEngine.resultStep.value
                color: "#d4cfc5"
                font.pointSize: 24
            }
        }
    }
}
