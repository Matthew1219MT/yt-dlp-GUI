#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>

class View;
class Model;

// Wires View <-> Model. All application logic lives here.
class Controller : public QObject {
    Q_OBJECT

public:
    Controller(View *view, Model *model, QObject *parent = nullptr);

private slots:
    // --- one slot per interactive widget -------------------------
    void onDownloadClicked();
    void onAddToListClicked();
    void onBrowseClicked();
    void onListDownloadClicked();
    void onClearClicked();

    void onUrlEdited(const QString &text);
    void onDirEdited(const QString &text);

    void onFormatChanged();
    void onListSelectionChanged(int row);

    // --- model -> view -------------------------------------------
    void onModelChanged();
    // -------------------------------------------------------------

private:
    View  *m_view;
    Model *m_model;
};

#endif // CONTROLLER_H