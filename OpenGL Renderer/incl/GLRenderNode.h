#ifndef _H_GLRENDERGROUP
#define _H_GLRENDERGROUP

#include <GL/glew.h>
#include <vector>
#include "GLVertexBuffer.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include "GLMaterial.h"

class GLRenderNode
{

    public:

        enum PrimitiveType
        {
            POINTS,
            LINES,
            LINE_LOOP,
            LINE_STRIP,
            TRIANGLES,
            TRIANGLE_STRIP,
            TRIANGLE_FAN,
            QUADS,
            QUAD_STRIP,
            POLYGON,
            INVALID_TYPE
        };

        GLRenderNode();

        GLRenderNode( PrimitiveType );

        GLRenderNode(std::vector<float>&, std::vector<float>&, std::vector<float>&, std::shared_ptr<GLMaterial> nodeMaterial, PrimitiveType);

        ~GLRenderNode();

        void setPosition(glm::vec3 pos)
        {
            m_PositionMatrix = glm::translate(m_PositionMatrix, pos);
        }

        void setScale(glm::vec3 pos)
        {
            m_PositionMatrix = glm::scale(m_PositionMatrix, pos);
        }
 
        void setRotation( glm::vec3 axis, float rad)
        {
            m_PositionMatrix = glm::rotate(m_PositionMatrix, rad, axis);
        }


        void bindLightsToShader(const UniformLightArray& light)
        {
            m_NodeMaterial->passLightsToShader(light);
        }

        void renderNode( glm::mat4& viewMatrix, glm::mat4& projectionMatrix, glm::mat4& normalMatrix )
        {
            glBindVertexArray(m_NodeVAO);
            m_NodeMaterial->bindMaterial(m_PositionMatrix, viewMatrix, projectionMatrix, normalMatrix);
            glDrawArrays(m_PrimitiveType, 0,m_VertexBuffer.getNoOfVertex());
        }

        PrimitiveType getPrimitiveType()
        {
            return          m_PrimitiveType;
        }

        void generateVertexBuffers(std::vector<float>& verts, std::vector<float>& texcoords, std::vector<float>& norms);

	private:

        GLuint          m_NodeVAO; 

        glm::mat4       m_PositionMatrix;

        std::shared_ptr<GLMaterial> m_NodeMaterial;

        GLVertexBuffer  m_VertexBuffer;

        PrimitiveType   m_PrimitiveType;

};


#endif
