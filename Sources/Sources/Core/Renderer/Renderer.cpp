#include "Renderer.h"
#include "../Math/Math.hpp"
#include "../Math/Interpolate.hpp"
#include "../Utility/Random.hpp"
#include <cstring>

Renderer::Renderer(HWND hWnd)
	:m_hWnd(hWnd), g_testColor{ Color(Random::GetRandomInteger(0, 255), Random::GetRandomInteger(0, 255), Random::GetRandomInteger(0, 255), 0),
Color(Random::GetRandomInteger(0, 255), Random::GetRandomInteger(0, 255), Random::GetRandomInteger(0, 255), 0),
Color(Random::GetRandomInteger(0, 255), Random::GetRandomInteger(0, 255), Random::GetRandomInteger(0, 255), 0),
Color(Random::GetRandomInteger(0, 255), Random::GetRandomInteger(0, 255), Random::GetRandomInteger(0, 255), 0),
Color(Random::GetRandomInteger(0, 255), Random::GetRandomInteger(0, 255), Random::GetRandomInteger(0, 255), 0),
Color(Random::GetRandomInteger(0, 255), Random::GetRandomInteger(0, 255), Random::GetRandomInteger(0, 255), 0)}
{
	// Init DirectDraw
	m_pDDraw = new DDraw;
	m_pDDraw->Init(m_hWnd);
	m_width = m_pDDraw->GetWidth();
	m_height = m_pDDraw->GetHeight();

	m_pRenderBuffer = new char[m_width * m_height * 4];
	m_pSwapBuffer = new char[m_width * 4];
	m_pZBuffer = new float[m_width * m_height];

	m_pInputManager = InputManager::GetInstance();
	m_pInputManager->SetScreenSize(m_width, m_height);
	m_pInputManager->SetCursorToCenter();

	memset(m_pRenderBuffer, BACKGROUND_COLOR, m_width * m_height * 4);

	Timer::Elapsed();
}

Renderer::~Renderer()
{
	if (m_pDDraw)
	{
		delete m_pDDraw;
		m_pDDraw = nullptr;
	}
	if (m_pRenderBuffer)
	{
		delete[] m_pRenderBuffer;
		m_pRenderBuffer = nullptr;
	}
	if (m_pSwapBuffer)
	{
		delete[] m_pSwapBuffer;
		m_pSwapBuffer = nullptr;
	}
	if (m_pZBuffer)
	{
		delete[] m_pZBuffer;
		m_pZBuffer = nullptr;
	}
}

// 좌하단을 원점으로 렌더링했을 때 버퍼를 뒤집어 일반적인 Viewport 렌더로 변경해주는 함수
[[deprecated]]
void Renderer::FilpBuffer()
{
	int widthSize = m_width * 4;
	for (uint32_t row = 0; row < m_height / 2; row++)
	{
		memcpy(m_pSwapBuffer, m_pRenderBuffer + (widthSize * row), widthSize);
		memcpy(m_pRenderBuffer + (widthSize * row), m_pRenderBuffer + (widthSize * (m_height - row - 1)), widthSize);
		memcpy(m_pRenderBuffer + (widthSize * (m_height - row - 1)), m_pSwapBuffer, widthSize);
	}
}

void Renderer::SetPixel(int x, int y, const Color& color)
{
	assert(x >= 0 && x < m_width && y >= 0 && y < m_height);
	memcpy(m_pRenderBuffer + (y * m_width * 4) + x * 4, &color, 4);
}

void Renderer::Render()
{
	float curTime = Timer::Elapsed();
	m_deltaTime = curTime - m_lastTime;
	m_lastTime = curTime;
	if (m_pCamera)
	{
		MoveCamera();
		RotateCamera();

		VertexShading();
		Rasterizer();
		FragmentShading();
		OutputMerging();
	}
	else
	{
		std::cerr << "Camera UnReferenced" << std::endl;
	}

	DrawScene();
}

void Renderer::VertexShading()
{
	/// Vertex Shading
	/// Carry out vertex transformation.
	/// transform each local objects to objects which is in clip space.
	/// transformation procedure:
	/// Local space -> world space -> camera space(view space) -> clip space
	
	for (const auto& v : m_pRenderObjects)
	{
		RenderObject* m = new RenderObject(*v);
		
		m->m_scale = { 2.0f, 2.0f, 2.0f };
		////////////////////
		// 1. Get Model Matrix which transform Local Space to World Space
		Mat4f model = Model(m);
		// 2. Get View Matrix which transform World space to Camera Space
		Mat4f view = LookAt(m_pCamera->GetEye(), m_pCamera->GetEye() - m_pCamera->GetFront(), {0.0f, 1.0f, 0.0f});
		// 3. Get Projection Matrix which transform Camera Space to Clip Space
		Mat4f projection = Projection(m_pCamera->GetFovY(), m_pCamera->GetAspect(), m_pCamera->GetNear(), m_pCamera->GetFar());

		for (int i = 0; i < m->m_pBuffer->m_vertices.size(); i++)
		{
			auto affine = m->m_pBuffer->m_vertices[i];
			affine = projection * view * model * affine;

			// Rasterizer 단계를 위한 좌표계 변경(right-hand -> left-hand)
			// 정점 재정렬도 수행해야 하지만 현재 정점 순서를 신경쓰고 있지 않으므로 제외함.
			// 뷰 볼륨 좌표가 (-1 ~ 1, -1 ~ 1, 0 ~ -1) 범위에서 (-1 ~ 1, -1 ~ 1, 0 ~ 1)로 변경됨.
			affine.z *= -1.0f;
			


			m->m_pBuffer->m_vertices[i] = affine;
		}
		m_pRasterizerQueue.push_back(m);
	}
}

void Renderer::Rasterizer()
{
	/// Rasterization
	/// process about interaction of vertices and view frustum(or view volumes)
	/// Carry out these processes.
	/// 1. Clipping
	/// 2. Perspective devision
	/// 3. Back-face Culling
	/// 4. Viewport Transformation
	/// 5. Scan conversion

	for (auto& v : m_pRasterizerQueue)
	{
		bool isClipped = false;
		for (int i = 0; i < v->m_pBuffer->m_vertices.size(); i++)
		{
			Vec4f& affine = v->m_pBuffer->m_vertices[i];
			//...

			// Perspective devision
			affine.x /= affine.w;
			affine.y /= affine.w;
			affine.z /= affine.w;
			affine.w /= affine.w;

			// simple clipping using z-distance
			if (affine.z < 0 || affine.z > 1)
			{
				isClipped = true;
				break;
			}
			// Viewport Transformation
			float aspect = m_pCamera->GetAspect();
			Mat4f viewport = Mat4f::Identity;
			// 1. Invert y axis
			viewport[1][1] = -1.0f;

			// Scale to axis size - (1, 1, 1) to (width, height, 1)
			viewport = Mat4f(
				{ (float)m_width / 2, 0.0f, 0.0f, 0.0f },
				{ 0.0f, (float)m_height / 2, 0.0f,  0.0f },
				{ 0.0f, 0.0f, 1.0f, 0.0f },
				{ 0.0f, 0.0f, 0.0f, 1.0f }
			) * viewport;

			// Transform center to left-top point
			viewport = Mat4f(
				{ 1.0f, 0.0f, 0.0f, (float)m_width / 2 },
				{ 0.0f, 1.0f, 0.0f, (float)m_height / 2 },
				{ 0.0f, 0.0f, 1.0f, NDC_MIN_Z },
				{ 0.0f, 0.0f, 0.0f, 1.0f }) * viewport;

			affine = viewport * affine;
			// Scan Coversion
			// ...
		}
		if (!isClipped)
		{
			m_pFragmentQueue.push_back(v);
		}
	}
	// TODO: change later
	m_pRasterizerQueue.clear();
}

void Renderer::FragmentShading()
{
	// Fragment Shading
	// 1. Texturing

	// TODO: Implement this
	for (auto& v : m_pFragmentQueue)
	{
		m_pOutputQueue.push_back(v);
	}

	m_pFragmentQueue.clear();
}

void Renderer::OutputMerging()
{
	// Output Merging
	
	// 1. z-buffering
	// Clear z-Buffer
	ClearZBuffer();

	// 2. alpha blending
	
	
	// 3. z-culling(If you can)

	// TODO: Implement this
}

void Renderer::AddModel(RenderObject* model)
{
	m_pRenderObjects.push_back(model);
}

void Renderer::DrawScene()
{
	m_pDDraw->BeginDraw();
	m_pDDraw->Clear();
	int colorIndex = 0;
	//write draw code here.
	Color rgbColor[3] = { Color(255,0,0,0), Color(0,255,0,0), Color(0,0,255,0) };
	for (const auto& v : m_pOutputQueue)
	{
		if (v->m_pBuffer->m_stride == 2)
		{
			for (int i = 0; i < v->m_pBuffer->m_vertices.size() / v->m_pBuffer->m_stride; i++)
			{
				Vec2i v0{ (int)v->m_pBuffer->m_vertices[i].x, (int)v->m_pBuffer->m_vertices[i].y };
				Vec2i v1{ (int)v->m_pBuffer->m_vertices[i + 1].x, (int)v->m_pBuffer->m_vertices[i + 1].y };

				Line(v0, v1, Color(255, 255, 255, 0), Color(255, 255, 255, 255));
			}
		}
		else if (v->m_pBuffer->m_stride == 3)
		{
			for (int i = 0; i < v->m_pBuffer->m_indices[0].size(); i++)
			{
				Vec3f v0 = v->m_pBuffer->m_vertices[v->m_pBuffer->m_indices[0][i].vertex].AffineToCartesian();
				Vec3f v1 = v->m_pBuffer->m_vertices[v->m_pBuffer->m_indices[1][i].vertex].AffineToCartesian();
				Vec3f v2 = v->m_pBuffer->m_vertices[v->m_pBuffer->m_indices[2][i].vertex].AffineToCartesian();
#ifdef DRAWMODE_WIREFRAME
				Line(Vec2i((int)v0.x, (int)v0.y), Vec2i((int)v1.x, (int)v1.y),Color(255,0,0,0), Color(255, 0, 0, 0));
				Line(Vec2i((int)v1.x, (int)v1.y), Vec2i((int)v2.x, (int)v2.y), Color(0, 255, 0, 0), Color(0,255,  0, 0));
				Line(Vec2i((int)v0.x, (int)v0.y), Vec2i((int)v2.x, (int)v2.y), Color(0,0,255,0), Color(0, 0, 255, 0));
#endif
#ifndef DRAWMODE_WIREFRAME
				Triangle(v0, v1, v2, rgbColor[i % 3]);
#endif
			}
		}
	}
	m_pDDraw->DrawBitmap(0, 0, m_width, m_height, m_pRenderBuffer);
	// end code.
	m_pDDraw->EndDraw();
	m_pDDraw->Blt();

	memset(m_pRenderBuffer, BACKGROUND_COLOR, m_width * m_height * 4);
	m_pOutputQueue.clear();
}

void Renderer::Point(Vec2i v, const Color& color)
{
	SetPixel(v.x, v.y, color);
}

void Renderer::Line(Vec2i v0, Vec2i v1, const Color& c0, const Color& c1)
{
	bool steep = false;
	if (std::abs(v0.x - v1.x) < std::abs(v0.y - v1.y))
	{
		std::swap(v0.x, v0.y);
		std::swap(v1.x, v1.y);
		steep = true;
	}
	if (v0.x > v1.x)
	{
		std::swap(v0.x, v1.x);
		std::swap(v0.y, v1.y);
	}
	int dx = v1.x - v0.x;
	int dy = v1.y - v0.y;

	const float slope = std::abs((float)dy / dx);
	float ds = 0.0f;
	int y = v0.y;
	for (int x = v0.x; x <= v1.x; x++)
	{
		// FIX: v0.x 랑 v1.x가 같은 값을 가져서 zero divide가 일어남
		/*auto t = (x - v0.x) / dx;
		Color color =  c0 * (float)t + c1 * (float)(1 - t);*/
		Color color = c0 * 0.5f + c1 * 0.5f;
		if (steep)
		{
			// For pixel clipping
			if ((y >= 0 && y < (int)m_width) && (x >= 0 && x < (int)m_height))
			{
				SetPixel(y, x, color);
			}
		}
		else
		{
			// For pixel clipping
			if (x >= 0 && x < (int)m_width && (y >= 0 && y < (int)m_height))
			{
				SetPixel(x, y, color);
			}
		}

		ds += slope;
		if (ds > 0.5f)
		{
			y += dy > 0.0f ? 1 : -1;
			ds -= 1.0f;
		}
	}
}

void Renderer::Triangle(Vec3f v0, Vec3f v1, Vec3f v2, const Color& color)
{
	int minXPos = (int)(v0.x < v1.x ? v0.x < v2.x ? v0.x : v2.x : v1.x < v2.x ? v1.x : v2.x);
	int minYPos = (int)(v0.y < v1.y ? v0.y < v2.y ? v0.y : v2.y : v1.y < v2.y ? v1.y : v2.y);
	int maxXPos = (int)(v0.x > v1.x ? v0.x > v2.x ? v0.x : v2.x : v1.x > v2.x ? v1.x : v2.x);
	int maxYPos = (int)(v0.y > v1.y ? v0.y > v2.y ? v0.y : v2.y : v1.y > v2.y ? v1.y : v2.y);

	float maxZPos = std::fmaxf(v0.z, std::fmaxf(v1.z, v2.z));
	float minZPos = std::fminf(v0.z, std::fminf(v1.z, v2.z));

	for (int y = minYPos; y <= maxYPos; y++)
	{
		// for pixel clipping
		if (y < 0 || y >= m_height)
		{
			continue;
		}
		for (int x = minXPos; x <= maxXPos; x++)
		{
			// for pixel clipping
			if (x < 0 || x >= (int)m_width)
			{
				continue;
			}
			Vec3f p = { (float)x, (float)y, 1 };
			Vec3f bc = Barycentric(v0, v1, v2, p);
			if (bc.x < 0.0f || bc.x > 1.0f || bc.y < 0.0f || bc.y > 1.0f || bc.z < 0.0f || bc.z > 1.0f)
				continue;
			p.z = bc.x * v0.z + bc.y * v1.z + bc.z * v2.z;
			
			// Depth test
			if (m_pZBuffer[((int)p.y * m_width) + (int)p.x] > p.z)
			{
				assert(p.z <= maxZPos && p.z >= minZPos);
				m_pZBuffer[(int)p.y * m_width + (int)p.x] = p.z;
				SetPixel((int)p.x, (int)p.y, color);
			}
		}
	}
	return;
}

void Renderer::GradiantTriangle(Vec3f v0, Vec3f v1, Vec3f v2, const Color& color0, const Color& color1, const Color& color2)
{
	int minXPos = (int)(v0.x < v1.x ? v0.x < v2.x ? v0.x : v2.x : v1.x < v2.x ? v1.x : v2.x);
	int minYPos = (int)(v0.y < v1.y ? v0.y < v2.y ? v0.y : v2.y : v1.y < v2.y ? v1.y : v2.y);
	int maxXPos = (int)(v0.x > v1.x ? v0.x > v2.x ? v0.x : v2.x : v1.x > v2.x ? v1.x : v2.x);
	int maxYPos = (int)(v0.y > v1.y ? v0.y > v2.y ? v0.y : v2.y : v1.y > v2.y ? v1.y : v2.y);

	for (int y = minYPos; y <= maxYPos; y++)
	{
		for (int x = minXPos; x <= maxXPos; x++)
		{
			Vec3f p = { (float)x, (float)y, 0 };
			Vec3f bc = Barycentric(v0, v1, v2, p);
			if (bc.x < 0.0f || bc.x > 1.0f || bc.y < 0.0f || bc.y > 1.0f || bc.z < 0.0f || bc.z > 1.0f)
				continue;
			Color lerpColor = color0 * bc.x + color1 * bc.y + color2 * bc.z;
			SetPixel((int)p.x, (int)p.y, lerpColor);
		}
	}
}

void Renderer::SetCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
	m_pCamera->SetAspect(m_width, m_height);
	m_pCamera->SetFov(45.0f);
}

void Renderer::MoveCamera()
{
	float alpha = 1.0f;
	if (m_deltaTime < FPS)
	{
		alpha *= m_deltaTime / FPS;
	}

	int front = 0;
	int right = 0;
	int up = 0;

	if (m_pInputManager->IsPressed(eInput::W))
	{
		front++;
	}
	if (m_pInputManager->IsPressed(eInput::S))
	{
		front--;
	}
	if (m_pInputManager->IsPressed(eInput::A))
	{
		right--;
	}
	if (m_pInputManager->IsPressed(eInput::D))
	{
		right++;
	}
	if (m_pInputManager->IsPressed(eInput::Q))
	{
		up++;
	}
	if (m_pInputManager->IsPressed(eInput::E))
	{
		up--;
	}
	m_pCamera->Move(front, right, up, alpha);
}

void Renderer::RotateCamera()
{
	float alpha = 1.0f;
	if (m_deltaTime < FPS)
	{
		alpha *= m_deltaTime / FPS;
	}
	POINT delta = m_pInputManager->GetMouseDelta();
	m_pCamera->Rotate((float)delta.x, (float)delta.y, alpha);
	if (m_pCamera->CanMove())
	{
		m_pInputManager->SetCursorToCenter();
	}
}

void Renderer::ClearZBuffer()
{
	for (int r = 0; r < m_height; r++)
	{
		for (int c = 0; c < m_width; c++)
		{
			m_pZBuffer[r * m_width + c] = MAX_DEPTH;
		}
	}
}

void Renderer::UpdateWindowPos()
{
	m_pDDraw->UpdateWindowPos();
}

void Renderer::UpdateWindowSize()
{
	// TODO: Implement this.
}