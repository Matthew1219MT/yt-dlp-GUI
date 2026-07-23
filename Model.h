#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QString>
#include <QStringList>

// Holds application state. Knows nothing about widgets.
class Model : public QObject {
    Q_OBJECT

public:
    explicit Model(QObject *parent = nullptr);

    QStringList urls() const;
    void addUrl(const QString &url);
    void clear();

signals:
    void changed();   // emitted whenever state changes

private:
    QStringList m_urls;
};

#endif // MODEL_H