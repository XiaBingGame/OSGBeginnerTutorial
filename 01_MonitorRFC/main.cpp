#include <iostream>
#include <osg/Referenced>
#include <osg/ref_ptr>

class MonitoringTarget : public osg::Referenced
{
public:
	MonitoringTarget(int id) : _id(id)
	{
		std::cout << "Constructing target " << _id << std::endl;
	}

protected:
	virtual ~MonitoringTarget()
	{
		std::cout << "Destroying target " << _id << std::endl;
	}

	int _id;
};

MonitoringTarget* createMonitoringTarget(int id)
{
	osg::ref_ptr<MonitoringTarget> target = new MonitoringTarget(id);
	return target.release();
}

int main(int argc, char** argv)
{
	osg::ref_ptr<MonitoringTarget> target = new MonitoringTarget(0);
	std::cout << "Referenced count before referring: "
		<< target->referenceCount() << std::endl;
	osg::ref_ptr<MonitoringTarget> anotherTarget = target;
	std::cout << "Referenced count after referring: "
		<< target->referenceCount() << std::endl;

	for (unsigned int i = 1; i < 5; i++)
	{
		osg::ref_ptr<MonitoringTarget> subTarget = new MonitoringTarget(i);
	}
	std::cout << std::endl;

	for (unsigned int i = 6; i < 10; i++)
	{
		MonitoringTarget* subTarget = createMonitoringTarget(i);
	}

	// system("pause");
	return 0;
}
