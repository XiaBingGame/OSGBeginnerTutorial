#include <iostream>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
#include <osgUtil/LineSegmentIntersector> 

// 鼠标点击创建框框, 通过 MatrixTransform 实现
// NodeMask 的使用
// 鼠标点击相交检测
// osgGA::GUIEventHandler
class PickHandler : public osgGA::GUIEventHandler
{
public:
	osg::Node* getOrCreateSelectionBox();
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
	osg::ref_ptr<osg::MatrixTransform> _selectionBox;
};

osg::Node* PickHandler::getOrCreateSelectionBox()
{
	if (!_selectionBox)
	{
		osg::ref_ptr<osg::Geode> geode = new osg::Geode;
		geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(), 1.0f)));

		_selectionBox = new osg::MatrixTransform;
		_selectionBox->setNodeMask(0x01);
		_selectionBox->addChild(geode.get());
		
		osg::StateSet* ss = _selectionBox->getOrCreateStateSet();
		ss->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
		ss->setAttributeAndModes(new osg::PolygonMode(
			osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE));
	}
	return _selectionBox.get();
}

bool PickHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	if (ea.getEventType() != osgGA::GUIEventAdapter::RELEASE ||
		ea.getButton() != osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON ||
		!(ea.getModKeyMask() & osgGA::GUIEventAdapter::MODKEY_CTRL))
		return false;

	osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
	if (viewer)
	{
		osg::ref_ptr<osgUtil::LineSegmentIntersector> intersector =
			new osgUtil::LineSegmentIntersector(
				osgUtil::Intersector::WINDOW, ea.getX(), ea.getY());
		osg::ref_ptr<osgUtil::IntersectionVisitor> iv =
			new osgUtil::IntersectionVisitor(intersector.get());
		iv->setTraversalMask(~0x01);
		viewer->getCamera()->accept(*(iv.get()));

		if (intersector->containsIntersections())
		{
			const osgUtil::LineSegmentIntersector::Intersection &result = 
				*(intersector->getIntersections().begin());
			osg::BoundingBox bb = result.drawable->getBoundingBox();
			osg::Vec3 worldCenter = bb.center() * osg::computeLocalToWorld(result.nodePath);
			_selectionBox->setMatrix(osg::Matrix::scale(bb.xMax() - bb.xMin(), bb.yMax() - bb.yMin(), bb.zMax() - bb.zMin())*
				osg::Matrix::translate(worldCenter));
		}
	}

	return false;
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Node> model1 = osgDB::readNodeFile("cessna.osg");
	osg::ref_ptr<osg::Node> model2 = osgDB::readNodeFile("cow.osg");
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(model1.get());
	root->addChild(model2.get());

	osg::ref_ptr<PickHandler> pickHandler = new PickHandler();
	root->addChild(pickHandler->getOrCreateSelectionBox());

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	viewer.addEventHandler(pickHandler.get());
	return viewer.run();
}
