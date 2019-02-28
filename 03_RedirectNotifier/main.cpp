#include <iostream>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <fstream>

class LogFileHandler : public osg::NotifyHandler
{
public:
	LogFileHandler(const char* filename)
	{
		_log.open(filename);
	}
	~LogFileHandler()
	{
		_log.close();
	}
	virtual void notify(osg::NotifySeverity severity, const char *message)
	{
		_log << message;
	}

private:
	std::ofstream _log;
};

int main(int argc, char** argv)
{
	osg::setNotifyLevel(osg::INFO);
	osg::setNotifyHandler(new LogFileHandler("output.txt"));

	osg::ref_ptr<osg::Node> root = osgDB::readNodeFile("cessna.osg");
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());

	OSG_FATAL << "Test Fatal notify.";

	return viewer.run();
}
