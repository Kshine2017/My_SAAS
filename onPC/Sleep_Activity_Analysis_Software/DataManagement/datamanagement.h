#ifndef DATAMANAGEMENT_H
#define DATAMANAGEMENT_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include "tablemodel.h"
#include "buttondelegate.h"
#include <QTableView>
/*
类描述：界面类《数据管理》，主要功能是管理数据。
关键接口：无
*/
class DataManagement : public QWidget
{
    Q_OBJECT
public:
    explicit DataManagement(QWidget *parent = 0,int WWW=0,int HHH=0);
    ~DataManagement();
signals:
public slots:
    void slot_tableclick(QModelIndex index);    //表格点击
    void slot_btn_Serach();
    void slot_auto_sort(int cmbxindex);
private:
    int thisWidth;              //屏宽
    int thisHeight;             //屏高
    //top 顶部
    QLabel* pageName;           //页面名称：数据分析
    //sec 筛选条件
    QComboBox* cmbx_first;          //数据状态
    QComboBox* cmbx_search_criteria;//搜索条件
    QLineEdit* le_search_insert;    //输入内容
    QPushButton* btn_search;        //搜索按钮
    QComboBox* cmbx_Default_sort;   //排序
    //table
    QTableView* tableView;
    QTableWidget* tablewidget;      //表格
    TableModel *m_model;
    ButtonDelegate *m_buttonDelegate;
    //bottom
    //翻页条

    //布局
    QVBoxLayout* ly_V_all;
    QHBoxLayout* ly_H_Top;
    QHBoxLayout* ly_H_Sec;
    QHBoxLayout* ly_H_Table;

    //初始化
    void init_PageTopInfo();
    void init_PageSecInfo();
    void init_PageTableInfo();
    void init_PageTableInfo_new();
    void init_PageAllLayout();
    void testTableData();           //写入测试数据
    void init_TableViewData();
};

#endif // DATAMANAGEMENT_H
