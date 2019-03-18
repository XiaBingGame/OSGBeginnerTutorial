#include <iostream>
#include <osg/Geode>
#include <osgDB/ReadFile>
#include <osgText/Text>
#include <osgViewer/Viewer>

class DataReceiverThread : public OpenThreads::Thread
{
public:
	static DataReceiverThread* instance()
	{
		static DataReceiverThread s_thread;
		return &s_thread;
	}

	virtual int cancel();
	virtual void run();

	void addToContent(int ch);
	bool getContent(std::string& str);

protected:
	OpenThreads::Mutex _mutex;
	std::string _content;
	bool _done;
	bool _dirty;
};

int DataReceiverThread::cancel()
{
	_done = true;
	while (isRunning()) YieldCurrentThread();
	return 0;
}

void DataReceiverThread::run()
{
	_done = false;
	_dirty = false;
	do 
	{
		YieldCurrentThread();
		char ch = 0;
		std::cin.get(ch);
		switch (ch)
		{
		case 0: break;
		case 9: // tab ¼ü
			_done = true; break;
		default:
			addToContent(ch); break;
		}
	} while (!_done);
}

void DataReceiverThread::addToContent(int ch)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	_content += ch;
	_dirty = true;
}

bool DataReceiverThread::getContent(std::string& str)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	if (_dirty)
	{
		str += _content;
		// _content = "";
		_dirty = false;
		return true;
	}
	return false;
}

class UpdateTextCallback : public osg::Drawable::UpdateCallback
{
public:
	virtual void update(osg::NodeVisitor* nv, osg::Drawable* drawable)
	{
		osgText::Text* text = dynamic_cast<osgText::Text*>(drawable);
		if (text)
		{
			std::string str("# ");
			if (DataReceiverThread::instance()->getContent(str))
			{
				text->setText(str);
			}
		}
	}
};

int main(int argc, char** argv)
{
	osg::ref_ptr<osgText::Text> text = new osgText::Text;
	text->setFont("fonts/arial.ttf");
	text->setAxisAlignment(osgText::TextBase::SCREEN);
	text->setDataVariance(osg::Object::DYNAMIC);
	text->setInitialBound(osg::BoundingBox(osg::Vec3(), osg::Vec3(400.0f, 20.0f, 20.0f)));
	text->setUpdateCallback(new UpdateTextCallback);

	// osg::ref_ptr<osg::Node> root = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(text.get());
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);


	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(geode.get());
	DataReceiverThread::instance()->startThread();
	viewer.run();
	DataReceiverThread::instance()->cancel();
	return 0;
}
