# OpenSceneGraph 3.0 - Beginner's Guide 源码
* 03_MonitorRFC
	- 查看引用计数的变化
* 03_ReadFromCommandLine
	- 从命令行读取文件
* 03_RedirectNotifier
	- 设置输出调试信息到文件
	- 派生类 osg::NotifyHandler
	- 重写 notify() 方法
	- osg::setNotifyLevel() / osg::setNotifyHandler() 方法
* 04_ColoredQuad
	- 绘制一个四边形
* 04_DrawOctahedron
	- 绘制一个多面体, 主要是讲图元
* 04_OpenGLTeapot
	- 自定义 drawable, 进行自定义绘制
	- osg::Drawable 派生, 重写 drawImplementation() 方法
* 04_PrimitiveFunctor
	- 仿函数, 可以统计多边形面信息
	- osg::TriangleFunctor, 这里使用其打印面信息
* 04_SimpleObject
	- 几个简单的形状 box, sphere, cone
* 04_TessellatePolygon
	- 凹多边形转凸多边形
	- osgUtil::Tessellator
* 05_AddModel
	- osg::Group::addChild
* 05_AnalyzeStructure
	- 遍历节点, 打印出其库名和类名
* 05_LodNode
	- osgUtil::Simplifier
	- osg::LOD
* 05_ProxyNode
	- osg::ProxyNode
		- setFileName
* 05_SwitchAnimate
	- osg::Switch
		- setValue 设置可见性
		- 重写 traverse 方法
* 05_SwitchNode
	- osg::Switch
		- addChild
* 05_TranslateNode
	- osg::MatrixTransform
		- setMatrix
* 06_BezierCurve
	- 使用几何着色器创建贝塞尔曲线
	- 使用 line adjacency 图元作为输入, 该输入有四个元素
* 06_CartoonCow
	- 使用着色器卡通着色
* 06_Fog
	- Fog 的使用

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
