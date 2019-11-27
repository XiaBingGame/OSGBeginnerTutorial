#include <iostream>
#include <osgDB/ReadFile>
#include <osg/Geometry>
#include <osg/Geode>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/Viewer>

#define RAND(min, max) \
        ((min) + (float)rand()/(RAND_MAX+1) * ((max)-(min)))

// 随机创建(-100.0, 1.0, -100.0) 到 (100.0, 100.0, 100.0) 范围内的大小为(1.0, 1.0) 的四边形
osg::Geode* createMassiveQuads(unsigned int number)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	for (unsigned int i = 0; i < number; ++i)
	{
		osg::Vec3 randomCenter;
		randomCenter.x() = RAND(-100.0f, 100.0f);
		randomCenter.y() = RAND(1.0f, 100.0f);
		randomCenter.z() = RAND(-100.0f, 100.0f);

		osg::ref_ptr<osg::Drawable> quad =
			osg::createTexturedQuadGeometry(
				randomCenter,
				osg::Vec3(1.0f, 0.0f, 0.0f),
				osg::Vec3(0.0f, 0.0f, 1.0f)
			);
		geode->addDrawable(quad.get());
	}
	return geode.release();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::OccluderNode> occluderNode = new osg::OccluderNode;
	osg::ref_ptr<osg::ConvexPlanarOccluder> cpo = new
		osg::ConvexPlanarOccluder;
	cpo->getOccluder().add(osg::Vec3(-120.0f, 0.0f, -120.0f));
	cpo->getOccluder().add(osg::Vec3(120.0f, 0.0f, -120.0f));
	cpo->getOccluder().add(osg::Vec3(120.0f, 0.0f, 120.0f));
	cpo->getOccluder().add(osg::Vec3(-120.0f, 0.0f, 120.0f));
	occluderNode->setOccluder(cpo.get());

	osg::ref_ptr<osg::Geode> occluderGeode = new osg::Geode;
	occluderGeode->addDrawable(osg::createTexturedQuadGeometry(
		osg::Vec3(-120.0f, 0.0f, -120.0f),
		osg::Vec3(240.0f, 0.0f, 0.0f),
		osg::Vec3(0.0f, 0.0f, 240.0f))
	);
	occluderNode->addChild(occluderGeode.get());

	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(createMassiveQuads(100000));
	root->addChild(occluderNode.get());
	root->getOrCreateStateSet()->setMode(
		GL_LIGHTING, osg::StateAttribute::OFF);

	osgViewer::Viewer viewer;
	viewer.addEventHandler(new osgViewer::StatsHandler);
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
