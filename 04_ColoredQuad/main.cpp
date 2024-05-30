#include <iostream>
#include <osgDB/ReadFile>
#include <osg/Geometry>
#include <osg/Node>
#include <osgViewer/Viewer>

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> verts = new osg::Vec3Array;
	verts->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	verts->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
	verts->push_back(osg::Vec3(1.0f, 0.0f, 1.0f));
	verts->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));
	osg::ref_ptr<osg::Vec3Array> colors = new osg::Vec3Array;
	colors->push_back(osg::Vec3(1.0f, 0.0f, 0.0f));
	colors->push_back(osg::Vec3(0.0f, 1.0f, 0.0f));
	colors->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));
	colors->push_back(osg::Vec3(1.0f, 1.0f, 1.0f));
	geom->setVertexArray(verts.get());
	geom->setNormalArray(normals.get());
	geom->setColorArray(colors.get());
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geom.get());

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(geode.get());
	return viewer.run();
}
