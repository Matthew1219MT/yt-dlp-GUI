#include "downloadmodel.h"
#include <QObject>
#include <QProcess>
#include <QCoreApplication>
#include <QFileInfo>

DownloadModel::DownloadModel(QObject *parent) : QObject(parent)
{
    m_proc.setProcessChannelMode(QProcess::MergedChannels);
    connect(&m_proc, &QProcess::readyReadStandardOutput,
            this, &DownloadModel::onStdout);
    connect(&m_proc, &QProcess::finished,
            this, &DownloadModel::onFinished);
    connect(&m_proc, &QProcess::errorOccurred, this, [this](QProcess::ProcessError)
            { emit failed(m_proc.errorString()); });
}

void DownloadModel::start(const QString &url, const QString &outDir)
{
    if (m_proc.state() != QProcess::NotRunning) return;

    const QString bin = binaryPath();
    if (!QFileInfo::exists(bin)) {
        emit failed("yt-dlp not found at " + bin);
        return;
    }

    QStringList args{"--newline", "-P", outDir, url};
    m_proc.start(bin, args);
}

void DownloadModel::cancel() { m_proc.kill(); }

void DownloadModel::onStdout()
{
    while (m_proc.canReadLine())
        emit progressText(QString::fromUtf8(m_proc.readLine()).trimmed());
}

void DownloadModel::onFinished(int code, QProcess::ExitStatus st)
{
    if (st == QProcess::CrashExit)
        emit failed("Process crashed");
    else
        emit finished(code);
}

QString DownloadModel::binaryPath() {
#ifdef Q_OS_WIN
    const QString name = "yt-dlp.exe";
#else
    const QString name = "yt-dlp";
#endif
    return QCoreApplication::applicationDirPath() + "/" + name;
}