#include <iostream>
#include <osg/MatrixTransform>
#include <osgGA/GUIEventAdapter>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

class ModelController : public osgGA::GUIEventHandler
{
public:
	ModelController(osg::MatrixTransform* node) :
		_model(node) {}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
	osg::ref_ptr<osg::MatrixTransform> _model;
};

bool ModelController::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if (!_model)
		return false;
	osg::Matrix matrix = _model->getMatrix();
	switch (ea.getEventType())
	{
	case osgGA::GUIEventAdapter::KEYDOWN:
		switch (ea.getKey())
		{
		case 'a':case 'A':
			matrix *= osg::Matrix::rotate(-0.1f, osg::Z_AXIS);
			break;
		case 'd':case 'D':
			matrix *= osg::Matrix::rotate(0.1f, osg::Z_AXIS);
			break;
		case 'w':case 'W':
			matrix *= osg::Matrix::rotate(-0.1f, osg::X_AXIS);
			break;
		case 's':case 'S':
			matrix *= osg::Matrix::rotate(0.1f, osg::X_AXIS);
			break;
		default:
			break;
		}
		_model->setMatrix(matrix);
		break;
	default:
		break;
	}
	return false;
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
	mt->addChild(model.get());
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(mt.get());

	osg::ref_ptr<ModelController> mc = new ModelController(mt.get());

	osgViewer::Viewer viewer;
	viewer.addEventHandler(mc.get());
	viewer.getCamera()->setViewMatrixAsLookAt(
		osg::Vec3(0.0, -100.0, 0.0), osg::Vec3(), osg::Vec3(0.0, 0.0, 1.0));
	viewer.getCamera()->setAllowEventFocus(false);

	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
