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
#ifndef RANGEDATA_H
#define RANGEDATA_H
#include <QList>
#include "math.h"

using namespace std;

class RangeData
{
private:
    QList<double> DataN;
    QList<double> DataDiff;
    QList<double> DataWavelength;

    double DataSingleN;
    double DataSingleDiff;
    //计算公式中的固定参数
private:
    const double p1 = 0.6961663;
    const double p2 = -pow(0.0684043,2);
    const double p3 = 0.4079426;
    const double p4 = -pow(0.1162414,2);
    const double p5 = 0.8974794;
    const double p6 = -pow(9.896161,2);
    //计算导数时候的步长选取，使用导数定义计算
    const double diffstep = 1e-10;
private:
    double calN(double );
    double calDiff(double );
public:
    RangeData();
//一定波长范围的
public:
    QList<QList<double> > getData();
    void setData(double lower,double upper,double step);
//单个波长的
public:
    QList<double> getSingleData();
    void setSingleData(double );
};

#endif // RANGEDATA_H
