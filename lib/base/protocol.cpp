/* 
 * File:   baseprotocol.cpp
 * Author: mkh
 * 
 * Created on 25 января 2023 г., 14:59
 */

#include "protocol.h"
#include "flv.h"
#include "mjpeg.h"
#include "api.h"

#include <cstring>
#include <cerrno>
#include <iostream>

base::protocolerror::protocolerror( const std::string &what )
    : std::runtime_error( what + std::string(" failed: ") + std::string(strerror( errno )) )
{}

base::protocol *base::protocol::create( base::screen *screen, const std::string &request, int sock, int flags )
{
    if( request.find( "\r\n\r\n" ) != std::string::npos )
    {
        auto params = http::parameter::parse( request );

        if( request.find( "GET /stream?" ) != std::string::npos )
        {
            const std::string *proto = nullptr;
            size_t view = 0;
            for( const auto &p : params )
            {
                if( p.field == "proto" )
                {
                    proto = &p.value;
                }
                else if( p.field == "view" && std::isdigit( p.value[0] ) )
                {
                    view = p.value[0] - '0';
                }
            }
            if( proto )
            {
                if( *proto == "flv" )
                {
                    return new flv::protocol( sock, flags, view );
                }
                else if( *proto == "mjpeg" )
                {
                    return new mjpeg::protocol( sock, flags, view );
                }
            }
        }
        if( request.find( "GET /scene?" ) != std::string::npos ) // запрос на смену сцены
        {
            return new api::protocol( sock, flags, screen );
        }
    }
    return nullptr;
}


const char * base::protocol::status_200 = "HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nContent-Type: application/json\r\n";
const char * base::protocol::status_404 = "HTTP/1.1 404 Not Found\r\nAccess-Control-Allow-Origin: *\r\n\r\n";


base::protocol::protocol( int sock, int flags )
    : fd_( sock )
    , flags_( flags )
{}



http::parameter::parameter( const std::string &request, size_t begin, size_t end )
{
    size_t eq = request.find( '=', begin );
    if( eq != std::string::npos )
    {
        field.assign( request, begin, eq - begin );
        value.assign( request, eq + 1, (end - (eq + 1) ) );
    }
    else
    {
        field.assign( request, begin, end - begin );
    }
}

std::vector< http::parameter > http::parameter::parse( const std::string &request )
{
    std::vector< http::parameter > params;

    size_t begin = request.find( '?' );
    if( begin != std::string::npos )
    {
        ++begin;

        size_t end = std::string::npos;
        while( (end = request.find( '&', begin )) != std::string::npos )
        {
            params.emplace_back( request, begin, end );
            begin = end + 1;
        }

        params.emplace_back( request, begin, request.find( ' ', begin ) );
    }

    return params;
}