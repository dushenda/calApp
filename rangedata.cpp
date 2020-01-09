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
#include "rangedata.h"


RangeData::RangeData()
{

}
//用途：计算折射率
//输入：波长
//输出：折射率
double RangeData::calN(double wv)
{
    double wv2 = pow(wv,2.0);
    double N = sqrt(1+p1*wv2/(wv2+p2)+p3*wv2/(wv2+p4)+p5*wv2/(wv2+p6));
    return N;
}
//用途：计算折射率一阶导
//输入：波长
//输出：一阶导数
double RangeData::calDiff(double wv)
{
    double diff = (calN(wv+diffstep)-calN(wv))/diffstep;
    return diff;
}
//用途：分开计算原函数和导函数，再合并数据
//输入：波长
//输出：无，数据保存至私有成员数据，由公有成员方法获取
void RangeData::setSingleData(double wv)
{//计算单波长的结果
    DataSingleN = calN(wv);
    DataSingleDiff = calDiff(wv);
}
//用途：用来获取单波长计算数据
//输入：无（实际上由函数去获取成员数据）
//输出：double容器数据
//备注：此函数应该放在setSingleData之后
QList<double> RangeData::getSingleData()
{
    QList<double> DataAll = {DataSingleN,DataSingleDiff};
    return DataAll;
}
//用途：计算一定波长范围的折射率和一阶导数
//输入：波长的下限，波长的上限，设置的波长的步长
//输出：无（实际上将数据保存至私有成员变量DataWavelength，DataN，DataDiff）
//备注：函数实际上是通过循环调用单波长计算函数实现功能的
void RangeData::setData(const double lower, const double upper, const double step)
{
    DataN.clear();
    DataDiff.clear();
    DataWavelength.clear();
    double s = step/1000;//纳米转微米
    double wv = lower;
    while (wv<upper) {
        DataWavelength.append(wv);
        DataN.append(calN(wv));
        DataDiff.append(calDiff(wv));
        wv = wv+s;
    }
}
//用途：获取范围波长计算数据
//输入：无
//输出：多个系列数据，波长，折射率，一阶导数的列表值
//备注：此函数应该放在setData之后，否则取到的值会出错
QList<QList<double> > RangeData::getData()
{
    QList<QList<double> > DataAll = {DataWavelength,DataN,DataDiff};
    return DataAll;
}
