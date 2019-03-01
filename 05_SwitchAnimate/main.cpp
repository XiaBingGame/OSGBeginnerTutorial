#include <iostream>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/Switch>

class AnimatingSwitch : public osg::Switch
{
public:
	AnimatingSwitch() : osg::Switch(), _count(0) {}
	AnimatingSwitch(const AnimatingSwitch& copy, const osg::CopyOp& copyop) :
		osg::Switch(copy, copyop), _count(copy._count) {}
	
	META_Node(osg, AnimatingSwitch);

	virtual void traverse(osg::NodeVisitor& nv);

protected:
	unsigned int _count;
};

void AnimatingSwitch::traverse(osg::NodeVisitor& nv)
{
	if (!((++_count) % 60))
	{
		setValue(0, !getValue(0));
		setValue(1, !getValue(1));
	}

	osg::Switch::traverse(nv);
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Node> model1 = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr<osg::Node> model2 = osgDB::readNodeFile("cessnafire.osg");
	osg::ref_ptr<AnimatingSwitch> root = new AnimatingSwitch;
	root->addChild(model1.get(), true);
	root->addChild(model2.get(), false);
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
