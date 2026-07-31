#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <string>
#include <QStringList>

struct DownloadTask {
    std::string url;
    std::string format;
    std::string dir;
};

// Holds application state. Knows nothing about widgets.
class Model : public QObject
{
    Q_OBJECT

public:
    explicit Model(QObject *parent = nullptr);
    QStringList urls() const;
    QString download(const std::string &url, const std::string &format, const std::string& dir);
    void addTask(const std::string &url, const std::string &format, const std::string& dir);
    DownloadTask getTask(const int i);
    void removeTask(const int i);
    void clearTask();
    QString getVersion();
    QString update();
signals:
    void changed(); // emitted whenever state changes
private:
    std::vector<DownloadTask> tasks;
};

#endif // MODEL_H