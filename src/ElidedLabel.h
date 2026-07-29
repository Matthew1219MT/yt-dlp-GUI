#ifndef ELIDEDLABEL_H
#define ELIDEDLABEL_H

#include <QLabel>

class ElidedLabel : public QLabel
{
    Q_OBJECT   
public:
    explicit ElidedLabel(const QString &text, QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *e) override;

private:
    QString m_full;
};

#endif