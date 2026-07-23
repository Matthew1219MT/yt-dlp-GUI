#ifndef VIEW_H
#define VIEW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QListWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// Owns the widgets. Knows nothing about yt-dlp or the Model.
class View : public QMainWindow {
    Q_OBJECT

public:
    explicit View(QWidget *parent = nullptr);
    ~View();

    // ---- Handles to every interactive widget --------------------
    // Assigned in the constructor. Use these instead of ui->...
    QLineEdit    *urlInput          = nullptr;
    QLineEdit    *dirInput          = nullptr;

    QPushButton  *downloadButton    = nullptr;
    QPushButton  *addToListButton   = nullptr;
    QPushButton  *browseButton      = nullptr;
    QPushButton  *listDownloadButton= nullptr;
    QPushButton  *clearButton       = nullptr;

    QRadioButton *mp3Radio          = nullptr;
    QRadioButton *mp4Radio          = nullptr;

    QListWidget  *itemList          = nullptr;
    // -------------------------------------------------------------

private:
    Ui::MainWindow *ui;
};

#endif // VIEW_H