#include "filemodel.h"

FileModel::FileModel(QObject *parent) : QAbstractListModel(parent)
{
    parseDirectory();
}

QVariant FileModel::data(const QModelIndex &index, int role) const
{
//    qDebug() << Q_FUNC_INFO << index << role;
    QVariant variant;
    if(index.row() >= 0 && index.row() < entries.size())
    {
        if(role == NameRole)
            variant = entries.at(index.row()).name;
        else if(role == ImageFileNameRole)
            variant = entries.at(index.row()).imageFileName;
        else if(role == LabelFileNameRole)
            variant = entries.at(index.row()).labelFileName;
        else if(role == BoxesModelRole)
            variant = QVariant::fromValue(entries.at(index.row()).boxesModel);
    }
    return variant;
}

void FileModel::setSourceDir(QString sourceDir)
{
    qDebug() << Q_FUNC_INFO << sourceDir;
#ifdef Q_OS_WIN //Windows
    sourceDir.remove(0, 8);
#else //Linux
    sourceDir.remove(0, 7);
#endif
    qDebug() << "using dir" << sourceDir;
    this->sourceDir = QDir(sourceDir).absolutePath();
    parseDirectory();
    qDebug() << "found" << entries.size() << "files";
    emit sourceDirChanged();
}

QVariant FileModel::getData(int index, int role)
{
    qDebug() << Q_FUNC_INFO << index << role;
    QVariant variant = data(this->index(index), role);
    qDebug() << variant;
    return variant;
}

BoxesModel *FileModel::getBoxesModel(int index)
{
    return entries.at(index).boxesModel;
}

void FileModel::addBox(int index, QRectF rect, int label)
{
    qDebug() << Q_FUNC_INFO << index << rect << label;
    if(index < 0 || index >= entries.size())
        qCritical() << "no such index";
    else
        entries.at(index).boxesModel->addBox(rect, label, false, "undef");
}

void FileModel::deleteBox(int entryIndex, int boxIndex)
{
    qDebug() << Q_FUNC_INFO << entryIndex << boxIndex;
    entries.at(entryIndex).boxesModel->deleteBox(boxIndex);
}

void FileModel::deleteAllBoxes(int entryIndex)
{
    qDebug() << Q_FUNC_INFO;
    entries.at(entryIndex).boxesModel->clear();
}

void FileModel::save(int entryIndex)
{
    qDebug() << Q_FUNC_INFO << entryIndex;
    if(entryIndex < 0 || entryIndex >= entries.size())
    {
        qCritical() << "bad index" << entryIndex;
        return;
    }
    FileEntry entry = entries.at(entryIndex);

    QFile labelFile(entry.labelFileName);
    if(labelFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&labelFile);
        for(int i = 0; i < entry.boxesModel->rowCount(); ++i)
        {
            QRectF rect= entry.boxesModel->data(entry.boxesModel->index(i), BoxesModel::BoxRoles::RectRole).toRectF();
            int label = entry.boxesModel->data(entry.boxesModel->index(i), BoxesModel::BoxRoles::LabelRole).toInt();

            QString labelString = QString::number(label);
            QString x = QString::number(rect.x() + rect.width() / 2);
            QString y = QString::number(rect.y() + rect.height() / 2);
            QString width = QString::number(rect.width());
            QString height = QString::number(rect.height());
            out <<  labelString << " " << x << " " << y << " " << width << " " << height << "\n";
        }
    }

    QFile angleLabelFile(entry.angleLabelFileName);
    if(angleLabelFile.open(QIODevice::WriteOnly | QIODevice::Text))

    {
        QTextStream out(&angleLabelFile);
        for(int i = 0; i < entry.boxesModel->rowCount(); ++i)
        {
            out << entry.boxesModel->data(entry.boxesModel->index(i), BoxesModel::BoxRoles::AngleTypeRole).toString();
            out << " ";
            out << entry.boxesModel->data(entry.boxesModel->index(i), BoxesModel::BoxRoles::AngleRole).toString();
            out << "\n";
        }
    }
}

void FileModel::parseDirectory()
{
    qDebug() << Q_FUNC_INFO;
    beginResetModel();
    entries.clear();

    QDir directory(sourceDir);
    QStringList images = directory.entryList(QStringList() << "*.jpg" << "*.JPG" << "*.png", QDir::Files);
    for(QString imageFileName: images)
    {
        QFileInfo imageFileInfo(sourceDir + "/" + imageFileName);
        QString labelFileName = imageFileInfo.absolutePath() + "/" + imageFileInfo.baseName() + ".txt";
        QString angleLabelFileName = imageFileInfo.absolutePath() + "/" + imageFileInfo.baseName() + "_angles.txt";
        qDebug() << imageFileName << labelFileName;
        QFileInfo labelFileInfo(labelFileName);
        QFileInfo angleLabelFileInfo(angleLabelFileName);

        if(!labelFileInfo.exists()) {
            qDebug() << "creating label file" << labelFileInfo;
            QFile file(labelFileName);
            file.open(QIODevice::WriteOnly);
            file.close();
        }

        QVector<bool> hasAngleVector;
        QVector<float> angleVector;
        QVector<QString> angleTypeVector;
        if(angleLabelFileInfo.exists()) {
            qDebug() << "found angle file" << angleLabelFileName;
            QFile angleLabelFile(angleLabelFileName);
            if(angleLabelFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&angleLabelFile);
                while(!in.atEnd()) {
                    QString line = in.readLine();
                    QStringList strings = line.split(" ");
                    qDebug() << strings;
                    if(strings.size() != 2)
                        throw std::runtime_error("badly formatted angle file" + angleLabelFileName.toStdString());
//                    hasAngleVector.append(strings.at(0).toInt());
                    if(strings.at(0) == "alpha" || strings.at(0) == "beta")
                    {
                        hasAngleVector.append(true);
                        angleTypeVector.append(strings.at(0));
                    }
                    else
                    {
                        hasAngleVector.append(false);
                        angleTypeVector.append("undef");
                    }
                    angleVector.append(strings.at(1).toFloat());
                }
            }
        }
        else
        {
            qDebug() << "no angle file" << angleLabelFileName;
        }

        qDebug() << hasAngleVector << angleVector;

        if(imageFileInfo.exists() && labelFileInfo.exists())
        {
            FileEntry entry;
            entry.name = imageFileName;
            entry.imageFileName = imageFileInfo.absoluteFilePath();
            entry.labelFileName = labelFileInfo.absoluteFilePath();
            entry.angleLabelFileName = angleLabelFileInfo.absoluteFilePath();

            QFile labelFile(labelFileName);
            if(labelFile.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream in(&labelFile);

                entry.boxesModel = new BoxesModel(this);
                int i = 0;
                while (!in.atEnd())
                {
                    QString line = in.readLine(); //read one line at a time
                    qDebug() << line;
                    QStringList strings = line.split(" ");
                    int label = strings.at(0).toInt();
                    double xCenter = strings.at(1).toDouble();
                    double yCenter = strings.at(2).toDouble();
                    double width = strings.at(3).toDouble();
                    double height = strings.at(4).toDouble();
                    double x = xCenter - width / 2;
                    double y = yCenter - height / 2;
                    QRectF rect = QRectF(x, y, width, height);
                    bool hasAngle = false;
                    float angle = 0;
                    QString angleType;
                    if(hasAngleVector.size() > i)
                    {
                        hasAngle = hasAngleVector.at(i);
                        angle = angleVector.at(i);
                        angleType = angleTypeVector.at(i);
                    }
                    entry.boxesModel->addBox(rect, label, hasAngle, angleType, angle);
                    ++i;
                }
            }

            entries.append(entry);
        }
    }

    endResetModel();
    qDebug() << Q_FUNC_INFO << "end";
}

QHash<int, QByteArray> FileModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[ImageFileNameRole] = "imageFileName";
    roles[LabelFileNameRole] = "labelFileName";
    roles[BoxesModelRole] = "boxesModel";
    return roles;
}
