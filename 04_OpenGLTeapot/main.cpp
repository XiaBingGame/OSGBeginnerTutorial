#include <iostream>
#include <GL/glut.h>
#include <OSG/Drawable>
#include <OSG/Geode>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

class TeapotDrawable : public osg::Drawable
{
public:
	TeapotDrawable(float size = 1.0f) : _size(size) { }
	TeapotDrawable(const TeapotDrawable& copy, 
		const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY) :
		osg::Drawable(copy, copyop), _size(copy._size) {}
	META_Object(osg, TeapotDrawable);
	virtual osg::BoundingSphere computeBound() const;
	virtual void drawImplementation(osg::RenderInfo& renderInfo) const;
protected:
	float _size;
};

osg::BoundingSphere TeapotDrawable::computeBound() const
{
	return osg::BoundingSphere(osg::Vec3(0.0f, 0.0f, 0.0f), _size);
}

void TeapotDrawable::drawImplementation(osg::RenderInfo& renderInfo) const
{
	glFrontFace(GL_CW);
	glutSolidTeapot(_size);
	glFrontFace(GL_CCW);
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Geode> root = new osg::Geode;
	root->addDrawable(new TeapotDrawable(1.0));
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
