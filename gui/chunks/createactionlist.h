#ifndef CREATE_ACTION_LIST_HEADER
#define CREATE_ACTION_LIST_HEADER

#include "actionlist.h"

namespace Gui
{

	/**
	 * Create actions group
	 */
	class CreateActionList : public ActionList
	{
	public:
		CreateActionList(QObject* parent = 0) : ActionList(parent) {}

		QString getName() const;

	private Q_SLOTS:
		void createBox();
		void createCylinder();
		void createSphere();
		void createCone();
		void createTorus();
		void createPlane();
		void createEllipsoid();

	private:
		void createActions();
	};

}

#endif // MODEL_ACTION_LIST_HEADER

