#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <string>
#include <QStringList>

// Holds application state. Knows nothing about widgets.
class Model : public QObject
{
    Q_OBJECT

public:
    explicit Model(QObject *parent = nullptr);

    QStringList urls() const;
    void download(const std::string &url, const std::string &format, const std::string& dir);

signals:
    void changed(); // emitted whenever state changes
};

#endif // MODEL_H