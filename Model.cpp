#include "model.h"

Model::Model(QObject *parent)
    : QObject(parent)
{
}

QStringList Model::urls() const
{
    return m_urls;
}

void Model::addUrl(const QString &url)
{
    m_urls.append(url);
    emit changed();
}

void Model::clear()
{
    m_urls.clear();
    emit changed();
}