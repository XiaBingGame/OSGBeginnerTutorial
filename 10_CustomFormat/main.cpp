#include <iostream>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgDB/FileNameUtils>
#include <osgDB/FileUtils>
#include <osgUtil/SmoothingVisitor>

class ReaderWriterTRI : public osgDB::ReaderWriter
{
public:
	ReaderWriterTRI();

	virtual ReadResult readNode(
		const std::string&, const osgDB::ReaderWriter::Options*) const;
	virtual ReadResult readNode(
		std::istream&, const osgDB::ReaderWriter::Options*) const;
};

ReaderWriterTRI::ReaderWriterTRI()
{
	supportsExtension("tri", "Triangle strip points");
}

ReaderWriterTRI::ReadResult ReaderWriterTRI::readNode(
	const std::string& file, const osgDB::ReaderWriter::Options* options) const
{
	std::string ext = osgDB::getLowerCaseFileExtension(file);
	if (!acceptsExtension(ext)) return
		ReadResult::FILE_NOT_HANDLED;
	std::string fileName = osgDB::findDataFile(file, options);
	if (fileName.empty()) return ReadResult::FILE_NOT_FOUND;

	std::ifstream stream(fileName.c_str(), std::ios::in);
	if (!stream) return ReadResult::ERROR_IN_READING_FILE;
	return readNode(stream, options);
}

ReaderWriterTRI::ReadResult ReaderWriterTRI::readNode(
	std::istream& stream, const osgDB::ReaderWriter::Options*) const
{
	unsigned int index = 0, numPoints = 0;
	stream >> numPoints;

	osg::ref_ptr<osg::Vec3Array> va = new osg::Vec3Array;
	while (index < numPoints && !stream.eof() &&
		!(stream.rdstate()&std::istream::failbit))
	{
		osg::Vec3 point;
		stream >> point.x() >> point.y() >> point.z();
		va->push_back(point);
		index++;
	}

	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	geom->setVertexArray(va.get());
	geom->addPrimitiveSet(
		new osg::DrawArrays(GL_TRIANGLE_STRIP, 0, numPoints));

	osgUtil::SmoothingVisitor smoother;
	smoother.smooth(*geom);

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(geom.get());
	return geode.release();
}

REGISTER_OSGPLUGIN(tri, ReaderWriterTRI)