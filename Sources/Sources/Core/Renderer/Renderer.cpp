#include "Renderer.h"

void Renderer::FilpBuffer()
{
	//TODO: Implement this.
	int widthSize = m_width * 4;
	for (uint32_t row = 0; row < m_height / 2; row++)
	{
		memcpy(m_pSwapBuffer, m_pViewport + (widthSize * row), widthSize);
		memcpy(m_pViewport + (widthSize * row), m_pViewport + (widthSize * (m_height - row - 1)), widthSize);
		memcpy(m_pViewport + (widthSize * (m_height - row - 1)), m_pSwapBuffer, widthSize);
	}
}

void Renderer::SetPixel(int x, int y, const Color& color)
{
	sizeof(Color);
	memcpy(m_pViewport + (y * m_width * 4) + x * 4, &color, 4);
}

Renderer::Renderer(HWND hWnd)
	:m_hWnd(hWnd)
{
	m_pCamera = new Camera;
	m_pDDraw = new DDraw;
	m_pDDraw->Init(m_hWnd);
	m_width = m_pDDraw->width();
	m_height = m_pDDraw->height();
	m_pViewport = new char[m_width * m_height * 4];
	m_pSwapBuffer = new char[m_width * 4];
	
	memset(m_pViewport, 0, m_width * m_height * 4);
	m_pTimer = new Timer;
	m_pTimer->Start();
}

Renderer::~Renderer()
{
	if (m_pDDraw)
	{
		delete m_pDDraw;
		m_pDDraw = nullptr;
	}
	if (m_pViewport)
	{
		delete[] m_pViewport;
		m_pViewport = nullptr;
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
	if (m_pTimer)
	{
		delete m_pTimer;
		m_pTimer = nullptr;
	}
}

void Renderer::DrawScene()
{
	m_pDDraw->BeginDraw();
	m_pDDraw->Clear();
	//write draw code here.
	m_pDDraw->DrawBitmap(0, 0, m_width, m_height, m_pViewport);

	// end code.
	m_pDDraw->EndDraw();
	m_pDDraw->Blt();
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

void Renderer::UpdateWindowPos()
{
	m_pDDraw->UpdateWindowPos();
}

void Renderer::UpdateWindowSize()
{
	// TODO: Implement this.
}
