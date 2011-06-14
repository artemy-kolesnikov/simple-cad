#ifndef ACTION_FACTORY_HEADER
#define ACTION_FACTORY_HEADER

namespace Action
{

	class TopoAction;

	class Factory
	{
	public:
		enum ActionType
		{
			Common,
			Cut,
			Fuse,
			CreateRectangle
		};

		static TopoAction* create(ActionType type);
	private:
		Factory() {}
	};

}

#endif // ACTION_FACTORY_HEADER

