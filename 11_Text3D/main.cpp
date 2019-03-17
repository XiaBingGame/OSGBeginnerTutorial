#include <iostream>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgText/Font3D>
#include <osgText/Text3D>
#include <osgViewer/Viewer>

osg::ref_ptr<osgText::Font3D> g_font3D = osgText::readFont3DFile("fonts/arial.ttf");

osgText::Text3D* createText3D(const osg::Vec3& pos,
	const std::string& content,
	float size, float depth)
{
	osg::ref_ptr<osgText::Text3D> text = new osgText::Text3D;
	text->setFont(g_font3D.get());
	text->setCharacterSize(size);
	text->setCharacterDepth(depth);
	text->setAxisAlignment(osgText::TextBase::XZ_PLANE);
	text->setPosition(pos);
	text->setText(content);
	return text.release();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Geode> textGeode = new osg::Geode;
	textGeode->addDrawable(
		createText3D(osg::Vec3(), "The Cessna", 20.0f, 10.0f));

	osg::ref_ptr<osg::MatrixTransform> textNode = new
		osg::MatrixTransform;
	textNode->setMatrix(osg::Matrix::translate(0.0f, 0.0f, 10.0f));
	textNode->addChild(textGeode.get());

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(osgDB::readNodeFile("cessna.osg"));
	root->addChild(textNode.get());
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
