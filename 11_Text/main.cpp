#include <iostream>
#include <osg/Camera>
#include <osgDB/ReadFile>
#include <osgText/Font>
#include <osgText/Text>
#include <osgViewer/Viewer>

osg::ref_ptr<osgText::Font> g_font = osgText::readFontFile("fonts/arial.ttf");

osg::Camera* createHUDCamera(double left, double right,
	double bottom, double top)
{
	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	camera->setClearMask(GL_DEPTH_BUFFER_BIT);
	camera->setRenderOrder(osg::Camera::POST_RENDER);
	camera->setAllowEventFocus(false);
	camera->setProjectionMatrix(
		osg::Matrix::ortho2D(left, right, bottom, top));
	return camera.release();
}

osgText::Text* createText(const osg::Vec3& pos,
	const std::string& content,
	float size)
{
	osg::ref_ptr<osgText::Text> text = new osgText::Text;
	text->setFont(g_font.get());
	text->setCharacterSize(size);
	text->setAxisAlignment(osgText::TextBase::XY_PLANE);
	text->setPosition(pos);
	text->setText(content);
	return text.release();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Geode> textGeode = new osg::Geode;
	textGeode->addDrawable(createText(
		osg::Vec3(150.0f, 500.0f, 0.0f),
		"The Cessna monoplane",
		20.0f)
	);
	textGeode->addDrawable(createText(
		osg::Vec3(150.0f, 450.0f, 0.0f),
		"Six-seat, low-wing and twin-engined",
		15.0f)
	);

	osg::Camera* camera = createHUDCamera(0, 1024, 0, 768);
	camera->addChild(textGeode.get());
	camera->getOrCreateStateSet()->setMode(
		GL_LIGHTING, osg::StateAttribute::OFF);

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(osgDB::readNodeFile("cessna.osg"));
	root->addChild(camera);

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
