#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include "validate.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_loadButton_clicked()
{
    QString path = QFileDialog::getOpenFileName(this,  "Выберите файл поля", "", "Text files (*.txt)");
    if (path.isEmpty()) return;
    auto field = loadFieldFromFile(path);
    if (field.size() == 0) {
        toErrorScreen();
        return;
    }

    auto vec = toVector(field);
    auto errors = validate_field(vec);
    vector<vector<bool>> errorCoords(10, vector<bool>(10, false));
    for (FieldError err : errors) {
        for (auto [x, y]: err.coords ) {
            errorCoords[y][x] = true;
        }
    }
    initScreen(errors, field, errorCoords);

    ui ->stackedWidget->setCurrentIndex(1);
}

void MainWindow::initScreen(vector<FieldError>& errors, QList<QList<char>>& field, vector<vector<bool>>& errorCoords) {
    initTable();
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            QColor color;
            if (errorCoords[i][j]) {
                color = Qt::red;
            } else if (field[i][j] == '*') {
                color = Qt::green;
            } else {
                color = Qt::lightGray;
            }
            ui->fieldTable->item(i, j)->setBackground(color);
        }
    }

    bool structureErrorMet = false;
    for (FieldError& err: errors) {
        QString msg = "- " + field_error_message(err);
        if (err.kind == SHIPS_CONNECTED) {
            if (!structureErrorMet) {
                ui->errorList->addItem(msg);
                structureErrorMet = true;
            }
        } else {
            ui->errorList->addItem(msg);
        }

    }

    auto label = ui->resultLabel;
    if (errors.empty()) {
        label->setStyleSheet("color: green;");
        label->setText("Ошибок не найдено");
        ui->errorList->setVisible(false);
    } else {
        label->setStyleSheet("color: red;");
        label->setText("Найдены ошибки");
    }
}

QString MainWindow::field_error_message(FieldError error) {
    switch (error.kind) {
    case TOO_LONG_SHIP:
        return "Найдены корабли длиннее 4 клеток";
    case NOT_ENOUGH_SHIPS:
        return QString("Не хватает %1 корректных кораблей длиной %2").arg(error.n).arg(error.len);
    case TOO_MANY_SHIPS:
        return QString("Слишком много кораблей длиной %1: %2").arg(error.len).arg(error.n);
    case SHIPS_CONNECTED:
        return "Найдена некорректная структура кораблей";
    }

    return "Неизвестная ошибка";
}


void MainWindow::initTable() {
    ui->fieldTable->clear();
    ui->fieldTable->setRowCount(10);
    ui->fieldTable->setColumnCount(10);
    ui->fieldTable->horizontalHeader()->hide();
    ui->fieldTable->verticalHeader()->hide();
    ui->fieldTable->horizontalHeader()->setDefaultSectionSize(44);
    ui->fieldTable->verticalHeader()->setDefaultSectionSize(44);
    ui->fieldTable->setFixedSize(442, 442);

    ui->errorList->clear();
    ui->errorList->setVisible(true);

    for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 10; col++) {
            auto item = new QTableWidgetItem();
            item->setBackground(Qt::lightGray);
            item->setFlags(Qt::ItemIsEnabled);
            ui->fieldTable->setItem(row, col, item);
        }
    }
}


QList<QList<char>> MainWindow::loadFieldFromFile(QString path) {
    QList<QList<char>> field;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return {};
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QList<char> row;
        for (QChar c: line) {
            if (c != '*' && c != '0') {
                toErrorScreen();
                return {};
            }
            row.push_back(c.toLatin1());
        }
        if (line.size() != 10)  {
            toErrorScreen();
            return {};
        }
        field.push_back(row);

    }
    if (field.size() != 10) return {};
    return field;
}

void MainWindow::toErrorScreen() {
    ui->stackedWidget->setCurrentIndex(2);
}

std::vector<std::vector<char>> MainWindow::toVector(const QList<QList<char>>& field) {
    std::vector<std::vector<char>> result;
    for (auto& row : field)
        result.push_back(std::vector<char>(row.begin(), row.end()));
    return result;
}

void MainWindow::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}


void MainWindow::on_backButton2_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

