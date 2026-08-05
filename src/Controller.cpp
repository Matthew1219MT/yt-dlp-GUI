#include "controller.h"
#include "view.h"
#include "model.h"
#include "ElidedLabel.h"

#include <QDebug>
#include <string>
#include <QFileDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QCoreApplication>
#include <QMessageBox>

Controller::Controller(View *view, Model *model, QObject *parent)
    : QObject(parent), m_view(view), m_model(model)
{
    // ---- Get yt-dlp version on load -----------------------------
    QString version = m_model->getVersion();
    if (version.toStdString().empty()) {
        QMessageBox::warning(nullptr, "Failure Report", "Cannot find yt-dlp, please make sure you have it in the same folder with this software");
        setUI(false);
    }
    m_view->versionLabel->setText("yt-dlp's version: " + version);
    // ---- View -> Controller -------------------------------------
    connect(m_view->downloadButton, &QPushButton::clicked,
            this, &Controller::onDownloadClicked);

    connect(m_view->addToListButton, &QPushButton::clicked,
            this, &Controller::onAddToListClicked);

    connect(m_view->browseButton, &QPushButton::clicked,
            this, &Controller::onBrowseClicked);

    connect(m_view->listDownloadButton, &QPushButton::clicked,
            this, &Controller::onListDownloadClicked);

    connect(m_view->clearButton, &QPushButton::clicked,
            this, &Controller::onClearClicked);

    connect(m_view->updateButton, &QPushButton::clicked,
            this, &Controller::onUpdateClicked);

    connect(m_view->urlInput, &QLineEdit::textChanged,
            this, &Controller::onUrlEdited);

    connect(m_view->dirInput, &QLineEdit::textChanged,
            this, &Controller::onDirEdited);

    connect(m_view->mp3Radio, &QRadioButton::toggled,
            this, &Controller::onFormatChanged);

    connect(m_view->itemList, &QListWidget::currentRowChanged,
            this, &Controller::onListSelectionChanged);

    // ---- Model -> Controller ------------------------------------
    connect(m_model, &Model::changed,
            this, &Controller::onModelChanged);
}

// =================================================================
// Stubs. Fill these in one at a time.
// =================================================================

void Controller::setUI(const bool flag)
{
    m_view->urlInput->setEnabled(flag);
    m_view->dirInput->setEnabled(flag);
    m_view->browseButton->setEnabled(flag);
    m_view->downloadButton->setEnabled(flag);
    m_view->addToListButton->setEnabled(flag);
    m_view->listDownloadButton->setEnabled(flag);
    m_view->clearButton->setEnabled(flag);
    m_view->mp3Radio->setEnabled(flag);
    m_view->mp4Radio->setEnabled(flag);
    m_view->updateButton->setEnabled(flag);
    for (int i = 0; i < m_view->itemList->count(); ++i) {
        QWidget* rowWidget = m_view->itemList->itemWidget(m_view->itemList->item(i));
        QPushButton* delBtn = rowWidget->findChild<QPushButton*>();
        delBtn->setEnabled(flag);
    }
    QCoreApplication::processEvents();
}

void Controller::onDownloadClicked()
{
    setUI(false);
    // Get download source
    std::string url = m_view->urlInput->text().toStdString();
    if (url.empty())
    {
        setUI(true);
        return;
    }
    // Get download format
    std::string format = m_view->mp3Radio->isChecked() ? "mp3" : "mp4";
    // Get download directory
    std::string dir = m_view->dirInput->text().toStdString();
    qDebug() << "[download] clicked, url =" << url << " , format: " << format << " , download to: " << dir;
    QString output = m_model->download(url, format, dir);
    qDebug() << "[download]" << output; 
    if (output.contains("[download] 100%", Qt::CaseInsensitive)) {
        // Clear input field
        m_view->urlInput->clear();
        QMessageBox::information(nullptr, "Download Result", "Download successful");
    } else {
        QMessageBox::warning(nullptr, "Download Result", output);
    }
    setUI(true);
}

void Controller::onAddToListClicked()
{
    qDebug() << "[addToList] clicked, url =" << m_view->urlInput->text();

    // Get download source
    std::string url = m_view->urlInput->text().toStdString();
    if (url.empty())
    {
        return;
    }
    // Get download format
    std::string format = m_view->mp3Radio->isChecked() ? "mp3" : "mp4";
    // Get download directory
    std::string dir = m_view->dirInput->text().toStdString();
    m_model->addTask(url, format, dir);

    // Create new list item for the list
    QListWidget *list = m_view->itemList;
    auto *item = new QListWidgetItem(m_view->itemList);
    // Combine format and url as label text
    std::string label_content = format + " | " + url;
    auto *label = new ElidedLabel(QString::fromStdString(label_content));
    auto *delBtn = new QPushButton("X");
    delBtn->setFixedWidth(28);
    auto *row = new QWidget;
    auto *lay = new QHBoxLayout(row);
    lay->setContentsMargins(4, 2, 4, 2);
    lay->addWidget(label, 1);
    lay->addWidget(delBtn);
    item->setSizeHint(QSize(0, row->sizeHint().height()));
    m_view->itemList->setItemWidget(item, row);
    QObject::connect(delBtn, &QPushButton::clicked, list, [this, list, item](){ 
        m_model->removeTask(list->row(item));
        delete list->takeItem(list->row(item));
    }, Qt::QueuedConnection);
    // Clear input field
    m_view->urlInput->clear();
}

void Controller::onBrowseClicked()
{
    qDebug() << "[browse] clicked";
    QString dir = QFileDialog::getExistingDirectory(
        nullptr,
        "Select download folder",
        // start where the field current points at
        m_view->dirInput->text());
    if (!dir.isEmpty())
    {
        m_view->dirInput->setText(dir);
        // model->setDownloadDir(dir);
    }
}

void Controller::onListDownloadClicked()
{
    int success = 0;
    int failed = 0;
    setUI(false);
    
    qDebug() << "[listDownload] clicked";
    // Get download directory
    const std::string dir = m_view->dirInput->text().toStdString();
    QListWidget *list = m_view->itemList;
    // Set label in all tasks to pending
    for (int i = 0; i < list->count(); ++i)
    {
        QListWidgetItem *item = list->item(i);
        QWidget *row = list->itemWidget(item);
        auto *label = row->findChild<ElidedLabel *>();
        const QString label_text = label->text();
        label->setText("Pending | " + label_text);
    }
    for (int i = 0; i < list->count(); ++i)
    {
        QListWidgetItem *item = list->item(i);
        const int item_index = list->row(item);
        // Set the label of the current task to downloading
        QWidget *row = list->itemWidget(item);
        auto *label = row->findChild<ElidedLabel *>();
        QString label_text = label->text();
        QString label_text_clean = QString::fromStdString(label_text.toStdString().erase(0, 10));
        label->setText("Downloading | " + label_text_clean);
        QCoreApplication::processEvents();
        
        // Get the config of current task from model
        DownloadTask task = m_model->getTask(item_index);
        qDebug() << task.format << task.url << task.dir;
        QString output = m_model->download(task.url, task.format, task.dir);
        qDebug() << "[download]" << output; 
        if (output.contains("[download] 100%", Qt::CaseInsensitive)) {
            label->setText("Completed | " + label_text_clean);
            success += 1;
        } else {
            label->setText("Failed | " + label_text_clean);
            failed += 1;
        }
        QCoreApplication::processEvents();
    }
    std::ostringstream oss;
    oss << "Successfully downloaded " << success << " files. \n Failed to download " << failed << " files.";
    QMessageBox::information(nullptr, "Download Result", QString::fromStdString(oss.str()));
    m_model->clearTask();
    m_view->itemList->clear();
    setUI(true);
}

void Controller::onClearClicked()
{
    auto reply = QMessageBox::question(nullptr, "Confirm Clear",
        "Clear all items in the list?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        qDebug() << "[clear] clicked";
        m_model->clearTask();
        m_view->itemList->clear();
    }
}

void Controller::onUpdateClicked()
{
    setUI(false);
    qDebug() << "[Controller] Updating yt-dlp...";
    QString update = m_model->update();
    QMessageBox::information(nullptr, "Update Result", update);
    QString version = m_model->getVersion();
    m_view->versionLabel->setText("yt-dlp's version: " + version);
    setUI(true);
}

void Controller::onUrlEdited(const QString &text)
{
    //qDebug() << "[urlInput] ->" << text;
}

void Controller::onDirEdited(const QString &text)
{
    qDebug() << "[dirInput] ->" << text;
}

void Controller::onFormatChanged()
{
    qDebug() << "[format] mp3 =" << m_view->mp3Radio->isChecked();
}

void Controller::onListSelectionChanged(int row)
{
    qDebug() << "[list] selected row" << row;
}

void Controller::onModelChanged()
{
    qDebug() << "[model] changed";
}