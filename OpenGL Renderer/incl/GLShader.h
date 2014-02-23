#ifndef _H_GLSHADER
#define _H_GLSHADER

#include <string>
#include <memory>
#include <vector>
#include <GL\glew.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "FileManager.h"
#include "GLLight.h"

class GLShader
{

public:

		typedef enum
		{
			VERTEX_SHADER = GL_VERTEX_SHADER,
			GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
			FRAGMENT_SHADER = GL_FRAGMENT_SHADER
		} ShaderType;

							GLShader();
							GLShader(const std::string& vertFileName, const std::string& fragFileName);
							GLShader(const std::string& vertFileName, const std::string& geoFileName, const std::string& fragFileName);

		virtual				~GLShader();

		bool				createShader( const std::string& vertFileName, const std::string& fragFileName);
		bool				createShader( const std::string& vertFileName, const std::string& geoFileName, const std::string& fragFileName);

        void                attach()
        {
            glUseProgram(m_shaderProgramHandle);
        }

        void                detach()
        {
            glUseProgram(0);
        }

        void                bindNormalMatrix(float* pMatrix)
        {
            glUniformMatrix4fv(m_NormalMatLocation, 1, GL_FALSE, pMatrix);
        }

        void                bindModelMatrix(float* pMatrix)
        {
            glUniformMatrix4fv(m_ModelMatLocation, 1, GL_FALSE, pMatrix);
        }

        void                bindViewMatrix(float* pMatrix)
        {
            glUniformMatrix4fv(m_ViewMatLocation, 1, GL_FALSE, pMatrix);
        }

        void                bindProjectionMatrix(float* pMatrix)
        {
            glUniformMatrix4fv(m_ProjectionMatLocation, 1, GL_FALSE, pMatrix);
        }

        void                bindAmbientTexture(GLuint textureUnit)
        {
            glUseProgram(m_shaderProgramHandle);

            glUniform1i( glGetUniformLocation(m_shaderProgramHandle, "ambientTexture"), textureUnit );
        }

        void                bindDiffuseTexture(GLuint textureUnit)
        {
            glUseProgram(m_shaderProgramHandle);

            glUniform1i( glGetUniformLocation(m_shaderProgramHandle, "diffuseTexture"), textureUnit );
        }

        void                bindSpecularTexture(GLuint textureUnit)
        {
            glUseProgram(m_shaderProgramHandle);

            glUniform1i( glGetUniformLocation(m_shaderProgramHandle, "specularTexture"), textureUnit );
        }

        void                bindNormalTexture(GLuint textureUnit)
        {
            glUseProgram(m_shaderProgramHandle);

            glUniform1i( glGetUniformLocation(m_shaderProgramHandle, "normalTexture"), textureUnit );
        }

        void                bindAlphaTexture(GLuint textureUnit)
        {
            glUseProgram(m_shaderProgramHandle);

            glUniform1i(glGetUniformLocation(m_shaderProgramHandle, "alphaTexture"), textureUnit);
        }

        void                bindAmbientMaterialComp(const glm::vec3& ambient)
        {
            glUseProgram(m_shaderProgramHandle);

            glUniform3fv( glGetUniformLocation( m_shaderProgramHandle, "Ka"), 1, glm::value_ptr(ambient) );
        }

        void                bindDiffuseMaterialComp(const glm::vec3& diffuse)
        {
            glUseProgram(m_shaderProgramHandle);

            glUniform3fv( glGetUniformLocation(m_shaderProgramHandle, "Kd"), 1, glm::value_ptr(diffuse) );
        }

        void                bindSpecularMaterialComp(const glm::vec3& specular)
        {
            glUseProgram(m_shaderProgramHandle);

            glUniform3fv( glGetUniformLocation(m_shaderProgramHandle, "Ks"), 1, glm::value_ptr(specular) );
        }

        void                bindSpecularMaterialExpo(float specular)
        {
            glUseProgram(m_shaderProgramHandle);

            glUniform1f(glGetUniformLocation(m_shaderProgramHandle, "Ns"),  specular);
        }

        void                bindLightInfoToShader(const UniformLightArray& lightList)
        {
            glUseProgram(m_shaderProgramHandle);

            glUniform3fv(glGetUniformLocation(m_shaderProgramHandle, "lightPositions"), MAX_NUMBER_OF_LIGHTS, reinterpret_cast<const float*>(&lightList.position));

            glUniform3fv(glGetUniformLocation(m_shaderProgramHandle, "lightAmbientComp"), MAX_NUMBER_OF_LIGHTS, reinterpret_cast<const float*>(&lightList.ambientColour));

            glUniform3fv(glGetUniformLocation(m_shaderProgramHandle, "lightDiffuseComp"), MAX_NUMBER_OF_LIGHTS, reinterpret_cast<const float*>(&lightList.diffuseColour));

            glUniform3fv(glGetUniformLocation(m_shaderProgramHandle, "lightSpecularComp"), MAX_NUMBER_OF_LIGHTS, reinterpret_cast<const float*>(&lightList.specularColour));

            glUniform1i(glGetUniformLocation(m_shaderProgramHandle, "numberOfLights"), lightList.numberOfActiveLights);
        }


private:
	
		GLShader(const GLShader&);

		GLShader(const GLShader&&);   

		GLShader& operator=(const GLShader&);

		GLShader& operator=(const GLShader&&);

		bool				loadAndCompileShader(const std::string& sourceFileName,GLuint &shaderHandle,ShaderType shaderType);

		char *				m_shaderSource;

		GLuint				m_vertexShaderHandle;
		GLuint				m_geometryShaderHandle;
		GLuint				m_fragmentShaderHandle;

		GLuint				m_shaderProgramHandle;

		GLint				m_ModelMatLocation;
		GLint				m_ViewMatLocation;
		GLint				m_ProjectionMatLocation;
        GLint               m_NormalMatLocation;

        GLint               m_ColorTextureLocation;
        GLint               m_NormalTextureLocation;
        GLint               m_DiffuseTextureLocation;
        GLint               m_SpecularTextureLocation;
        GLint               m_SpecularHLTextureLocation;
        GLint               m_AlphaTextureLocation;

};

#endif