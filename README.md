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
	- osgAnimation::UpdateMatrixTransform
		- getStackedTransforms()
			- 这是个容器， 保存 osgAnimation::StackedTranslateElement 元素 和 osgAnimation::StackedQuaternionElement 元素，这些名称的名字和之前 channel 的名字一样
		- 这是一个节点更新回调
	- osgAnimation::BasicAnimationManager
		- registerAnimation 注册一个动画
		- 这也是一个更新回调
		- playAnimation 播放动画
* 08_AnimationPath
	- 实现动画路径
	- osg::AnimationPath
		- setLoopMode: 设置循环模式
		- insert: 插入一个控制点
	- osg::AnimationPathCallback 节点回调
		- setAnimationPath 设置路径(osg::AnimationPath)
	- 通常设置为节点更新回调
* 08_FadingIn
	- 设置对象的状态属性更新回调
	- osg::StateAttributeCallback 派生自 osg::Callback
		- bool run()
		- void operator(), 两个参数, 分别为 osg::StateAttribute 和 osg::NodeVisitor 指针
			- 本例修改 osg::Material 内 Diffuse 的 alpha 值, osg::Material 从 osg::StateAttribute 指针而来
		- osg::StateAttribute 可以设置更新回调
	- osgAnimation::InOutCubicMotion
		- 类型别名 osgAnimation::MathMotionTemplate<InOutCubicFunction>
		- 结构体 MathMotionTemplate 派生自 osgAnimation::Motion
			- osgAnimation::Motion
				- 纯基类函数 getValueInNormalizedRange, 第一个参数是时间比例(当前时间/总时间),第二个参数为返回的结果
			- MathMotionTemplate 重写了函数 getValueInNormalizedRange, 调用了模板类的 getValueAt 函数
		- 结构体 InOutCubicFunction 重写了 getValueAt 函数, 三次方来回运动
* 08_Flashing
	- 设置闪烁动画, 使用 osg::ImageSequence
		- addImage() --- 添加图像 
		- setLength() --- 设置时间长度
		- play() --- 播放
	- 可以学会如何创建 osg::Image
		- allocateImage 分配内存
		- data() 某一行列的数据指针
	- osg::Texture2D 纹理
		- setImage() 目标可以是 osg::ImageSequence
* 08_GeometryDynamically
	- 通过更新回调更改一个几何体, 该回调为 osg::Drawable::UpdateCallback 的派生类
	- 更新几何数组后调用函数 osg::Geometry::dirtyDisplayList() 和 osg::Geometry::dirtyBound() 更新显示列表和围绕盒
* 08_SwitchUpdate
	- 通过更新回调切换Switch节点子节点的状态
* 09_DrivingCessna
	- osg::Camera
		- setAllowEventFocus() 是否允许其关联窗口产生的事件影响到该相机
		- setViewMatrixAsLookAt() 设置视图矩阵
	- osgViewer::Viewer
		- addEventHandler() 添加事件处理(处理窗口事件)
	- osgGA::GUIEventHandler 派生窗口事件处理类
		- 重写 bool handle() 函数
			- osgGA::GUIEventAdapter 获取键盘鼠标事件
* 09_GCTraits
	- osg::GraphicsContext::Traits 创建一个 traits
	- osg::GraphicsContext::createGraphicsContext 使用该 traits 创建一个窗口 osg::GraphicsContext
	- osg::Camera 创建一个相机
		- setGraphicsContext 设置其窗口上下文
		- setProjectionMatrixAsPerspective 设置投影矩阵
	- osgViewer::Viewer
		- setCamera 设置相机
* 09_PickingGeometry --- 选择框
	- 展示了如何创建选择框
		- 线框,禁光照, 矩阵变换节点
		- 通过矩阵变换绘制和移动线框
	- 鼠标点击创建框框, 通过 MatrixTransform 实现
	- NodeMask 的使用
	- 鼠标点击相交检测
	- osgUtil::IntersectionVisitor 访问, 使用相机 accept 调用该 visitor
	- osg::computeLocalToWorld 计算矩阵

* 09_UserTimer --- 可以添加一个用户事件
	- osgGA::GUIEventAdapter::FRAME 可以处理帧事件
	- viewer->getEventQueue()->userEvent() --- 添加一个用户事件
* 09_Win32Handler --- Win32 API 和 osg
	- WM_CREATE 事件创建一个 osgViewer 而后用线程推进
	- 设置 traits->inheritedWindowData = windata(为窗口句柄)

* 10_CustomFormat --- 自定义格式插件读写
	- 派生自 osgDB::ReaderWriter
	- REGISTER_OSGPLUGIN 宏注册

* 11_Billboard 公告板
