#include <iostream>
#include <osgAnimation/BasicAnimationManager>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

int main(int argc, char** argv)
{
	osg::ArgumentParser arguments(&argc, argv);
	bool listAll = false;
	std::string animationName;
	arguments.read("--animation", animationName);
	animationName = "Idle_Main";
	if (arguments.read("--listall")) listAll = true;
	listAll = true;


	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("bignathan.osg");
	if (!model) return 1;

	osgAnimation::BasicAnimationManager* manager =
		dynamic_cast<osgAnimation::BasicAnimationManager*>
		(model->getUpdateCallback());
	if (!manager) return 1;

	const osgAnimation::AnimationList& animations =
		manager->getAnimationList();
	if (listAll) std::cout << "**** Animations ****" << std::endl;
	for (unsigned int i = 0; i < animations.size(); ++i)
	{
		const std::string& name = animations[i]->getName();
		if (name == animationName)
			manager->playAnimation(animations[i].get());
		if (listAll) std::cout << name << std::endl;
	}
	if (listAll)
	{
		std::cout << "********************" << std::endl;
		//return 0;
	}

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(model.get());
	return viewer.run();
}
