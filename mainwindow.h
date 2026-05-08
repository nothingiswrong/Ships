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

private slots:
    void on_loadButton_clicked();


    void on_backButton_clicked();

    void on_backButton2_clicked();

private:
     QList<QList<char>> loadFieldFromFile(QString path);
    void toErrorScreen();
     std::vector<std::vector<char>> toVector(const QList<QList<char>>& field);
    void initTable();
     QString field_error_message(FieldError error);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
