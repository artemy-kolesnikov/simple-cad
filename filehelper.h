#ifndef FILE_HELPER_HEADER
#define FILE_HELPER_HEADER

#include <QString>

#include <AIS_InteractiveContext.hxx>

class FileHelper
{
public:
	static bool readFile(QString& fileName,
		const Handle(AIS_InteractiveContext)& context);

	static bool writeFile(QString& fileName,
		const Handle(AIS_InteractiveContext)& context);
};

#endif // FILE_HELPER_HEADER

