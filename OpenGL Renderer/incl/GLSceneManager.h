#ifndef _H_SCENEMANAGER
#define _H_SCENEMANAGER

#include "GLCamera.h"
#include "GLFirstPersonCamera.h"
#include "GLLight.h"
#include "GLObjLoader.h"
#include "GLRenderNode.h"
#include <vector>
#include "GL/glew.h"
#include <Windows.h>

class GLSceneManager
{

public:
		                        GLSceneManager();

		                        ~GLSceneManager();

		bool                    initTestScene();

		void                    renderScene();

        void                    addRenderNode( GLRenderNode* renderNode )
        {
            m_RenderNodeList.push_back(renderNode);
        }

        void                    addLight( GLLight& light )
        {
            m_LightList.push_back(light);
        }

		GLFirstPersonCamera*	getPlayerCamera()
		{
			return &m_currentCamera;
		}

private:

		GLSceneManager( const GLSceneManager& );
		GLSceneManager( const GLSceneManager&& );

		GLSceneManager& operator= (const GLSceneManager&);
		GLSceneManager& operator = (const GLSceneManager&&);   

        void                        populateUniformLightArray();

		GLFirstPersonCamera			m_currentCamera;

        std::vector<GLRenderNode*>  m_RenderNodeList;

        std::vector<GLRenderNode*>::const_iterator m_RenderNodeIter;

        std::vector<GLLight>		m_LightList;

        UniformLightArray           m_LightUniformArray;

		glm::mat4					m_worldMatrix;

		glm::mat4					m_ProjectionMatrix;

		glm::mat4					m_ViewMatrix;

        glm::mat4                   m_NormalMatrix;
};

#endif