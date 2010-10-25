#ifndef CAD_APPLICATION_HEADER
#define CAD_APPLICATION_HEADER

#include <QApplication>

class CADApplication : public QApplication
{
	Q_OBJECT

public:
	CADApplication(int argc, char* argv[]);
};

#endif // CAD_APPLICATION_HEADER
