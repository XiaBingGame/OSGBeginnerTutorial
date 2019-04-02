#include <iostream>
#include <osg/BlendFunc>
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/PositionAttitudeTransform>
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osgDB/FileUtils>
#include <osgGA/StateSetManipulator>

osg::Geode* createQuad(osg::Vec4 color)
{
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices->push_back(osg::Vec3(-0.5f, -0.5f, -0.5f));
	vertices->push_back(osg::Vec3(0.5f, -0.5f, -0.5f));
	vertices->push_back(osg::Vec3(0.5f, -0.5f, 0.5f));
	vertices->push_back(osg::Vec3(-0.5f, -0.5f, 0.5f));
	
	vertices->push_back(osg::Vec3(-0.5f, 0.0f, -0.5f));
	vertices->push_back(osg::Vec3(0.5f, 0.0f, -0.5f));
	vertices->push_back(osg::Vec3(0.5f, 0.0f, 0.5f));
	vertices->push_back(osg::Vec3(-0.5f, 0.0f, 0.5f));
	
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));


	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(color);

	osg::ref_ptr<osg::Geometry> quad = new osg::Geometry;
	quad->setVertexArray(vertices.get());
	quad->setNormalArray(normals.get());
	quad->setNormalBinding(osg::Geometry::BIND_OVERALL);
	quad->setColorArray(colors.get());
	quad->setColorBinding(osg::Geometry::BIND_OVERALL);
	quad->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, vertices->size()));
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(quad.get());

	return geode.release();
}


osg::Geode* createSphere(int slices, int stacks, GLfloat radius)
{
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
	
	// 
	float horz_angle_step = osg::PI * 2.0 / stacks;
	float vert_angle_step = osg::PI / slices;
	float start_vert_angle = -osg::PI_2;
	for (int i = 1; i < slices-1; i++)
	{
		float cur_vert_angle = start_vert_angle + i * vert_angle_step;
		float nxt_vert_angle = start_vert_angle + (i + 1)*vert_angle_step;
		float bottom_height = sin(cur_vert_angle) * radius;
		float top_height = sin(nxt_vert_angle) * radius;
		float bottom_radius = cos(cur_vert_angle) * radius;
		float top_radius = cos(nxt_vert_angle) * radius;
		float bottom_sin = sin(cur_vert_angle);
		float top_sin = sin(nxt_vert_angle);
		float bottom_cos = cos(cur_vert_angle);
		float top_cos = cos(nxt_vert_angle);
		for (int j = 0; j < stacks; j++)
		{
			float cur_horz_angle = j * horz_angle_step;
			float nxt_horz_angle = (j + 1) * horz_angle_step;

			float cur_normal_x = cos(cur_horz_angle);
			float cur_normal_y = sin(cur_horz_angle);
			float nxt_normal_x = cos(nxt_horz_angle);
			float nxt_normal_y = sin(nxt_horz_angle);

			osg::Vec3 left_bottom_vert = osg::Vec3(cur_normal_x*bottom_radius, cur_normal_y*bottom_radius, bottom_height);
			osg::Vec3 right_bottom_vert = osg::Vec3(nxt_normal_x*bottom_radius, nxt_normal_y*bottom_radius, bottom_height);
			osg::Vec3 left_top_vert = osg::Vec3(cur_normal_x*top_radius, cur_normal_y*top_radius, top_height);
			osg::Vec3 right_top_vert = osg::Vec3(nxt_normal_x*top_radius, nxt_normal_y*top_radius, top_height);
			osg::Vec3 left_bottom_normal = osg::Vec3(cur_normal_x*bottom_cos, cur_normal_y*bottom_cos, bottom_sin);
			osg::Vec3 right_bottom_normal = osg::Vec3(nxt_normal_x*bottom_cos, nxt_normal_y*bottom_cos, bottom_sin);
			osg::Vec3 left_top_normal = osg::Vec3(cur_normal_x*top_cos, cur_normal_y*top_cos, top_sin);
			osg::Vec3 right_top_normal = osg::Vec3(nxt_normal_x*top_cos, nxt_normal_y*top_cos, top_sin);

			vertices->push_back(left_bottom_vert);
			vertices->push_back(right_bottom_vert);
			vertices->push_back(right_top_vert);

			vertices->push_back(left_bottom_vert);
			vertices->push_back(right_top_vert);
			vertices->push_back(left_top_vert);
			/*
			normals->push_back(left_bottom_normal);
			normals->push_back(right_bottom_normal);
			normals->push_back(right_top_normal);

			normals->push_back(left_bottom_normal);
			normals->push_back(right_bottom_normal);
			normals->push_back(right_top_normal);
			*/
		}

	}

	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
	colors->push_back(osg::Vec4(1.0, 0.0, 0.0, 0.5));

	normals->push_back(osg::Vec3(0.0f, -1.0f, 0.0f));

	osg::ref_ptr<osg::Geometry> sphere = new osg::Geometry;
	sphere->setVertexArray(vertices.get());
	sphere->setNormalArray(normals.get());
	sphere->setNormalBinding(osg::Geometry::BIND_OVERALL);
	sphere->setColorArray(colors.get());
	sphere->setColorBinding(osg::Geometry::BIND_OVERALL);
	sphere->addPrimitiveSet(new osg::DrawArrays(GL_TRIANGLES, 0, vertices->size()));
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(sphere.get());
	return geode.release();
}


void setupShader(osg::StateSet* ss)
{
	std::string radarVertPath = osgDB::findDataFile("radar.vert");
	if (radarVertPath.empty())
	{
		osg::notify(osg::WARN) << "(Infrare::CreateOEInfrareShader): could not find radar.vert! " << std::endl;
		return;
	}

	std::string radarFragPath = osgDB::findDataFile("radar.frag");
	if (radarFragPath.empty())
	{
		osg::notify(osg::WARN) << "(Infrare::CreateOEInfrareShader): could not find radar.frag! " << std::endl;
		return;
	}

	osg::Program* program = new osg::Program;

	osg::ref_ptr<osg::Shader> vertObj = new osg::Shader(osg::Shader::VERTEX);
	osg::ref_ptr<osg::Shader> fragObj = new osg::Shader(osg::Shader::FRAGMENT);
	bool bret = vertObj->loadShaderSourceFromFile(radarVertPath.c_str());
	bret = fragObj->loadShaderSourceFromFile(radarFragPath.c_str());
	program->addShader(vertObj);
	program->addShader(fragObj);
	ss->setAttributeAndModes(program, osg::StateAttribute::ON);
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Group> root = new osg::Group;

	osg::ref_ptr<osg::BlendFunc> blendFunc = new osg::BlendFunc;
	blendFunc->setFunction(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	osg::StateSet* stateset = root->getOrCreateStateSet();
	stateset->setAttributeAndModes(blendFunc);
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	//stateset->setMode(GL_CULL_FACE, osg::StateAttribute::ON);
	stateset->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	//setupShader(stateset);

	osg::ref_ptr<osg::Uniform>  _radiusUniform = new osg::Uniform("radius", float(1.0));
	osg::ref_ptr<osg::Uniform>  _timeUniform = new osg::Uniform("time", float(0.0));
	osg::ref_ptr<osg::Uniform> clipHeightRatioUniform = new osg::Uniform("clipheight_ratio",0.6);
	stateset->addUniform(_radiusUniform.get());
	// stateset->addUniform(_timeUniform);
	// stateset->addUniform(clipHeightRatioUniform.get());

	// osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	// geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0, 0.0, 0.0), 1.0)));
	osg::ref_ptr<osg::Geode> geode = createSphere(8, 8, 1.0);

	osg::ref_ptr<osg::Geode> redQuad = createQuad(osg::Vec4(1.0, 0.0, 0.0, 0.5));
	// osg::ref_ptr<osg::Geode> blueQuad = createQuad(osg::Vec4(1.0, 0.0, 0.0, 0.5));

	osg::ref_ptr<osg::PositionAttitudeTransform> redNode = new osg::PositionAttitudeTransform;
	redNode->setPosition(osg::Vec3(0.0, 0.0, 0.0));
	redNode->addChild(redQuad.get());
	osg::ref_ptr<osg::PositionAttitudeTransform> blueNode = new osg::PositionAttitudeTransform;
	// blueNode->setPosition(osg::Vec3(0.25, -0.5, 0.0));
	// blueNode->addChild(blueQuad.get());

	//root->addChild(redNode.get());
	//root->addChild(blueNode.get());
	root->addChild(geode.get());

	osgViewer::Viewer viewer;
	viewer.getCamera()->setClearColor(osg::Vec4(0.0, 0.0, 0.0, 0.0));
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.addEventHandler(new osgGA::StateSetManipulator(viewer.getCamera()->getOrCreateStateSet()));//ÏÔÊ¾Íø¸ñ
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
