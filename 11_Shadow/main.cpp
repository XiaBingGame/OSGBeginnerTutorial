#include <iostream>
#include <osg/AnimationPath>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>
#include <osgViewer/Viewer>
#include <osg/NodeVisitor>
#include <osgGA/TrackballManipulator>
#include <osg/Program>
#include <string>
#include <osg/PolygonMode>
#include <osgDB/FileUtils>


namespace osgCookBook
{
	osg::Camera* createHUDCamera(double left, double right, double bottom, double top)
	{
		osg::ref_ptr<osg::Camera> camera = new osg::Camera;
		// 参考帧为绝对坐标系
		camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		camera->setClearMask(GL_DEPTH_BUFFER_BIT);
		// 渲染至纹理设置 PRE_RENDER, HUD 则用 POST_RENDER
		camera->setRenderOrder(osg::Camera::POST_RENDER);
		// 该相机关联的窗口是否允许生成事件
		camera->setAllowEventFocus(false);
		camera->setProjectionMatrix(
			osg::Matrix::ortho2D(left, right, bottom, top));
		// 禁止光照
		camera->getOrCreateStateSet()->setMode(
			GL_LIGHTING, osg::StateAttribute::OFF);

		// 减少计数, 由其他部分控制该指针
		return camera.release();
	}


	osg::AnimationPathCallback* createAnimationPathCallback(float radius, float time)
	{
		osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
		path->setLoopMode(osg::AnimationPath::LOOP);

		unsigned int numSamples = 32;
		float delta_yaw = 2.0f * osg::PI / ((float)numSamples - 1.0f);
		float delta_time = time / (float)numSamples;

		for (unsigned int i = 0; i < numSamples; i++)
		{
			float yaw = delta_yaw * (float)i;
			osg::Vec3 pos(sinf(yaw)*radius, cosf(yaw)*radius, 0.0f);
			osg::Quat rot(-yaw, osg::Z_AXIS);
			path->insert(delta_time*(float)i, osg::AnimationPath::ControlPoint(pos, rot));
		}

		osg::ref_ptr<osg::AnimationPathCallback> apcb = new osg::AnimationPathCallback;
		apcb->setAnimationPath(path.get());
		return apcb.release();
	}

	osg::Geode* createScreenQuad(float width, float height, float scale)
	{
		osg::Geometry* geom = osg::createTexturedQuadGeometry(
			osg::Vec3(), osg::Vec3(width, 0.0f, 0.0f), osg::Vec3(0.0f, height, 0.0f),
			0.0f, 0.0f, width*scale, height*scale);
		osg::ref_ptr<osg::Geode> quad = new osg::Geode;
		quad->addDrawable(geom);

		int values = osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED;
		quad->getOrCreateStateSet()->setAttribute(
			new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::FILL), values);
		quad->getOrCreateStateSet()->setMode(GL_LIGHTING, values);
		return quad.release();
	}

	osg::Camera* createRTTCamera(osg::Camera::BufferComponent buffer, osg::Texture* tex, bool isAbsolute)
	{
		osg::ref_ptr<osg::Camera> camera = new osg::Camera;
		camera->setClearColor(osg::Vec4());
		camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
		camera->setRenderOrder(osg::Camera::PRE_RENDER);
		if (tex)
		{
			tex->setFilter(osg::Texture2D::MIN_FILTER, osg::Texture2D::LINEAR);
			tex->setFilter(osg::Texture2D::MAG_FILTER, osg::Texture2D::LINEAR);
			camera->setViewport(0, 0, tex->getTextureWidth(), tex->getTextureHeight());
			camera->attach(buffer, tex);
		}

		if (isAbsolute)
		{
			camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
			camera->setProjectionMatrix(osg::Matrix::ortho2D(0.0, 1.0, 0.0, 1.0));
			camera->setViewMatrix(osg::Matrix::identity());
			camera->addChild(createScreenQuad(1.0f, 1.0f, 1.0));
		}

		return camera.release();
	}
}

class VertexVisitor : public osg::NodeVisitor
{
public:
	VertexVisitor();
	virtual void apply(osg::Geode& geode);

	std::vector<osg::Geometry*>& getGeometrys();

private:
	std::vector<osg::Geometry*> _geometrys;
};

std::vector<osg::Geometry*>& VertexVisitor::getGeometrys()
{
	return _geometrys;
}

VertexVisitor::VertexVisitor() :
	osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
{
}

void VertexVisitor::apply(osg::Geode& geode)
{
	for (size_t i = 0; i < geode.getNumDrawables(); i++)
	{
		osg::Geometry* geom = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
		osg::Vec3Array* va = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
		if (va)
		{
			for (int i = 0; i < va->size();i++)
			{
				(*va)[i].z() = 0.0;
			}
		}
		geom->dirtyDisplayList();
		_geometrys.push_back(geom);
	}
}

osg::AnimationPath* createAnimationPath(float radius, float time)
{
	osg::ref_ptr<osg::AnimationPath> path =
		new osg::AnimationPath;
	path->setLoopMode(osg::AnimationPath::LOOP);

	unsigned int numSamples = 32;
	float delta_yaw = 2.0f * osg::PI / ((float)numSamples - 1.0f);
	float delta_time = time / (float)numSamples;
	for (unsigned int i = 0; i < numSamples; ++i)
	{
		float yaw = delta_yaw * (float)i;
		osg::Vec3 pos(sinf(yaw)*radius, cosf(yaw)*radius, 0.0f);
		osg::Quat rot(-yaw, osg::Z_AXIS);
		path->insert(delta_time * (float)i,
			osg::AnimationPath::ControlPoint(pos, rot)
		);
	}
	return path.release();
}

osg::Program* generateShaderProgram(const std::string& vert_file_path, const std::string& frag_file_path)
{
	osg::ref_ptr<osg::Program> program = new osg::Program;

	osg::ref_ptr<osg::Shader> vertObj = new osg::Shader(osg::Shader::VERTEX);
	osg::ref_ptr<osg::Shader> fragObj = new osg::Shader(osg::Shader::FRAGMENT);
	program->addShader(vertObj);
	program->addShader(fragObj);

	std::string fqFileName = osgDB::findDataFile(vert_file_path);
	bool bret;
	if (fqFileName.length() != 0)
	{
		bret = vertObj->loadShaderSourceFromFile(fqFileName.c_str());
		if (bret == false)
		{
			std::cout << "shader " << vert_file_path << " wrong!" << std::endl;
			return 0;
		}
	}
	else
	{
		return 0;
	}
	fqFileName = osgDB::findDataFile(frag_file_path);
	if (fqFileName.length() != 0)
	{
		bret = fragObj->loadShaderSourceFromFile(fqFileName.c_str());
		if (bret == false)
		{
			std::cout << "shader " << frag_file_path << " wrong!" << std::endl;
			return 0;
		}
	}
	else
	{
		return 0;
	}

	return program.release();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Texture2D> tex2D = new osg::Texture2D;
	tex2D->setTextureSize(1024, 1024);
	tex2D->setInternalFormat(GL_DEPTH_COMPONENT24);
	tex2D->setSourceFormat(GL_DEPTH_COMPONENT);
	tex2D->setSourceType(GL_FLOAT);

	osg::ref_ptr<osg::Camera> rttCamera = osgCookBook::createRTTCamera(osg::Camera::DEPTH_BUFFER, tex2D.get(), false);
	//rttCamera->addChild(scene.get());

	osg::ref_ptr<osg::Camera> hudCamera = osgCookBook::createHUDCamera(0.0, 1.0, 0.0, 1.0);
	hudCamera->addChild(osgCookBook::createScreenQuad(1.0f, 1.0f, 1.0));
	hudCamera->getOrCreateStateSet()->setTextureAttributeAndModes(0, tex2D.get());

	osg::Program* hudprogram = generateShaderProgram("hud.vert", "hud.frag");
	hudCamera->getOrCreateStateSet()->setAttributeAndModes(hudprogram, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);


	unsigned int rcvShadowMask = 0x1;
	unsigned int castShadowMask = 0x2;

	osg::ref_ptr<osg::MatrixTransform> groundNode =
		new osg::MatrixTransform;
	groundNode->addChild(osgDB::readNodeFile("lz.osgt"));
	VertexVisitor vv;
	groundNode->accept(vv);

	osg::Program* gprogram = generateShaderProgram("ground.vert", "ground.frag");
	//groundNode->getOrCreateStateSet()->setAttributeAndModes(gprogram, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	osg::PolygonMode* polymode = new osg::PolygonMode;
	polymode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	//groundNode->getOrCreateStateSet()->setAttributeAndModes(polymode, osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);


	osg::Program* terrainprogram = generateShaderProgram("terrain.vert", "terrain.frag");

	//groundNode->getOrCreateStateSet()->setAttributeAndModes(terrainprogram, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);


	groundNode->setMatrix(osg::Matrix::translate(0.0f, 0.0f, 0.0f));
	groundNode->setNodeMask(rcvShadowMask);

	osg::ref_ptr<osg::MatrixTransform> cessnaNode =
		new osg::MatrixTransform;
	cessnaNode->addChild(osgDB::readNodeFile("cessna.osg.0,0,90.rot"));
	cessnaNode->setNodeMask(castShadowMask);
	osg::ref_ptr<osg::AnimationPathCallback> apcb =new osg::AnimationPathCallback;
	apcb->setAnimationPath(createAnimationPath(50.0f, 6.0f));
	cessnaNode->setUpdateCallback(apcb.get());

	float x = 0.0;
	float y = 0.0;
	//float h = 97.0;
	float h = 5.0;
	float dist = 95.0;

	osg::ref_ptr<osg::MatrixTransform> truckNode =
		new osg::MatrixTransform;
	//truckNode->addChild(osgDB::readNodeFile("dumptruck.osg.5,-5,0.rot"));
	truckNode->addChild(osgDB::readNodeFile("dumptruck.osg"));
	truckNode->setMatrix(osg::Matrix::translate(x, y, h));
	truckNode->setNodeMask(rcvShadowMask | castShadowMask);

	osg::Program* program = generateShaderProgram("modelVR.vert", "modelVR.frag");
	
	truckNode->getOrCreateStateSet()->setAttributeAndModes(program, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);

	osg::ref_ptr<osg::LightSource> source = new osg::LightSource;
	source->getLight()->setPosition(osg::Vec4(4.0, 4.0, 10.0, 0.0));
	source->getLight()->setAmbient(osg::Vec4(0.3, 0.3, 0.3, 1.0));
	source->getLight()->setDiffuse(osg::Vec4(1.0, 1.0, 1.0, 1.0));

	osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
	sm->setLight(source.get());
	sm->setTextureSize(osg::Vec2s(1024, 1024));
	sm->setTextureUnit(1);
	
	
	osg::ref_ptr<osgShadow::ShadowedScene> root =
		new osgShadow::ShadowedScene;
	root->setShadowTechnique(sm.get());
	root->setReceivesShadowTraversalMask(rcvShadowMask);
	root->setCastsShadowTraversalMask(castShadowMask);
	
	//osg::ref_ptr<osg::Group> root = new osg::Group;

	root->addChild(groundNode.get());
	//root->addChild(cessnaNode.get());
	root->addChild(truckNode.get());
	root->addChild(source.get());

	rttCamera->addChild(root.get());
	osg::ref_ptr<osg::Group> sroot = new osg::Group;
	sroot->addChild(rttCamera.get());
	//sroot->addChild(hudCamera.get());
	sroot->addChild(root.get());

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(sroot.get());
	osgGA::TrackballManipulator* tm = new osgGA::TrackballManipulator;
	float vd = 30.0;
	tm->setHomePosition(osg::Vec3d(x + 40, y - 5, h + 10), osg::Vec3d(x - 10, y, h), osg::Vec3d(0.0, 0.0, 1.0));
	//tm->setHomePosition(osg::Vec3d(x + 4*vd, y +4*vd, h + 10*vd), osg::Vec3d(x, y, h), osg::Vec3d(0.0, 0.0, 1.0));
	viewer.setCameraManipulator(tm);
	viewer.getCamera()->setClearColor(osg::Vec4(0.4, 0.4, 0.9, 1.0));
	//viewer.getCamera()->setClearColor(osg::Vec4(0.8, 0.8, 0.8, 1.0));
	return viewer.run();
}
