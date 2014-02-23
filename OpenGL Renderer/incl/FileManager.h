#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <string>

#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>

#define OBJECT_DIRECTORY "resource/"

class FileManager
{
	public:
					 		FileManager();
							FileManager(const std::string&);

							FileManager::FileManager(const std::string& fileName, std::string args);

							~FileManager();

        void                seekStart();

		unsigned int		getFileLength();

		unsigned char		getNextChar();

		FILE*				getFileStream()
		{
			return			m_fileStream;
		}

		unsigned int		isEOF();

        char*               getNextLine()
        {
            if (m_fileStream == NULL)
            {
                return NULL;
            }

            if (!fgets(m_readBuffer, sizeof(m_readBuffer), m_fileStream))
            {
                return NULL;
            }

            return m_readBuffer;
        }



	private:

		FILE*				m_fileStream;

		errno_t				m_errorNo;

		std::string			m_fileName;

        // Can only read in a buffer of maximum size 256
        char       m_readBuffer[256];

		FileManager(const FileManager&);
		FileManager& operator= (const FileManager&);

		FileManager(const FileManager&&);
		FileManager& operator= (const FileManager&&);
};

#endif