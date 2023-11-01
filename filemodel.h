#pragma once

#include <QObject>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QAbstractListModel>

#include "boxesmodel.h"

class FileEntry {
public:
    QString name;
    QString imageFileName;
    QString labelFileName;
    QString angleLabelFileName;
    BoxesModel* boxesModel = nullptr;
};

class FileModel : public QAbstractListModel
{
    Q_OBJECT
public:

    Q_PROPERTY(QString sourceDir READ getSourceDir WRITE setSourceDir NOTIFY sourceDirChanged);
    Q_PROPERTY(int currentIndex READ getCurrentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged);
    Q_PROPERTY(int rowCount READ rowCount NOTIFY sourceDirChanged)

    enum FileEntryRoles {
        NameRole,
        ImageFileNameRole,
        LabelFileNameRole,
        BoxesModelRole
    };
    explicit FileModel(QObject *parent = nullptr);
    Q_INVOKABLE int rowCount(const QModelIndex & parent = QModelIndex()) const { return entries.length(); }
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    Q_INVOKABLE QString getSourceDir() { return sourceDir; }
    Q_INVOKABLE void setSourceDir(QString sourceDir);

    Q_INVOKABLE QVariant getData(int index, int role);
    Q_INVOKABLE BoxesModel* getBoxesModel(int index);
    Q_INVOKABLE void addBox(int index, QRectF rect, int label);
    Q_INVOKABLE void deleteBox(int entryIndex, int boxIndex);
    Q_INVOKABLE void deleteAllBoxes(int entryIndex);
    Q_INVOKABLE void save(int entryIndex);
    Q_INVOKABLE void setCurrentIndex(int index) { qDebug() << Q_FUNC_INFO << index; currentIndex = index; emit currentIndexChanged(); }
    Q_INVOKABLE int getCurrentIndex() { return currentIndex; }

signals:
    void sourceDirChanged();
    void currentIndexChanged();

private:
    QList<FileEntry> entries;
    QString sourceDir = "";
    void parseDirectory();
    int currentIndex{-1};

protected:
    QHash<int, QByteArray> roleNames() const;

};
