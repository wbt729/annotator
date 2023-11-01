#include "boxesmodel.h"

BoxesModel::BoxesModel(QObject *parent) : QAbstractListModel(parent)
{
    qRegisterMetaType<BoxesModel*>();
}

QVariant BoxesModel::data(const QModelIndex &index, int role) const
{
//    qDebug() << Q_FUNC_INFO << index << role;
    QVariant variant;
    if(role == RectRole)
        variant = boxes.at(index.row()).rect;
    else if(role == LabelRole)
        variant = boxes.at(index.row()).label;
    else if(role == HasAngleRole)
        variant = boxes.at(index.row()).hasAngle;
    else if(role == AngleRole)
        variant = boxes.at(index.row()).angle;
    else if(role == AngleTypeRole)
        variant = boxes.at(index.row()).angleType;
    return variant;
}

void BoxesModel::addBox(QRectF rect, int label, bool hasAngle, QString angleType, float angle)
{
    qDebug() << Q_FUNC_INFO << rect << label;
    beginResetModel();
    Box box;
    box.rect = rect;
    box.label = label;
    box.hasAngle = hasAngle;
    box.angleType = angleType;
    box.angle = angle;
    boxes.append(box);
    endResetModel();
    emit rowCountChanged();
}

void BoxesModel::deleteBox(int index)
{
    beginResetModel();
    boxes.removeAt(index);
    endResetModel();
    emit rowCountChanged();
}

void BoxesModel::setAngle(int index, float angle, QString angleType)
{
    qDebug() << Q_FUNC_INFO << index << angle;
    if(index >= 0 && index < boxes.size())
    {
        boxes[index].hasAngle = true;
        boxes[index].angle = angle;
        boxes[index].angleType = angleType;
        emit dataChanged(this->index(index), this->index(index));
    }
}

void BoxesModel::clear()
{
    qDebug() << Q_FUNC_INFO;
    beginResetModel();
    boxes.clear();
    endResetModel();
}

QHash<int, QByteArray> BoxesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[RectRole] = "rect";
    roles[LabelRole] = "label";
    roles[HasAngleRole] = "hasAngle";
    roles[AngleRole] = "angle";
    roles[AngleTypeRole] = "angleType";
    return roles;
}
