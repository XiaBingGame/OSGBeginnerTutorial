#include <iostream>
#include <osg/LightSource>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

osg::Node* createLightSource(unsigned int num,
	const osg::Vec3& trans,
	const osg::Vec4& color)
{
	osg::ref_ptr<osg::Light> light = new osg::Light;
	light->setLightNum(num);
	light->setDiffuse(color);
	light->setPosition(osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));

	osg::ref_ptr<osg::LightSource> lightSource = new
		osg::LightSource;
	lightSource->setLight(light);
	osg::ref_ptr<osg::MatrixTransform> sourceTrans =
		new osg::MatrixTransform;
	sourceTrans->setMatrix(osg::Matrix::translate(trans));
	sourceTrans->addChild(lightSource.get());
	return sourceTrans.release();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(model.get());
	osg::Node* light0 = createLightSource(
		0, osg::Vec3(-20.0f, 0.0f, 0.0f), osg::Vec4(
			1.0f, 1.0f, 0.0f, 1.0f));
	osg::Node* light1 = createLightSource(
		1, osg::Vec3(0.0f, -20.0f, 0.0f), osg::Vec4(0.0f, 1.0f, 1.0f, 1.0f)
	);
	root->getOrCreateStateSet()->setMode(GL_LIGHT0,
		osg::StateAttribute::ON);
	root->getOrCreateStateSet()->setMode(GL_LIGHT1,
		osg::StateAttribute::ON);
	root->addChild(light0);
	root->addChild(light1);

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
