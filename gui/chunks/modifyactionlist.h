#ifndef MODIFY_ACTION_LIST_HEADER
#define MODIFY_ACTION_LIST_HEADER

#include "actionlist.h"

namespace Gui
{

	/**
	 * Modify actions group
	 */
	class ModifyActionList : public ActionList
	{
	public:
		ModifyActionList(QObject* parent = 0) : ActionList(parent) {}

		QString getName() const;

	private Q_SLOTS:
		void removeShape();
		void shapesFusion();
		void shapesCommon();
		void shapesCut();

	private:
		void createActions();
	};

}

#endif // MODIFY_ACTION_LIST_HEADER

