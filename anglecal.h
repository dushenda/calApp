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
#ifndef ANGLECAL_H
#define ANGLECAL_H

#include <QDateTime>
#include <QList>

class angleCal
{
public:
    angleCal();
private:
    double calZenith(QDateTime dt,double lon,double lat);
    double calAzimuth(QDateTime dt,double lon,double lat);
    double calED(QDateTime BjDt);//计算赤纬
    double calTau(QDateTime BjDt,double lon);//计算时角
    double calSunCape(QDateTime BjDt);//计算日角
    double deg2rad(double deg);
    double rad2deg(double rad);
public:
    QList<double> calAngleSingle(QDateTime dt,double lon,double lat);
    QList<QList<double> > calAngleRange(QDateTime sdt,QDateTime edt,int step,double lon,double lat);
};

#endif // ANGLECAL_H
