#include <iostream>
#include <osg/PolygonMode>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("glider.osg");
	osg::ref_ptr<osg::MatrixTransform> transform1 = new osg::MatrixTransform;
	transform1->setMatrix(osg::Matrix::translate(osg::Vec3(-0.5f, 0.0f, 0.0f)));
	osg::ref_ptr<osg::MatrixTransform> transform2 = new osg::MatrixTransform;
	transform2->setMatrix(osg::Matrix::translate(osg::Vec3(0.5f, 0.0f, 0.0f)));
	transform1->addChild(model);
	transform2->addChild(model);
	// osg::ref_ptr<osg::PolygonMode> pm = new osg::PolygonMode;
	// pm->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	// transform1->getOrCreateStateSet()->setAttribute(pm.get());

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(transform1);
	root->addChild(transform2);

	transform1->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	transform2->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
	root->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
