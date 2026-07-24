#include <QObject>
#include <QProcess>

class DownloadModel : public QObject
{
    Q_OBJECT
public:
    explicit DownloadModel(QObject *parent = nullptr);
    void start(const QString &url, const QString &outDir);
    void cancel();

signals:
    void progressText(const QString &line); // raw stdout line
    void finished(int exitCode);
    void failed(const QString &reason);

private slots:
    void onStdout();
    void onFinished(int code, QProcess::ExitStatus status);

private:
    QProcess m_proc;
    static QString binaryPath();
    static constexpr const char *kBinary = "yt-dlp.exe";
};