#pragma once

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>

class FileHandler : public QObject
{
    Q_OBJECT
public:
    explicit FileHandler(QObject *parent = nullptr);
    Q_INVOKABLE void copyGood(QString imageFileName);
    Q_INVOKABLE void copyBad(QString imageFileName);

signals:

private:
    void makeSubDirectories(QString basePath);
};
