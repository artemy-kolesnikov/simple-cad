#ifndef VIEW_PROVIDER_HEADER
#define VIEW_PROVIDER_HEADER

#include <TopoDS_Shape.hxx>
#include <map>

class SoGroup;
class SoVertexShape;

class ViewProvider
{
public:
	void computeEdges(SoGroup* edgeRoot, const TopoDS_Shape &shape);
	void computeVertices(SoGroup* vertexRoot, const TopoDS_Shape &shape);
	void computeFaces(SoGroup* faceRoot, const TopoDS_Shape &shape, double deflection);

	TopoDS_Shape getShape(SoVertexShape* shape) const;

private:
	std::map<SoVertexShape*, TopoDS_Shape> soShapeMap;
};

#endif // VIEW_PROVIDER_HEADER

