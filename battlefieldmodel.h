#pragma once
#include <QAbstractListModel>

class BattleFieldModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles {
        IsShipRole  = Qt::UserRole + 1,
        IsErrorRole = Qt::UserRole + 2
    };

    struct Cell {
        bool isShip  = false;
        bool isError = false;
    };

    explicit BattleFieldModel(QObject *parent = nullptr);

    void setField(const QList<QList<char>>& field);
    void setErrors(const QList<QPair<int,int>>& errorCoords); // список координат ошибок

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<Cell> cells_;
};