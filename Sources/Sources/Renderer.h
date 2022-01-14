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
		char* _swapBuffer;
		
		unsigned long _width;
		unsigned long _height;
		// TODO: Implement this.
		void FlipBuffer();
		void Set(int x, int y, const Color& color);
	public:
		Renderer(HWND hWnd);
		~Renderer(); 
		void DrawScene();
		void Line(Vec2i v0, Vec2i v1, const Color& color);
	};
}