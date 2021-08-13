#include "CheckBoxVectorModel.h"

CheckBoxVectorModel::CheckBoxVectorModel(QWidget* parent) 
{
    
}

int CheckBoxVectorModel::rowCount(const QModelIndex &parent) const 
{
    return _Data.size();
}

QVariant CheckBoxVectorModel::data(const QModelIndex &index, int role) const 
{
    if (!index.isValid()) {
        return QVariant();
    }
    if (index.row() >= _Data.size()) {
        return QVariant();
    }
    if (role == Qt::DisplayRole | role == Qt::EditRole) {
        QVariant variant;
        variant.setValue(_Data[index.row()]);
        return variant;
    }
    return QVariant();
}

QVariant CheckBoxVectorModel::headerData(int section, Qt::Orientation orientation, int role) const 
{
    Q_ASSERT(false && "DONT CALL THIS FUNCTION!");
    return QVariant();
}

Qt::ItemFlags CheckBoxVectorModel::flags(const QModelIndex &index) const 
{
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }
    return QAbstractItemModel::flags(index)| Qt::ItemIsEditable;
}

bool CheckBoxVectorModel::setData(const QModelIndex &index, const QVariant &value, int role) 
{
    if (!index.isValid() || role == Qt::EditRole) {
        _Data[index.row()].IsChecked = value.value<CheckBoxModelData>().IsChecked;
        _Data[index.row()].DisplayString = value.value<CheckBoxModelData>().DisplayString;
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

bool CheckBoxVectorModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (uint32_t i = 0; i < count; i++) {
        _Data.insert(row, CheckBoxModelData());
    }
    endInsertRows();
    return true;
}

bool CheckBoxVectorModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (uint32_t i = 0; i < count; i++) {
        _Data.remove(row, count);
    }
    endInsertRows();
    return true;
}
