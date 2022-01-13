#pragma once

#include <iostream>
#include <vector>
#include <cmath>

#include "DDraw.h"


namespace hs {
	class Renderer
	{
	private:
		DDraw* _ddraw;
		HWND _hWnd;

		char* _renderBuffer;
		
		unsigned long _width;
		unsigned long _height;

		void FlipBuffer();
		void Set(int x, int y, const Color& color);
	public:
		Renderer(HWND hWnd, unsigned long width, unsigned long height);
		~Renderer(); 
		void DrawScene();
		void AddObject(Vec2i v0, Vec2i v1, const Color& color);
	};
}