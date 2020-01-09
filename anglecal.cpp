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
#include "anglecal.h"
#include <math.h>
#include <QDebug>

angleCal::angleCal()
{

}

QList<double> angleCal::calAngleSingle(QDateTime dt, double lon, double lat)
{
    QList<double> angle;
    double zenith = calZenith(dt,lon,lat);
    double azimuth = calAzimuth(dt,lon,lat);
    angle << zenith<<azimuth;
    return angle;
}

QList<QList<double> > angleCal::calAngleRange(QDateTime sdt, QDateTime edt, int step, double lon, double lat)
{//step单位为秒
    QList<QList<double> > angle;
    QList<double> zenithAngle;
    QList<double> azimuthAngle;

    for(QDateTime dt=sdt;dt<edt;dt=dt.addSecs(step))
    {
//        qDebug()<<dt;
        double zenith = calZenith(dt,lon,lat);
        double azimuth = calAzimuth(dt,lon,lat);
        zenithAngle<<zenith;
        azimuthAngle<<azimuth;
    }
    angle << zenithAngle<<azimuthAngle;
    return angle;
}
//计算天顶角(deg)
//输入北京时间，经度（deg），纬度（deg）
double angleCal::calZenith(QDateTime BjDt, double lon, double lat)
{
    lat = deg2rad(lat);
    double ED = calED(BjDt);
    double tau = calTau(BjDt,lon);
    double sinh = sin(ED)*sin(lat)+cos(ED)*cos(lat)*cos(tau); // 太阳高度角
    double zenith = rad2deg(acos(sinh));
    return zenith;
}
//计算方位角(deg)
//输入北京时间，经度（deg），纬度（deg）
double angleCal::calAzimuth(QDateTime BjDt, double lon, double lat)
{
    lat = deg2rad(lat);
    double ED = calED(BjDt);
    double tau = calTau(BjDt,lon);
    double sinh = sin(ED)*sin(lat)+cos(ED)*cos(lat)*cos(tau); // 太阳高度角
    double cosh = sqrt(1-pow(sinh,2));
    double cosA = (sin(ED)-sinh*sin(lat))/(cosh*cos(lat));
    double azimuth = acos(cosA);
    azimuth = rad2deg(azimuth);
    if(tau>0)
        azimuth = 360-azimuth;
    return  azimuth;
}
//计算赤纬(rad)
//输入北京时间
double angleCal::calED(QDateTime BjDt)
{
    double theta = calSunCape(BjDt);
    double ED = 0.3723+23.2567*sin(theta)+0.1149*sin(2*theta)-0.1712*sin(3*theta)-0.758*cos(theta)+0.3656*cos(2*theta)+0.0201*cos(3*theta);
    ED = deg2rad(ED);
    return  ED;
}
//计算时角
//输入北京时间，经度(deg)
double angleCal::calTau(QDateTime BjDt,double lon)
{
    double theta = calSunCape(BjDt);
    double Et = 0.0028-1.9857*sin(theta)+9.9059*sin(2*theta)-7.0924*cos(theta)-0.6882*cos(2*theta);// 计算时差
    double gapLonMinute = (120 - lon)*4*60;// 二者相差的经度
    BjDt = BjDt.addSecs(-int(gapLonMinute));// 地方时获取
    BjDt = BjDt.addSecs(int(Et*60));
    double tau = (double(BjDt.time().hour())+double(BjDt.time().minute())/60-12)*15;
    tau = deg2rad(tau);
    return tau;
}
//计算日角(rad)
//输入北京时间
double angleCal::calSunCape(QDateTime BjDt)
{
    int N = BjDt.date().dayOfYear();//计算年积日
    double N0 = 79.6764+0.2422*(BjDt.date().year()-1985)-floor((BjDt.date().year()-1985)/4);
    double t = N-N0;
    double theta = 2*M_PI*t/365.2422; // 日角
    return theta;
}

double angleCal::deg2rad(double deg)
{
    return deg*M_PI/180;
}

double angleCal::rad2deg(double rad)
{
    return rad*180/M_PI;
}
