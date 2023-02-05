/* 
 * File:   qservice.cpp
 * Author: mkh
 * 
 * Created on 5 февраля 2023 г., 16:47
 */

#include "qservice.h"
#include "../graphics/screens/qscreen.h"
#include <QApplication>

qservice::qservice( int argc, char * argv[] )
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(3,3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL, true);
    QApplication::setAttribute(Qt::AA_ForceRasterWidgets, false);

    screen_.reset( new qscreen( frame_.get() ) );
}

void qservice::f_run()
{
    result_ = qApp->exec();
}

int qservice::f_stop()
{
    screen_.reset();
    return result_;
}

void qservice::onsignal( int )
{
    qApp->quit();
}