/*
 * Copyright 2019 CALIBRATION
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDoubleValidator>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QLocale>
#include <QMouseEvent>
#include <qwt_picker_machine.h>
#include <qwt_plot_renderer.h>
#include <qwt_date_scale_draw.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}
/*===============================槽函数🐷===============================*/

//软件菜单栏action
void MainWindow::on_actionHome_triggered()
{// Home Page
    ui->stackedWidget->setCurrentIndex(0);
//    ui->statusbar->showMessage(pageName.at(0));
}

void MainWindow::on_actioncalN_triggered()
{//reflect Page
    ui->stackedWidget->setCurrentIndex(1);
//    ui->statusbar->showMessage(pageName.at(1));
}

void MainWindow::on_actioncalAngle_triggered()
{// Angle Page
    ui->stackedWidget->setCurrentIndex(2);
//    ui->statusbar->showMessage(pageName.at(2));
}

void MainWindow::on_actionBRDF_triggered()
{// BRDF Page
    ui->stackedWidget->setCurrentIndex(3);
//    ui->statusbar->showMessage(pageName.at(3));
}

void MainWindow::on_actionCalConv_triggered()
{//Conv Page
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_actionExit_triggered()
{//Exit
    this->close();
}

void MainWindow::on_actionOpen_triggered()
{// Open Local Dir
    QDesktopServices::openUrl(QUrl(QDir::currentPath()));
}

void MainWindow::on_actionCommit_triggered()
{//使用反馈
    QDesktopServices::openUrl(QUrl("mailto:dushenda@outlook.com?subject=科研计算小软件使用反馈&body=经过一段时间的使用，我反馈如下：\n"));
}

// 主页按钮
void MainWindow::on_btnHomeCalN_clicked()
{
    on_actioncalN_triggered();
}

void MainWindow::on_btnHomeCalAngle_clicked()
{
    on_actioncalAngle_triggered();
}

void MainWindow::on_btnHomeBRDF_clicked()
{
    on_actionBRDF_triggered();
}

void MainWindow::on_btnHomeCalConv_clicked()
{
    on_actionCalConv_triggered();
}
//折射率计算截面按钮
void MainWindow::on_btnCalNCal_clicked()
{//Calculate for signle and range
    //判断需要计算哪个，再进行计算
    //注意单位，步长单位为nm
    bool flagSuccess1 = false;
    bool flagSuccess2 = false;
    double wv = ui->lineEditLambda->text().toDouble();
    double wvLower = ui->lineEditLambdaLower->text().toDouble();
    double wvStep = ui->lineEditLambdaStep->text().toDouble();
    double wvUpper = ui->lineEditLambdaUpper->text().toDouble();
    qDebug()<<wv;
    if((wv-0) > compareNum)
    {//计算单个波长
        data.setSingleData(wv);
        QList<double> result = data.getSingleData();
        ui->lineEditN->setText(QString::number(result.at(0)));
        ui->lineEditDiff->setText(QString::number(result.at(1)));
        flagSuccess1 = true;
    }

    if((wvLower-0)>compareNum && (wvUpper-0)>compareNum && (wvStep-0)>compareNum)
    {
        resultRange.clear();
        data.setData(wvLower,wvUpper,wvStep);
        resultRange = data.getData();
        qDebug()<<resultRange;
        flagSuccess2 = true;
    }
    if(flagSuccess1 && flagSuccess2)
    {
        plot(resultRange,ui->graphicsViewN,ui->graphicsViewDiff);
        QMessageBox::information(this,"信息","范围波长和单波长数据计算成功");        
    }else {
        if(flagSuccess1)
            QMessageBox::information(this,"信息","单波长数据计算成功");
        else if(flagSuccess2){
            plot(resultRange,ui->graphicsViewN,ui->graphicsViewDiff);
            QMessageBox::information(this,"信息","范围波长计算成功");
       }else
            QMessageBox::critical(this,"错误","计算失败");
    }
}

void MainWindow::on_btncalNBackHome_clicked()
{//cal N Page back home button
    on_actionHome_triggered();
}

void MainWindow::on_btnCalNExportN_clicked()
{//Export File
    if(resultRange.isEmpty())
    {
        QMessageBox::critical(this,"错误","内存中无数据，请先计算");
    }else {
        QString filePath = QFileDialog::getSaveFileName(this,"保存文件",QDir::currentPath(),"csv file(*.csv);;txt files(*.txt)");
        qDebug()<<filePath;
        if(!filePath.isEmpty())
            exportData(filePath,resultRange);
    }
}

void MainWindow::on_btnCalNCal_3_clicked()
{
    replotQCP(ui->graphicsViewN);
    replotQCP(ui->graphicsViewDiff);
}



void MainWindow::on_pushButtonSingle_clicked()
{
    QDateTime dt = ui->dateTimeEditOne->dateTime();
    double lon = ui->lineEditLon->text().toDouble();
    double lat = ui->lineEditLat->text().toDouble();
    QList<double> result = ac.calAngleSingle(dt,lon,lat);
    ui->lineEditZenith->setText(QString::number(result.at(0)));
    ui->lineEditAzimuth->setText(QString::number(result.at(1)));
}

void MainWindow::on_pushButtonRange_clicked()
{//计算并且画图
    QDateTime sdt = ui->dateTimeEditStart->dateTime();
    QDateTime edt = ui->dateTimeEditEnd->dateTime();
    int step = ui->lineEditSecondGap->text().toInt();
    double lon = ui->lineEditLon->text().toDouble();
    double lat = ui->lineEditLat->text().toDouble();

    rstRange.clear();
    rstRange = ac.calAngleRange(sdt,edt,step,lon,lat);
    QList<QDateTime> dtL;
    for (QDateTime dt = sdt;dt<edt;dt=dt.addSecs(step)) {
        dtL.append(dt);
    }
    bool succZenith = plotAngle(ui->qwtPlotZenith,"太阳天顶角",dtL,rstRange.at(0));

    bool succAzimuth  = plotAngle(ui->qwtPlotAzimuth,"太阳方位角",dtL,rstRange.at(1));
    if(succZenith&&succAzimuth)
        QMessageBox::information(this,"成功","计算成功😄!");
    else {
        QMessageBox::critical(this,"错误","计算失败😭");
    }
}

void MainWindow::on_pushButtonExport_clicked()
{//导出数据文件
    if(rstRange.isEmpty())
    {
        QMessageBox::critical(this,"错误","内存中无数据，请先计算😡");
    }else{
        QDateTime sdt = ui->dateTimeEditStart->dateTime();
        QDateTime edt = ui->dateTimeEditEnd->dateTime();
        int step = ui->lineEditSecondGap->text().toInt();
        QList<QDateTime> dtL;
        for (QDateTime dt = sdt;dt<edt;dt=dt.addSecs(step)) {
            dtL.append(dt);
        }

        QString filePath = QFileDialog::getSaveFileName(this,"保存文件",QDir::currentPath(),"csv file(*.csv);;txt files(*.txt)");
        qDebug()<<filePath;
        if(!filePath.isEmpty())
            ExportDataAngle(filePath,dtL,rstRange);
    }
}

void MainWindow::on_pushButtonExport_Img1_clicked()
{//导出天顶角图片
    QwtPlotRenderer r;
    r.setDiscardFlag(QwtPlotRenderer::DiscardBackground);
    r.exportTo(ui->qwtPlotZenith,"天顶角计算结果",QSize(320,270),97);
}

void MainWindow::on_pushButtonExport_Img2_clicked()
{//导出方位角图片
    QwtPlotRenderer r;
    r.exportTo(ui->qwtPlotAzimuth,"方位角",QSize(320,270),97);
}

void MainWindow::on_pushButtonCalAngleBack_clicked()
{//返回主页
    on_actionHome_triggered();
}



void MainWindow::on_btnImportDataConv_clicked()
{//读取外部的光谱卷积数据并且画图，@张允详
    QString filePath = QFileDialog::getOpenFileName(this,"导入数据",QDir::currentPath(),
                                                    "文本数据(*.csv *.txt);;所有类型(*.*)");
    if(!filePath.isEmpty())
        sc.importData(this,filePath,ui->graphicsViewImportData);
}

void MainWindow::on_btnCalConv_clicked()
{//计算卷积
    double start = ui->lineEditStartConv->text().toDouble();
    double step = ui->lineEditStepConv->text().toDouble();
    sc.calConv(this,ui->graphicsViewCaltData,start,step);
}

void MainWindow::on_btnGrpOrigin_clicked()
{
    replotQCP(ui->graphicsViewImportData);
    replotQCP(ui->graphicsViewCaltData);
}

void MainWindow::on_btnExportDataConv_clicked()
{//导出数据
    QString filePath = QFileDialog::getSaveFileName(this,"导入数据",QDir::currentPath(),
                                                    "文本数据(*.csv *.txt);;所有类型(*.*)");
    if(!filePath.isEmpty())
        sc.exportData(this,filePath);
}

void MainWindow::on_btnExportDataConv_2_clicked()
{//导出原始数据图片
    saveGraph(ui->graphicsViewImportData);
}

void MainWindow::on_btnExportDataConv_3_clicked()
{//导出结果图片
    saveGraph(ui->graphicsViewCaltData);
}

void MainWindow::on_btnBackHome_clicked()
{//返回主页
    on_actionHome_triggered();
}
/*===============================用户自定义函数❗===============================*/

void MainWindow::init()
{//initial function
    ui->stackedWidget->setCurrentIndex(0);
    QDoubleValidator *val = new QDoubleValidator;
    val->setBottom(0);
    ui->lineEditLambda->setValidator(val);
    ui->lineEditLambdaLower->setValidator(val);
    ui->lineEditLambdaStep->setValidator(val);
    ui->lineEditLambdaUpper->setValidator(val);
//    ui->statusbar->showMessage(pageName.at(0));
    connect(ui->graphicsViewN,&QCustomPlot::mouseDoubleClick,this,&MainWindow::saveGraph1);
    connect(ui->graphicsViewDiff,&QCustomPlot::mouseDoubleClick,this,&MainWindow::saveGraph2);
    QDoubleValidator *valLon = new QDoubleValidator;//设置经度框输入范围
    valLon->setRange(-180.0,180.0);
    ui->lineEditLon->setValidator(valLon);
    QDoubleValidator *valLat = new QDoubleValidator;//设置纬度输入框范围
    valLat->setRange(-90.0,90.0);
    ui->lineEditLat->setValidator(valLat);
    QIntValidator *valStep = new QIntValidator;//设置时间间隔
    valStep->setBottom(0);
    ui->lineEditSecondGap->setValidator(valStep);
    //设置datetime控件初始值
    ui->dateTimeEditOne->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEditStart->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEditEnd->setDateTime(QDateTime::currentDateTime());
}

void MainWindow::exportData(const QString filePath, const QList<QList<double> > data)
{
    QFile f(filePath);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::information(this,"错误","创建文件错误");
        return ;
    }
    QTextStream dataOut(&f);
    dataOut<<"wavelength(um)"<<","<<"effective_index"<<","<<"derivative_EffIn"<<","<<"\n";
    for (int i=0;i<data.at(0).length();++i) {
        for (int j=0;j<data.length();++j) {
            dataOut<<data.at(j).at(i)<<",";
        }
        dataOut<<"\n";
    }
    f.close();
    QMessageBox::information(this,"信息","文件保存成功");
}

void MainWindow::plot(const QList<QList<double> > data,QCustomPlot* c1,QCustomPlot* c2)
{
    c1->clearGraphs();
    c1->clearItems();
    QVector<double> x = data.at(0).toVector();
    QVector<double> yN = data.at(1).toVector();
    QVector<double> yDiff = data.at(2).toVector();
    c1->addGraph();
    QPen pen;
    pen.setColor(QColor(65,105,225));//Royal Blue
    pen.setWidth(3);
    c1->graph()->setPen(pen);
    c1->graph()->setData(x,yN);
    c1->xAxis->setLabel("波长");
    c1->yAxis->setLabel("折射率");
    c1->graph()->rescaleAxes(true);
    c1->xAxis->rescale(true);
    c1->yAxis->rescale(true);
    //set graph title
    if(c1->plotLayout()->elementCount()==1)
    {
        c1->plotLayout()->insertRow(0);
        c1->plotLayout()->addElement(0,0,new QCPTextElement(c1,"波长与折射率之间的关系",QFont("sans",12,QFont::Bold)));
    }
    c1->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    c1->replot();

    c2->clearGraphs();
    c2->clearItems();
    c2->addGraph();
    QPen pen2;
    pen2.setColor(QColor(244,164,96));//SandyBrown
    pen2.setWidth(3);
    c2->graph()->setPen(pen2);
    c2->graph()->setData(x,yDiff);
    //set graph title
    if(c2->plotLayout()->elementCount()==1)
    {
        c2->plotLayout()->insertRow(0);
        c2->plotLayout()->addElement(0,0,new QCPTextElement(c2,"波长与折射率一阶导数之间的关系",QFont("sans",12,QFont::Bold)));
    }
    c2->xAxis->setLabel("波长");
    c2->yAxis->setLabel("导数");
    c2->graph()->rescaleAxes(true);
    c2->xAxis->rescale(true);
    c2->yAxis->rescale(true);
    c2->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    c2->replot();
}

void MainWindow::saveGraph1()
{
    saveGraph(ui->graphicsViewN);
}

void MainWindow::saveGraph2()
{
    saveGraph(ui->graphicsViewDiff);
}

void MainWindow::saveGraph(QCustomPlot *c)
{
    QString fileFilter = "jpg file(*.jpg);;png files(*.png);;bmp file(*bmp);;pdf files(*.pdf)";
    QStringList fileSuffix = {"jpg","png","bmp","pdf"};
    QString filePath = QFileDialog::getSaveFileName(this,"保存折射率",QDir::currentPath(),fileFilter);
    if(filePath.isEmpty())
        return;
    QFile f(filePath);
    QFileInfo fInfo(filePath);
    QString suffixName = fInfo.suffix();
    int pos = fileSuffix.indexOf(suffixName);
    pos++;
    bool saveSuccess = false;
    switch (pos) {
    case 1:{
        saveSuccess = c->saveJpg(filePath,500,500,1.0,100);
        break;
    }
    case 2:{saveSuccess = c->savePng(filePath);
        break;
    }
    case 3:{saveSuccess = c->saveBmp(filePath);
        break;
    }
    case 4:{saveSuccess = c->savePdf(filePath);
        break;
    }
    default:{
        break;
    }
    }
    if(saveSuccess)
    {
        QMessageBox::information(this,"信息","导出图表成功");
    }else{
        QMessageBox::critical(this,"错误","导出图表出错");
    }
}



void MainWindow::ExportDataAngle(const QString filePath, const QList<QDateTime> dt, const QList<QList<double> > data)
{
    QFile f(filePath);
    if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::information(this,"错误","创建文件错误");
        return ;
    }
    QTextStream dataOut(&f);
    dataOut<<"DateTime"<<","<<"Zenith(deg)"<<","<<"Azimuth(deg)"<<","<<"\n";
    for (int i=0;i<dt.length();++i) {
        dataOut<<dt.at(i).toString("yyyy/MM/dd HH:mm:ss")<<","<<
                 data.at(0).at(i)<<","
              <<data.at(1).at(i)<<"\n";
    }
    f.close();
    QMessageBox::information(this,"信息","文件保存成功");
}

bool MainWindow::plotAngle(QwtPlot *qwt,const QString title,QList<QDateTime> xO,QList<double> yO)
{
    qwt->detachItems();//先清除再重画
    bool succ = false;

    deleteNanData(xO,yO);

    QVector<QDateTime> x = xO.toVector();
    QVector<double> y = yO.toVector();
    QVector<double> xd;
    for (int i=0;i<x.size();++i) {
        xd.append(x.at(i).toMSecsSinceEpoch());
    }

    qwt->setTitle(title);
    qwt->setCanvasBackground(Qt::white);
    qwt->setAxisTitle(QwtPlot::xBottom,"时间");
    qwt->setAxisTitle(QwtPlot::yLeft,"角度/deg");

    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableXMin(true);
    grid->enableYMin(true);
    grid->enableX(true);
    grid->enableY(true);
    grid->setMajorPen(QColor(190,190,190));
    grid->setMinorPen(QColor(211,211,211),0,Qt::DotLine);
    grid->attach(qwt);

    QwtDateScaleDraw *timeScale = new QwtDateScaleDraw;
    QString scaleFormat = "h:mm\nM月d日";
    timeScale->setDateFormat(QwtDate::Second,scaleFormat);
    qwt->setAxisScaleDraw(QwtPlot::xBottom,timeScale);

    QwtPlotCurve *curve = new QwtPlotCurve;
    curve->attach(qwt);
    curve->setSamples(xd,y);
    curve->setStyle(QwtPlotCurve::Lines);
    curve->setCurveAttribute(QwtPlotCurve::Fitted, true);//使曲线更光滑
    curve->setRenderHint(QwtPlotItem::RenderAntialiased,true);
    curve->setPen(QPen(QColor(65,105,225),3));//设置画笔

    QwtPlotZoomer *m_zoomer = new QwtPlotZoomer(qwt->canvas());
    QwtPlotPanner *m_panner = new QwtPlotPanner(qwt->canvas());
    m_zoomer->setMousePattern(QwtEventPattern::MouseSelect3, Qt::RightButton);
    m_zoomer->setZoomBase(true);
    m_panner->setMouseButton(Qt::RightButton);

    qwt->setAxisAutoScale(QwtPlot::xBottom);
    qwt->setAxisAutoScale(QwtPlot::yLeft);

    qwt->update();
    qwt->updateAxes();
    qwt->replot();
    qwt->show();

    succ = true;
    return succ;
}

void MainWindow::deleteNanData(QList<QDateTime> &x, QList<double> &y)
{//删除nan数据，nan数据在qwt画图时会导致整串数据不显示
    for (int i=0;i<y.size();++i) {
        if(isnan(y.at(i)))
        {
            y.removeAt(i);
            x.removeAt(i);
        }
    }
}



void MainWindow::replotQCP(QCustomPlot *qcp)
{
    qcp->xAxis->rescale(true);
    qcp->yAxis->rescale(true);
    qcp->replot();
}

