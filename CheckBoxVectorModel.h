#pragma once

#include <QAbstractItemModel>

struct CheckBoxModelData
{
    QString DisplayString;
    bool IsChecked;
};

Q_DECLARE_METATYPE(CheckBoxModelData);

class CheckBoxVectorModel : public QAbstractItemModel
{
    Q_OBJECT
public:

    CheckBoxVectorModel(QWidget* parent = nullptr);


    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override { return hasIndex(row, column) ? createIndex(row, column) : QModelIndex(); }
    QModelIndex parent(const QModelIndex &child) const override { return QModelIndex(); }
    int columnCount(const QModelIndex &parent = QModelIndex()) const override { return 0; };

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    QVector<CheckBoxModelData> _Data;
};

