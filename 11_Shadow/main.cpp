#include <iostream>
#include <osg/AnimationPath>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgShadow/ShadowedScene>
#include <osgShadow/ShadowMap>
#include <osgViewer/Viewer>

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

int main(int argc, char** argv)
{
	unsigned int rcvShadowMask = 0x1;
	unsigned int castShadowMask = 0x2;

	osg::ref_ptr<osg::MatrixTransform> groundNode =
		new osg::MatrixTransform;
	groundNode->addChild(osgDB::readNodeFile("lz.osg"));
	groundNode->setMatrix(osg::Matrix::translate(0.0f, 0.0f, -200.0f));
	groundNode->setNodeMask(rcvShadowMask);

	osg::ref_ptr<osg::MatrixTransform> cessnaNode =
		new osg::MatrixTransform;
	cessnaNode->addChild(osgDB::readNodeFile("cessna.osg.0,0,90.rot"));
	cessnaNode->setNodeMask(castShadowMask);
	osg::ref_ptr<osg::AnimationPathCallback> apcb =new osg::AnimationPathCallback;
	apcb->setAnimationPath(createAnimationPath(50.0f, 6.0f));
	cessnaNode->setUpdateCallback(apcb.get());

	osg::ref_ptr<osg::MatrixTransform> truckNode =
		new osg::MatrixTransform;
	truckNode->addChild(osgDB::readNodeFile("dumptruck.osg"));
	truckNode->setMatrix(osg::Matrix::translate(0.0f, 0.0f, -100.0f));
	truckNode->setNodeMask(rcvShadowMask | castShadowMask);

	osg::ref_ptr<osg::LightSource> source = new osg::LightSource;
	source->getLight()->setPosition(osg::Vec4(4.0, 4.0, 10.0, 0.0));
	source->getLight()->setAmbient(osg::Vec4(0.2, 0.2, 0.2, 1.0));
	source->getLight()->setDiffuse(osg::Vec4(0.8, 0.8, 0.8, 1.0));

	osg::ref_ptr<osgShadow::ShadowMap> sm = new osgShadow::ShadowMap;
	sm->setLight(source.get());
	sm->setTextureSize(osg::Vec2s(1024, 1024));
	sm->setTextureUnit(1);

	osg::ref_ptr<osgShadow::ShadowedScene> root =
		new osgShadow::ShadowedScene;
	root->setShadowTechnique(sm.get());
	root->setReceivesShadowTraversalMask(rcvShadowMask);
	root->setCastsShadowTraversalMask(castShadowMask);

	root->addChild(groundNode.get());
	root->addChild(cessnaNode.get());
	root->addChild(truckNode.get());
	root->addChild(source.get());

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
