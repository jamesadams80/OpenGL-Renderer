#ifndef _GLTEXTURE_H_
#define _GLTEXTURE_H_

#include <string>
#include "GL\glew.h"


class GLTexture
{
public:

        typedef enum
        {
            AMBIENT_TEXTURE = 0,
            NORMAL_TEXTURE,
            DIFFUSE_TEXTURE,
            SPECULAR_TEXTURE,
            SPECULAR_COEF_TEXTURE,
            ALPHA_MAP
        } TextureUnitIndex;
			
                    GLTexture();
                    GLTexture( std::string );

                    ~GLTexture();

        bool        loadTexture( std::string );

        bool        loadJPGTexture( std::string );

        bool        createXORTexture();

        void        bindTexture(TextureUnitIndex textureUnit)
        {
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, m_textureHandle);
        }

private:

        std::string     m_filename;

        GLuint      m_textureHandle;
};

#endif