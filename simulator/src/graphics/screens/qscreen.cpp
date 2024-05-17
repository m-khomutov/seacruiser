/* 
 * File:   qscreen.cpp
 * Author: mkh
 * 
 * Created on 5 февраля 2023 г., 14:43
 */

#include "qscreen.h"
#include "json.h"
#include <QGridLayout>

TQscreen::TQscreen()
{
    QGridLayout *mainLayout = new QGridLayout;

    try
    {
        // создать сцены (расположение в файле из конфигурации screen_layout), отображаемые экраном
        const auto &layout = NUtils::scene_config_t(NUtils::TConfig()["screen_layout"]);
        for( const auto &cfg : layout )
        {
            try
            {
                sc_.emplace_back( new TScene( cfg ) );
                mainLayout->addWidget( sc_.back(), cfg.y,  cfg.x );
            }
            catch( const std::exception &scene_err ) {
                qDebug() << scene_err.what();
            }
        }
    }
    catch( const std::exception &e )
    {
        qDebug() << e.what();
    }

    setLayout(mainLayout);
}

void TQscreen::closeEvent( QCloseEvent *event )
{
    for( auto & scene : sc_ )
    {
        scene->clear();
    }

    QWidget::closeEvent( event );
}

void TQscreen::f_run_scene_display()
{
    // отобразить окно на дисплее
    show();
    // запустить таймер
    timer_id_ = startTimer(0);
}

void TQscreen::f_stop_scene_display()
{
    // остановить таймер
    if( timer_id_ != -1 )
    {
        killTimer( timer_id_ );
    }
    timer_id_ = -1;
}

// Обработка события таймера
void TQscreen::timerEvent( QTimerEvent * )
{
    if( timer_id_ != -1 )
    {
        // выполнить команды, если есть
        f_exec_command();

        for( size_t s(0); s < sc_.size(); ++s )
        {
            sc_[s]->update();
        }
    }
}
