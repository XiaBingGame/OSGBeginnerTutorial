#include <iostream>
#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#include <osgViewer/Viewer>

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::ShapeDrawable> shape1 = new osg::ShapeDrawable;
	osg::ref_ptr<osg::ShapeDrawable> shape2 = new osg::ShapeDrawable;
	osg::ref_ptr<osg::ShapeDrawable> shape3 = new osg::ShapeDrawable;
	shape1->setShape(new osg::Box(osg::Vec3(-3.0f, 0.0f, 0.0f), 2.0f, 2.0f, 1.0f));
	shape2->setShape(new osg::Sphere(osg::Vec3(3.0f, 0.0f, 0.0f), 1.0f));
	shape3->setShape(new osg::Cone(osg::Vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f));
	shape2->setColor(osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f));
	shape3->setColor(osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f));

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(shape1.get());
	geode->addDrawable(shape2.get());
	geode->addDrawable(shape3.get());

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(geode.get());
	return viewer.run();
}
