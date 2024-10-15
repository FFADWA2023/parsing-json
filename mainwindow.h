#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>
#include <QJsonArray>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void printAllJSON();              // Declaration for printAllJSON
    void parseJSON();                 // Declaration for parseJSON
    void parseObject(const QJsonObject &jsonObj, int indentLevel = 0);  // Declaration for parseObject
    void parseArray(const QJsonArray &jsonArray, int indentLevel = 0);  // Declaration for parseArray
    void printIndent(int indentLevel);  // Declaration for printIndent

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
