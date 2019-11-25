# OpenSceneGraph 3.0 - Beginner's Guide 源码
* 03_MonitorRFC
	- 查看引用计数的变化
	- osg::Referenced::referenceCount() --- 查看引用计数
* 03_ReadFromCommandLine
	- 从命令行读取文件
	- osg::ArgumentParser
* 03_RedirectNotifier
	- 设置输出调试信息到文件
	- 派生类 osg::NotifyHandler
		- 重写 notify() 方法
	- osg::setNotifyLevel() / osg::setNotifyHandler() 方法
* 04_ColoredQuad
	- 绘制一个四边形
	- Geode
		- Geometry
			- VertexArray
			- NormalArray
			- ColorArray
			- PrimitiveSet
* 04_DrawOctahedron
	- 绘制一个多面体, 主要是讲图元, osg::DrawElementsUInt
	- osgUtil::SmoothingVisitor::smooth 平滑一个几何体。
* 04_OpenGLTeapot
	- 自定义 drawable, 进行自定义绘制
	- osg::Drawable::computeBound() --- 返回对应的围绕盒
	- osg::Drawable::drawImplementation() --- 绘制部分， 其内可以调用opengl的相关函数进行绘制。
* 04_PrimitiveFunctor
	- 仿函数, 可以统计多边形面信息
	- osg::TriangleFunctor, 这里使用其打印面信息，调用模板类的 operator() 函数进行自定义操作
	- 主要原理 osg::Drawable::accept(Functor), 使用 Functor 的函数代替 OpenGL 的绘制函数。
* 04_SimpleObject
	- 几个简单的形状 box, sphere, cone
	- osg::ShapeDrawable::setShape()
* 04_TessellatePolygon
	- 凹多边形转凸多边形(分形化一个 Geometry)
	- osgUtil::Tessellator
* 05_AddModel
	- osg::Group::addChild
* 05_AnalyzeStructure
	- 遍历节点, 打印出其库名和类名
	- 从 osg::NodeVisitor 派生
* 05_LodNode
	- osgUtil::Simplifier
	- osg::LOD --- addChild, 添加子节点。
* 05_ProxyNode
	- osg::ProxyNode
		- setFileName --- 显示的时候再加载
* 05_SwitchAnimate
	- osg::Switch
		- setValue 设置可见性
		- 重写 osg::Switch::traverse 方法
* 05_SwitchNode
	- osg::Switch
		- addChild 方法
* 05_TranslateNode
	- osg::MatrixTransform
		- setMatrix
	- osg::Matrix::translate
* 06_BezierCurve
	- 使用几何着色器创建贝塞尔曲线， 几何着色器插值生成顶点
	- 设置输出类型为 GL_LINE_STRIP， 输入类型为 GL_LINES_ADJACENCY_EXT
	- 使用 line adjacency 图元作为输入, 该输入有四个元素
* 06_CartoonCow
	- 使用着色器进行卡通着色
* 06_Fog
	- Fog 的使用
		- setMode，setStart，setEnd，setColor
* 06_Lighting
	- 创建 osg::LightSource --- 光源实体
		- 创建 osg::Light --- 光源属性
		- setLight
			- setLightNum --- 设置光源序号
		- 将 LightSource 添加至场景中
* 06_PolygonMode
	- 线框模式绘制
	- osg::PolygonMode
		- setMode
* 06_StateSetInherit
	- 状态属性的继承关系
	-  osg::StateAttribute::OVERRIDE，osg::StateAttribute::PROTECTED
* 06_Texture2D
	- 创建一个四边形，然后贴上纹理
* 06_Translucent --- 创建透明物体
	- 创建球体几何体
	- osg::BlendFunc
* 07_FrameLoop
	- osgViewer::Viewer::getFrameStamp()::getFrameNumber(): 得到帧号
* 07_HUD
	- 创建一个相机绘制 HUD 内容，相机有自己的矩阵，而后添加子节点
* 07_MultipleScene(多窗口场景)
	- osgViewer::View --- 创建一个单独的窗口，在其内绘制视图场景
	- osgViewer::CompositeViewer::addView --- 添加这样的视图窗口
* 07_MultiSampling(多重采样)
	- osg::DisplaySettings::setNumMultiSamples() --- 设置多重采样
* 07_RTT(渲染到纹理)
	- osg::Camera::setRenderTargetImplementation() --- 设置渲染到帧缓存对象
	- osg::Camera::attach() --- 渲染至纹理

* 08_AnimateCharacter
	- osgAnimation::BasicAnimationManager --- 派生自更新回调
		- getAnimationList() --- 得到动画列表
		- playAnimation() --- 应用上面动画列表中的一个动画
* 08_AnimationChannel
	- osgAnimation::Vec3Keyframe, osgAnimation::QuatKeyframe 关键帧组成 osgAnimation::Vec3KeyframeContainer 和 osgAnimation::QuatKeyframeContainer 两个容器
	- osgAnimation::Vec3LinearChannel/osgAnimation::QuatSphericalLinearChannel
		- getOrCreateSampler()
			- getOrCreateKeyframeContainer --- 得到容器
	- osgAnimation::Animation
		- setPlayMode
		- addChannel

* 08_GeometryDynamically
	- 通过更新回调更改一个几何体
* 08_AnimationPath
	- 实现动画路径
* 08_FadingIn
	- 设置对象的状态属性更新回调
* 08_Flashing
	- 设置闪烁动画, 使用 osg::ImageSequence
* 09_PickingGeometry
	- 鼠标点击创建框框, 通过 MatrixTransform 实现
	- NodeMask 的使用
	- 鼠标点击相交检测
