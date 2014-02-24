#ifndef _H_GLVERTEXBUFFER
#define _H_GLVERTEXBUFFER

#include <GL/glew.h>
#include <vector>

class GLVertexBuffer
{
public:
                        GLVertexBuffer();

                        GLVertexBuffer( const std::vector<float>&, const std::vector<float>&, const std::vector<float>& );

                        ~GLVertexBuffer();

        bool            loadVertexData( const std::vector<float>&, const std::vector<float>&, const std::vector<float>& );

        unsigned int    getNoOfVertex()
        {
            return      m_NoOfVertex;
        }

        unsigned int    getNoOfTexVertex()
        {
            return      m_NoOfTexCoord;
        }

        unsigned int    getNoOfNormals()
        {
            return      m_NoOfNormals;
        }

        GLuint getVertexBuffer()
        {
            return      m_vertexBuffer;
        }

        GLuint getTexBuffer()
        {
            return      m_texBuffer;
        }

        GLuint getNormalBuffer()
        {
            return      m_normalBuffer;
        }

private:

        GLuint      m_vertexBuffer;

        GLuint      m_texBuffer;

        GLuint      m_normalBuffer;	
        
        int         m_NoOfVertex;

        int         m_NoOfTexCoord;

        int         m_NoOfNormals;
};

#endif