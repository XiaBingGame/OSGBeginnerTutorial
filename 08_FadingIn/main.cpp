#include <iostream>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/BlendFunc>
#include <osg/Material>
#include <osgAnimation/EaseMotion>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

class AlphaFadingCallback : public osg::StateAttributeCallback
{
public:
	AlphaFadingCallback()
	{
		_motion = new osgAnimation::InOutCubicMotion(0.0f, 1.0f);
	}

	virtual void operator()(osg::StateAttribute*, osg::NodeVisitor*);
protected:
	osg::ref_ptr<osgAnimation::InOutCubicMotion> _motion;
};

void AlphaFadingCallback::operator()(osg::StateAttribute* sa,
	osg::NodeVisitor* nv)
{
	osg::Material* material = static_cast<osg::Material*>(sa);
	if (material)
	{
		_motion->update(0.005);

		float alpha = _motion->getValue();
		material->setDiffuse(osg::Material::FRONT_AND_BACK,
			osg::Vec4(0.0f, 1.0f, 1.0f, alpha)
		);
	}
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Drawable> quad = osg::createTexturedQuadGeometry(
			osg::Vec3(-0.5f, 0.0f, -0.5f),
			osg::Vec3(1.0f, 0.0f, 0.0f), osg::Vec3(0.0f, 0.0f, 1.0f)
		);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(quad.get());

	osg::ref_ptr<osg::Material> material = new osg::Material;
	material->setAmbient(osg::Material::FRONT_AND_BACK,
		osg::Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	material->setDiffuse(osg::Material::FRONT_AND_BACK,
		osg::Vec4(0.0f, 1.0f, 1.0f, 0.5f));
	material->setUpdateCallback(new AlphaFadingCallback);

	geode->getOrCreateStateSet()->setAttributeAndModes(
		material.get());
	geode->getOrCreateStateSet()->setAttributeAndModes(
		new osg::BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	geode->getOrCreateStateSet()->setRenderingHint(
		osg::StateSet::TRANSPARENT_BIN);

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(geode.get());
	root->addChild(osgDB::readNodeFile("glider.osg"));

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
