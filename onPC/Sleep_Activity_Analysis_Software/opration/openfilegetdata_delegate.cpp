#include "openfilegetdata_delegate.h"

OpenFileGetData_Delegate::OpenFileGetData_Delegate(QWidget *parent) : QStyledItemDelegate(parent)
{

}

QWidget *OpenFileGetData_Delegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(parent)
    Q_UNUSED(option)
    Q_UNUSED(index)
    return NULL;//editor;
}

void OpenFileGetData_Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    bool checked = index.model()->data(index, Qt::DisplayRole).toBool();

    if(index.column() == 0){
        QStyleOptionButton checkBoxStyleOption;
        checkBoxStyleOption.state |= QStyle::State_Enabled;
        //            checkBoxStyleOption.state |= checked? QStyle::State_On : QStyle::State_Off;
        //            checkBoxStyleOption.rect = CheckBoxRect(option);
        checkBoxStyleOption.rect = QRect(option.rect.left()+7,option.rect.top(),option.rect.width(),option.rect.height());
        if (checked)
        {
            checkBoxStyleOption.iconSize = QSize(30,30);
            checkBoxStyleOption.icon = QIcon(":/picture/operation/checked.png");
            // option.state = QStyle::State_On;
        }
        else
        {
            checkBoxStyleOption.iconSize = QSize(30,30);
            checkBoxStyleOption.icon = QIcon(":/picture/operation/unchecked.png");
            // option.state = QStyle::State_Off;
        }

        QApplication::style()->drawControl(QStyle::CE_CheckBoxLabel,&checkBoxStyleOption,painter);
    }else{
        QStyledItemDelegate::paint(painter, option, index);
    }
}

void OpenFileGetData_Delegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(editor)
    Q_UNUSED(option)
    Q_UNUSED(index)
//    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
//    checkBox->setGeometry(0,0,40,40);
}

bool OpenFileGetData_Delegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if(index.column() == 0) //如果是第一列
    {
        //如果鼠标 松开 或者 双击
        if((event->type() == QEvent::MouseButtonRelease) ||(event->type() == QEvent::MouseButtonDblClick))
        {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);

            if(mouseEvent->button() != Qt::LeftButton || !option.rect.contains(mouseEvent->pos()))
            {
                return true;
            }
            if(event->type() == QEvent::MouseButtonDblClick)
            {
                return true;
            }
        }
        else if(event->type() == QEvent::KeyPress)
        {
            if(static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space && static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select)
            {
                return false;
            }
        }
        else
        {
            return false;
        }

        bool checked = index.model()->data(index, Qt::DisplayRole).toBool();
        return model->setData(index, !checked, Qt::EditRole);

    }
    else //如果不是 第一列
    {
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }
}

void OpenFileGetData_Delegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    int value = index.model()->data(index, Qt::DisplayRole).toInt();

    QCheckBox *checkBox = static_cast<QCheckBox*>(editor);
    if(value == 1)
        checkBox->setCheckState(Qt::Checked);
    else
        checkBox->setCheckState(Qt::Unchecked);
}
