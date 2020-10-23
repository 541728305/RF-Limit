#include "multicurvesplot.h"
#include "qDebug.h"
#include <QColorDialog>


#pragma execution_character_set("utf-8")

extern QVector<CurveData> allCurvesData;
extern QVector<CurveData> limitCurvesData;
extern QVector<CurveData> bezierCurvesData;//贝塞尔曲线的数据,allCurvesData[n]中又含有两个QVector，也即第n条曲线的x、y值
QVector<CurveData> bezierControlData(50);//贝塞尔曲线控制点
extern double filterCurvesData[50][20000];
extern int graphSelect ;
extern double graphCoordX;
extern double graphCoordY;
QCPGraph* upLimitGraph;
QCPGraph* downLimitGraph;
QCPGraph* bezierGraph;
QCPGraph* bezierControlGraph;
int bezierControlSelect =-1;
int point_count = 0;
QVector<QPointF> bezier_point;
QVector<QPointF> point;
#define hitColor 0x8CE600
#define origColor 0x018bba
#define bezierOpen 1
#define bezierClose 0
int bezierOpenState = bezierClose;
int  bezierControlIdx=0;
int bezierGraphIdx=0;
typedef struct
{
    double X;
    double Y;
} PointF;
QMap<int, QCPGraph*> curveIdx2graphPtr;//记录或查询：从曲线索引(key)->graph指针(val)。读之前必须查询key的存在性contains(key)?
QMap<QCPGraph*, int> graphPtr2curveIdx;//记录或查询：从graph指针(key)->曲线索引(val)。读之前必须查询key的存在性contains(key)?

QMap<int, QCPGraph*> curveIdx2graphPtrData;//记录或查询：从曲线索引(key)->graph指针(val)。读之前必须查询key的存在性contains(key)?
QMap<QCPGraph*, int> graphPtr2curveIdxData;//记录或查询：从graph指针(key)->曲线索引(val)。读之前必须查询key的存在性contains(key)?

QMap<int, QCPGraph*> curveIdx2graphPtrBezier;//记录或查询：从曲线索引(key)->graph指针(val)。读之前必须查询key的存在性contains(key)?
QMap<QCPGraph*, int> graphPtr2curveIdxBezier;//记录或查询：从graph指针(key)->曲线索引(val)。读之前必须查询key的存在性contains(key)?

QMap<int, QCPGraph*> curveIdx2graphPtrBezierControl;//记录或查询：从曲线索引(key)->graph指针(val)。读之前必须查询key的存在性contains(key)?
QMap<QCPGraph*, int> graphPtr2curveIdxBezierControl;//记录或查询：从graph指针(key)->曲线索引(val)。读之前必须查询key的存在性contains(key)?



QVector<QCPGraph*> pGraph(5000);



MultiCurvesPlot::MultiCurvesPlot(int _curvesCnt, QWidget *parent):
    QCustomPlot(parent)

    ,curvesCnt(allCurvesData.size())
    ,autoScroll(false)
    ,tracer(new QCPItemTracer(this))
    ,tracerEnable(false)
    ,lastPoint(new QPointF(0, 0))
{
    this->setInteractions(
                QCP::iSelectPlottables //可选中曲线
                | QCP::iRangeZoom //可滚轮缩放
                | QCP::iRangeDrag //可平移
                | QCP::iSelectLegend
                );//可选中图例
    this->setNoAntialiasingOnDrag(true);//禁用抗锯齿，以提高性能

    connect(this, SIGNAL(selectionChangedByUser()), this, SLOT(when_selectionChangedByUser()));
    connect(parent,SIGNAL(BezierOpen()), this, SLOT(bezierFunctionOpen()));
    connect(parent,SIGNAL(BezierClose(int)), this, SLOT( bezierFunctionClose(int)));
//    connect(this, SIGNAL(legendDoubleClick(QCPLegend*, QCPAbstractLegendItem*, QMouseEvent*)), this, SLOT(when_legendDoubleClick(QCPLegend*, QCPAbstractLegendItem*, QMouseEvent*)));

    for(int i=0;i<1000;i++)
    {
        this->addGraph();
    }
    this->legend->setVisible(true);//使能图例
    this->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignTop|Qt::AlignLeft);//设置图例的位置
    //this->setOpenGl(true);
//    qDebug() << "QCustomplot opengl status = " << this->openGl();
    this->yAxis2->setVisible(false);
//    connect(yAxis, SIGNAL(rangeChanged(QCPRange)), yAxis2, SLOT(setRange(QCPRange)));//左右y轴同步放缩

//    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);//日期做X轴
//    dateTicker->setDateTimeFormat("hh:mm:ss.zzz\nyyyy-MM-dd");//日期格式(可参考QDateTime::fromString()函数)
    //QDateTime::fromString("");
//    this->xAxis->setTicker(dateTicker);//设置X轴为时间轴
    this->xAxis->rescale(true);
    this->yAxis->rescale(true);
    this->xAxis->setTickLabels(true);//显示刻度标签
    this->xAxis->setPadding(8);
    this->xAxis->setOffset(8);
    this->yAxis->setPadding(8);
    this->yAxis->setOffset(8);


    /*显示数值的提示框*/
    textLabel = new QCPItemText(this);
    textLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignLeft);//文本框的原点位置
    textLabel->position->setType(QCPItemPosition::ptAxisRectRatio);//位置类型（当前轴范围的比例）
    textLabel->position->setCoords(0, 0); // place position at center/top of axis rect
    textLabel->setText(0);
    textLabel->setFont(QFont(font().family(), 16)); // make font a bit larger
    textLabel->setPen(QPen(Qt::black)); // show black border around text
    textLabel->setPadding(QMargins(2,2,2,2));//边界宽度
    textLabel->setBackgroundColor(Qt::transparent);


    /*自定义文本提示框*/

    nameLabel = new QCPItemText(this);
    nameLabel->setPositionAlignment(Qt::AlignTop|Qt::AlignHCenter);//文本框的原点位置
    nameLabel->position->setType(QCPItemPosition::ptAxisRectRatio);//位置类型（当前轴范围的比例）
    nameLabel->position->setCoords(0.5, 0); // place position at center/top of axis rect
    nameLabel->setText("Text Item Demo");
    nameLabel->setFont(QFont(font().family(), 24)); // make font a bit larger
    nameLabel->setPen(QPen(Qt::transparent)); // show black border around text
    nameLabel->setPadding(QMargins(2,2,2,2));//边界宽度
    nameLabel->setBackgroundColor(Qt::transparent);
    nameLabel->setVisible(false);


    //单击时指向数值的箭头:
    arrow = new QCPItemLine(this);
    arrow->start->setParentAnchor(textLabel->bottom);    
    //arrow->end->setCoords(4, 1.6); // point to (4, 1.6) in x-y-plot coordinates
    arrow->setHead(QCPLineEnding::esSpikeArrow);
    textLabel->setVisible(false);
    arrow->setVisible(false);

    //游标以及游标文本框设置
    this->setMouseTracking(true);
    tracer->setInterpolating(false);//禁用插值
    tracer->setPen(QPen(Qt::DashLine));//虚线游标
    tracer->setStyle(QCPItemTracer::tsCrosshair);//游标样式：十字星、圆圈、方框等
    //游标的X值文本框
    tracerXText = new QCPItemText(this);
    tracerXText->setPositionAlignment(Qt::AlignBottom|Qt::AlignRight);
    tracerXText->position->setType(QCPItemPosition::ptAxisRectRatio);//位置类型（当前轴范围的比例）
    tracerXText->position->setCoords(0, 0.99); // 在plot中的位置
    tracerXText->position->setParentAnchorX(tracer->position);//X位置锚定到游标的X位置
    tracerXText->setText("no curves..");
    tracerXText->setFont(QFont(font().family(), 12));
    tracerXText->setPen(QPen(Qt::black));
    tracerXText->setBackgroundColor(Qt::green);
    tracerXText->setPadding(QMargins(2,2,2,2));//边界宽度
    tracerXText->setVisible(false);
    //游标的Y值文本框
    tracerYText = new QCPItemText(this);
    tracerYText->setPositionAlignment(Qt::AlignTop|Qt::AlignLeft);
    tracerYText->position->setType(QCPItemPosition::ptAxisRectRatio);//位置类型（当前轴范围的比例）
    tracerYText->position->setCoords(0.01, 0); // 在plot中的位置
    tracerYText->setText("no curves..");
    tracerYText->position->setParentAnchorY(tracer->position);//Y位置锚定到游标的Y位置
    tracerYText->setFont(QFont(font().family(), 12));
    tracerYText->setPen(QPen(Qt::black));
    tracerYText->setBackgroundColor(Qt::green);
    tracerYText->setPadding(QMargins(2,2,2,2));//边界宽度
    tracerYText->setVisible(false);

    /*两点差值的显示框*/
    diffText = new QCPItemText(this);
    diffText->setPositionAlignment(Qt::AlignTop|Qt::AlignRight);
    diffText->setTextAlignment(Qt::AlignTop|Qt::AlignLeft);//文本框的原点位置
    diffText->position->setType(QCPItemPosition::ptAxisRectRatio);//位置类型（当前轴范围的比例）
    diffText->position->setCoords(0.98, 0); // place position at center/top of axis rect
    diffText->setText("dt=0\r\ndy=0");
    diffText->setFont(QFont(font().family(), 16)); // make font a bit larger
    diffText->setPen(QPen(Qt::black)); // show black border around text
    diffText->setPadding(QMargins(2,2,2,2));//边界宽度
    diffText->setBackgroundColor(Qt::green);
    diffText->setVisible(false);

    //右键菜单
    this->setContextMenuPolicy(Qt::ActionsContextMenu);
    //菜单-按钮1
    actshowAllGraph = new QAction("重新扫描坐标");
    connect(actshowAllGraph, SIGNAL(triggered(bool)), this, SLOT(showAllGraph()));
    this->addAction(actshowAllGraph);

    //菜单-按钮2
    actClearDatas = new QAction("清空所有数据");
    connect(actClearDatas, SIGNAL(triggered(bool)), this, SLOT(clearAllData()));
    this->addAction(actClearDatas);




    getColor.resize(curvesCnt);
    for(int i = 0; i < getColor.size();i++)
    {
        getColor[i] = QColor(qrand()%255, qrand()%255, qrand()%255);//生成随机颜色
    }
    getName.resize(curvesCnt);
    for(int i = 0; i < getName.size();i++)//设置默认曲线名称
    {
        getName[i] = QString("curve %1").arg(i);
    }

    for(int i = 0; i < allCurvesData.size();i++)//清空备份的数据源
    {
        allCurvesData[i].keyVec.clear();
        allCurvesData[i].valVec.clear();
    }

    startTimer(30, Qt::CoarseTimer);//定频刷新曲线图
}


/*
QList<uint16_t> idxList;
list << 0 << 3;
plot->showCurves(idxList);//显示出第0和第3条曲线
*/
void MultiCurvesPlot::showCurves(QList<uint16_t> _idxList, int start, int stop,int startOffset,int isLimit)
{
//    qDebug() << "MultiCurvesPlot::showCurves";
//    qDebug() << "MultiCurvesPlot::showCurves:" << idxList;

//    QVector<CurveData> currentData(5000);
//    this->clearGraphs();//先移除所有的graph
//    idxList = _idxList;//备份，可能别的函数需要它
//    curveIdx2graphPtr.clear();//清除曲线编号->graph指针的映射
//    graphPtr2curveIdx.clear();//清除graph指针->曲线编号的映射
//    int graphIdx;
//    if(isLimit)
//        graphIdx = 0;
//    else
//        graphIdx = 0;

//    for(QList<uint16_t>::const_iterator it = idxList.begin(); it != idxList.end(); it++)
//    {   //*it为每一个要显示的曲线编号
//        uint16_t curveIdx = *it;
//        if(curveIdx > allCurvesData.size())
//        {
//            qDebug() << QString("warning: MultiCurvesPlot::showCurves->超出数据源max index").arg(*it);
//            continue;
//        }
//        for(int i=start;i<stop;i++)
//        {
//            currentData[curveIdx].keyVec.append(i-startOffset);
//            currentData[curveIdx].valVec.append(allCurvesData[curveIdx].valVec.at(i));
//        }

//        this->addGraph(this->xAxis, this->yAxis);

//        QCPGraph* pGraph = graph(graphIdx);

////        curveIdx2graphPtr[curveIdx] = pGraph;//记录：曲线索引->graph指针的映射
////        graphPtr2curveIdx[pGraph] = curveIdx;//记录：graph指针->曲线索引的映射

//        pGraph->setData(currentData[curveIdx].keyVec, currentData[curveIdx].valVec, true);//数据源 （todo:*it的合法性）
//        if(graphIdx<4)
//        {
//            QPen pen;
//            pen.setColor(QRgb(0x018bba));
////            pen.setBrush(Qt::SolidPattern);
//            pen.setWidth(3);
//            QPen penhit = pen;
//            penhit.setWidth(4);
//            pGraph->setPen(pen);//规格的颜色
//            pGraph->selectionDecorator()->setPen(penhit);
//            connect(pGraph, SIGNAL(selectionChanged(QCPDataSelection)), this, SLOT(calloutSeries1(QCPDataSelection)));
////            connect(pGraph, SIGNAL(selectionChanged(QCPDataSelection)), this, SLOT(calloutSeries2(QCPDataSelection)));
//        }
//        else
//        {
//            graph(graphIdx)->setSelectable(QCP::stNone);
//            pGraph->setPen(QPen(getColor[curveIdx]));//线的颜色随机

//        }

//        pGraph->setLineStyle(QCPGraph::lsLine);//每个点由直线连接
//        //this->graph(graphIdx)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 3));//显示散点
//        this->graph(graphIdx)->setName(getName[curveIdx]);
//        graphIdx++;

//    }
//    if(graphCount() > 0)
//        traceGraph = graph(0);
//    else
//        traceGraph = NULL;
////    showAllGraph();




//    this->replot();
}
extern int limitFlag;
extern int dataFlag;
extern int creatFlag;
/************************************************************************************************/

/************************************************************************************************/
int showLimitSeriesFlag = 0;
void MultiCurvesPlot::showLimitSeries(int start, int stop,int startOffset,int upLimit, int downLimit)
{

    QVector<CurveData> currentData(50);
    qDebug()<<"removeLimit"<< __LINE__;
    removeLimitSeries();

    QList<uint32_t>_idxList;

    for(int i=upLimit;i<=downLimit;i++)
    {
        _idxList<<i;
    }
//    idxList = _idxList;//备份，可能别的函数需要它
    int  graphIdx = this->graphCount();
    int gIdx = 0;
    for(int i=0;i<currentData.size();i++)
    {
        currentData[i].keyVec.clear();
        currentData[i].valVec.clear();
    }
    for(QList<uint32_t>::const_iterator it = _idxList.begin(); it != _idxList.end(); it++)
    {   //*it为每一个要显示的曲线编号
        int32_t curveIdx = *it;
        if(curveIdx > limitCurvesData.size())
        {
            qDebug() << QString("warning: MultiCurvesPlot::showCurves->超出数据源max index").arg(*it);
            continue;
        }
        int key=0;
        for(int i=start;i<stop;i++)
        {
            currentData[curveIdx].keyVec.append(key);
            currentData[curveIdx].valVec.append(limitCurvesData[curveIdx].valVec.at(i));
//            qDebug()<<"filterCurvesData[curveIdx][i]"<<filterCurvesData[curveIdx][i];
            key++;

        }

        this->addGraph(this->xAxis, this->yAxis);

        QCPGraph* pGraph = graph(graphIdx);

        QPen pen;
        pen.setColor(QRgb(origColor));


        QPen penhit = pen;
        penhit.setColor(QRgb(hitColor));

         penhit.setWidth(4);
        if(gIdx<2)//保险起见，只绘制两条曲线
        {
            if(gIdx==0)
            {
                upLimitGraph = pGraph;
                curveIdx2graphPtr[graphIdx] = upLimitGraph;//记录：曲线索引->graph指针的映射
                graphPtr2curveIdx[upLimitGraph] = graphIdx;//记录：graph指针->曲线索引的映射
                qDebug()<<"上曲线索引"<<graphPtr2curveIdx[upLimitGraph];
                upLimitGraph->setData(currentData[curveIdx].keyVec, currentData[curveIdx].valVec, true);//数据源 （todo:*it的合法性）

                if(graphSelect==1)
                {
                    pen.setWidth(4);
                    pen.setColor(QRgb(hitColor));
                }
                else
                {
                    pen.setWidth(3);
                    pen.setColor(QRgb(origColor));
                }
                upLimitGraph->setPen(pen);//规格的颜色
                upLimitGraph->selectionDecorator()->setPen(penhit);

                upLimitGraph->setVisible(true);
//                connect(upLimitGraph, SIGNAL(selectionChanged(QCPDataSelection)), this, SLOT(calloutSeries1(QCPDataSelection)));
            }
            if(gIdx==1)
            {
                downLimitGraph = pGraph;
                curveIdx2graphPtr[graphIdx] = downLimitGraph;//记录：曲线索引->graph指针的映射
                graphPtr2curveIdx[downLimitGraph] = graphIdx;//记录：graph指针->曲线索引的映射
                qDebug()<<"下曲线索引"<<graphPtr2curveIdx[downLimitGraph];
                downLimitGraph->setData(currentData[curveIdx].keyVec, currentData[curveIdx].valVec, true);//数据源 （todo:*it的合法性）
                if(graphSelect==2)
                {
                    pen.setWidth(4);
                    pen.setColor(QRgb(hitColor));
                }
                else
                {
                    pen.setWidth(3);
                    pen.setColor(QRgb(origColor));
                }
                downLimitGraph->setPen(pen);//规格的颜色
                downLimitGraph->selectionDecorator()->setPen(penhit);
                downLimitGraph->setVisible(true);
//                connect(downLimitGraph, SIGNAL(selectionChanged(QCPDataSelection)), this, SLOT(calloutSeries2(QCPDataSelection)));
            }
        }

        pGraph->setLineStyle(QCPGraph::lsLine);//每个点由直线连接
        this->graph(graphIdx)->setName(getName[curveIdx]);
        graphIdx++;
        gIdx++;

    }
    if(graphCount() > 0)
        traceGraph = graph(0);
    else
        traceGraph = NULL;
    showLimitSeriesFlag=1;
    this->replot();
}
void MultiCurvesPlot::finish()
{
    qDebug()<<"结束线程";
    m_thread->exit();
}

int graphStart =0;
int graphStop = 0;
void MultiCurvesPlot::showDataSeries(int start, int stop,int startOffset,int seriesStart, int seriesStop)
{

    QVector<CurveData> currentTemp(5000);
    QList<QCPGraph*> QCPGraphTemp;

    removeDataSeries();//移除数据曲线并清除数据
    qDebug()<<"上索引" <<(graphPtr2curveIdx[upLimitGraph]);
    qDebug()<<"下索引" <<(graphPtr2curveIdx[downLimitGraph]);
    QList<uint32_t>_idxList;
    idxList.clear();
    for(int i=seriesStart;i<seriesStop;i++)
    {
        _idxList<<i;
    }
//    idxList = _idxList;//备份，可能别的函数需要它

    int graphIdx = 0;
    graphStart = graphIdx;
    for(int i=0;i<currentTemp.size();i++)
    {
        currentTemp[i].keyVec.clear();
        currentTemp[i].valVec.clear();
    }
    qDebug()<<"临时数据清空完成";
    QCPGraphTemp=this->getGraphs();
    qDebug()<<QCPGraphTemp.size();
    QList<QCPGraph*>::const_iterator itG = QCPGraphTemp.begin();

    for(QList<uint32_t>::const_iterator it = _idxList.begin(); it != _idxList.end(); it++)
    {
            //*it为每一个要显示的曲线编号
        int32_t curveIdx = *it;
        if(curveIdx > allCurvesData.size())
        {
            qDebug() << QString("warning: MultiCurvesPlot::showCurves->超出数据源max index").arg(*it);
            continue;
        }
//        qDebug()<<"graphIdx="<<graphIdx;
        int key=0;
        for(int i=start;i<stop;i++)
        {
            if(allCurvesData[curveIdx].valVec.count()<stop)
                continue;
//            qDebug()<<"i="<<i;
            currentTemp[curveIdx].keyVec.append(key);
            currentTemp[curveIdx].valVec.append(allCurvesData[curveIdx].valVec.at(i));
//            qDebug()<<"allCurvesData"<<curveIdx<<i<<allCurvesData[curveIdx].valVec.at(i);
            key++;
        }

//        this->addGraph(this->xAxis, this->yAxis);

        pGraph[graphIdx] = *itG;

        curveIdx2graphPtrData[graphIdx] = pGraph[graphIdx];//记录：曲线索引->graph指针的映射
        graphPtr2curveIdxData[pGraph[graphIdx]] = graphIdx;//记录：graph指针->曲线索引的映射

        pGraph[graphIdx]->setData(currentTemp[curveIdx].keyVec, currentTemp[curveIdx].valVec, true);//数据源 （todo:*it的合法性）

        pGraph[graphIdx]->setSelectable(QCP::stNone);
        pGraph[graphIdx]->setPen(QPen(getColor[curveIdx]));//线的颜色随机

        pGraph[graphIdx]->setLineStyle(QCPGraph::lsLine);//每个点由直线连接
        this->graph(graphIdx)->setName(getName[curveIdx]);
        graphStop=graphIdx;
        graphIdx++;
        itG++;
//        qDebug()<<"itG"<<&itG;
        if(graphIdx == QCPGraphTemp.size())
            break;
    }

    if(graphCount() > 0)
        traceGraph = graph(0);
    else
        traceGraph = NULL;
//    showAllGraph();

    this->replot();
    qDebug()<<"曲线绘制完成";

}

void  my_Thread::show(int count)
{
    qDebug() << "show:" << QThread::currentThreadId();
    qDebug() << "count:" << count;

    for(int i=graphStart;i<count;i++)
    {
       if((graphPtr2curveIdx[upLimitGraph] ==i) || (graphPtr2curveIdx[downLimitGraph]==i))
       {
            qDebug()<<"规格索引成功调过数据"<<i;
            continue;
       }
       if(curveIdx2graphPtrData.contains(i))
        {
           pGraph[i]->data().data()->removeAfter(0);
//           this->removeGraph(curveIdx2graphPtrData[i]);
//           qDebug()<<"数据索引成功删除数据"<<i;
        }
    }


//    curveIdx2graphPtrData.clear();//清除曲线编号->graph指针的映射
//    graphPtr2curveIdxData.clear();//清除graph指针->曲线编号的映射
    qDebug()<<"线程数据曲线清空完成";

    emit finish();
};

void MultiCurvesPlot::calloutSeries1(QCPDataSelection select)
{

    if(select.dataPointCount()>0)
    {
//        qDebug()<<"dataRanges1"<<select.dataRanges();
//        qDebug()<<"span1"<<select.span();
//        qDebug()<<"dataRange"<<select.dataRange(0);

        graphSelect = 1;
        qDebug()<<graphSelect;
    }
    else
    {
        graphSelect = 0;
        qDebug()<<graphSelect;
    }
}
void MultiCurvesPlot::calloutSeries2(QCPDataSelection select)
{

    if(select.dataPointCount()>0)
    {
//        qDebug()<<"dataRanges2"<<select.dataRanges();
//        qDebug()<<"span2"<<select.span();
//        qDebug()<<"dataRange"<<select.dataRange(0);
        graphSelect = 2;
        qDebug()<<graphSelect;
    }
    else
    {
        graphSelect = 0;
        qDebug()<<graphSelect;
    }

}

void MultiCurvesPlot::removeLimitSeries()
{
    if(showLimitSeriesFlag==1)
    {
        if(graphPtr2curveIdx.contains(downLimitGraph)==true)
        {
            downLimitGraph->data().data()->clear();
            qDebug()<<"下曲线索引成功"<<graphPtr2curveIdx[downLimitGraph];
            this->removeGraph(graphPtr2curveIdx[downLimitGraph]);
        }
        if(graphPtr2curveIdx.contains(upLimitGraph)==true)
        {
            upLimitGraph->data().data()->clear();
            qDebug()<<"上曲线索引成功"<<graphPtr2curveIdx[upLimitGraph];
            this->removeGraph(graphPtr2curveIdx[upLimitGraph]);
        }
        curveIdx2graphPtr.clear();//清除曲线编号->graph指针的映射
        graphPtr2curveIdx.clear();//清除graph指针->曲线编号的映射
    }
    showLimitSeriesFlag =0;
}



void MultiCurvesPlot::removeDataSeries()
{
    int count =this->plottableCount();
    QElapsedTimer time;
    time.start();
    qDebug()<<"start"<<time.elapsed();
    int times=0;
    for(int i=graphStart;i<count;i++)
    {

        if(bezierOpenState == bezierOpen)
        {
            if( (graphPtr2curveIdxBezier[bezierGraph]==i) || (graphPtr2curveIdxBezierControl[bezierControlGraph]==i) )
            {
                qDebug()<<"索引到bezier"<<i;
                continue;
            }
        }

       if((graphPtr2curveIdx[upLimitGraph] ==i) || (graphPtr2curveIdx[downLimitGraph]==i))
       {
            qDebug()<<"规格索引成功跳过数据"<<i;
            continue;
       }

       if(curveIdx2graphPtrData.contains(i))
        {
            times++;
            pGraph[i]->data().data()->clear();
//           this->removeGraph(curveIdx2graphPtrData[i]);
           //qDebug()<<"数据索引成功删除数据"<<i;
        }
//       pGraph[i]->data();
    }
//    this->clearGraphs();
     qDebug()<<"曲线数据量"<<pGraph.isEmpty();
    qDebug()<<"stop"<<time.elapsed();
    curveIdx2graphPtrData.clear();//清除曲线编号->graph指针的映射
    graphPtr2curveIdxData.clear();//清除graph指针->曲线编号的映射
    qDebug()<<"数据曲线清空完成";
    qDebug()<<"当前曲线数"<<this->plottableCount();
    qDebug()<<"循环删除次数"<<times;
    qDebug()<<"曲线开始"<<graphStart;
    qDebug()<<"曲线结束"<<graphStop;
    qDebug()<<"曲线总数"<<count;

}
void MultiCurvesPlot::setNameText(QString text, int show)
{
    nameLabel->setText(text);
    if(show)
    {
        nameLabel->setVisible(true);
    }
    else
    {
        nameLabel->setVisible(false);
    }
}


void MultiCurvesPlot:: bezierFunctionOpen()
{
    qDebug()<<"bezierFunctionOpen";
    bezierOpenState = bezierOpen;
//    QLinearGradient plotGradient;

//    plotGradient.setColorAt(0, QColor(80, 80, 80));
//    plotGradient.setColorAt(1, QColor(50, 50, 50));
//    this->setBackground(plotGradient);      // 设置背景颜色

    int  graphIdx = this->graphCount();
    this->addGraph(this->xAxis, this->yAxis);
    QCPGraph* ControlGraph = graph(graphIdx);
    bezierControlGraph = ControlGraph;
    QPen pen;
    pen.setColor(QRgb(0xF28500));
    pen.setWidth(3);
    pen.setStyle(Qt::PenStyle::DashLine);
    bezierControlGraph->setPen(pen);
    bezierControlGraph->setLineStyle(QCPGraph::lsLine);//每个点由直线连接
    bezierControlGraph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
    bezierControlGraph->setSelectable(QCP::stNone);
    curveIdx2graphPtrBezierControl[bezierControlIdx]=bezierControlGraph;//索引
    graphPtr2curveIdxBezierControl[bezierControlGraph]=bezierControlIdx;//索引


    this->addGraph(this->xAxis, this->yAxis);
    QCPGraph* Graph =  graph(this->graphCount()-1);
    bezierGraph = Graph;
    QPen pen2;
    pen2.setColor(QRgb(0XE60000));
    pen2.setWidth(3);
    pen2.setStyle(Qt::PenStyle::SolidLine);
    bezierGraph->setPen(pen2);//规格的颜色
    bezierGraph->setLineStyle(QCPGraph::lsLine);//每个点由直线连接
    bezierGraph->setSelectable(QCP::stNone);
    curveIdx2graphPtrBezier[bezierGraphIdx]=bezierGraph;
    graphPtr2curveIdxBezier[bezierGraph]=bezierGraphIdx;

    this->setBackground(QColor(140, 117, 118));

}

PointF tmp_points[150];
void MultiCurvesPlot:: bezierFunctionClose(int bezierSave)
{

    qDebug()<<"bezierFunctionClose"<<bezierSave;
    bezierOpenState = bezierClose;

    if(graphPtr2curveIdxBezierControl.contains(bezierControlGraph)==true)
    {
        bezierControlGraph->data().data()->clear();
        qDebug()<<"控制点图层索引成功"<<graphPtr2curveIdxBezierControl[bezierControlGraph];
        this->removeGraph(graphPtr2curveIdxBezierControl[bezierControlGraph]);
    }
    if(graphPtr2curveIdxBezier.contains(bezierGraph)==true)
    {
        bezierGraph->data().data()->clear();
        qDebug()<<"BEZIER图层索引成功"<<graphPtr2curveIdxBezier[bezierGraph];
        this->removeGraph(graphPtr2curveIdxBezier[bezierGraph]);
    }
    bezier_point.clear();
    point.clear();
    point_count=0;
    bezierControlData[0].keyVec.clear();
    bezierControlData[0].valVec.clear();
    for(int i=0;i<150;i++)
    {
        tmp_points[i].X=0;
        tmp_points[i].Y=0;
    }
    curveIdx2graphPtrBezier.clear();
    graphPtr2curveIdxBezier.clear();
    curveIdx2graphPtrBezier.clear();
    graphPtr2curveIdxBezier.clear();
//    QLinearGradient plotGradient;
//    plotGradient.setColorAt(0, QColor(0, 0, 0));
//    plotGradient.setColorAt(1, QColor(0, 0, 0));
//    this->setBackground(plotGradient);      // 设置背景颜色

    this->setBackground(QColor(255,255,255));

}

void MultiCurvesPlot:: updateBezierControl()
{
    qDebug()<<"添加数据";
    bezierControlGraph->data().clear();
    bezierControlGraph->setData(bezierControlData[0].keyVec,bezierControlData[0].valVec,true);
    bezierGraph->data().clear();
    bezierCurvesData[0].keyVec.clear();
    bezierCurvesData[0].valVec.clear();

    for(int i=0;i<bezier_point.size();i++)
    {
       bezierCurvesData[0].keyVec.append(bezier_point[i].rx());
       bezierCurvesData[0].valVec.append(bezier_point[i].ry());
    }
    bezierGraph->setData(bezierCurvesData[0].keyVec,bezierCurvesData[0].valVec,true);
//    qDebug()<<"bezier_point"<<bezier_point;
    this->replot();
}


PointF bezier_interpolation_func(double t, PointF *points)
{

    for (int i = 1; i < point_count; ++i)
    {

        for (int j = 0; j < point_count - i; ++j)
        {

            if (i == 1)
            {
                tmp_points[j].X = (double)(points[j].X * (1 - t) + points[j + 1].X * t);
                tmp_points[j].Y = (double)(points[j].Y * (1 - t) + points[j + 1].Y * t);
                continue;
            }
            tmp_points[j].X = (double)(tmp_points[j].X * (1 - t) + tmp_points[j + 1].X * t);
            tmp_points[j].Y = (double)(tmp_points[j].Y * (1 - t) + tmp_points[j + 1].Y * t);
        }

    }
    return tmp_points[0];
}
#define N 1000
void draw_bezier_curves(PointF *points, PointF *out_points)
{
    double step = 1.0 / N;
    double t = 0;
    for (int i = 0; i < N; i++)
    {
        PointF temp_point = bezier_interpolation_func(t, points); //二阶贝塞尔曲线
        t += step;
        out_points[i] = temp_point;
    }

}

void bezier(QVector<QPointF> point)
{
    QPointF bpoint;
    PointF pointf;
    int x, y;
    PointF *in = (PointF *)malloc(point_count * sizeof(PointF));
    for (int i = 0; i < point_count; i++)
    {
        in[i].X = point[i].x();
        in[i].Y = point[i].y();
        qDebug()<<"y"<<point[i].y();
    }

    PointF out[N];
    draw_bezier_curves(in, out);

    for (int j = 0; j < N; j++)
    {
        bpoint.setX(out[j].X);
        bpoint.setY(out[j].Y);
        bezier_point.push_back(bpoint);
    }


    free(in);

}





/************************************************************************************************/

/************************************************************************************************/
void MultiCurvesPlot::storeAxisScope(bool x, bool y)
{
    if(x)
        Xscope = this->xAxis->range();
    if(y)
        Yscope = this->yAxis->range();
}

void MultiCurvesPlot::resumeAxisScope(bool x, bool y)
{

    if(x)
        this->xAxis->setRange(Xscope);
    if(y)
        this->yAxis->setRange(Yscope);

}


void MultiCurvesPlot::setCurveName(int idx, const QString newName)
{
    if(idx < getName.size())
        getName[idx] = newName;
    else
        qDebug() << "warning: MultiCurvesPlot::setCurveName -> invalid idx!";
}
/*
如果参数_nameVec成员过多，则截断；过少则补长
*/
void MultiCurvesPlot::setCurvesName(QVector<QString> _nameVec)
{
    _nameVec.resize(getName.size());//截断或补长
    getName = _nameVec;
}
/*
典型用法示例：
int idx = 0;//曲线0
double x = (double)(QDateTime::currentMSecsSinceEpoch()) / 1000.0;//当前时间
double y = qSin(x);
plot->addData(idx, x, y);
*/
void MultiCurvesPlot::addData(int idx, double x, double y)
{
    if(idx > allCurvesData.size())
    {
        qDebug() << QString("warning:下位机企图绘制编号为%1的曲线！").arg(idx);
        return;
    }
    if(y > 1e10 || y < -1e10)//接收到的异常值直接不绘制
        return;



    if(curveIdx2graphPtr.contains(idx))//第idx个曲线正在显示中
    {
        QCPGraph* pGraph = curveIdx2graphPtr[idx];
        pGraph->addData(x, y);
        pGraph->data().data()->removeBefore(0);
    }

}

void MultiCurvesPlot::setDiffSolveEnable(bool enable)
{
    diffText->setVisible(enable);
}

void MultiCurvesPlot::clearAllData()
{
    if(QMessageBox::No == QMessageBox::information(this,
                                "请再次确认",
                                "确认清空吗？",
                                QMessageBox::Yes,
                                QMessageBox::No | QMessageBox::Default))
    {
        return;
    }

    for(int i = 0; i < graphCount(); i++)
    {
        graph(i)->data().data()->clear();
    }

    for(int idx = 0; idx < allCurvesData.size(); idx++)
    {
        allCurvesData[idx].keyVec.clear();
        allCurvesData[idx].valVec.clear();
    }
    for (int idx = 0; idx<limitCurvesData.size();idx++)
    {
        limitCurvesData[idx].keyVec.clear();
        limitCurvesData[idx].valVec.clear();
    }

    curveIdx2graphPtr.clear();//清除曲线编号->graph指针的映射
    graphPtr2curveIdx.clear();//清除graph指针->曲线编号的映射

    curveIdx2graphPtrData.clear();//清除曲线编号->graph指针的映射
    graphPtr2curveIdxData.clear();//清除graph指针->曲线编号的映射
    limitFlag=1;
    dataFlag=1;
    creatFlag=1;

    emit clearFlag();
}

void MultiCurvesPlot::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if(autoScroll)
    {
        double curSeclf = (double)(QDateTime::currentMSecsSinceEpoch()) / 1000.0;
        this->xAxis->setRange(curSeclf - xAxis->range().size(), curSeclf);
    }

    this->replot();
}

/*
1、 按下时，显示按下点的数值，松手时隐藏
2、 计算游标值，并emit
*/
int count=10;
void MultiCurvesPlot::mousePressEvent(QMouseEvent *event)
{
    QCustomPlot::mousePressEvent(event);

    if(event->buttons() & Qt::LeftButton)
    {
        textLabel->setVisible(true);
        arrow->setVisible(true);
        double x = xAxis->pixelToCoord(event->pos().x());//像素坐标转plot坐标
        double y = yAxis->pixelToCoord(event->pos().y());
        graphCoordX = x;
        graphCoordY = y;
        arrow->end->setCoords(x, y); // 箭头的终点
        //        QString xTime = QDateTime::fromMSecsSinceEpoch(x * 1000.0).toString("hh:mm:ss.zzz");
        textLabel->setText(QString("x = %1\ny = %2").arg(x).arg(y));//箭头框显示点击值
        solveDifference(QPointF(x, y));
    }

    if(bezierOpenState==bezierClose)
    {
         this->setContextMenuPolicy(Qt::ActionsContextMenu);
        if(showLimitSeriesFlag)
        {
            qDebug()<<upLimitGraph->selected();
            if(upLimitGraph->selected()==true || downLimitGraph->selected()==true)
            {

            }
            else
            {
                graphSelect=0;
                QPen pen;
                pen.setColor(QRgb(origColor));

                pen.setWidth(3);
                upLimitGraph->setPen(pen);//规格的颜色
                downLimitGraph->setPen(pen);//规格的颜色
                this->replot();
            }
        }
    }else if(bezierOpenState==bezierOpen)
    {
         this->setContextMenuPolicy(Qt::NoContextMenu);
        if(event->buttons() & Qt::LeftButton)
        {
            double coordDiffX=this->xAxis->range().upper-this->xAxis->range().lower;
            double coordDiffY=this->yAxis->range().upper-this->yAxis->range().lower;
            double x = xAxis->pixelToCoord(event->pos().x());//像素坐标转plot坐标
            double y = yAxis->pixelToCoord(event->pos().y());//像素坐标转plot坐标
            qDebug()<<"selected"<<bezierControlGraph->selected();

//            if(bezierControlGraph->selected()==true)
            if(1)
            {
                for(int i=0;i<bezierControlData[0].keyVec.size();i++)
                {
                    qDebug()<<round(x)<<round(bezierControlData[0].keyVec.at(i));
                    if((bezierControlData[0].valVec.at(i)-coordDiffY/(7*15))<y && (bezierControlData[0].valVec.at(i)+coordDiffY/(7*15))>y)
                    if((bezierControlData[0].keyVec.at(i)-coordDiffX/(7*15))<x && (bezierControlData[0].keyVec.at(i)+coordDiffX/(7*15))>x)
                    {
                        bezierControlSelect=i;
                        qDebug()<<"选择控制点";
                    }
                }

            }else
            {
                bezierControlSelect=-1;
            }
        }
        if(event->buttons() & Qt::RightButton)
        {
            double x = xAxis->pixelToCoord(event->pos().x());//像素坐标转plot坐标
            double y = yAxis->pixelToCoord(event->pos().y());
            if(x>=0)
            {
                point_count++;

                bezierControlData[0].keyVec.append(x);
                bezierControlData[0].valVec.append(y);

                qDebug()<<"bezierControlData"<<bezierControlData[0].keyVec.at(0);
                qDebug()<<"bezierControlData"<<bezierControlData[0].valVec.at(0);
                QPointF Pt;
                Pt.setX(x);
                Pt.setY(y);
                point.push_back(Pt);
                if (point_count > 2)
                {
                    bezier_point.clear();
                    bezier(point);
                }
                updateBezierControl();
            }
        }
        else if (event->button() == Qt::MidButton)
        {
            if (point_count > 0)
            {
                --point_count;
                point.pop_back();
                bezierControlData[0].keyVec.pop_back();
                bezierControlData[0].valVec.pop_back();
                bezier_point.clear();
                if (point_count > 2)
                {
                    bezier(point);
                }
                updateBezierControl();
            }
        }
    }


}

/*
按下ctrl时，实时显示游标
关联：this->setmouseTracking(true)
*/
void MultiCurvesPlot::mouseMoveEvent(QMouseEvent *event)
{
    QCustomPlot::mouseMoveEvent(event);
//    qDebug() << "mouseMoveEvent";

    if(bezierOpenState==bezierOpen)
    {
//        if(bezierControlGraph->selected()==true)
        {
            double x = xAxis->pixelToCoord(event->pos().x());//像素坐标转plot坐标
            double y = yAxis->pixelToCoord(event->pos().y());
            if(bezierControlSelect>=0 && x>=0)
            {

                qDebug()<<"x,y"<<x<<y;
                point[bezierControlSelect].setX(x);
                point[bezierControlSelect].setY(y);
                if (point_count > 2)
                {
                    bezier_point.clear();
                    bezier(point);
                }
                bezierControlData[0].keyVec.replace(bezierControlSelect,x);
                bezierControlData[0].valVec.replace(bezierControlSelect,y);
                updateBezierControl();

            }
        }
    }








    if(tracerEnable)//下面这段计算量略大
    {
        if(getGraphs().contains(traceGraph) && traceGraph->data().data()->size() != 0)
        {
            //qDebug() <<  "trace graph point cnt = " << traceGraph->data().data()->size();
            double x = xAxis->pixelToCoord(event->pos().x());//像素坐标转plot坐标
            tracer->setGraph(traceGraph);
            tracer->setGraphKey(x);
            double traceX = tracer->position->key();
            double traceY = tracer->position->value();

            tracerXText->setText(QDateTime::fromMSecsSinceEpoch(traceX * 1000.0).toString("hh:mm:ss.zzz"));//游标文本框
            tracerYText->setText(QString::number(traceY));

            for(int i = 0; i < graphCount(); i++)//计算所有曲线的游标值
            {
                QCPDataContainer<QCPGraphData>::const_iterator coorPoint = graph(i)->data().data()->findBegin(traceX);//搜索左边离traceX最近的key对应的点
                if(graphPtr2curveIdx.contains(graph(i)))
                {
                    int curveIdx = graphPtr2curveIdx[graph(i)];
                    emit noniusValueChanged(curveIdx, QString::number(coorPoint->value));
                }
            }
        }
    }
}
void MultiCurvesPlot::mouseReleaseEvent(QMouseEvent *event)
{
    QCustomPlot::mouseReleaseEvent(event);
    bezierControlSelect=-1;
    if(event->button() == Qt::LeftButton)
    {
        textLabel->setVisible(false);
        arrow->setVisible(false);
    }
}
//void MultiCurvesPlot::keyPressEvent(QKeyEvent *event)
//{
//    QCustomPlot::keyPressEvent(event);
//}
//void MultiCurvesPlot::keyReleaseEvent(QKeyEvent *event)
//{
//    QCustomPlot::keyReleaseEvent(event);
//}
void MultiCurvesPlot::setColors(QVector<QColor> _colorsIn)
{
    if(_colorsIn.size() != getColor.size())
    {
        qDebug() << "warning: MultiCurvesPlot::setColors failed!";
        return;
    }

    getColor = _colorsIn;
}

/*用户点选了图中的元素时：同步选中曲线和曲线图例*/
void MultiCurvesPlot::when_selectionChangedByUser()
{
    qDebug()<<upLimitGraph->selected();
    if(upLimitGraph->selected()==true)
    {
        graphSelect=1;
        QPen pen;
        pen.setColor(QRgb(hitColor));

        pen.setWidth(4);
        upLimitGraph->setPen(pen);//规格的颜色

        QPen pen2;
        pen2.setColor(QRgb(origColor));

        pen2.setWidth(3);
        downLimitGraph->setPen(pen2);//规格的颜色
        emit coordChange();


    }else
    {
        if(bezierOpenState==bezierClose)
        {
            QPen pen;
            pen.setColor(QRgb(origColor));

            pen.setWidth(3);
            upLimitGraph->setPen(pen);//规格的颜色
        }
    }


    if(downLimitGraph->selected()==true)
    {

        graphSelect=2;
        QPen pen;
        pen.setColor(QRgb(hitColor));

        pen.setWidth(4);
        downLimitGraph->setPen(pen);//规格的颜色

        QPen pen2;
        pen2.setColor(QRgb(origColor));

        pen2.setWidth(3);
        upLimitGraph->setPen(pen2);//规格的颜色
        emit coordChange();

    }else
    {
        if(bezierOpenState==bezierClose)
        {
            QPen pen;
            pen.setColor(QRgb(origColor));

            pen.setWidth(3);
            downLimitGraph->setPen(pen);//规格的颜色
        }
    }
    this->replot();
    qDebug()<<graphSelect;
//    for (int i = 0; i < this->graphCount(); ++i)
//     {
//       QCPGraph *graph = this->graph(i);

//       QCPPlottableLegendItem *item = this->legend->itemWithPlottable(graph);
//       if (item->selected() || graph->selected())//选中了哪条曲线或者曲线的图例
//       {
////            item->setSelected(true);//同时选中曲线和图例
////            graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
//            traceGraph = graph;
//       }
//     }
}
/*
双击图例修改曲线颜色
*/
void MultiCurvesPlot::when_legendDoubleClick (QCPLegend *legend, QCPAbstractLegendItem *legendItem, QMouseEvent *event)
{
    Q_UNUSED(legend);
    Q_UNUSED(event);

    for (int i = 0; i < this->graphCount(); ++i)
    {
        QCPGraph *pGraph = this->graph(i);
        if(legendItem == this->legend->itemWithPlottable(pGraph))//查询被点击的图例项是哪个graph的图例项
        {
            if(graphPtr2curveIdx.contains(pGraph))
            {
                int curveIdx = graphPtr2curveIdx[pGraph];//被选中的曲线编号
//                qDebug() << QString("被双击的curve %1").arg(curveIdx);
                QColor newColor = QColorDialog::getColor(getColor[curveIdx], //默认颜色
                                                         this,//父窗体
                                                         "选择新颜色");
                if(newColor.isValid())//用户点击了确定
                {
                    getColor[curveIdx] = newColor;
//                    pGraph->setPen(QPen(newColor));//修改颜色，这行代码不管用，原因未知
//                    showCurves(idxList);//只能用这个来修改颜色了

                }
            }
            this->deselectAll();//取消所有的点选
            break;
        }
    }
}

/*求差，并更新到显示框*/
void MultiCurvesPlot::solveDifference(QPointF newPoint)
{
    diffText->setText(QString("dt=%1秒\r\ndy=%2")
                      .arg(newPoint.x()-lastPoint->x())
                      .arg(newPoint.y()-lastPoint->y()));
    *lastPoint = newPoint;
}
/*
如果设置了主图，那么本图将被作为副图，本图的X轴范围会跟着主图变
*/
void MultiCurvesPlot::setMainPlot(QCustomPlot *_mainPlot)
{
    //connect
}


void MultiCurvesPlot::setTracerEnable(bool enable)
{
    tracerEnable = enable;
    if(tracerEnable)
    {
        tracer->setVisible(true);
        tracerXText->setVisible(true);
        tracerYText->setVisible(true);
    }
    else
    {
        tracer->setVisible(false);
        tracerXText->setVisible(false);
        tracerYText->setVisible(false);
    }
}

void MultiCurvesPlot::setScatterPointEnable(bool enable)
{
    for (int i = 0; i < this->graphCount(); ++i)
    {
        if(enable)
            this->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));//显示散点
        else
            this->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));//不显示散点
    }
}

const QVector<CurveData> *MultiCurvesPlot::getAllData() const
{
    return &allCurvesData;
}

QString MultiCurvesPlot::getCurveName(int idx)
{
    if(idx < getName.size())
        return getName[idx];
    else
        return "notExist";
}
