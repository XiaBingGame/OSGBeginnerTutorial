#include <iostream>
#include <osgGA/TrackballManipulator>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("lz.osg");
	osgViewer::Viewer viewer;
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(model.get());
	while (!viewer.done())
	{
		viewer.frame();
		std::cout << "Frame number: " <<
			viewer.getFrameStamp()->getFrameNumber() << std::endl;
	}
	return 0;
}
