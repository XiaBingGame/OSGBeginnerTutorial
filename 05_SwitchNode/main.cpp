#include <iostream>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/Switch>

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Node> model1 = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr<osg::Node> model2 = osgDB::readNodeFile("cessnafire.osg");
	osg::ref_ptr<osg::Switch> root = new osg::Switch;
	root->addChild(model1, false);
	root->addChild(model2, true);
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
