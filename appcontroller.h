#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QUrl>
#include <QStringList>
#include "battlefieldmodel.h"

class AppController : public QObject {
    Q_OBJECT
    Q_PROPERTY(BattleFieldModel* fieldModel READ fieldModel CONSTANT)
    Q_PROPERTY(bool isValid  READ isValid  NOTIFY isValidChanged)
    Q_PROPERTY(bool isLoaded READ isLoaded NOTIFY isLoadedChanged)
    Q_PROPERTY(QStringList errorMessages READ errorMessages NOTIFY errorMessagesChanged)

public:
    explicit AppController(QObject *parent = nullptr);

    BattleFieldModel* fieldModel();
    bool isValid();
    bool isLoaded();
    QStringList errorMessages();
    void resetState();

signals:
    void isValidChanged();
    void isLoadedChanged();
    void errorMessagesChanged();

public slots:
    bool loadField(const QString& path);

private:
    static std::vector<std::vector<char>> toVector(const QList<QList<char>>& field);


    BattleFieldModel* model_;
    bool isValid_;
    bool isLoaded_;
    QStringList errorMessages_;
};