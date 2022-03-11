#include "Renderer.h"
#include "../Math/Math.hpp"
#include "../Math/Interpolate.hpp"

#include <cstring>

Renderer::Renderer(HWND hWnd)
	:m_hWnd(hWnd)
{
	// Init DirectDraw
	m_pDDraw = new DDraw;
	m_pDDraw->Init(m_hWnd);
	m_width = m_pDDraw->GetWidth();
	m_height = m_pDDraw->GetHeight();

	m_pRenderBuffer = new char[m_width * m_height * 4];
	m_pSwapBuffer = new char[m_width * 4];

	// Init Camera
	m_pCamera = new Camera;
	m_pCamera->SetAspect(m_width, m_height);
	m_pCamera->SetFov(45.0f);

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
	if (m_pCamera)
	{
		delete m_pCamera;
		m_pCamera = nullptr;
	}
}

// 좌하단을 원점으로 렌더링했을 때 버퍼를 뒤집어 일반적인 Viewport 렌더로 변경해주는 함수
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
	memcpy(m_pRenderBuffer + (y * m_width * 4) + x * 4, &color, 4);
}

void Renderer::Render()
{
	float curTime = Timer::Elapsed();
	m_deltaTime = curTime - m_lastTime;
	m_lastTime = curTime;

	MoveCamera();
	RotateCamera();

	VertexShading();
	Rasterizer();
	FragmentShading();
	OutputMerging();

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
		RenderObject* m = new RenderObject;
		//memcpy(m, v, sizeof(*v));
		*m = *v;
		
		////////////////////
		// 1. Get Model Matrix which transform Local Space to World Space
		Mat4f model = Model(m);
		// 2. Get View Matrix which transform World space to Camera Space
		Mat4f view = LookAt(m_pCamera->GetEye(), m_pCamera->GetEye() - m_pCamera->GetFront(), {0.0f, 1.0f, 0.0f});
		// 3. Get Projection Matrix which transform Camera Space to Clip Space
		Mat4f projection = Projection(m_pCamera->GetFovY(), m_pCamera->GetAspect(), m_pCamera->GetNear(), m_pCamera->GetFar());

		for (int i = 0; i < m->m_vertices.size(); i++)
		{
			auto affine = m->m_vertices[i];
			affine = projection * view * model * affine;

			// Rasterizer 단계를 위한 좌표계 변경(right-hand -> left-hand)
			// 정점 재정렬도 수행해야 하지만 현재 정점 순서를 신경쓰고 있지 않으므로 제외함.
			affine.z *= -1.0f;
			m->m_vertices[i] = affine;
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
		for (int i = 0; i < v->m_vertices.size(); i++)
		{
			Vec4f& affine = v->m_vertices[i];
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

			// Scale to axis size( (1, 1, 0) to (width, height, 1) )
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
	//write draw code here.
	for (const auto& v : m_pOutputQueue)
	{
		if (v->m_stride == 2)
		{
			for (int i = 0; i < v->m_vertices.size() / v->m_stride; i++)
			{
				Vec2i v0{ (int)v->m_vertices[i].x, (int)v->m_vertices[i].y };
				Vec2i v1{ (int)v->m_vertices[i + 1].x, (int)v->m_vertices[i + 1].y };

				Line(v0, v1, Color(255, 0, 255, 0));
			}
		}
		else if (v->m_stride == 3)
		{
			for (int i = 0; i < v->m_indices[0].size(); i++)
			{
				Vec3f v0 = v->m_vertices[v->m_indices[0][i].vertex].AffineToCartesian();
				Vec3f v1 = v->m_vertices[v->m_indices[1][i].vertex].AffineToCartesian();
				Vec3f v2 = v->m_vertices[v->m_indices[2][i].vertex].AffineToCartesian();
				
				Triangle(v0, v1, v2, Color(255, 0, 255, 0));
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

void Renderer::Line(Vec2i v0, Vec2i v1, const Color& color)
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
		if (steep)
			SetPixel(y, x, color);
		else
			SetPixel(x, y, color);

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
			if (x < 0 || x >= m_width)
			{
				continue;
			}
			Vec3f p = { (float)x, (float)y, 0 };
			Vec3f bc = Barycentric(v0, v1, v2, p);
			if (bc.x < 0.0f || bc.x > 1.0f || bc.y < 0.0f || bc.y > 1.0f || bc.z < 0.0f || bc.z > 1.0f)
				continue;
			SetPixel((int)p.x, (int)p.y, color);
		}
	}
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
	m_pInputManager->SetCursorToCenter();
}

void Renderer::UpdateWindowPos()
{
	m_pDDraw->UpdateWindowPos();
}

void Renderer::UpdateWindowSize()
{
	// TODO: Implement this.
}

