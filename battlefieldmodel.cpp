#include "battlefieldmodel.h"

BattleFieldModel::BattleFieldModel(QObject *parent)
    : QAbstractListModel(parent) {}

void BattleFieldModel::setField(const QList<QList<char>>& field) {
    beginResetModel();
    cells_.clear();
    for (auto& row : field)
        for (char c : row)
            cells_.append(Cell{ c == '*', false });
    endResetModel();
}

void BattleFieldModel::setErrors(const QList<QPair<int,int>>& errorCoords) {
    for (auto& [x, y] : errorCoords) {
        int idx = y * 10 + x;
        if (idx >= 0 && idx < cells_.size()) {
            cells_[idx].isError = true;
            emit dataChanged(index(idx), index(idx), {IsErrorRole});
        }
    }
}

int BattleFieldModel::rowCount(const QModelIndex&) const {
    return cells_.size();
}

QVariant BattleFieldModel::data(const QModelIndex& idx, int role) const {
    if (!idx.isValid() || idx.row() >= cells_.size())
        return {};
    if (role == IsShipRole)  return cells_[idx.row()].isShip;
    if (role == IsErrorRole) return cells_[idx.row()].isError;
    return {};
}

QHash<int, QByteArray> BattleFieldModel::roleNames() const {
    return {
        {IsShipRole,  "isShip"},
        {IsErrorRole, "isError"}
    };
}