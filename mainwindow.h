#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qtmetamacros.h>
#include "validate.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static const int fieldSize = 10;

private slots:
    void on_loadButton_clicked();


    void on_backButton_clicked();

    void on_backButton2_clicked();

private:
     QList<QList<char>> loadFieldFromFile(QString path);
    void toErrorScreen();
     void toFieldScreen();
    void toStartScreen();
     std::vector<std::vector<char>> toVector(const QList<QList<char>>& field);
    void initTable();
     QString field_error_message(FieldError error);
    void initScreen(vector<FieldError>& errors, QList<QList<char>>& field, vector<vector<bool>>& errorCoords);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
