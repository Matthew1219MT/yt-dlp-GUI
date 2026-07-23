#include "view.h"
#include "ui_mainwindow.h"

View::View(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Bind each widget from the .ui file to a named handle.
    urlInput           = ui->urlLineEdit;
    dirInput           = ui->dirLineEdit;

    downloadButton     = ui->downloadButton;
    addToListButton    = ui->addToListButton;
    browseButton       = ui->browseButton;
    listDownloadButton = ui->listDownloadButton;
    clearButton        = ui->clearButton;

    mp3Radio           = ui->mp3RadioButton;
    mp4Radio           = ui->mp4RadioButton;

    itemList           = ui->itemListWidget;
}

View::~View()
{
    delete ui;
}