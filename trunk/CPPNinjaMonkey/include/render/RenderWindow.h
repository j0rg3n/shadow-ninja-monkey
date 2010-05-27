#pragma once
#ifndef RENDERWINDOW_H_INCLUDED
#define RENDERWINDOW_H_INCLUDED


class RenderThreadContext;


// -----------------------------------------------------------------------------


class RenderWindow
{
public:
	RenderWindow();
	~RenderWindow();

	void Init();
	void Shutdown();

	RenderThreadContext* CreateRenderThreadContext();

private:
	class Impl;

	Impl* m_pImpl;
};


#endif // RENDERWINDOW_H_INCLUDED