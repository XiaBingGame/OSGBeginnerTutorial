#include <iostream>
#include <osgDB/ReadFile>
#include <osg/ShapeDrawable>
#include <osg/PagedLOD>
#include <osgDB/WriteFile>
#include <sstream>
#include <osgViewer/Viewer>

float* g_data = NULL;
float g_dx = 1.0f;
float g_dy = 1.0f;
unsigned int g_minCols = 64;
unsigned int g_minRows = 64;
unsigned int g_numCols = 1024;
unsigned int g_numRows = 1024;

#define RAND(min, max) \
        ((min) + (float)rand()/(RAND_MAX+1) * ((max)-(min)))
void createMassiveData()
{
	g_data = new float[g_numCols * g_numRows];
	for (unsigned int i = 0; i < g_numRows; ++i)
	{
		for (unsigned int j = 0; j < g_numCols; ++j)
			g_data[i*g_numCols + j] = RAND(0.5f, 0.0f);
	}
}
float getOneData(unsigned int c, unsigned int r)
{
	return g_data[osg::minimum(r, g_numRows - 1) * g_numCols +
		osg::minimum(c, g_numCols - 1)];
}

std::string createFileName(unsigned int lv, unsigned int x, unsigned int y)
{
	std::stringstream sstream;
	sstream << "quadtree_L" << lv << "_X" << x << "_Y" << y << ".osg";
	return sstream.str();
}

// 递归构建一个四叉树的所有子单元
osg::Node* outputSubScene(unsigned int lv,
	unsigned int x, unsigned int y,
	const osg::Vec4& color)
{
	unsigned int numInUnitCol = g_numCols / (int)powf(2.0f,
		(float)lv);
	unsigned int numInUnitRow = g_numRows / (int)powf(2.0f,
		(float)lv);
	unsigned int xDataStart = x * numInUnitCol, xDataEnd = (x + 1) * numInUnitCol;
	unsigned int yDataStart = y * numInUnitRow, yDataEnd = (y + 1) * numInUnitRow;

	bool stopAtLeafNode = false;
	osg::ref_ptr<osg::HeightField> grid = new osg::HeightField;
	grid->setSkirtHeight(1.0f);
	grid->setOrigin(osg::Vec3(g_dx*(float)xDataStart,
		g_dy*(float)yDataStart, 0.0f));

	if (xDataEnd - xDataStart <= g_minCols &&
		yDataEnd - yDataStart <= g_minRows)
	{
		grid->allocate(xDataEnd - xDataStart + 1, yDataEnd - yDataStart + 1
		);
		grid->setXInterval(g_dx);
		grid->setYInterval(g_dy);
		for (unsigned int i = yDataStart; i <= yDataEnd; ++i)
		{
			for (unsigned int j = xDataStart; j <= xDataEnd; ++j)
			{
				grid->setHeight(j - xDataStart, i - yDataStart,
					getOneData(j, i));
			}
		}
		stopAtLeafNode = true;
	}
	else
	{
		unsigned int jStep = (unsigned int)ceilf(
			(float)(xDataEnd - xDataStart) / (float)g_minCols);
		unsigned int iStep = (unsigned int)ceilf(
			(float)(yDataEnd - yDataStart) / (float)g_minRows);

		grid->allocate(g_minCols + 1, g_minRows + 1);
		grid->setXInterval(g_dx * jStep);
		grid->setYInterval(g_dy * iStep);
		for (unsigned int i = yDataStart, ii = 0; i <= yDataEnd;
			i += iStep, ++ii)
		{
			for (unsigned int j = xDataStart, jj = 0; j <= xDataEnd;
				j += jStep, ++jj)
			{
				grid->setHeight(jj, ii, getOneData(j, i));
			}
		}
	}
	osg::ref_ptr<osg::ShapeDrawable> shape =
		new osg::ShapeDrawable(grid.get());
	shape->setColor(color);
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(shape.get());
	if (stopAtLeafNode) 
		return geode.release();

	osg::ref_ptr<osg::Group> group = new osg::Group;
	group->addChild(
		outputSubScene(lv + 1, x * 2, y * 2, osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f))
	);
	group->addChild(
		outputSubScene(lv + 1, x * 2, y * 2 + 1, osg::Vec4(0.0f, 1.0f, 0.0f, 1.0f))
	);
	group->addChild(
		outputSubScene(lv + 1, x * 2 + 1, y * 2 + 1,
			osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f)));
	group->addChild(
		outputSubScene(lv + 1, x * 2 + 1, y * 2, osg::Vec4(1.0f, 1.0f, 0.0f, 1.0f))
	);

	osg::ref_ptr<osg::PagedLOD> plod = new osg::PagedLOD;
	std::string filename = createFileName(lv, x, y);
	plod->insertChild(0, geode.get());
	plod->setFileName(1, filename);
	osgDB::writeNodeFile(*group, filename);

	plod->setCenterMode(osg::PagedLOD::USER_DEFINED_CENTER);
	plod->setCenter(geode->getBound().center());
	plod->setRadius(geode->getBound().radius());
	float cutoff = geode->getBound().radius() * 5.0f;
	plod->setRange(0, cutoff, FLT_MAX);
	plod->setRange(1, 0.0f, cutoff);
	return plod.release();
}

int main(int argc, char** argv)
{
	createMassiveData();
	osg::ref_ptr<osg::Group> root = new osg::Group;
	root->addChild(
		outputSubScene(0, 0, 0, osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f)));
	osgDB::writeNodeFile(*root, "quadtree.osg");
	delete g_data;
	return 0;
}
