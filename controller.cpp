#include "controller.h"
#include "view.h"
#include "model.h"

#include <QDebug>
#include <string>
#include <QFileDialog>
#include <QLabel>
#include <QHBoxLayout>

Controller::Controller(View *view, Model *model, QObject *parent)
    : QObject(parent), m_view(view), m_model(model)
{
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

void Controller::onDownloadClicked()
{
    // Get download source
    std::string url = m_view->urlInput->text().toStdString();
    // Get download format 
    std::string format = m_view->mp3Radio->isChecked() ? "mp3" : "mp4";
    // Get download directory
    std::string dir = m_view->dirInput->text().toStdString();
    qDebug() << "[download] clicked, url =" << url << " , format: " << format << " , download to: " << dir;
}

void Controller::onAddToListClicked()
{
    qDebug() << "[addToList] clicked, url =" << m_view->urlInput->text();

    QListWidget *list = m_view->itemList;
    auto *item = new QListWidgetItem(m_view->itemList);

    // Get download source
    std::string url = m_view->urlInput->text().toStdString();
    // Get download format 
    std::string format = m_view->mp3Radio->isChecked() ? "mp3" : "mp4";
    if (url.empty()) { return; }
    std::string label_content = format + " | " + url;
    auto *label = new QLabel(QString::fromStdString(label_content));
    auto *delBtn = new QPushButton("X");
    delBtn->setFixedWidth(28);

    auto *row    = new QWidget;
    auto *lay = new QHBoxLayout(row);
    lay->setContentsMargins(4, 2, 4, 2);
    lay->addWidget(label);
    lay->addStretch();
    lay->addWidget(delBtn);

    item->setSizeHint(row->sizeHint());
    m_view->itemList->setItemWidget(item, row);

    QObject::connect(delBtn, &QPushButton::clicked, list, [list, item]() {
        delete list->takeItem(list->row(item));
    }, Qt::QueuedConnection);
}

void Controller::onBrowseClicked()
{
    qDebug() << "[browse] clicked";
    QString dir = QFileDialog::getExistingDirectory(
        nullptr,
        "Select download folder",
        // start where the field current points at
        m_view->dirInput->text() 
    );
    if (!dir.isEmpty()) {
        m_view->dirInput->setText(dir);
        // model->setDownloadDir(dir);
    }
}

void Controller::onListDownloadClicked()
{
    qDebug() << "[listDownload] clicked";
}

void Controller::onClearClicked()
{
    qDebug() << "[clear] clicked";
}

void Controller::onUrlEdited(const QString &text)
{
    qDebug() << "[urlInput] ->" << text;
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
    qDebug() << "[model] changed, count =" << m_model->urls().size();

    // Repaint the list from the model.
    m_view->itemList->clear();
    m_view->itemList->addItems(m_model->urls());
}