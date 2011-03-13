#ifndef INVENTOR_VIEWER_HEADER
#define INVENTOR_VIEWER_HEADER

#include <Inventor/Qt/viewers/SoQtViewer.h>
#include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
#include <Inventor/Qt/viewers/SoQtFullViewer.h>

class InventorViewer : public SoQtExaminerViewer
{
public:
	InventorViewer(QWidget* parent = 0);
};

#endif // INVENTOR_VIEWER_HEADER

