#include <iostream>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/Switch>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIEventHandler>

struct TimeInfo : public osg::Referenced
{
	TimeInfo(unsigned int c) : _count(c) {}
	unsigned int _count;
};

class TimerHandler : public osgGA::GUIEventHandler
{
public:
	TimerHandler(osg::Switch* sw) : _switch(sw), _count(0) {}
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter &aa);

private:
	osg::ref_ptr<osg::Switch> _switch;
	unsigned int _count;
};

bool TimerHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter &aa)
{
	switch (ea.getEventType())
	{
	case osgGA::GUIEventAdapter::FRAME:
		if (_count % 100 == 0)
		{
			osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
			if (viewer)
			{
				viewer->getEventQueue()->userEvent(new TimeInfo(_count));
			}
		}
		_count++;
		break;
	case osgGA::GUIEventAdapter::USER:
		if (_switch.valid())
		{
			const TimeInfo* ti = dynamic_cast<const TimeInfo*>(ea.getUserData()
					);
			std::cout << "Timer event at: " << ti->_count << std::endl;

			_switch->setValue(0, !_switch->getValue(0));
			_switch->setValue(1, !_switch->getValue(1));
		}
		break;
	default:
		break;
	}
	return false;
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Node> model1 = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr<osg::Node> model2 = osgDB::readNodeFile("cessnafire.osg");
	osg::ref_ptr<osg::Switch> root = new osg::Switch;
	root->addChild(model1.get(), false);
	root->addChild(model2.get(), true);

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	viewer.addEventHandler(new TimerHandler(root.get()));
	return viewer.run();
}
