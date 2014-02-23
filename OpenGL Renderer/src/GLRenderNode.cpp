#include "GLRenderNode.h"
#include <iostream>

GLRenderNode::GLRenderNode( ) : m_NodeVAO(0),
                                m_PositionMatrix(glm::mat4()),
                                m_NodeMaterial(),
                                m_VertexBuffer(),
                                m_PrimitiveType(INVALID_TYPE)
{
    glGenVertexArrays(1, &m_NodeVAO);
}

GLRenderNode::GLRenderNode( PrimitiveType primitive ) : m_NodeVAO(0),
                                                        m_PositionMatrix(glm::mat4()),
                                                        m_NodeMaterial(),
                                                        m_VertexBuffer(),
                                                        m_PrimitiveType(primitive)
{ 
    glGenVertexArrays(1, &m_NodeVAO);
    m_PositionMatrix = glm::rotate(m_PositionMatrix, 90.0f, glm::vec3(0, 1, 0));
}

GLRenderNode::GLRenderNode(std::vector<float>& verts,
                            std::vector<float>& texcoords,
                            std::vector<float>& norms,
                            std::shared_ptr<GLMaterial> nodeMaterial,
                            PrimitiveType primitive) 
                            : m_NodeVAO(0),
                              m_PositionMatrix(glm::mat4()),
                              m_NodeMaterial(nodeMaterial),
                              m_VertexBuffer( ),
                              m_PrimitiveType(primitive)
{
    glGenVertexArrays(1, &m_NodeVAO);
    glBindVertexArray(m_NodeVAO);

    m_VertexBuffer.loadVertexData(verts, texcoords, norms);

    //m_PositionMatrix = glm::rotate(m_PositionMatrix, 90.0f, glm::vec3(-1, -1, 0));
     m_PositionMatrix = glm::scale(m_PositionMatrix, 0.1f, 0.1f, 0.1f);
}


GLRenderNode::~GLRenderNode()
{
    glDeleteVertexArrays(1, &m_NodeVAO);
}

void GLRenderNode::generateVertexBuffers(std::vector<float>& verts, std::vector<float>& texcoords, std::vector<float>& norms)
{
    glBindVertexArray(m_NodeVAO);

    m_VertexBuffer.loadVertexData(verts, texcoords, norms);

    m_NodeMaterial->testTexture();

    m_PositionMatrix = glm::rotate(m_PositionMatrix, 90.0f, glm::vec3(-1, 0, 0));
}