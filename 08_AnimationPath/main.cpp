#include <iostream>
#include <osg/AnimationPath>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

osg::AnimationPath* createAnimationPath(float radius, float time)
{
	osg::ref_ptr<osg::AnimationPath> path = new
		osg::AnimationPath;
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
	osg::ref_ptr<osg::Node> model =
		osgDB::readNodeFile("cessna.osg.0,0,90.rot");
	osg::ref_ptr<osg::MatrixTransform> root = new
		osg::MatrixTransform;
	root->addChild(model.get());

	osg::ref_ptr<osg::AnimationPathCallback> apcb = new
		osg::AnimationPathCallback;
	apcb->setAnimationPath(createAnimationPath(50.0f, 6.0f));
	root->setUpdateCallback(apcb.get());

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
