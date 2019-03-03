#include <iostream>
#include <string>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

class InfoVisitor : public osg::NodeVisitor
{
public:
	InfoVisitor() : _level(0)
	{
		setTraversalMode(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN);
	}

	std::string space()
	{
		return std::string(_level * 2, ' ');
	}

	virtual void apply(osg::Node& node);
	virtual void apply(osg::Geode& geode);

private:
	unsigned int _level;
};

void InfoVisitor::apply(osg::Node& node)
{
	std::cout << space() << node.libraryName() << "::"
		<< node.className() << std::endl;

	_level++;
	traverse(node);
	_level--;
}

void InfoVisitor::apply(osg::Geode& geode)
{
	std::cout << space() << geode.libraryName() << "::"
		<< geode.className() << std::endl;

	_level++;
	for (unsigned int i = 0; i < geode.getNumDrawables(); i++)
	{
		osg::Drawable* drawable = geode.getDrawable(i);
		std::cout << space() << drawable->libraryName() << "::"
			<< drawable->className() << std::endl;
	}

	traverse(geode);
	_level--;
}

int main(int argc, char** argv)
{
	osg::ref_ptr<osg::Node> root = osgDB::readNodeFile("cessnafire.osg");
	InfoVisitor infoVisitor;
	root->accept(infoVisitor);
	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(root.get());
	return viewer.run();
}
