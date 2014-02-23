#include "GLTexture.h"
#include "FileManager.h"
#include <iostream>

#include "SOIL\SOIL.h"

GLTexture::GLTexture() : m_textureHandle(0),
                         m_filename()
{
	glGenTextures(1, &m_textureHandle);
}


GLTexture::GLTexture(std::string filename) : m_textureHandle(0),
                                             m_filename( filename )
{
	glGenTextures(1, &m_textureHandle);

	loadTexture(filename);
}

GLTexture::~GLTexture()
{
	glDeleteTextures(1, &m_textureHandle);
}

bool GLTexture::loadTexture( std::string filename )
{
	int width, height, channels;
	unsigned char *pImgData = NULL;

    if (filename[filename.size()] == 0)
    {
        filename.resize(filename.size() - 1);
    }

    pImgData = SOIL_load_image( filename.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);

	if (!pImgData)
	{
        std::cout << "Cannot find texture " << filename.c_str() << "\n";
        createXORTexture();
		return false;
	}

    unsigned int glChannelVal;

    switch (channels)
    {
        case 3:
            glChannelVal = GL_RGB; 
        break;

        case 4: 
            glChannelVal = GL_RGBA;
        break;

        default:
            glChannelVal = GL_RGB;
        break;
    }
    

	glBindTexture(GL_TEXTURE_2D, m_textureHandle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, glChannelVal, width, height, 0, glChannelVal, GL_UNSIGNED_BYTE, pImgData);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

	std::cout << "Texture loaded... width = " << width << " height = " << height << " comps = " << channels << "\n";

	if (pImgData)
	{
		free(pImgData);
	}

	return true;
}

bool GLTexture::createXORTexture()
{
    unsigned char pImgData[256][256][3];

    for (int i = 0; i < 256; i++)
    {
        for (int j = 0; j < 256; j++)
        {
            pImgData[i][j][0] = i ^ j;
            pImgData[i][j][1] = 0;
            pImgData[i][j][2] = i ^ j;
        }
    }

    glBindTexture(GL_TEXTURE_2D, m_textureHandle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, pImgData);

    glBindTexture(GL_TEXTURE_2D, 0);

    return true;
}