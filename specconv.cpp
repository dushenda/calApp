/*
 * Copyright 2020 CALIBRATION
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
#include "specconv.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>
//#include <QAxObject>
#include <iostream>
#include <math.h>
#include "string"
#include "string.h"

#include "qtcsv/stringdata.h"
#include "qtcsv/variantdata.h"
#include "qtcsv/reader.h"
#include "qtcsv/writer.h"


//#include "libxl.h"

//using namespace libxl;

SpecConv::SpecConv()
{

}

void SpecConv::importData(QWidget *parent,const QString filePath,QCustomPlot *qcp)
{//导入数据为原始数据，导入到成员变量中
//读入的文件数据需从第二行开始，丢弃表头
    QFile file(filePath);
    QFileInfo fInfo(filePath);
    QString fileType = fInfo.suffix();
    xWaveLengthOrigin.clear();yRadOrigin.clear();
    if(fileType=="csv"||fileType=="txt")
    {
        QList<QStringList> readData =QtCSV::Reader::readToList(filePath);
        for(int i=1;i<readData.size();++i)
        {
            xWaveLengthOrigin.append(readData.at(i).at(0).toDouble());
            yRadOrigin.append(readData.at(i).at(1).toDouble());
        }
        if(isEmptyImportData())
        {
            QMessageBox::critical(parent,"错误","数据缺失");
            return;
        }
    }
    //============================================不看======================
    //不知道为啥这个库用不了libxl
//    else if (fileType=="xls") {
//        libxl::Book* book = xlCreateBook(); // xlCreateXMLBook() for xlsx
//        if(book)
//        {
//            libxl::Sheet* sheet = book->addSheet(L"Sheet1");
//            if(sheet)
//            {
//                sheet->writeStr(2, 1, L"Hello, World !");
//                sheet->writeNum(3, 1, 1000);
//            }
//            book->save(L"example.xls");
//            book->release();
//        }
//        }
//        Book *book = xlCreateXMLBook();
//        const wchar_t* f = reinterpret_cast<const wchar_t *>(filePath.utf16());
//        if(book->load(f))
//        {
//            Sheet* sheet = book->getSheet(0);

//            for(int row = sheet->firstRow(); row < sheet->lastRow(); ++row)
//            {
//                if(sheet)
//                {
//                    qDebug()<<1;
//                }

////                qDebug()<<row<<"\n"
////                       <<sheet->firstRow()<<sheet->lastRow();
////                CellType cellType1 = sheet->cellType(row, sheet->firstCol());
////                if(cellType1==CELLTYPE_NUMBER)
////                {
////                    qDebug()<<1;
//////                    double x = sheet->readNum(row,sheet->firstCol());
//////                    double y = sheet->readNum(row,sheet->firstCol()+1);
//////                    //                   xWaveLengthOrigin.append(x);
//////                    //                   yRadOrigin.append(y);
//////                    qDebug()<<x<<y;
////                }else {
////                    continue;
////                }
//            }

//        }
////        const char *msg = book->errorMessage();
////        QString MSG = QString(QLatin1String(msg));
////        if(MSG.compare("OK",Qt::CaseInsensitive))
////            QMessageBox::critical(parent,"错误",MSG);
//        book->release();
//    }
    //===================================不看==================================
    else{//excel
            QMessageBox::critical(parent,"错误","不能打开此类文件");
            return;
    }

    plot(qcp,"高斯滤波前",1);
    QMessageBox::information(parent,"信息","数据导入成功🎈");
}

void SpecConv::calConv(QWidget *parent,QCustomPlot *qcp,double start, const double step)
{//计算成员变量的数据，存入待输出的成员变量数据
    //start是开始计算的起点
    //step是每次高斯函数移动的距离
    if(isEmptyImportData()){
        QMessageBox::critical(parent,"错误","请先导入数据");
        return;
    }
    if(start<1e-15){
        start = xWaveLengthOrigin.at(0);
    }
    double signalStart = xWaveLengthOrigin.at(0);
    double signalEnd = xWaveLengthOrigin.last();//信号的终点值
    double fstart = start;//高斯函数起始点
    double sigma = calVar(start,0);//标准差
    double width = 6*sigma;//高斯函数计算范围,此范围外填充0
    double fend = fstart+width;//高斯函数结束点
    int i = 0;//循环起始点
    double xCenWave = 0;//中心波长
    double xVar = 0;//函数方差
    double yConv = 0;//卷积结果
    xCentralWave.clear();
    xVarWave.clear();
    yConvResult.clear();
    while (fend<=signalEnd) {
        xCenWave = (fstart+fend)/2;
        xVar = calVar(signalStart,i);
        qDebug()<<xVar;
        yConv = getGaussResult(xWaveLengthOrigin,yRadOrigin,fstart,fend,xVar);

        if(yConv+1<1e-15){//yConv == -1
            QMessageBox::critical(parent,"错误","向量不匹配");
            return;
        }

        xCentralWave.append(xCenWave);
        xVarWave.append(xVar);
        yConvResult.append(yConv);

        i++;
        fstart += step*i;//每次初始值平移
        width = 6*xVar;
        fend = fstart+width;
    }

    if(isEmptyExportData())
    {
        QMessageBox::critical(parent,"错误","数据缺失");
        return;
    }
    plot(qcp,"高斯滤波后",2);
}

void SpecConv::exportData(QWidget *parent,const QString filePath)
{//输入的为输出文件带路径全名，将成员变量中的数据输出
    if(xCentralWave.isEmpty()||xVarWave.isEmpty()||yConvResult.isEmpty())
    {
        QMessageBox::critical(parent,"错误","内存中无数据，请先计算😡");
    }else{
        QtCSV::StringData strData;
        QStringList strList;
        strList<<"central_wavelength"<<"conv result"<<"variable";
        strData.addRow(strList);
        for(int i=0;i<xCentralWave.size();++i)
        {
            strList.clear();
            if(isnan(yConvResult.at(i)))
            {
                continue;
            }
            strList << QString::number(xCentralWave.at(i))<<QString::number(yConvResult.at(i))<<QString::number(xVarWave.at(i));
            strData.addRow(strList);
        }
        QtCSV::Writer::write(filePath,strData);
    }
}

void SpecConv::plot(QCustomPlot *qcp,QString title,const int mode)
{//画图，数据从成员数据中取，输入参数为图的指针
    qcp->clearGraphs();
    qcp->clearItems();
//    qcp->clearPlottables();
    QVector<double> x,yN;
    if(mode==1)
    {
        x = xWaveLengthOrigin.toVector();
        yN = yRadOrigin.toVector();
    }else {
        x = xCentralWave.toVector();
        yN = yConvResult.toVector();
    }
    qcp->addGraph();
    QPen pen;
    pen.setColor(QColor(65,105,225));//Royal Blue
    pen.setWidth(3);
    qcp->graph()->setPen(pen);
    qcp->graph()->setData(x,yN);
    qcp->xAxis->setLabel(QString("波长(um)").toUtf8());
    qcp->yAxis->setLabel("辐亮度");
    qcp->graph()->rescaleAxes(true);
    qcp->xAxis->rescale(true);
    qcp->yAxis->rescale(true);
    //set graph title
    qDebug()<<qcp->plotLayout()->elementCount();
    if(qcp->plotLayout()->elementCount()==1)
    {
        qcp->plotLayout()->insertRow(0);
        qcp->plotLayout()->addElement(0,0,new QCPTextElement(qcp,title,QFont("sans",12,QFont::Bold)));
    }
    qcp->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    qcp->replot();
}

bool SpecConv::isEmptyImportData()
{
    bool isEmpty = xWaveLengthOrigin.isEmpty()||yRadOrigin.isEmpty();
    return isEmpty;
}

bool SpecConv::isEmptyExportData()
{
    bool isEmpty = xCentralWave.isEmpty()||xVarWave.isEmpty()||yConvResult.isEmpty();
    return isEmpty;
}

double SpecConv::calVar(double start, int n)
{
    double FWHM = 0,sigma =0;
    FWHM = 10000/((10000/start)-0.625*(n+1))-10000/((10000/start)-0.625*n);
    sigma = FWHM/(2*sqrt(2*log(2)));
    return sigma;
}

double SpecConv::getGaussResult(const QList<double> signalX,QList<double> signalY, double fstart, double fend,double var)
{//signalX 为信号的波长，signalY为信号的幅值，fstart为分段的起始点,fend为分段的中点，其余填充0，var为高斯函数的方差
    //返回值为该高斯函数下对信号求响应值
    double y = 0;
    double gaussCentral = (fstart+fend)/2;//该函数的均值
    QList<double> yL;
    for (int i=0; i<signalX.size(); ++i) {
        //lambda为起始波长
        double lambda = signalX.at(i);
        if (lambda>fstart&&lambda<fend) {
            y = 1/(var*lambda*sqrt(2*M_PI))*exp((lambda-gaussCentral)/(2*pow(var,2)));
            yL.append(y);
        }else {
            yL.append(0);
        }
    }
    if(yL.size()!=signalY.size()&&signalY.size()!=0){
        return -1;//-1代表出错
    }

    double resultUp = 0;//分子
    double resultDown = 0;//分母
    for(int i=0;i<signalY.size();++i)
    {//计算信号的在该点下的滤波值
        resultUp += signalY.at(i)*yL.at(i);
        resultDown += yL.at(i);
    }
//    qDebug()<<resultUp<<"\t"<<resultDown;
    return resultUp/resultDown;
}
