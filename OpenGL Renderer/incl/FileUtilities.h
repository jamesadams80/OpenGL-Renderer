#ifndef _H_FILEUTIL
#define _H_FILEUTIL

#include <cstdio>

class FileUtilities
{
public:

		static int				getFileLength(FILE* file);

private:
		
								FileUtilities();

								~FileUtilities();

								FileUtilities(const FileUtilities&);

								void operator= (const FileUtilities&);

};
#endif

