#include "model.h"
#include <string>
#include <QDebug>
#include <QProcess>
#include <QCoreApplication>

Model::Model(QObject *parent)
    : QObject(parent)
{
}

QString Model::download(const std::string &url, const std::string &format, const std::string& dir)
{
    qDebug() << "[model]" << "Model download" << url << format;
    const QString exe = QCoreApplication::applicationDirPath() + "/yt-dlp.exe";
    QString cmd = QString("yt-dlp -t %1 -P \"%2\" %3").arg(format, dir, url);
    qDebug() << "[model]" << cmd;
    QProcess proc;
    proc.setProcessChannelMode(QProcess::MergedChannels);
    proc.startCommand(cmd);
    proc.waitForFinished(-1);
    QString output = QString::fromLocal8Bit(proc.readAll());
    return output;
}

void Model::addTask(const std::string &url, const std::string &format, const std::string& dir)
{
    tasks.push_back({url, format, dir});
}

DownloadTask Model::getTask(const int i)
{
    return tasks[i];
}

void Model::removeTask(const int i)
{
    tasks.erase(tasks.begin() + i);
    for (const auto& t : tasks) {
        qDebug() << t.url << t.format << t.dir;
    }
}

void Model::clearTask()
{
    tasks.clear();
}

QString Model::getVersion()
{
    const QString exe = QCoreApplication::applicationDirPath() + "/yt-dlp.exe";
    QString cmd = QString("yt-dlp --version");
    qDebug() << "[model]" << cmd;
    QProcess proc;
    proc.setProcessChannelMode(QProcess::MergedChannels);
    proc.startCommand(cmd);
    proc.waitForFinished(-1);
    QString output = QString::fromLocal8Bit(proc.readAll());
    qDebug() << output;
    return output;
}

QString Model::update()
{
    const QString exe = QCoreApplication::applicationDirPath() + "/yt-dlp.exe";
    QString cmd = QString("yt-dlp --update");
    qDebug() << "[model]" << cmd;
    QProcess proc;
    proc.setProcessChannelMode(QProcess::MergedChannels);
    proc.startCommand(cmd);
    proc.waitForFinished(-1);
    QString output = proc.readAllStandardOutput();
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    QString lastLine = lines.isEmpty() ? QString() : lines.last();
    return lastLine;
}