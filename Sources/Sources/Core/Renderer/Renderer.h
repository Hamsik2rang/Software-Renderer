#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>

#include "DDraw.h"
#include "Camera.h"
#include "../Drawable/Model.h"
#include "../Math/Matrix.hpp"
#include "../Math/Interpolate.hpp"
#include "../Utility/Timer.hpp"

class Renderer
{
private:
	const float NDC_MIN_Z{ 0.0f };
	const float NDC_MAX_Z{ 1.0f };
	const float FPS{ 1000.0f / 60.0f };

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

	std::vector<Model*> m_pRenderObjects;
	std::vector<Model*> m_pVertexQueue;
	std::vector<Model*> m_pRasterizerQueue;
	std::vector<Model*> m_pFragmentQueue;
	std::vector<Model*> m_pOutputQueue;

	void FilpBuffer();
	void SetPixel(int x, int y, const Color& color);

	void VertexShading();
	void Rasterizer();
	void FragmentShading();
	void OutputMerging();

public:
	Renderer(HWND hWnd);
	~Renderer();

	void AddModel(Model* model);

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

	void MoveCamera(int vertical, int horizontal);

	void UpdateWindowPos();
	//TODO: Implement this
	void UpdateWindowSize();
};
