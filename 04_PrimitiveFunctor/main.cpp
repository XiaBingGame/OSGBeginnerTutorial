#include <iostream>
#include <osgDB/ReadFile>
#include <osg/TriangleFunctor>
#include <osgViewer/Viewer>

std::ostream& operator<<(std::ostream& os, const osg::Vec3& v)
{
	os << v.x() << ", " << v.y() << ", " << v.z();
	return os;
}

struct FaceCollector
{
	void operator()(const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3)
	{
		std::cout << "Face vertices: " << v1 << "; " << v2 << "; " << v3 << std::endl;
	}

	void operator()(const osg::Vec3& v1, const osg::Vec3& v2, const osg::Vec3& v3, bool)
	{
		std::cout << "Face vertices: " << v1 << "; " << v2 << "; " << v3 << std::endl;
	}
};

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	vertices->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
	vertices->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
	vertices->push_back(osg::Vec3(1.0f, 0.0f, 1.5f));
	vertices->push_back(osg::Vec3(2.0f, 0.0f, 0.0f));
	vertices->push_back(osg::Vec3(2.0f, 0.0f, 1.0f));
	vertices->push_back(osg::Vec3(3.0f, 0.0f, 0.0f));
	vertices->push_back(osg::Vec3(3.0f, 0.0f, 1.5f));
	vertices->push_back(osg::Vec3(4.0f, 0.0f, 0.0f));
	vertices->push_back(osg::Vec3(4.0f, 0.0f, 1.0f));

	osg::ref_ptr<osg::Vec3Array> normal = new osg::Vec3Array;
	normal->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	geom->setVertexArray(vertices);
	geom->setNormalArray(normal);
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
	geom->addPrimitiveSet(new osg::DrawArrays(GL_QUAD_STRIP, 0, 10));

	osg::TriangleFunctor<FaceCollector> triangleFunctor;
	geom->accept(triangleFunctor);

	osg::ref_ptr<osg::Geode> root = new osg::Geode;
	root->addDrawable(geom.get());
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
