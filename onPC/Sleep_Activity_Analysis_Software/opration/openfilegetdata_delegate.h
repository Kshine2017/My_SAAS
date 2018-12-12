#ifndef OPENFILEGETDATA_DELEGATE_H
#define OPENFILEGETDATA_DELEGATE_H

#include <QWidget>
#include <QStyledItemDelegate>
#include <QAbstractTableModel>
#include <QCheckBox>
#include <QVariant>
#include <QMap>
#include <QStyledItemDelegate>
#include <QPushButton>
#include <QEvent>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
/*
类描述：功能类，Checkbox代理
关键接口：
        无
*/
class OpenFileGetData_Delegate: public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit OpenFileGetData_Delegate(QWidget *parent = 0);
protected:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index);
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
private:

  QMap<QModelIndex, QStyleOptionButton*> Btns;

signals:
  /// \brief 按钮点击信号
  ///
  /// \param QModelIndex   包含点击位置信息(表的行列)
  /// \return       void
  /// \see 无
  void CheckBoxClicked(const QModelIndex&);
};

#endif // OPENFILEGETDATA_DELEGATE_H
