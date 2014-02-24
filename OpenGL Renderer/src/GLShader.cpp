#include "GLShader.h"

GLShader::GLShader() : m_shaderSource(0),
                       m_vertexShaderHandle(0),
                       m_geometryShaderHandle(0),
                       m_fragmentShaderHandle(0),
                       m_shaderProgramHandle(0)

{
}

GLShader::GLShader(const std::string& vertFileName,const std::string& fragFileName) : m_shaderSource(0),
                                                                                      m_vertexShaderHandle(0),
                                                                                      m_geometryShaderHandle(0),
                                                                                      m_fragmentShaderHandle(0),
                                                                                      m_shaderProgramHandle(0)
{
    createShader(vertFileName, fragFileName);
}

GLShader::GLShader(const std::string& vertFileName,const std::string& geoFileName,const std::string& fragFileName) : m_shaderSource(0),
                                                                                                                     m_vertexShaderHandle(0),
                                                                                                                     m_geometryShaderHandle(0),
                                                                                                                     m_fragmentShaderHandle(0),
                                                                                                                     m_shaderProgramHandle(0)
{
    createShader(vertFileName, geoFileName, fragFileName);
}

GLShader::~GLShader()
{
    glUseProgram(0);
    glDeleteProgram(m_shaderProgramHandle);

    if(m_vertexShaderHandle)
    {
        glDeleteShader(m_vertexShaderHandle);
    }

    if(m_geometryShaderHandle)
    {
        glDeleteShader(m_geometryShaderHandle);
    }

    if(m_fragmentShaderHandle)
    {
        glDeleteShader(m_fragmentShaderHandle);
    }
}

bool GLShader::createShader(const std::string& vertFileName,const std::string& fragFileName)
{
    GLint shaderCompilationStatus;
    GLint programLinkStatus;

    if(!loadAndCompileShader(vertFileName,m_vertexShaderHandle,VERTEX_SHADER))
    {
        return false;
    }

    glGetShaderiv(m_vertexShaderHandle,GL_COMPILE_STATUS,&shaderCompilationStatus);

    if(shaderCompilationStatus == GL_FALSE)
    {
        std::cout << "Failed to compile vertex shader " << vertFileName << std::endl;
        return false;
    }

    if(!loadAndCompileShader(fragFileName,m_fragmentShaderHandle,FRAGMENT_SHADER))
    {
        return false;
    }

    glGetShaderiv(m_fragmentShaderHandle,GL_COMPILE_STATUS,&shaderCompilationStatus);

    if(shaderCompilationStatus == GL_FALSE)
    {
        std::cout << "Failed to compile fragment shader " << fragFileName << std::endl;
        return false;
    }

    m_shaderProgramHandle = glCreateProgram();

    glAttachShader(m_shaderProgramHandle,m_vertexShaderHandle);
    glAttachShader(m_shaderProgramHandle,m_fragmentShaderHandle);

    glLinkProgram(m_shaderProgramHandle);

    glGetProgramiv(m_shaderProgramHandle,GL_LINK_STATUS,&programLinkStatus);

    if(programLinkStatus == GL_FALSE)
    {
        return false;
    }

    //TODO:  Could write something to parse code and get uniforms dynamically?  For now this will do as a little hack job.

    m_ModelMatLocation = glGetUniformLocation(m_shaderProgramHandle,"ModelMatrix");
    m_ViewMatLocation = glGetUniformLocation(m_shaderProgramHandle,"ViewMatrix");
    m_ProjectionMatLocation = glGetUniformLocation(m_shaderProgramHandle,"ProjectionMatrix");
    m_NormalMatLocation = glGetUniformLocation(m_shaderProgramHandle, "NormalMatrix");

    std::cout << "ModelMatrix location " << m_ModelMatLocation << " ViewMatrix location " << m_ViewMatLocation << " ProjectionMatrix location " << m_ProjectionMatLocation <<  std::endl;

    return true;
}

bool GLShader::createShader(const std::string& vertFileName, const std::string& geoFileName, const std::string& fragFileName)
{
    GLint shaderCompilationStatus;
    GLint programLinkStatus;

    if(!loadAndCompileShader(vertFileName,m_vertexShaderHandle,VERTEX_SHADER))
    {
        std::cout << "Failed to compile vertex shader " << vertFileName << std::endl;
        return false;
    }

    glGetShaderiv(m_vertexShaderHandle,GL_COMPILE_STATUS,&shaderCompilationStatus);

    if(shaderCompilationStatus == GL_FALSE)
    {
        std::cout << "Failed to compile vertex shader" << vertFileName << std::endl;
        return false;
    }

    if(!loadAndCompileShader(geoFileName,m_geometryShaderHandle,GEOMETRY_SHADER))
    {
        std::cout << "Failed to compile geometry shader" <<  std::endl;
        return false;
    }

    glGetShaderiv(m_geometryShaderHandle,GL_COMPILE_STATUS,&shaderCompilationStatus);

    if(shaderCompilationStatus == GL_FALSE)
    {
        std::cout << "Failed to compile geometry shader" <<  std::endl;
        return false;
    }

    if(!loadAndCompileShader(fragFileName,m_fragmentShaderHandle,FRAGMENT_SHADER))
    {
        std::cout << "Failed to compile fragment shader" <<  std::endl;
        return false;
    }

    glGetShaderiv(m_fragmentShaderHandle,GL_COMPILE_STATUS,&shaderCompilationStatus);

    if(shaderCompilationStatus == GL_FALSE)
    {
        std::cout << "Failed to compile fragment shader" <<  std::endl;
        return false;
    }

    m_shaderProgramHandle = glCreateProgram();

    glAttachShader(m_shaderProgramHandle,m_vertexShaderHandle);
    glAttachShader(m_shaderProgramHandle,m_geometryShaderHandle);
    glAttachShader(m_shaderProgramHandle,m_fragmentShaderHandle);

    glLinkProgram(m_shaderProgramHandle);

    glGetProgramiv(m_shaderProgramHandle,GL_LINK_STATUS,&programLinkStatus);

    if(programLinkStatus == GL_FALSE)
    {
        std::cout << "Failed to link shader programs \n";
        return false;
    }


    //TODO:  Could write something to parse code and get uniforms dynamically?  For now this will do as a little hack job.

    m_ModelMatLocation = glGetUniformLocation(m_shaderProgramHandle,"ModelMatrix");
    m_ViewMatLocation = glGetUniformLocation(m_shaderProgramHandle,"ViewMatrix");
    m_ProjectionMatLocation = glGetUniformLocation(m_shaderProgramHandle,"ProjectionMatrix");
    m_NormalMatLocation = glGetUniformLocation(m_shaderProgramHandle, "NormalMatrix");

    std::cout << "ModelMatrix location " << m_ModelMatLocation << " ViewMatrix location " << m_ViewMatLocation << " ProjectionMatrix location " << m_ProjectionMatLocation <<  std::endl;

    return true;
}

bool GLShader::loadAndCompileShader(const std::string& sourceFileName,GLuint& shaderHandle,ShaderType shaderType)
{
    FileManager shaderSourceFile(sourceFileName);

    int fileLength = shaderSourceFile.getFileLength();

    m_shaderSource = new char[fileLength + 1];

    int index = 0;
    while(index < fileLength)
    {
        m_shaderSource[index] = shaderSourceFile.getNextChar();
        index++;
    }
    m_shaderSource[index] = '\0';

    shaderHandle = glCreateShader(shaderType);

    glShaderSource(shaderHandle,1,const_cast<const char**>(&m_shaderSource),&fileLength);

    glCompileShader(shaderHandle);

    delete[] m_shaderSource;
    m_shaderSource = NULL;

    return true;
}
