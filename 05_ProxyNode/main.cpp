#include <iostream>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/ProxyNode>

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::ProxyNode> root = new osg::ProxyNode;
	root->setFileName(0, "cow.osg");
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
