#include <iostream>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

int main(int argc, char** argv)
{
	osg::DisplaySettings::instance()->setNumMultiSamples(4);
	osg::ref_ptr<osg::Node> root = osgDB::readNodeFile("cessna.osg");
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
