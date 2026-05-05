import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs

ApplicationWindow {
    width: 860
    height: 640
    visible: true

    FileDialog {
        id: fileDialog
        onAccepted: {
            var ok = controller.loadField(fileDialog.selectedFile)
            if (ok) {
                startScreen.visible = false
                fieldScreen.visible = true
            } else {
                startScreen.visible = false
                inputErrorScreen.visible = true
            }
        }
    }


    Item {
        id: startScreen
        anchors.fill: parent

        Button {
            anchors.centerIn: parent
            text: "Загрузить поле"
            onClicked: fileDialog.open()
        }
    }


    Item {
        id: fieldScreen
        visible: false
        anchors.fill: parent

        Button {
            text: "Назад"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 10
            onClicked: {
                fieldScreen.visible = false
                startScreen.visible = true
            }
        }

        Row {
            anchors.centerIn: parent
            spacing: 24

            BattleField {
                id: battleField
                model: controller.fieldModel
            }

            Rectangle {
                width: 260
                height: battleField.height
                color: "#f8f8f8"
                border.color: "#b0b0b0"
                radius: 8

                Column {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 10

                    Text {
                        text: controller.isValid ? "Поле заполнено корректно" : "Ошибки"
                        color: controller.isValid ? "darkgreen" : "darkred"
                        font.pixelSize: 20
                        font.bold: true
                        wrapMode: Text.WordWrap
                        width: parent.width
                    }

                    Text {
                        visible: controller.isValid
                        text: "Ошибок не найдено"
                        color: "darkgreen"
                        font.pixelSize: 15
                        wrapMode: Text.WordWrap
                        width: parent.width
                    }

                    ListView {
                        id: errorList
                        visible: !controller.isValid
                        width: parent.width
                        height: parent.height - y
                        clip: true
                        spacing: 8
                        model: controller.errorMessages

                        delegate: Text {
                            width: errorList.width
                            text: "- " + modelData
                            color: "darkred"
                            font.pixelSize: 15
                            wrapMode: Text.WordWrap
                        }
                    }
                }
            }
        }
    }

    Item {
        visible: false
        id: inputErrorScreen
        Text {
            id: inputErrorText
            text: "Неправильный формат ввода"
            color: "red"
            font.pixelSize: 25
            wrapMode: Text.WordWrap
            width: parent.width
        }
        Button {
            text: "Назад"
            anchors.top: inputErrorText.top
            anchors.left: parent.left
            anchors.margins: 50
            onClicked: {
                inputErrorScreen.visible = false
                startScreen.visible = true
            }
        }
    }
}