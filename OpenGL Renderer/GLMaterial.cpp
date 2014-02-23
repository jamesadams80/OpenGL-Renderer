#include "GLMaterial.h"

GLMaterial::GLMaterial() : m_AmbientColor(glm::vec3(1.0f,1.0f,1.0f)),
                           m_DiffuseColor(glm::vec3(1.0f, 1.0f, 1.0f)),
                           m_SpecularColor(glm::vec3(1.0f, 1.0f, 1.0f)),
                           m_SpecularCoef(1.0f),
                           m_MaterialShader(NULL),
                           m_AmbientTexture(NULL),
                           m_NormalMap(NULL),
                           m_DiffuseMap(NULL),
                           m_SpecularColorMap(NULL),
                           m_SpecularHighlightMap(NULL),
                           m_AlphaMap(NULL)

{
    // Creates default color which is just going to be white.  
}

GLMaterial::~GLMaterial()
{
    if (m_AmbientTexture)
    {
        delete m_AmbientTexture;
    }

    if (m_NormalMap)
    {
        delete m_NormalMap;
    }

    if (m_DiffuseMap)
    {
        delete m_DiffuseMap;
    }

    if (m_SpecularColorMap)
    {
        delete m_SpecularColorMap;
    }

    if (m_SpecularHighlightMap)
    {
        delete m_SpecularHighlightMap;
    }

    if (m_AlphaMap)
    {
        delete m_AlphaMap;
    }

    if (m_MaterialShader)
    {
        delete m_MaterialShader;
    }
}

void GLMaterial::testTexture()
{
    m_AmbientTexture = new GLTexture("resource/Dassault_Mirage-2000_N_P01.png");

    createShader();

    m_MaterialShader->bindAmbientTexture(0);
}

void GLMaterial::createShader()
{
    if (m_MaterialShader)
    {
        delete m_MaterialShader;
    }

    if (m_AmbientTexture && m_DiffuseMap && m_AlphaMap)
    {
        m_MaterialShader = new GLShader("shaders/ambientdiffusealphalight.vert", "shaders/ambientdiffusealphalight.frag");
        m_MaterialShader->bindAmbientTexture(GLTexture::AMBIENT_TEXTURE);
        m_MaterialShader->bindDiffuseTexture(GLTexture::DIFFUSE_TEXTURE);
        m_MaterialShader->bindAlphaTexture(GLTexture::ALPHA_MAP);
    }
    else if (m_AmbientTexture && m_DiffuseMap)
    {
        m_MaterialShader = new GLShader("shaders/ambientdiffuselight.vert", "shaders/ambientdiffuselight.frag");
        m_MaterialShader->bindAmbientTexture(GLTexture::AMBIENT_TEXTURE);
        m_MaterialShader->bindAlphaTexture(GLTexture::ALPHA_MAP);
    }
    else if (m_AmbientTexture)
    {
        m_MaterialShader = new GLShader("shaders/texturenolight.vert", "shaders/texturenolight.frag");
        m_MaterialShader->bindAmbientTexture(GLTexture::AMBIENT_TEXTURE);
    }
    else if (m_DiffuseMap)
    {
        m_MaterialShader = new GLShader("shaders/diffuselight.vert", "shaders/diffuselight.frag");
        m_MaterialShader->bindAmbientTexture(GLTexture::DIFFUSE_TEXTURE);
    }
    else
    {
        m_MaterialShader = new GLShader("shaders/test.vert", "shaders/test.frag");
    }

    // Shader has been created sec material components....

    m_MaterialShader->bindAmbientMaterialComp(m_AmbientColor);
    m_MaterialShader->bindDiffuseMaterialComp(m_DiffuseColor);
    m_MaterialShader->bindSpecularMaterialComp(m_SpecularColor);
    m_MaterialShader->bindSpecularMaterialExpo(m_SpecularCoef);
}

void GLMaterial::genDefaultMaterial()
{
    if (m_MaterialShader)
    {
        delete m_MaterialShader;
    }

    m_AmbientColor = glm::vec3(1.0f, 0.0f, 1.0f);
    m_SpecularColor = glm::vec3(1.0f, 0.0f, 1.0f);
    m_AmbientColor = glm::vec3(1.0f, 1.0f, 1.0f);


    m_AmbientTexture = new GLTexture();
    m_AmbientTexture->createXORTexture();

    m_MaterialShader = new GLShader("shaders/texturenolight.vert", "shaders/texturenolight.frag");

    m_MaterialShader->bindAmbientTexture(GLTexture::AMBIENT_TEXTURE);
}