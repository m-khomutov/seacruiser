/* 
 * File:   qservice.cpp
 * Author: mkh
 * 
 * Created on 5 февраля 2023 г., 16:47
 */

#include "qservice.h"
#include <QApplication>
#include <QSurfaceFormat>

qservice::qservice( basescreen *screen )
: baseservice( screen )
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3,3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    QSurfaceFormat::setDefaultFormat(format);
    QApplication::setAttribute(Qt::AA_ForceRasterWidgets, false);
}

void qservice::f_run()
{
    result_ = qApp->exec();
}

int qservice::f_stop()
{
    return result_;
}

void qservice::onsignal( int )
{
    qApp->quit();
}
