#ifndef _H_GLMATERIAL
#define _H_GLMATERIAL

#include "GLTexture.h"
#include "GLShader.h"
#include "glm\glm.hpp"
#include "glm/ext.hpp"
#include "GLLight.h"


class GLMaterial
{
public:

        GLMaterial();

        ~GLMaterial(); 

        void bindMaterial(glm::mat4& m_ModelMatrix, glm::mat4& viewMatrix, glm::mat4& projectionMatrix, glm::mat4& normalMatrix)
        {
            assert(m_MaterialShader);

            normalMatrix = glm::inverseTranspose( viewMatrix);

            m_MaterialShader->attach();
            m_MaterialShader->bindModelMatrix(glm::value_ptr(m_ModelMatrix));
            m_MaterialShader->bindViewMatrix(glm::value_ptr(viewMatrix));
            m_MaterialShader->bindProjectionMatrix(glm::value_ptr(projectionMatrix));
            m_MaterialShader->bindNormalMatrix(glm::value_ptr(normalMatrix));

            if (m_AmbientTexture)
            {
                m_AmbientTexture->bindTexture(GLTexture::AMBIENT_TEXTURE);
            }
            if (m_DiffuseMap)
            {
                m_DiffuseMap->bindTexture(GLTexture::DIFFUSE_TEXTURE);
            }
            if (m_AlphaMap)
            {
                m_AlphaMap->bindTexture(GLTexture::ALPHA_MAP);
            }
        }

        void passLightsToShader(const UniformLightArray& light)
        {
            m_MaterialShader->bindLightInfoToShader(light);
        }

        void setAmbientColor(const glm::vec3&  ambientColor)
        {
            m_AmbientColor = ambientColor;
        }

        void setDiffuseColor(const glm::vec3& diffColor)
        {
            m_DiffuseColor = diffColor;
        }

        void setSpecularColor(const glm::vec3& specColor)
        {
            m_SpecularColor = specColor;
        }

        void setSpecularCoef(float specCoef)
        {
            m_SpecularCoef = specCoef;
        }

        void setAmbientMap(const std::string& filename)
        {
            m_AmbientTexture = new GLTexture(filename);
        }

        void setDiffuseMap(const std::string& filename)
        {
            m_DiffuseMap = new GLTexture(filename);
        }

        void setSpecularMap(const std::string& filename)
        {
            m_SpecularColorMap = new GLTexture(filename);
        }

        void setNormalMap(const std::string& filename)
        {
            m_NormalMap = new GLTexture(filename);
        }

        void setAlphaMap(const std::string& filename)
        {
            m_AlphaMap = new GLTexture(filename);
        }

        void setSpecCoefMap(const std::string& filename)
        {
            m_SpecularHighlightMap = new GLTexture(filename);
        }

        bool isFinalised()
        {
            if (m_MaterialShader)
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        void createShader();

        void testTexture();

        void genDefaultMaterial();

private:

        glm::vec3           m_AmbientColor;
        
        glm::vec3           m_DiffuseColor;

        glm::vec3           m_SpecularColor;

        float               m_SpecularCoef;

        GLShader*           m_MaterialShader;

        GLTexture*          m_AmbientTexture;

        GLTexture*          m_NormalMap;

        GLTexture*          m_DiffuseMap;

        GLTexture*          m_SpecularColorMap;

        GLTexture*          m_SpecularHighlightMap;

        GLTexture*          m_AlphaMap;
};

#endif