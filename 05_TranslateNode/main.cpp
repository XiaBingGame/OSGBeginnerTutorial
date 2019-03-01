#include <iostream>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/MatrixTransform>

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr<osg::MatrixTransform> transformation1 = new osg::MatrixTransform;
	transformation1->setMatrix(osg::Matrix::translate(-25.0f, 0.0f, 0.0f));
	transformation1->addChild(model.get());
	osg::ref_ptr<osg::MatrixTransform> transformation2 = new osg::MatrixTransform;
	transformation2->setMatrix(osg::Matrix::translate(25.0f, 0.0f, 0.0f));
	transformation2->addChild(model.get());

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(transformation1.get());
	root->addChild(transformation2.get());

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
