#include <Windows.h>
#include <iostream>
#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/Geode>

#include <osgUtil/Optimizer>

#include <osgDB/Registry>
#include <osgDB/ReadFile>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>

#include <osgSim/OverlayNode>

#include <osgText/Font>
#include <osgText/Text>

#include <osgViewer/Viewer>
#include <iostream>

static const char *microshaderVertSource = {
	"// microshader - colors a fragment based on its position\n"
	"#version 330 compatibility\n"
	"varying vec2 texcoord;\n"
	"void main(void)\n"
	"{\n"
	"	 texcoord = gl_MultiTexCoord0.xy;\n"
	"    gl_Position = gl_Vertex;\n"
	"}\n"
};

static const char *microshaderFragSource = {
	"#version 330 compatibility\n"
	"varying vec2 texcoord;\n"
	"void main(void)\n"
	"{\n"
	"    gl_FragColor = vec4(texcoord.x, texcoord.y, 0.0, 1.0);\n"
	"}\n"
};

//创建一个四边形节点
osg::ref_ptr<osg::Node> createNode()
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();

	//设置顶点
	osg::ref_ptr<osg::Vec3Array> vc = new osg::Vec3Array();
	vc->push_back(osg::Vec3(-1.0f, -1.0f, 0.5f));
	vc->push_back(osg::Vec3(1.0f, -1.0f, 0.5f));
	vc->push_back(osg::Vec3(1.0f, 1.0f, 0.5f));
	vc->push_back(osg::Vec3(-1.0f, 1.0f, 0.5f));

	geom->setVertexArray(vc.get());

	//设置纹理坐标
	osg::ref_ptr<osg::Vec2Array> vt = new osg::Vec2Array();
	vt->push_back(osg::Vec2(0.0f, 0.0f));
	vt->push_back(osg::Vec2(1.0f, 0.0f));
	vt->push_back(osg::Vec2(1.0f, 1.0f));
	vt->push_back(osg::Vec2(0.0f, 1.0f));

	geom->setTexCoordArray(0, vt.get());

	//设置法线
	osg::ref_ptr<osg::Vec3Array> nc = new osg::Vec3Array();
	nc->push_back(osg::Vec3(0.0f, 0.0f, 1.0f));

	geom->setNormalArray(nc.get());
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

	//添加图元
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	//绘制
	geode->addDrawable(geom.get());

	osg::StateSet* ss = geode->getOrCreateStateSet();
	osg::Program* program = new osg::Program;
	program->setName("microshader");
	program->addShader(new osg::Shader(osg::Shader::VERTEX, microshaderVertSource));
	program->addShader(new osg::Shader(osg::Shader::FRAGMENT, microshaderFragSource));
	ss->setAttributeAndModes(program, osg::StateAttribute::ON);

	return geode.get();
}

class FindTextureVisitor : public osg::NodeVisitor
{
public:
	FindTextureVisitor(osg::Texture* tex) : _texture(tex)
	{
		setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
	}

	virtual void apply(osg::Node& node)
	{
		replaceTexture(node.getStateSet());
		traverse(node);
	}

	virtual void apply(osg::Geode& geode)
	{
		replaceTexture(geode.getStateSet());
		for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
		{
			replaceTexture(geode.getDrawable(i)->getStateSet());
		}
		traverse(geode);
	}

	void replaceTexture(osg::StateSet* stateset)
	{
		if (stateset)
		{
			osg::Texture* oldTexture = dynamic_cast<osg::Texture*>(
				stateset->getTextureAttribute(0, osg::StateAttribute::TEXTURE));
			if (oldTexture) stateset->setTextureAttribute(0, _texture.get());
		}
	}

protected:
	osg::ref_ptr<osg::Texture> _texture;
};

int main(int argc, char** argv)
{
	// setMinimumNumAlphaBits(1);
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("lz.osg");
	osg::ref_ptr<osg::Node> sub_model = osgDB::readNodeFile("glider.osg");
	osg::ref_ptr<osg::Node> custom_node = createNode();

	int tex_width = 1024, tex_height = 1024;

	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setTextureSize(tex_width, tex_height);
	texture->setInternalFormat(GL_RGBA);
	texture->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
	texture->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);

	FindTextureVisitor ftv(texture.get());
	if (model.valid()) model->accept(ftv);

	osg::ref_ptr<osg::Camera> camera = new osg::Camera;
	camera->setViewport(0, 0, tex_width, tex_height);
	camera->setClearColor(osg::Vec4(1.0f, 1.0f, 1.0f, 0.0f));
	camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->setRenderOrder(osg::Camera::PRE_RENDER);
	camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
	camera->attach(osg::Camera::COLOR_BUFFER, texture.get());

	camera->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
	// camera->addChild(sub_model.get());
	camera->addChild(custom_node.get());

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(model.get());
	root->addChild(camera.get());

	osgViewer::Viewer viewer;
	viewer.setSceneData(root.get());
	viewer.setCameraManipulator(new osgGA::TrackballManipulator);

	float delta = 0.1f, bias = 0.0f;
	osg::Vec3 eye(0.0f, -5.0f, 5.0f);
	viewer.setUpViewInWindow(100, 100, 800, 600);
	while (!viewer.done())
	{
		if (bias < -1.0f) delta = 0.1f;
		else if (bias > 1.0f) delta = -0.1f;
		bias += delta;
		camera->setViewMatrixAsLookAt(eye, osg::Vec3(), osg::Vec3(bias, 1.0f, 1.0f));
		viewer.frame();
	}
	return 0;
}
