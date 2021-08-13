#include "CheckBoxDelegate.h"

#include <QCheckBox>
#include <CheckBoxVectorModel.h>

CheckBoxDelegate::CheckBoxDelegate(QWidget* parent)
    : QItemDelegate(parent)
{
    
}

QWidget* CheckBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const 
{
    QCheckBox* checkBox = new QCheckBox(parent);
    return checkBox;
}

void CheckBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const 
{
    QCheckBox* checkBox = reinterpret_cast<QCheckBox*>(editor);
    checkBox->setCheckState(index.data().value<CheckBoxModelData>().IsChecked ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
}

void CheckBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const 
{
    QCheckBox* checkBox = reinterpret_cast<QCheckBox*>(editor);
    QVariant variant;
    variant.setValue(CheckBoxModelData{.IsChecked = checkBox->isChecked(), .DisplayString = checkBox->text()});
    model->setData(index, variant);
}

void CheckBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const 
{
    editor->setGeometry(option.rect);
}
