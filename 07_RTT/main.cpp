#include <iostream>
#include <osg/Camera>
#include <osg/Texture2D>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>



class FindTextureVisitor : public osg::NodeVisitor
{
public:
	FindTextureVisitor(osg::Texture* tex) : _texture(tex)
	{
		setTraversalMode(
			osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
	}

	virtual void apply(osg::Node& node);
	virtual void apply(osg::Geode& geode);
	void replaceTexture(osg::StateSet* ss)
	{
		if (ss)
		{
			osg::Texture* oldTexture = dynamic_cast<osg::Texture*>(
				ss->getTextureAttribute(0, osg::StateAttribute::TEXTURE)
				);
			if (oldTexture) ss->setTextureAttribute(
				0, _texture.get());
		}
	}

protected:
	osg::ref_ptr<osg::Texture> _texture;
};

void FindTextureVisitor::apply(osg::Node& node)
{
	replaceTexture(node.getStateSet());
	traverse(node);
}
void FindTextureVisitor::apply(osg::Geode& geode)
{
	replaceTexture(geode.getStateSet());
	for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
	{
		replaceTexture(geode.getDrawable(i)->getStateSet());
	}
	traverse(geode);
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("lz.osg");
	osg::ref_ptr<osg::Node> sub_model = osgDB::readNodeFile("glider.osg");

	int tex_width = 1024, tex_height = 1024;
	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setTextureSize(tex_width, tex_height);
	texture->setInternalFormat(GL_RGBA);
	texture->setFilter(osg::Texture2D::MIN_FILTER,
		osg::Texture2D::LINEAR);
	texture->setFilter(osg::Texture2D::MAG_FILTER,
		osg::Texture2D::LINEAR);

	FindTextureVisitor ftv(texture.get());
	if (model.valid()) model->accept(ftv);

	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setViewport(0, 0, tex_width, tex_height);
	camera->setClearColor(osg::Vec4(1.0f, 1.0f, 1.0f, 0.0f));
	camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->setRenderOrder(osg::Camera::PRE_RENDER);
	camera->setRenderTargetImplementation(
		osg::Camera::FRAME_BUFFER_OBJECT);
	camera->attach(osg::Camera::COLOR_BUFFER, texture.get());

	camera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	camera->addChild(sub_model.get());

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(model.get());
	root->addChild(camera.get());

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);
	float delta = 0.1f, bias = 0.0f;
	osg::Vec3 eye(0.0f, -5.0f, 5.0f);
	while (!viewer.done())
	{
		if (bias < -1.0f) delta = 0.1f;
		else if (bias > 1.0f) delta = -0.1f;
		bias += delta;
		camera->setViewMatrixAsLookAt(eye, osg::Vec3(),
			osg::Vec3(bias, 1.0f, 1.0f));
		viewer.frame();
	}
	return 0;
}
