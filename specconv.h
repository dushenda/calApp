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
#ifndef SPECCONV_H
#define SPECCONV_H

#include <QList>
#include "qcustomplot.h"

class SpecConv
{
public:
    SpecConv();
private:
    QList<double> xWaveLengthOrigin;
    QList<double> yRadOrigin;

private:
    QList<double> xCentralWave;//计算的高斯函数的中心波长
    QList<double> xVarWave;//计算高斯函数的方差
    QList<double> yConvResult;//卷积之后的结果

public:
    void importData(QWidget *,const QString ,QCustomPlot *);//导入原始数据
    void calConv(QWidget *parent,QCustomPlot *qcp,double start,const double step);//计算卷积和
    void exportData(QWidget*,const QString filePath);//导出结果
    void plot(QCustomPlot *,QString,const int mode);//画图，画原始导入数据

private:
    bool isEmptyImportData();//判断成员变量是否为空，如果为空则返回True
    bool isEmptyExportData();
    double calVar(double start,int n);
    double getGaussResult(const QList<double> signalX,QList<double> signalY,double fstart,double fend,double var);
};

#endif // SPECCONV_H
