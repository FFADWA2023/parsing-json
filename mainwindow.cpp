#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QFileDialog>
#include <cstdio>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect the "Print all" button to print all JSON content
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::printAllJSON);

    // Connect the "Parse JSON" button to recursively parse the JSON
    connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::parseJSON);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::printAllJSON()
{
    // Open a file dialog to allow the user to select a JSON file
    QString fileName = QFileDialog::getOpenFileName(this, "Open JSON File", "", "JSON Files (*.json)");
    
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file selected.");
        return;
    }

    // Load the JSON file
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Unable to open the JSON file.");
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    // Print the whole JSON content
    printf("JSON Content:\n%s\n", QString(data).toUtf8().constData());
}

void MainWindow::parseJSON()
{
    // Open a file dialog to allow the user to select a JSON file
    QString fileName = QFileDialog::getOpenFileName(this, "Open JSON File", "", "JSON Files (*.json)");
    
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Error", "No file selected.");
        return;
    }

    // Load the JSON file
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Unable to open the JSON file.");
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    file.close();

    if (!jsonDoc.isObject()) {
        QMessageBox::warning(this, "Error", "Invalid JSON format.");
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();

    // Parse and print each JSON object recursively
    printf("Parsing JSON:\n");
    parseObject(jsonObj);
}

void MainWindow::parseObject(const QJsonObject &jsonObj, int indentLevel)
{
    for (QJsonObject::const_iterator iter = jsonObj.begin(); iter != jsonObj.end(); ++iter) {
        QString key = iter.key();
        QJsonValue value = iter.value();

        // Print the key with indentation
        printIndent(indentLevel);
        printf("Key: %s\n", key.toUtf8().constData());

        // Check the value type and print it accordingly
        if (value.isObject()) {
            // Recursively parse the object
            parseObject(value.toObject(), indentLevel + 1);
        } else if (value.isArray()) {
            // Parse the array
            parseArray(value.toArray(), indentLevel + 1);
        } else if (value.isString()) {
            // Print string value
            printIndent(indentLevel + 1);
            printf("String Value: %s\n", value.toString().toUtf8().constData());
        } else if (value.isDouble()) {
            // Print numeric value
            printIndent(indentLevel + 1);
            printf("Numeric Value: %f\n", value.toDouble());
        } else if (value.isBool()) {
            // Print boolean value
            printIndent(indentLevel + 1);
            printf("Boolean Value: %s\n", value.toBool() ? "true" : "false");
        }
    }
}

void MainWindow::parseArray(const QJsonArray &jsonArray, int indentLevel)
{
    for (int i = 0; i < jsonArray.size(); ++i) {
        QJsonValue value = jsonArray[i];

        // Print index with indentation
        printIndent(indentLevel);
        printf("Array Index [%d]\n", i);

        if (value.isObject()) {
            // Recursively parse object in the array
            parseObject(value.toObject(), indentLevel + 1);
        } else if (value.isArray()) {
            // Recursively parse nested array
            parseArray(value.toArray(), indentLevel + 1);
        } else if (value.isString()) {
            printIndent(indentLevel + 1);
            printf("String Value: %s\n", value.toString().toUtf8().constData());
        } else if (value.isDouble()) {
            printIndent(indentLevel + 1);
            printf("Numeric Value: %f\n", value.toDouble());
        } else if (value.isBool()) {
            printIndent(indentLevel + 1);
            printf("Boolean Value: %s\n", value.toBool() ? "true" : "false");
        }
    }
}

void MainWindow::printIndent(int indentLevel)
{
    for (int i = 0; i < indentLevel; ++i) {
        printf("  ");  // Indent with two spaces
    }
}
