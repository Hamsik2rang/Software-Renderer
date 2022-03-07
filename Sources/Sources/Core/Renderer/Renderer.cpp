#include "Renderer.h"
#include <cmath>
#include "../Math/Math.hpp"

Renderer::Renderer(HWND hWnd)
	:m_hWnd(hWnd)
{
	// Init DirectDraw
	m_pDDraw = new DDraw;
	m_pDDraw->Init(m_hWnd);
	m_width = m_pDDraw->width();
	m_height = m_pDDraw->height();

	m_pRenderBuffer = new char[m_width * m_height * 4];
	m_pSwapBuffer = new char[m_width * 4];

	// Init Camera
	m_pCamera = new Camera;
	m_pCamera->SetAspect(1.0f);
	m_pCamera->SetFov(45.0f);

	POINT cursorPos;
	::GetCursorPos(&cursorPos);
	m_cursorLastXPos = cursorPos.x;
	m_cursorLastYPos = cursorPos.y;

	memset(m_pRenderBuffer, 0, m_width * m_height * 4);

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

void Renderer::FilpBuffer()
{
	//TODO: Implement this.
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
	// simple clipping
	// TODO: Fix bug
	if (x < 0 || x >= m_width || y < 0|| y >= m_height)
	{
		return;
	}
	memcpy(m_pRenderBuffer + (y * m_width * 4) + x * 4, &color, 4);
}

void Renderer::VertexShading()
{
	/// Vertex Shading
	/// Carry out vertex transformation.
	/// transform each local objects to objects which is in clip space.
	/// transformation procedure:
	/// Local space -> world space -> camera space(view space) -> clip space

	for (auto& v : m_pRenderObjects)
	{
		Model* m = new Model;
		for (int i = 0; i < v->m_vertices.size(); i++)
		{
			auto affine = v->m_vertices[i];
			// 1. Local Space to World Space(world/model transformation)
			Mat4f model = GetTransform(v->m_position.x, v->m_position.y, v->m_position.z)
				* GetRotate(v->m_rotation.x, v->m_rotation.y, v->m_rotation.z)
				* GetScale(v->m_scale.x, v->m_scale.y, v->m_scale.z);

			affine = model * affine;

			// 2. World Space to Camera Space(view transformation)
			Vec3f cameraPos = m_pCamera->GetEye();
			Vec3f cameraRight = m_pCamera->GetRight();
			Vec3f cameraUp = m_pCamera->GetUp();
			Vec3f cameraFront = m_pCamera->GetFront();	// !Notice: actually, camerafront is BACK direction vector of camera.

			Mat4f view = Mat4f(
				{ cameraRight.x, cameraRight.y, cameraRight.z, 0.0f },
				{ cameraUp.x , cameraUp.y, cameraUp.z, 0.0f },
				{ cameraFront.x, cameraFront.y, cameraFront.z, 0.0f },
				{0.0f, 0.0f, 0.0f, 1.0f})
				* GetTransform(-cameraPos.x, -cameraPos.y, -cameraPos.z);

			affine = view * affine;

			// 3. Camera Space to Clip Space(projection transformation)
			Mat4f projection(
				Vec4f(1.0f / (m_pCamera->GetAspect() * std::tan(DegreeToRadian(m_pCamera->GetFovY() / 2))), 0.0f, 0.0f, 0.0f),
				Vec4f(0.0f, 1.0f / std::tan(DegreeToRadian(m_pCamera->GetFovY() / 2)), 0.0f, 0.0f),
				Vec4f(0.0f, 0.0f, m_pCamera->GetFar() / (m_pCamera->GetFar() - m_pCamera->GetNear()), m_pCamera->GetNear() * m_pCamera->GetFar() / (m_pCamera->GetFar() - m_pCamera->GetNear())),
				Vec4f(0.0f, 0.0f, -1.0f, 0.0f));

			affine = projection * affine;
			m->m_vertices.push_back(affine);
		}
		m_pRasterizerQueue.push_back(m);
	}

	assert(m_pRasterizerQueue.size() == m_pRenderObjects.size());
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
		for (int i = 0; i < v->m_vertices.size(); i++)
		{
			Vec4f& affine = v->m_vertices[i];
			//...

			// Perspective devision
			affine.x /= affine.w;
			affine.y /= affine.w;
			affine.z /= affine.w;
			affine.w /= affine.w;

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

			affine = viewport * affine;	//BUG: º¤ÅÍ Z°ª º¸Á¤ ¾ÈµÊ
			// Scan Coversion
			// ...

		}
		m_pFragmentQueue.push_back(v);
	}
	assert(m_pFragmentQueue.size() == m_pRasterizerQueue.size());
	// TODO: change later
	m_pRasterizerQueue.clear();
}

void Renderer::FragmentShading()
{
	// Fragment Shading
	// 1. Texturing

	// TODO: Implement this
	// NOTE: For Test Code
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

void Renderer::AddModel(Model* model)
{
	m_pRenderObjects.push_back(model);
}

void Renderer::Render()
{
	float curTime = Timer::Elapsed();
	m_deltaTime = curTime - m_lastTime;
	m_lastTime = curTime;
	MoveCamera();
	//RotateCamera();

	VertexShading();
	Rasterizer();
	FragmentShading();
	OutputMerging();

	DrawScene();
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

				Line(v0, v1, Color(255, 255, 255, 255));
			}
		}
		else if (v->m_stride == 3)
		{
			for (int i = 0; i < v->m_vertices.size() / v->m_stride; i++)
			{
				Vec3f v0 = v->m_vertices[i].AffineToCartesian();
				Vec3f v1 = v->m_vertices[i + 1].AffineToCartesian();
				Vec3f v2 = v->m_vertices[i + 2].AffineToCartesian();

				Triangle(v0, v1, v2, Color(255, 255, 255, 255));
			}
		}
	}
	m_pDDraw->DrawBitmap(0, 0, m_width, m_height, m_pRenderBuffer);

	// end code.
	m_pDDraw->EndDraw();
	m_pDDraw->Blt();

	memset(m_pRenderBuffer, 0, m_width * m_height * 4);
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
		for (int x = minXPos; x <= maxXPos; x++)
		{
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

void Renderer::OnKeyDown(WPARAM wParam)
{
	if (wParam == 'W')
	{
		m_isKeyDownUp = true;
	}
	if (wParam == 'S')
	{
		m_isKeyDownDown = true;
	}
	if (wParam == 'A')
	{
		m_isKeyDownLeft = true;
	}
	if (wParam == 'D')
	{
		m_isKeyDownRight = true;
	}
}

void Renderer::OnKeyUP(WPARAM wParam)
{
	if (wParam == 'W')
	{
		m_isKeyDownUp = false;
	}
	if (wParam == 'S')
	{
		m_isKeyDownDown = false;
	}
	if (wParam == 'A')
	{
		m_isKeyDownLeft = false;
	}
	if (wParam == 'D')
	{
		m_isKeyDownRight = false;
	}
}

void Renderer::OnMouseMove()
{
	POINT cursorPos;
	::GetCursorPos(&cursorPos);
	m_cursorDeltaXPos += cursorPos.x - m_cursorLastXPos;
	m_cursorDeltaYPos += cursorPos.y - m_cursorLastYPos;

	m_cursorLastXPos = cursorPos.x;
	m_cursorLastYPos = cursorPos.y;
}

void Renderer::MoveCamera()
{
	float alpha = 1.0f;
	if (m_deltaTime < FPS)
	{
		alpha *= m_deltaTime / FPS;
	}

	int vertical = 0;
	int horizontal = 0;
	if (m_isKeyDownUp)
	{
		vertical += 1;
	}
	if (m_isKeyDownDown)
	{
		vertical -= 1;
	}
	if (m_isKeyDownLeft)
	{
		horizontal -= 1;
	}
	if (m_isKeyDownRight)
	{
		horizontal += 1;
	}
	m_pCamera->Move(vertical, horizontal, alpha);
}

void Renderer::RotateCamera()
{
	float alpha = 1.0f;
	if (m_deltaTime < FPS)
	{
		alpha *= m_deltaTime / FPS;
	}
	
	m_pCamera->Rotate(m_cursorDeltaYPos, m_cursorDeltaXPos, alpha);
	m_cursorDeltaXPos = 0;
	m_cursorDeltaYPos = 0;
}

void Renderer::UpdateWindowPos()
{
	m_pDDraw->UpdateWindowPos();
}

void Renderer::UpdateWindowSize()
{
	// TODO: Implement this.
}
