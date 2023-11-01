#include "filehandler.h"

FileHandler::FileHandler(QObject *parent) : QObject(parent)
{

}

void FileHandler::copyGood(QString imageFileName)
{
    qDebug() << Q_FUNC_INFO << imageFileName;
    QFileInfo fileInfo(imageFileName);
    QString basePath = fileInfo.absolutePath();
    makeSubDirectories(basePath);

    QString targetImage = QDir::cleanPath(basePath + QDir::separator() + "good" + QDir::separator() + fileInfo.fileName());
    QFile::copy(imageFileName, targetImage);

    QString textFileName = fileInfo.baseName() + ".txt";
    QString sourceAnnotation = QDir::cleanPath(basePath + QDir::separator() + textFileName);
    QString targetAnnotation = QDir::cleanPath(basePath + QDir::separator() + "good" + QDir::separator() + textFileName);
    QFile::copy(sourceAnnotation, targetAnnotation);
}

void FileHandler::copyBad(QString imageFileName)
{
    qDebug() << Q_FUNC_INFO << imageFileName;
    QFileInfo fileInfo(imageFileName);
    QString basePath = fileInfo.absolutePath();
    makeSubDirectories(basePath);

    QString targetImage = QDir::cleanPath(basePath + QDir::separator() + "bad" + QDir::separator() + fileInfo.fileName());
    QFile::copy(imageFileName, targetImage);

    QString textFileName = fileInfo.baseName() + ".txt";
    QString sourceAnnotation = QDir::cleanPath(basePath + QDir::separator() + textFileName);
    QString targetAnnotation = QDir::cleanPath(basePath + QDir::separator() + "bad" + QDir::separator() + textFileName);
    QFile::copy(sourceAnnotation, targetAnnotation);
}

void FileHandler::makeSubDirectories(QString basePath)
{
    qDebug() << Q_FUNC_INFO << basePath;
    QString goodDir = QDir::cleanPath(basePath + QDir::separator() + "good");
    QString badDir = QDir::cleanPath(basePath + QDir::separator() + "bad");
    if(!QDir(goodDir).exists())
        QDir().mkdir(goodDir);
    if(!QDir(badDir).exists())
        QDir().mkdir(badDir);
}
