#include <iostream>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/LOD>
#include <osgUtil/Simplifier>

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Node> model3 = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr<osg::Node> model2 = dynamic_cast<osg::Node*>(model3->clone(osg::CopyOp::DEEP_COPY_ALL));
	osg::ref_ptr<osg::Node> model1 = dynamic_cast<osg::Node*>(model3->clone(osg::CopyOp::DEEP_COPY_ALL));

	osgUtil::Simplifier simplifier;
	simplifier.setSampleRatio(0.5);
	model2->accept(simplifier);
	simplifier.setSampleRatio(0.1);
	model1->accept(simplifier);

	osg::ref_ptr<osg::LOD> root = new osg::LOD;
	root->addChild(model1, 200.0f, FLT_MAX);
	root->addChild(model2, 50.0f, 200.0);
	root->addChild(model3, 0.0f, 50.0);

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
