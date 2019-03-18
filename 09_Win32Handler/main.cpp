#include <windows.h>
#include <process.h>
#include <iostream>
#include <osgDB/ReadFile>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgViewer/api/Win32/GraphicsWindowWin32>

osg::ref_ptr<osgViewer::Viewer> g_viewer;
bool g_finished;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void render(void*);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT("gui");
	HWND         hwnd;
	MSG          msg;
	WNDCLASS     wndclass; // 要注册的窗口类

	wndclass.style = CS_HREDRAW | CS_VREDRAW;	// 如果移动或者客户区域大小变化时重绘
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Windows NT!"),
			szAppName, MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(szAppName,                  // window class name
		TEXT("OSG and Win32 Window"), // window caption
		WS_OVERLAPPEDWINDOW,        // window style
		100,              // initial x position
		100,              // initial y position
		800,              // initial x size
		600,              // initial y size
		NULL,                       // parent window handle
		NULL,                       // window menu handle
		hInstance,                  // program instance handle
		NULL);                     // creation parameters

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}


/*! \brief 窗口处理函数模板.
*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC         hdc;
	PAINTSTRUCT ps;
	RECT        rect;

	switch (message)
	{
	case WM_CREATE:
	{
		osg::ref_ptr<osg::Referenced> windata = new osgViewer::GraphicsWindowWin32::WindowData(hwnd);
		osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
		traits->x = 0;
		traits->y = 0;
		traits->width = 800;
		traits->height = 600;
		traits->doubleBuffer = true;
		traits->windowDecoration = false;
		traits->inheritedWindowData = windata;
		osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());
		osg::ref_ptr<osg::Camera> camera = new osg::Camera;
		camera->setGraphicsContext(gc.get());
		camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
		camera->setClearMask(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		camera->setClearColor(osg::Vec4(0.2, 0.2, 0.4, 1.0));
		camera->setProjectionMatrixAsPerspective(30.0, (double)traits->width / (double)traits->height,
			1.0, 1000.0);

		g_viewer = new osgViewer::Viewer;
		g_viewer->setCamera(camera.get());
		g_viewer->setSceneData(osgDB::readNodeFile("cessna.osg"));
		g_viewer->setKeyEventSetsDone(0);
		g_viewer->setCameraManipulator(new osgGA::TrackballManipulator);
		g_finished = false;
		_beginthread(render, 0, NULL);



		return 0;
	}
	case WM_PAINT:
		return 0;

	case WM_DESTROY:
		g_viewer->setDone(true);
		while (!g_finished) Sleep(10);

		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

void render(void*)
{
	while (!g_viewer->done())
	{
		g_viewer->frame();
	}
	g_finished = true;
}