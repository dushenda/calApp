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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "rangedata.h"
#include "qcustomplot.h"
#include "anglecal.h"
#include "specconv.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actioncalN_triggered();

    void on_btnHomeCalN_clicked();

    void on_actionHome_triggered();

    void on_actioncalAngle_triggered();

    void on_actionBRDF_triggered();

    void on_btnHomeCalAngle_clicked();

    void on_btnHomeBRDF_clicked();

    void on_actionExit_triggered();

    void on_actionOpen_triggered();

    void on_btnCalNCal_clicked();

    void on_btncalNBackHome_clicked();

    void on_btnCalNExportN_clicked();

    void saveGraph1();

    void saveGraph2();

    void on_actionCommit_triggered();
//公用
    void on_pushButtonSingle_clicked();

    void on_pushButtonRange_clicked();

    void on_pushButtonExport_clicked();

    void on_pushButtonExport_Img1_clicked();

    void on_pushButtonExport_Img2_clicked();

    void on_pushButtonCalAngleBack_clicked();

    void on_btnImportDataConv_clicked();

    void on_btnHomeCalConv_clicked();

    void on_actionCalConv_triggered();

    void on_btnCalConv_clicked();

    void on_btnExportDataConv_clicked();

    void on_btnExportDataConv_2_clicked();

    void on_btnBackHome_clicked();

    void on_btnExportDataConv_3_clicked();

    void on_btnGrpOrigin_clicked();

    void on_btnCalNCal_3_clicked();

private:
    void init();
    Ui::MainWindow *ui;
//计算波长及其导数
private:
    RangeData data;
    QList<QList<double> > resultRange;//范围计算结果
    const double compareNum = 1e-10;

private:
    void plot(const QList<QList<double> > resultRange,QCustomPlot* c1,QCustomPlot* c2);
    void exportData(const QString filePath,const QList<QList<double> > data);
    void saveGraph(QCustomPlot *c);
//计算角度
private:
    angleCal ac;
    QList<QList<double> > rstRange;
    bool plotAngle(QwtPlot*,const QString,QList<QDateTime> x,QList<double> y);
    void ExportDataAngle(const QString filePath,const QList<QDateTime> dt, const QList<QList<double> > data);
    void deleteNanData(QList<QDateTime> &x,QList<double> &y);
    void replotQCP(QCustomPlot *);
//计算光谱卷积@ZYX
    SpecConv sc;
};

#endif // MAINWINDOW_H

