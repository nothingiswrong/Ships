import QtQuick

GridView {
    model: battleFieldModel

    width: 53 * 10
    height: 53 * 10

    cellWidth: 53
    cellHeight: 53

    delegate: Rectangle {
        width: 48
        height: 48
        border.color: "black"

        color: {
            if (model.isError) return "red"
            if (model.isShip)  return "green"
            return "lightblue"
        }
    }
}