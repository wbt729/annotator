#pragma once
#include <QObject>
#include <QDebug>
#include <QRect>
#include <QAbstractListModel>

class Box {
public:
    QRectF rect;
    int label;
    bool hasAngle;
    float angle;
    QString angleType = "undef";
};

class BoxesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum BoxRoles {
        RectRole,
        LabelRole,
        HasAngleRole,
        AngleRole,
        AngleTypeRole
    };

    Q_PROPERTY(int rowCount READ rowCount NOTIFY rowCountChanged)

    explicit BoxesModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const { return boxes.length(); }
    void addBox(QRectF rect, int label, bool hasAngle, QString angleType, float angle = 0);
    Q_INVOKABLE void deleteBox(int index);
    Q_INVOKABLE void setAngle(int index, float angle, QString angleType);
    void clear();

signals:
    void rowCountChanged();

private:
    QList<Box> boxes;

protected:
    QHash<int, QByteArray> roleNames() const;
};

Q_DECLARE_METATYPE(BoxesModel*)
