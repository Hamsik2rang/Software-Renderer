#pragma once

#include <iostream>
#include <vector>

#include "DDraw.h"
#include "Camera.h"
#include "../Math/Interpolate.hpp"
#include "../Utility/Timer.hpp"

class Renderer
{
private:
	HWND m_hWnd;
	DDraw* m_pDDraw;
	Camera* m_pCamera;
	Timer* m_pTimer;

	char* m_pViewport;
	char* m_pSwapBuffer;

	uint32_t m_width;
	uint32_t m_height;

	void FilpBuffer();
	void SetPixel(int x, int y, const Color& color);

public:
	Renderer(HWND hWnd);
	~Renderer();
	void DrawScene();
	void Point(Vec2i v, const Color& color);
	void Line(Vec2i v0, Vec2i v1, const Color& color);
	void Triangle(Vec3f v0, Vec3f v1, Vec3f v2, const Color& color);
	void GradiantTriangle(Vec3f v0, Vec3f v1, Vec3f v2, const Color& color0, const Color& color1, const Color& color2);

	void UpdateWindowPos();
	void UpdateWindowSize();
};
