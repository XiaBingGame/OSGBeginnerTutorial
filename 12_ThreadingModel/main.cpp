#include <iostream>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/CompositeViewer>

#define RAND(min, max) \
	((min) + (float)rand()/(RAND_MAX+1)*((max)-(min)))

osg::Geode* createMassiveQuads(unsigned int number)
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	for (unsigned int i = 0; i < number; ++i)
	{
		osg::Vec3 randomCenter;
		randomCenter.x() = RAND(-100.0f, 100.0f);
		randomCenter.y() = RAND(1.0f, 100.0f);
		randomCenter.z() = RAND(-100.0f, 100.0f);

		osg::ref_ptr<osg::Drawable> quad =
			osg::createTexturedQuadGeometry(
				randomCenter,
				osg::Vec3(1.0f, 0.0f, 0.0f),
				osg::Vec3(0.0f, 0.0f, 1.0f)
			);
		geode->addDrawable(quad.get());
	}
	return geode.release();
}

osgViewer::View* createView(int x, int y, int w, int h, osg::Node* scene)
{
	osg::ref_ptr<osgViewer::View> view = new osgViewer::View;
	view->setSceneData(scene);
	view->setUpViewInWindow(x, y, w, h);
	return view.release();
}

int main(int argc, char** argv)
{
	osg::ArgumentParser arguments(&argc, argv);
	osgViewer::ViewerBase::ThreadingModel th =
		osgViewer::ViewerBase::AutomaticSelection;
	if (arguments.read("--single")) th = osgViewer::ViewerBase::SingleThreaded;
	else if (arguments.read("--useContext")) th = osgViewer::ViewerBase::ThreadPerContext;
	else if (arguments.read("--useCamera")) th = osgViewer::ViewerBase::ThreadPerCamera;

	osgViewer::View* view1 = createView(50, 50, 640, 480,
		createMassiveQuads(10000));
	osgViewer::View* view2 = createView(50, 550, 320, 240,
		createMassiveQuads(5000));
	osgViewer::View* view3 = createView(370, 550, 320, 240,
		createMassiveQuads(5000));
	view1->addEventHandler(new osgViewer::StatsHandler);

	osgViewer::CompositeViewer viewer;
	viewer.setThreadingModel(th);
	viewer.addView(view1);
	viewer.addView(view2);
	viewer.addView(view3);
	return viewer.run();
}
