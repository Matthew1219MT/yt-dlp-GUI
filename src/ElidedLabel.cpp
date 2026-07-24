#include "ElidedLabel.h"
#include <QResizeEvent>

ElidedLabel::ElidedLabel(const QString &text, QWidget *parent)
    : QLabel(parent), m_full(text)
{
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
    setToolTip(text);
}

void ElidedLabel::resizeEvent(QResizeEvent *e)
{
    QLabel::resizeEvent(e);
    setText(fontMetrics().elidedText(m_full, Qt::ElideMiddle, width()));
}