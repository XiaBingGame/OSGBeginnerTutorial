#include <iostream>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
// #include <osg/AugumentParser>

int main(int argc, char** argv)
{
	osg::ArgumentParser arguments(&argc, argv);
	std::string filename;
	arguments.read("--model", filename);

	osg::ref_ptr<osg::Node> root = osgDB::readNodeFile(filename);
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
