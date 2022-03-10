#pragma once

#include "DDraw.h"
#include "../Math/Vector.hpp"
#include "../Math/Matrix.hpp"
#include "./Camera.h"
#include "../Utility/Timer.hpp"
#include "../Drawable/RenderObject.h"

#include <iostream>
#include <vector>
#include <queue>
#include <cmath>


class Renderer
{
private:
	const float NDC_MIN_Z{ 0.0f };
	const float NDC_MAX_Z{ 1.0f };
	const float FPS{ 1000.0f / 60.0f };
	const unsigned char BACKGROUND_COLOR{ 88 };

	HWND m_hWnd;
	DDraw* m_pDDraw;
	Camera* m_pCamera;

	char* m_pRenderBuffer;
	char* m_pSwapBuffer;

	uint32_t m_width;
	uint32_t m_height;
	uint8_t m_depth = 1;

	float m_lastTime;
	float m_deltaTime;

	std::vector<RenderObject*> m_pRenderObjects;
	std::vector<RenderObject*> m_pVertexQueue;
	std::vector<RenderObject*> m_pRasterizerQueue;
	std::vector<RenderObject*> m_pFragmentQueue;
	std::vector<RenderObject*> m_pOutputQueue;

	bool m_isKeyDownUp;
	bool m_isKeyDownDown;
	bool m_isKeyDownRight;
	bool m_isKeyDownLeft;

	int m_cursorDeltaXPos;
	int m_cursorDeltaYPos;

	unsigned int m_cursorLastXPos;
	unsigned int m_cursorLastYPos;

	void FilpBuffer();
	void SetPixel(int x, int y, const Color& color);

	void VertexShading();
	void Rasterizer();
	void FragmentShading();
	void OutputMerging();

public:
	Renderer(HWND hWnd);
	~Renderer();

	void AddModel(RenderObject* model);

	// 1. vertex shading
	// 2. rasterizer
	// 3. fragment shading
	// 4. output merging
	void Render();
	// Blt viewport buffer
	void DrawScene();

	// Primitive draw functions
	void Point(Vec2i v, const Color& color);
	void Line(Vec2i v0, Vec2i v1, const Color& color);
	void Triangle(Vec3f v0, Vec3f v1, Vec3f v2, const Color& color);
	void GradiantTriangle(Vec3f v0, Vec3f v1, Vec3f v2, const Color& color0, const Color& color1, const Color& color2);

	void OnKeyDown(WPARAM wParam);
	void OnKeyUP(WPARAM wParam);
	void OnMouseMove();

	void MoveCamera();
	void RotateCamera();

	void UpdateWindowPos();
	//TODO: Implement this
	void UpdateWindowSize();
};
