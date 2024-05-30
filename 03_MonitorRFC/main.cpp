#include <iostream>
#include <osg/Referenced>
#include <osg/ref_ptr>
#include <osg/Matrixd>
#include <osg/Vec3d>

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

	osg::Matrixd m = osg::Matrixd::rotate(osg::DegreesToRadians(70.0), osg::X_AXIS)  * 
		osg::Matrixd::rotate(osg::DegreesToRadians(30.0), osg::Y_AXIS) * osg::Matrixd::rotate(osg::DegreesToRadians(90.0), osg::Z_AXIS);
	osg::Vec3d v = osg::Vec3d(0.0, 1.0, 0.0);
	v = v * m;
	std::cout << v.x() << ", " << v.y() << ", " << v.z() << std::endl;
	osg::Vec3d v2(v.x(), v.y(), 0.0);
	v2.normalize();
	double yaw = osg::RadiansToDegrees(acos(v2.y()));
	if (v2.x() > 0)
		yaw = -yaw;
	std::cout << "yaw: " << yaw << std::endl;

	double pv = v2.x()*v.x() + v2.y()*v.y();
	double pitch = acos(pv);
	pitch = osg::RadiansToDegrees(pitch);
	std::cout << "pitch: "<< pitch << std::endl;

	system("pause");
	return 0;
}
