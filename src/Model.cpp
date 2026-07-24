#include "model.h"
#include <string>
#include <QDebug>
#include <QProcess>
#include <QCoreApplication>

Model::Model(QObject *parent)
    : QObject(parent)
{
}

void Model::download(const std::string &url, const std::string &format, const std::string& dir)
{
    qDebug() << "[download]" << "Model download" << url << format;
    const QString exe = QCoreApplication::applicationDirPath() + "/yt-dlp.exe";
    QString cmd = QString("yt-dlp -t %1 -P \"%2\" %3").arg(format, dir, url);
    qDebug() << "[download]" << cmd;
    QProcess proc;
    proc.setProcessChannelMode(QProcess::MergedChannels);
    proc.startCommand(cmd);
    proc.waitForFinished(-1);
    QString::fromLocal8Bit(proc.readAll());
    qDebug() << QString::fromLocal8Bit(proc.readAll());;
}