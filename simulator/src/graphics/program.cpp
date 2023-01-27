/* 
 * File:   program.cpp
 * Author: mkh
 * 
 * Created on 24 января 2023 г., 10:55
 */

#include "program.h"
#include <dirent.h> 
#include <string.h> 
#include <iostream>


program_error::program_error( const std::string &what )
: std::runtime_error( what )
{
}

program::program( const std::string & shader_dir )
: id_( glCreateProgram() )
{
    try
    {
        DIR *dir;
        struct dirent *entry;

    	if( !(dir = opendir( shader_dir.c_str() ) ) )
            throw program_error( shader_dir + std::string(" error: ") + std::string(strerror( errno ) ) );

        while( (entry = readdir(dir))  )
        {
            if( entry->d_type == DT_REG && strstr( entry->d_name, ".glsl" ) )
            {
                f_emplace_shader( shader_dir + "/" + entry->d_name );
            }
        }
        closedir( dir );
    }
    catch( const std::runtime_error & err )
    {
    	glDeleteProgram( id_ );
        throw;
    }
    f_link();
}

program::~program()
{
    glDeleteProgram( id_ );
}

void program::f_emplace_shader( const std::string & path )
{
    shaders_.emplace_back( new shader( path ) );
    glAttachShader( id_, *shaders_.back() );
}

void program::f_link()
{
    if( shaders_.empty() )
    {
        throw program_error( "no shaders" );
    }
    glLinkProgram( id_ );
    f_check();
    
    GLint linked;
    glGetProgramiv( id_, GL_LINK_STATUS, &linked );
    if( linked != 1 )
    {
        f_throw_error();
    }
    shaders_.clear();
    f_get_attributes();
    f_get_uniforms();
    f_get_uniform_blocks();
}

GLuint program::attribute_index( const std::string &name ) const
{
    auto attr = attributes_.find( name );
    if( attr == attributes_.end() )
        throw program_error( std::string("attribute '" + name + "' not found") );
    return (*attr).second.first;
}

GLuint program::uniform_index( const std::string &name ) const
{
    auto attr = uniforms_.find( name );
    if( attr == uniforms_.end() )
        throw program_error( std::string("uniform '" + name + "' not found") );
    return (*attr).second.first;
}

/*UniformBlock *program::uniformBlock(const std::string &name) {
    auto blk = m_uniform_blocks.find( name );
    if( blk == m_uniform_blocks.end() )
        throw ProgramException( std::string("uniform block'" + name + "' not found") );
    return (*blk).second.get();
}*/

void program::f_check() {
    int glErr = glGetError();
    std::string what;
    while( glErr != GL_NO_ERROR ) {
        what.append( std::to_string( glErr ) + " ");
        glErr = glGetError();
    }
    if( !what.empty() )
        throw program_error( what );
}

void program::f_throw_error() {
    int len = 0;
    glGetProgramiv( id_, GL_INFO_LOG_LENGTH, &len );
    if( len > 0 ) {
        int chWrittn = 0;
        std::vector< char > log( len, 0 );
        glGetProgramInfoLog( id_, len, &chWrittn, log.data() );
        throw program_error( log.data() );
    }
}

void program::f_get_attributes() {
    GLint num;
    glGetProgramInterfaceiv( id_, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &num );
    GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };
    for( int i(0); i < num; ++i ) {
        GLint results[3];
        glGetProgramResourceiv( id_, GL_PROGRAM_INPUT, i, 3, properties, 3, nullptr, results );
        std::string name( results[0] + 1, ' ' );
        glGetProgramResourceName( id_, GL_PROGRAM_INPUT, i, results[0] + 1, nullptr, name.data() );
        attributes_.emplace( std::string(name.data()), std::make_pair( results[2], results[1] ) );
    }
    std::cerr << "attributes: \n";
    for( auto a : attributes_ )
        std::cerr << "\t'" << a.first << "': index=" << a.second.first << " type=" << a.second.second << std::endl;
}

void program::f_get_uniforms() {
    GLint num;
    glGetProgramInterfaceiv( id_, GL_UNIFORM, GL_ACTIVE_RESOURCES, &num );
    GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };
    for( int i(0); i < num; ++i ) {
        GLint results[4];
        glGetProgramResourceiv( id_, GL_UNIFORM, i, 4, properties, 4, nullptr, results );
        std::string name( results[0] + 1, ' ' );
        glGetProgramResourceName( id_, GL_UNIFORM, i, results[0] + 1, nullptr, name.data() );
        if( results[3] == -1 ) {
            uniforms_.emplace( std::string(name.data()), std::make_pair( results[2], results[1] ) );
        }
        else
            std::cerr << "Blocked uniform: " << name << std::endl;
    }
    std::cerr << "uniforms: \n";
    for( auto a : uniforms_ )
        std::cerr << "\t'" << a.first << "': index=" << a.second.first << " type=" << a.second.second << std::endl;
}

void program::f_get_uniform_blocks() {
    GLint num;
    glGetProgramInterfaceiv( id_, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &num );
    GLenum props[] = { GL_NAME_LENGTH };
    for( int i(0); i < num; ++i ) {
        GLint results[2];
        glGetProgramResourceiv( id_, GL_UNIFORM_BLOCK, i, 1, props, 1, nullptr, results );
        std::string name( results[0] + 1, ' ' );
        glGetProgramResourceName( id_, GL_UNIFORM_BLOCK, i, results[0] + 1, nullptr, name.data() );
        //uniform_blocks_.emplace( std::string(name.data()), new UniformBlock( m_ID, name.data() ) );
    }
    //std::cerr << "Uniform blocks: \n";
    //for( auto a : uniform_blocks_ )
    //    std::cerr << "\t'" << a.first << "'" << std::endl;
}