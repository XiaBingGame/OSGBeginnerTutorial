#include <iostream>
#include <osg/ImageSequence>
#include <osg/Texture2D>
#include <osg/Geometry>
#include <osg/Geode>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>

osg::Image* createSpotLight(const osg::Vec4& centerColor,
	const osg::Vec4& bgColor,
	unsigned int size, float power)
{
	osg::ref_ptr<osg::Image> image = new osg::Image;
	image->allocateImage(size, size, 1, GL_RGBA,
		GL_UNSIGNED_BYTE);

	float mid = (float(size) - 1) * 0.5f;
	float div = 2.0f / float(size);
	for (unsigned int r = 0; r < size; ++r)
	{
		unsigned char* ptr = image->data(0, r);
		for (unsigned int c = 0; c < size; ++c)
		{
			float dx = (float(c) - mid)*div;
			float dy = (float(r) - mid)*div;
			float r = powf(1.0f - sqrtf(dx*dx + dy*dy), power);
			if (r < 0.0f) r = 0.0f;
			osg::Vec4 color = centerColor*r + bgColor*(1.0f - r);
			*ptr++ = (unsigned char)((color[0]) * 255.0f);
			*ptr++ = (unsigned char)((color[1]) * 255.0f);
			*ptr++ = (unsigned char)((color[2]) * 255.0f);
			*ptr++ = (unsigned char)((color[3]) * 255.0f);
		}
	}
	return image.release();
}

int main(int argc, char** argv)
{
	osg::Vec4 centerColor(1.0f, 1.0f, 0.0f, 1.0f);
	osg::Vec4 bgColor(0.0f, 0.0f, 0.0f, 1.0f);
	osg::ref_ptr<osg::ImageSequence> sequence = new
		osg::ImageSequence;
	sequence->addImage(createSpotLight(centerColor, bgColor, 64,
		3.0f));
	sequence->addImage(createSpotLight(centerColor, bgColor, 64,
		3.5f));
	sequence->addImage(createSpotLight(centerColor, bgColor, 64,
		4.0f));
	sequence->addImage(createSpotLight(centerColor, bgColor, 64,
		3.5f));

	osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
	texture->setImage(sequence.get());

	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->addDrawable(osg::createTexturedQuadGeometry(
		osg::Vec3(), osg::Vec3(1.0, 0.0, 0.0), osg::Vec3(0.0, 0.0, 1.0))
	);
	geode->getOrCreateStateSet()->setTextureAttributeAndModes(
		0, texture.get(), osg::StateAttribute::ON);
	sequence->setLength(0.5);
	sequence->play();

	osgViewer::Viewer viewer;
	viewer.setUpViewInWindow(50, 50, 800, 600);
	viewer.setSceneData(geode.get());
	return viewer.run();
}
