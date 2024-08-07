/* 
 * File:   main.cpp
 * Author: mkh
 *
 * Created on 23 января 2023 г., 18:31
 */

#include "service.h"
#include "jpeg.h"
#include "utils.h"

#include <QApplication>
#include "service/qservice.h"
#include "graphics/screens/qscreen.h"

#include <stdlib.h>
#include <signal.h>
#include <getopt.h>

#include <iostream>

namespace
{

static std::unique_ptr< base::service > service;
void signal_handler( int s )
{
    if( service)
    {
        service->onsignal( s );
    }
}

void show_options_and_exit( const char *prog, int rc )
{
    std::cerr << "Запуск: " << prog <<  "[-h] [-s] [-t] [-p] [-q] [-d] [-w] [-o] [-c]\n\noean 3d\n\n";
    std::cerr << "обязательные аргументы:\n";
    std::cerr << "\t-s\tкаталог с шейдерами\n"; 
    std::cerr << "\t-t\tкаталог с текстурами\n"; 
    std::cerr << "Опциональные аргументы:\n";
    std::cerr << "\t-p\tпорт прослушки (def. 2232)\n"; 
    std::cerr << "\t-w\tразмеры окна (def. 800x600)\n";
    std::cerr << "\t-q\tкачество сжатия % (def. 80)\n";
    std::cerr << "\t-d\tдлительность кадра мс (def. 40)\n";
    std::cerr << "\t-c\tфайл конфигурации\n";
    std::cerr << "\t-o\tкаталог с объектами blender\n"; 
    std::cerr << "\t-h\tвывод параметров запуска\n";
    ::exit( rc );   
}
}

int main(int argc, char** argv)
{
    try
    {
        utils::settings( argc, argv );
        utils::settings()["shaders"];
        utils::settings()["textures"];
    }
    catch( const std::runtime_error &e )
    {
        std::cerr << e.what() <<std::endl;
        show_options_and_exit( argv[0], EXIT_FAILURE );
    }

    signal( SIGHUP,  signal_handler );
    signal( SIGTERM, signal_handler );
    signal( SIGSEGV, signal_handler);
    signal( SIGINT,  signal_handler);
    
    try
    {
        QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL, true);
        QApplication a(argc, argv);
        std::unique_ptr< base::screen > scr( new TQscreen( new jpeg::frame( utils::settings()["window"],
                                                                            utils::settings()["quality"],
                                                                            utils::settings()["duration"] ) ) );
        service.reset( new TQService( scr.get(), utils::settings()["port"] ) );

        std::setlocale( LC_NUMERIC,"C" );

        service->start_screen();

        service->stop_screen();
    }
    catch( const std::runtime_error &err )
    {
        std::cerr << "error: " << err.what() << "\n";
    }
    catch( ...)
    {
        std::exception_ptr ep = std::current_exception();
        if( ep )
        {
            try
            {
                std::rethrow_exception( ep );
            }
            catch( const std::exception& e )
            {
                std::cout << "exception: " << e.what() << "\n";
            }
        }
    }

    return (EXIT_FAILURE);
}
