#include "Common.h"
#include <windows.h>
#include <osgText/Font>
#include <osgText/Text>

namespace osgCookBook
{
	osg::ref_ptr<osgText::Font> g_font = osgText::readFontFile("fonts/arial.ttf");
	
	osg::Camera* createHUDCamera(double left, double right, double bottom, double top)
	{
		osg::ref_ptr<osg::Camera> camera = new osg::Camera;
		// 参考帧为绝对坐标系
		camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
		camera->setClearMask(GL_DEPTH_BUFFER_BIT);
		// 渲染至纹理设置 PRE_RENDER, HUD 则用 POST_RENDER
		camera->setRenderOrder(osg::Camera::POST_RENDER);
		// 该相机关联的窗口是否允许生成事件
		camera->setAllowEventFocus(false);
		camera->setProjectionMatrix(
			osg::Matrix::ortho2D(left, right, bottom, top));
		// 禁止光照
		camera->getOrCreateStateSet()->setMode(
			GL_LIGHTING, osg::StateAttribute::OFF);
	
		// 减少计数, 由其他部分控制该指针
		return camera.release();
	}
	
	osgText::Text* createText(const osg::Vec3& pos, const std::string& content, float size)
	{
		osg::ref_ptr<osgText::Text> text = new osgText::Text;
		text->setDataVariance(osg::Object::DYNAMIC);
		text->setFont(g_font.get());
		text->setCharacterSize(size);
		text->setAxisAlignment(osgText::TextBase::XY_PLANE);
		text->setPosition(pos);
		text->setText(content);
		return text.release();
	}
}
