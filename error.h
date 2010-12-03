#ifndef ERROR_HEADER
#define ERROR_HEADER

#include <QString>
#include <exception>

class CadError : public std::exception
{
public:
	CadError(QString what) : whatStr(what) {}
	~CadError() throw() {}

	const char* what() { return whatStr.toStdString().c_str(); }

private:
	QString whatStr;
};

class FileError : public CadError
{
public:
	FileError(QString what) : CadError(what) {}
};

#endif // ERROR_HEADER

