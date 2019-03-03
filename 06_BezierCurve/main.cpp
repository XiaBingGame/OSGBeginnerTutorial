#include <iostream>
#include <osg/Program>
#include <osg/LineWidth>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

static const char* vertSource = {
	"#version 120\n"
	"#extension GL_EXT_geometry_shader4 : enable\n"
	"void main()\n"
	"{ gl_Position = ftransform(); }\n"
};

static const char* geomSource = {
	"#version 120\n"
	"#extension GL_EXT_geometry_shader4 : enable\n"
	"uniform int segments;\n"
	"void main(void)\n"
	"{\n"
	"    float delta = 1.0 / float(segments);\n"
	"    vec4 v;\n"
	"    for ( int i=0; i<=segments; ++i )\n"
	"    {\n"
	"        float t = delta * float(i);\n"
	"        float t2 = t * t;\n"
	"        float one_minus_t = 1.0 - t;\n"
	"        float one_minus_t2 = one_minus_t * one_minus_t;\n"
	"        v = gl_PositionIn[0] * one_minus_t2 * one_minus_t + \n"
	"            gl_PositionIn[1] * 3.0 * t * one_minus_t2 +\n"
	"            gl_PositionIn[2] * 3.0 * t2 * one_minus_t +\n"
	"            gl_PositionIn[3] * t2 * t;\n"
	"        gl_Position = v;\n"
	"        EmitVertex();\n"
	"    }\n"
	"    EndPrimitive();\n"
	"}\n"
};

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
	vertices->push_back(osg::Vec3(0.0f, 0.0f, 0.0f));
	vertices->push_back(osg::Vec3(1.0f, 1.0f, 1.0f));
	vertices->push_back(osg::Vec3(2.0f, 1.0f, -1.0f));
	vertices->push_back(osg::Vec3(3.0f, 0.0f, 0.0f));
	osg::ref_ptr<osg::Geometry> controlPoints = new osg::Geometry;
	controlPoints->setVertexArray(vertices.get());
	controlPoints->addPrimitiveSet(
		new osg::DrawArrays(GL_LINES_ADJACENCY_EXT, 0, 4));
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(controlPoints.get());

	int segments = 10;
	osg::ref_ptr<osg::Program> program = new osg::Program;
	program->addShader(
		new osg::Shader(osg::Shader::VERTEX, vertSource));
	program->addShader(
		new osg::Shader(osg::Shader::GEOMETRY, geomSource));
	program->setParameter(GL_GEOMETRY_VERTICES_OUT_EXT, segments + 1);
	program->setParameter(GL_GEOMETRY_INPUT_TYPE_EXT,
		GL_LINES_ADJACENCY_EXT);
	program->setParameter(GL_GEOMETRY_OUTPUT_TYPE_EXT,
		GL_LINE_STRIP);

	osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth;
	lineWidth->setWidth(2.0f);

	osg::StateSet* stateset = geode->getOrCreateStateSet();
	stateset->setAttributeAndModes(program.get());
	stateset->setAttribute(lineWidth.get());
	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	stateset->addUniform(new osg::Uniform("segments", segments));

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(geode.get());
	return viewer.run();
}
