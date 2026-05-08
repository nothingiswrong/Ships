#include "appcontroller.h"
#include <vector>
#include "validate.h"

AppController::AppController(QObject *parent)
    : QObject(parent)
    , model_(new BattleFieldModel(this))
    , isValid_(false)
    , isLoaded_(false) {}

BattleFieldModel* AppController::fieldModel() { return model_; }
bool AppController::isValid()  { return isValid_; }
bool AppController::isLoaded() { return isLoaded_; }
QStringList AppController::errorMessages() { return errorMessages_; }
QStringList AppController::coordNumbers() {return  {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}; }
QStringList AppController::coordLetters() {return  {"А", "Б", "В", "Г", "Д", "Е", "Ж", "З", "И", "К"}; }

bool AppController::loadField(const QString& path) {
    QString cleanPath = QUrl(path).toLocalFile();

    QFile file(cleanPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        resetState();
        return false;
    }


    QList<QList<char>> field;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QList<char> row;
        for (QChar c : line) {
            if (c != '0' && c != '*') {
                resetState();
                return false;
            }
             row.append(c.toLatin1());
        }

        field.append(row);
    }

    if (field.size() != 10) {
         resetState();
        return false;
    }
    for (auto& row : field) {
        if (row.size() != 10) {
             resetState();
            return false;
        }
    }

    auto vec = toVector(field);
    model_->setField(field);

    auto errors = validate_field(vec);
    isValid_ = errors.empty();
    errorMessages_.clear();

    QList<QPair<int,int>> errorCoords;
    for (auto& error : errors) {
        QString message = field_error_message(error);
        if (!errorMessages_.contains(message))
            errorMessages_.append(message);

        for (auto& coord : error.coords)
            errorCoords.append({coord.x, coord.y});
    }

    // for (auto& error: errors) {
    //     qDebug() << error.kind;
    //     for (auto cord : error.coords) {
    //         qDebug() << cord.x << ", " << cord.y;
    //     }
    //     qDebug() << "\n";
    // }

    model_->setErrors(errorCoords);

    emit isValidChanged();
    emit errorMessagesChanged();

    isLoaded_ = true;
    emit isLoadedChanged();

    return true;
}

void AppController::resetState() {
    isValid_ = false;
    isLoaded_ = false;
    errorMessages_.clear();
    model_->setField(QList<QList<char>>());
    model_->setErrors({});
    emit isValidChanged();
    emit isLoadedChanged();
    emit errorMessagesChanged();
}


std::vector<std::vector<char>> AppController::toVector(const QList<QList<char>>& field) {
    std::vector<std::vector<char>> result;
    for (auto& row : field)
        result.push_back(std::vector<char>(row.begin(), row.end()));
    return result;
}