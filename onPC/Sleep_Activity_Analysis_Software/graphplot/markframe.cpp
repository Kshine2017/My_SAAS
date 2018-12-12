#include "markframe.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>     //画笔

#define NO_FOUND -223
int MarkFrame::X_isInOneDuan(int the_X)
{
    QVector<struct_Duan>::iterator it;
    int off_V=0;
    for(it=DUAN.begin();it!=DUAN.end()-1;it++)
    {
        if(the_X >it->startX && the_X <it->endX)
        {
          return off_V;
        }
        off_V++;
    }
    off_V=NO_FOUND;//不在段中
    return off_V;
}

int MarkFrame::DeleteMidDuan_ByLast()
{
    QVector<struct_Duan>::iterator it;
    int counter=0;
    int last_S_X=DUAN.last().startX;
    int last_E_X=DUAN.last().endX;
    if(last_E_X<last_S_X)//删除操作
    {
        int tem =last_S_X;
        last_S_X=last_E_X;
        last_E_X=tem;
    }
    for(it=DUAN.begin();it!=DUAN.end()-1;it++)
    {
        if(it->startX>last_S_X && it->endX <last_E_X)
        {
          counter++;
          DUAN.erase(it);//遍历过程中，不要进行删除操作。（有两种方案，一种是直接记录起始删除点，一种是it停一轮不动）
          it--;//采用方案2
        }
    }
    return counter;
}

MarkFrame::MarkFrame(QWidget *parent):QFrame(parent)
{
    Press_Flag=0;
    Move_Flag=0;
    //counter_DUAN=0;
    temdata={0,10};
    counter=0;
    DUAN.clear();
   // DUAN.push_back(temdata);
    setStyleSheet("background-color:white;");//transparent不可以透明，因为檫除会无效

}

MarkFrame::~MarkFrame()
{
    qDebug()<<"析构---markframe";
}

void MarkFrame::clearData()
{
    DUAN.clear();
    update();
}

void MarkFrame::autoMaticMark(const QVector<struct_Duan> &autoDUAN)
{
    DUAN=autoDUAN;
    update();
}


void MarkFrame::mousePressEvent(QMouseEvent *ev)
{
    counter=0;
    qDebug()<<"鼠标按下"<<"xy("<<ev->x()<<","<<ev->y()<<"), globalxy("<<ev->globalX()<<","<<ev->globalY()<<")";
    Press_Flag=1;
    temdata.startX=ev->x();//得到最新的段的起始点
    qDebug()<<"鼠标按下"<<"temdata.startX="<<temdata.startX<<"temdata.endX="<<temdata.endX<<";";

}
//鼠标按住移动
void MarkFrame::mouseMoveEvent(QMouseEvent *ev)
{
    counter++;
    //qDebug()<<"move"<<"xy("<<ev->x()<<","<<ev->y()<<"), globalxy("<<ev->globalX()<<","<<ev->globalY()<<")";
    //if(Move_Flag==1)DUAN.pop_back();//移动过程中,清除末尾临时段//只有第一次绘制正常//猜测原因，pop之后，立即push可能会响应不及时
    //if(Move_Flag==1)DUAN.erase(DUAN.end());//段错误
    temdata.endX=ev->x();//得到最新的段的结束点
  //  qDebug()<<"移动"<<"temdata.startX="<<temdata.startX<<"temdata.endX="<<temdata.endX<<";";
    DUAN.push_back(temdata);//更新末尾段
    update();
    Move_Flag=1;//正 在移动
}


void MarkFrame::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug()<<"鼠标松开"<<"xy("<<ev->x()<<","<<ev->y()<<"), globalxy("<<ev->globalX()<<","<<ev->globalY()<<")";
    Press_Flag=0;//松开标志
    Move_Flag=0;//停止移动
    if(DUAN.size()<=0)return;
    qDebug()<<"鼠标松开"<<"temdata.startX="<<temdata.startX<<"temdata.endX="<<temdata.endX<<";";
    qDebug()<<"数据未处理前（段个数）："<<DUAN.size()<<"临时数据的个数："<<counter-1;

    //清理临时段
    QVector<struct_Duan>::iterator it;
//    struct_Duan DUAN_XX = DUAN[0];
    if(counter-1>0)//临时数据的个数
    {
        it=DUAN.begin();
        DUAN.erase(it+DUAN.size()-counter,DUAN.end()-1);//新增的这一些（-1）全删
    }

    //将最后一段与其他的合并
    if(DUAN.size()>1)
    {
        //是否中间包括-------------------------------
        qDebug()<<"清理中间包括："<<DeleteMidDuan_ByLast();
        it =DUAN.begin();//用于删除段点
        //-----------------------------------------
        int S_X=DUAN.last().startX;
        int E_X=DUAN.last().endX;
        int off_V_S = X_isInOneDuan(S_X);//存放 起点所在的段 的地址
        int off_V_E = X_isInOneDuan(E_X);//存放 起点所在的段 的地址
        if(S_X < E_X)//从左向右，画绿条
        {
           if(off_V_S == NO_FOUND && off_V_E ==NO_FOUND)//新段
           {
               //保留最后一段

               //检查中间是否有其他段，有则删除
           }
           else if(off_V_S !=NO_FOUND &&off_V_E ==NO_FOUND)
           {
               //头在尾不在
               qDebug()<<"右扩";
               (it+off_V_S)->endX=E_X;//头所在的段的 尾节点改成EX
               //DUAN.pop_back();//不及时
               DUAN.erase(&DUAN.last());//删除
           }
           else if(off_V_S ==NO_FOUND &&off_V_E !=NO_FOUND)
           {
               //头不在，尾在
               qDebug()<<"左扩";
               (it+off_V_E)->startX=S_X;
               DUAN.erase(&DUAN.last());//删除
           }
           else if(off_V_S !=NO_FOUND &&off_V_E !=NO_FOUND)
           {
               if(off_V_S==off_V_E)//在同一段里面（包含），删除即可
               {
                   DUAN.erase(&DUAN.last());//删除
               }
               else//在不同的里面(删除后者和末尾)
               {
                   qDebug()<<"合并";
                   (it+off_V_S)->endX=(it+off_V_E)->endX;
                   DUAN.erase(&DUAN.last());//删除
                   DUAN.erase(it+off_V_E);
               }
           }
        }
        if(S_X > E_X)//从右向左，擦除
        {
            if(off_V_S == NO_FOUND && off_V_E ==NO_FOUND)//新段
            {
                qDebug()<<"空删";
                DUAN.erase(&DUAN.last());//删除
            }
            else if(off_V_S !=NO_FOUND &&off_V_E ==NO_FOUND)
            {
                qDebug()<<"左删";
                (it+off_V_S)->startX=S_X;
                DUAN.erase(&DUAN.last());//删除
            }
            else if(off_V_S ==NO_FOUND &&off_V_E !=NO_FOUND)
            {
                //头不在，尾在
                qDebug()<<"右删";
                (it+off_V_E)->endX=E_X;
                DUAN.erase(&DUAN.last());//删除
            }
            else if(off_V_S !=NO_FOUND &&off_V_E !=NO_FOUND)
            {
                //修改前后段 的尾 和 头
                if(off_V_S==off_V_E)//在同一段里面（包含），删除即可
                {
                    qDebug()<<"分删";
                    DUAN.last().endX=(it+off_V_S)->endX;
                    (it+off_V_S)->endX=E_X;//形成新的左端
                }
                else//在不同的里面
                {
                    qDebug()<<"跨删";
                    (it+off_V_S)->startX=S_X;
                    (it+off_V_E)->endX=E_X;
                    DUAN.erase(&DUAN.last());//删除
                }
            }
        }
    }

//    for(it=DUAN.begin();it!=DUAN.end();it++)
//    {
//        if(it->startX==0&&it->endX==0)
//            DUAN.erase(it);
//    }
    qDebug()<<endl<<"DUAN容器长度："<<DUAN.length();


}
void MarkFrame::paintEvent(QPaintEvent *ev)
{
    Q_UNUSED(ev);
    QPainter painter;
    QVector<struct_Duan>::iterator it;
    if(DUAN.size()<=0)return;

    //绘制所有分段
    painter.begin(this);
    struct_Duan DUAN_XX =DUAN[0];//得到第一个标记段
    for(it=DUAN.begin();it!=DUAN.end();it++)
    {
        if(DUAN_XX.startX<DUAN_XX.endX)
        //使用画刷填充一个矩形区域
        painter.fillRect(QRect(QPoint(DUAN_XX.startX,0),QPoint(DUAN_XX.endX,this->height())), QBrush(Qt::green));

        if(DUAN_XX.startX>DUAN_XX.endX)
        //擦除一个矩形区域的内容
        painter.eraseRect(QRect(QPoint(DUAN_XX.startX,0),QPoint(DUAN_XX.endX,this->height())));

        DUAN_XX = *it;
    }
    painter.end();
}

