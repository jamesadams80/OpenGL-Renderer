#include "GLVertexBuffer.h"
#include <iostream>

GLVertexBuffer::GLVertexBuffer() : m_vertexBuffer(),
                                   m_texBuffer(),
                                   m_normalBuffer(),
                                   m_NoOfVertex(0),
                                   m_NoOfTexCoord(0),
                                   m_NoOfNormals(0)

{

    glGenBuffers(1, &m_vertexBuffer);
    glGenBuffers(1, &m_texBuffer);
    glGenBuffers(1, &m_normalBuffer);
}

GLVertexBuffer::GLVertexBuffer( const std::vector<float>& vertexVec,
                                const std::vector<float>& texCoordVec,
                                const std::vector<float>& normalVec ) : m_vertexBuffer(),
                                                                        m_texBuffer(),
                                                                        m_normalBuffer(),
                                                                        m_NoOfVertex(0),
                                                                        m_NoOfTexCoord(0),
                                                                        m_NoOfNormals(0)
{

    glGenBuffers(1, &m_vertexBuffer);
    glGenBuffers(1, &m_texBuffer);
    glGenBuffers(1, &m_normalBuffer);

    GLVertexBuffer::loadVertexData(vertexVec, texCoordVec, normalVec);
}

GLVertexBuffer::~GLVertexBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &m_vertexBuffer);
    glDeleteBuffers(1, &m_texBuffer);
    glDeleteBuffers(1, &m_normalBuffer);
}

bool GLVertexBuffer::loadVertexData(const std::vector<float>& vertexVec,
                                    const std::vector<float>& texCoordVec,
                                    const std::vector<float>& normalVec)
{
    GLenum errCode;
    errCode = glGetError();
    // Load vertex buffer
    if (vertexVec.size() > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, (vertexVec.size() * sizeof(float)), &vertexVec[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 0, 0);
    }

    errCode = glGetError();

    m_NoOfVertex = vertexVec.size() / 3;

    if (errCode)
    {
        std::cout << "Vertex Buffer Generation failed errcode: " << errCode << ".\n";
        return false;
    }

    std::cout << "Vertex Data generated.  Number of vertex loaded = " << m_NoOfVertex << "\n";

    // Load texture coord buffer
    if (texCoordVec.size() > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_texBuffer);
        glBufferData(GL_ARRAY_BUFFER, (texCoordVec.size() * sizeof(float)), &texCoordVec[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_TRUE, 0, 0);
    }

    errCode = glGetError();

    m_NoOfTexCoord = texCoordVec.size() / 2;

    if (errCode)
    {
        std::cout << "Tex Coord Buffer Generation failed errcode: " << errCode << ".\n";
        return false;
    }

    std::cout << "Tex Coord Data generated.  Number of vertex loaded = " << m_NoOfTexCoord << "\n";

    // Load normal vector buffer
    if (normalVec.size() > 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, (normalVec.size() * sizeof(float)), &normalVec[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, 0, 0);
    }

    errCode = glGetError();

    m_NoOfNormals = normalVec.size() / 3;

    if (errCode)
    {
        std::cout << "Normal Buffer Generation failed errcode: " << errCode << ".\n";
        return false;
    }

    std::cout << "Normal Data generated.  Number of vertex loaded = " << m_NoOfNormals << "\n";

    return true;
}
