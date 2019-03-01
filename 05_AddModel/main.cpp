// osg::Group::addChild

#include <iostream>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Node> model1 = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr<osg::Node> model2 = osgDB::readNodeFile("cow.osg");
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(model1.get());
	root->addChild(model2.get());
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
