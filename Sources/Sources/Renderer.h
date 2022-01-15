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
		Vec3f Barycentric(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f p);
	public:
		Renderer(HWND hWnd);
		~Renderer(); 
		void DrawScene();
		void Line(Vec2i v0, Vec2i v1, const Color& color);
		void Triangle(Vec3f v0, Vec3f v1, Vec3f v2, const Color& color);

		void UpdateWindowPos();
		void UpdateWindowSize();
	};
}