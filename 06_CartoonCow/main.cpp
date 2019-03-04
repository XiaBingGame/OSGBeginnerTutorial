#include <iostream>
#include <osg/Program>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

static const char* vertSource = {
	"varying vec3 normal;\n"
	"void main()\n"
	"{\n"
	"    normal = normalize(gl_NormalMatrix * gl_Normal);\n"
	"    gl_Position = ftransform();\n"
	"}\n"
};

static const char* fragSource = {
	"uniform vec4 color1;\n"
	"uniform vec4 color2;\n"
	"uniform vec4 color3;\n"
	"uniform vec4 color4;\n"
	"varying vec3 normal;\n"
	"void main()\n"
	"{\n"
	"    float intensity = dot(vec3(gl_LightSource[0].position), normal); \n"
	"    if (intensity > 0.95) gl_FragColor = color1;\n"
	"    else if (intensity > 0.5) gl_FragColor = color2;\n"
	"    else if (intensity > 0.25) gl_FragColor = color3;\n"
	"    else gl_FragColor = color4;\n"
	"}\n"
};

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Shader> vertShader =
		new osg::Shader(osg::Shader::VERTEX, vertSource);
	osg::ref_ptr<osg::Shader> fragShader =
		new osg::Shader(osg::Shader::FRAGMENT, fragSource);
	osg::ref_ptr<osg::Program> program = new osg::Program;
	program->addShader(vertShader.get());
	program->addShader(fragShader.get());

	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("cow.osg");
	osg::StateSet* stateset = model->getOrCreateStateSet();
	stateset->setAttributeAndModes(program.get());
	stateset->addUniform(
		new osg::Uniform("color1", osg::Vec4(
			1.0f, 0.5f, 0.5f, 1.0f)));
	stateset->addUniform(
		new osg::Uniform("color2", osg::Vec4(
			0.5f, 0.2f, 0.2f, 1.0f)));
	stateset->addUniform(
		new osg::Uniform("color3", osg::Vec4(
			0.2f, 0.1f, 0.1f, 1.0f)));
	stateset->addUniform(
		new osg::Uniform("color4", osg::Vec4(
			0.1f, 0.05f, 0.05f, 1.0f)));

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(model.get());
	return viewer.run();
}
