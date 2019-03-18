#include <iostream>
#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgFX/Outline>
#include <osgViewer/Viewer>

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("cessna.osg");
	
	osg::ref_ptr<osgFX::Outline> outline = new osgFX::Outline;
	outline->setWidth(8);
	outline->setColor(osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f));
	outline->addChild(model.get());
	
	osg::DisplaySettings::instance()->setMinimumNumStencilBits(1);


	osgViewer::Viewer viewer;
	viewer.getCamera()->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(outline.get());
	return viewer.run();
}
