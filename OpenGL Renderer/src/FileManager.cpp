#include "FileManager.h"
#include <iostream>

FileManager::FileManager() : m_fileStream(NULL)
{
}

FileManager::FileManager( const std::string& fileName, std::string args ) : m_fileStream(NULL),
																			m_fileName(fileName)
{
	m_errorNo = fopen_s(&m_fileStream, fileName.c_str(), args.c_str());
	
	if( m_errorNo )
	{
		std::cout << "Error opening file: " << fileName << " error code = " << m_errorNo << "\n";
	}
}

FileManager::FileManager( const std::string& fileName ) : m_fileStream(NULL),
														  m_fileName(fileName)
{
	m_errorNo = fopen_s(&m_fileStream, fileName.c_str(), "r");

	if (m_errorNo)
	{
        std::cout << "Error opening file: " << fileName << " error code = " << m_errorNo << "\n";
	}
}


FileManager::~FileManager()
{
	if( m_fileStream )
	{
		m_errorNo = fclose( m_fileStream );

		if( m_errorNo )
		{
            std::cout << "Error closing file: " << m_fileName << " error code = " << m_errorNo << "\n";
		}
	}
}

void FileManager::seekStart()
{
    fseek(m_fileStream, 0, SEEK_SET);
}

unsigned int FileManager::getFileLength()
{
	int length;

	if( !m_fileStream )
	{
		return 0;
	}

	fseek( m_fileStream, 0, SEEK_END );

	length = ftell( m_fileStream );

	fseek( m_fileStream, 0, SEEK_SET );

	return length;
}

unsigned char FileManager::getNextChar()
{
	if( !m_fileStream )
	{
		return 0;
	}

	return fgetc(m_fileStream);
}

unsigned int FileManager::isEOF()
{
	if( !m_fileStream )
	{
		return 0;
	}

	return	feof(m_fileStream);
}