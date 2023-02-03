/* 
 * File:   figure.h
 * Author: mkh
 *
 * Created on 31 января 2023 г., 14:35
 */

#ifndef FIGURE_H
#define FIGURE_H

#include "../texture.h"
#include "../program.h"
#include "../../../../share/utils.h"
#include <GL/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class figure {
public:
    figure();
    figure(const figure& orig) = delete;
    figure &operator=(const figure& orig) = delete;
    virtual ~figure();

    void initialize();
    void draw( double currentTime );

protected:   
    std::unique_ptr< program > program_;
    std::unique_ptr< texture > texture_;    
    glm::mat4 model_ { glm::rotate( glm::mat4(1.0f), glm::radians( 0.0f ), glm::vec3(1.0f, 0.0f, 0.0f) ) };
    glm::mat4 view_ { glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f) ) };
    glm::mat4 projection_;
    float angle_ = 0.0f;
    float scene_position_ { 0.0f };
    float speed_ { 0.001f };
    float direction_ { -1.0f };
    glm::vec3 offset_ { 0.0f, 0.0f, 0.0f };

protected:
    void set_attribute( const GLchar * name, float value );
    void set_attribute( const GLchar * name, GLuint value );
    void set_attribute( const GLchar * name, glm::vec3 );
    void set_attribute( const GLchar * name, glm::vec4 );
    void set_attribute( const GLchar * name, glm::mat3 );
    void set_attribute( const GLchar * name, glm::mat4 );
    void set_subroutine( const GLchar * uniform_name, const GLchar * subroutine_name, GLenum shader_type );

private:
    virtual char const *f_shader_name() const = 0;
    virtual void f_initialize() = 0;
    virtual void f_draw( double currentTime ) = 0;
};

#endif /* FIGURE_H */
