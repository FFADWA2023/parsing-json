#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QFileDialog>
#include <cstdio>  // Pour utiliser printf

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connexion du bouton "Convert to CSV" au slot qui lit le fichier JSON
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::parseJSON);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parseJSON()
{
    // Ouvrir une boîte de dialogue pour permettre à l'utilisateur de sélectionner un fichier JSON
    QString fileName = QFileDialog::getOpenFileName(this, "Open JSON File", "", "JSON Files (*.json)");
    
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Aucun fichier sélectionné.");
        return;
    }

    // Charger le fichier JSON
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Erreur", "Impossible d'ouvrir le fichier JSON.");
        return;
    }

    QByteArray data = file.readAll();
    QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
    file.close();

    if (!jsonDoc.isObject()) {
        QMessageBox::warning(this, "Erreur", "Le fichier JSON n'est pas valide.");
        return;
    }

    QJsonObject jsonObj = jsonDoc.object();
    QJsonArray optionsArray = jsonObj["radioOptions"].toArray();

    // Afficher une valeur du JSON dans la console (par exemple, la première option)
    if (optionsArray.size() > 0) {
        QJsonObject firstOption = optionsArray[0].toObject();
        QString label = firstOption["label"].toString();
        QString value = firstOption["value"].toString();

        // Affichage dans la console
        printf("Option: %s, Valeur: %s\n", label.toUtf8().constData(), value.toUtf8().constData());
    } else {
        printf("Aucune option trouvée dans le fichier JSON.\n");
    }
}
