// SOFApp.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Week6App.h"

Week6App* app;
int main()
{
	app = new Week6App();
	SOF::Window::CreateWindow(1024, 768, "Week6", app);
	SOF::Window::Run();

    return 0;
}

