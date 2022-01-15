#include "Renderer.h"

void hs::Renderer::FlipBuffer()
{
	//TODO: Implement this.
	int widthSize = _width * 4;
	for (int row = 0; row < _height/2; row++)
	{
		memcpy(_swapBuffer, _renderBuffer + (widthSize * row), widthSize);
		memcpy(_renderBuffer + (widthSize * row), _renderBuffer + (widthSize * (_height - row - 1)), widthSize);
		memcpy(_renderBuffer + (widthSize * (_height - row - 1)), _swapBuffer, widthSize);
	}
}

void hs::Renderer::Set(int x, int y, const Color& color)
{
	sizeof(Color);
	memcpy(_renderBuffer + (y * _width * 4) + x * 4, &color, 4);
}

hs::Vec3f hs::Renderer::Barycentric(Vec3f v0, Vec3f v1, Vec3f v2, Vec3f p)
{
	Vec3f eqx(v2.x - v0.x, v1.x - v0.x, v0.x - p.x);
	Vec3f eqy(v2.y - v0.y, v1.y - v0.y, v0.y - p.y);

	Vec3f u = eqx ^ eqy;
	if (std::abs(u.z) < 1.0f)
		return Vec3f(-1.0f, -1.0f, -1.0f);
	return Vec3f(1.0f - (u.x + u.y) / u.z, u.x / u.z, u.y / u.z);
}

hs::Renderer::Renderer(HWND hWnd)
	:_hWnd(hWnd)
{
	_ddraw = new DDraw;
	_ddraw->Init(_hWnd);
	_width = _ddraw->width();
	_height = _ddraw->height();
	_renderBuffer = new char[_width * _height * 4];
	_swapBuffer = new char[_width * 4];
	
	memset(_renderBuffer, 0, _width * _height * 4);
}

hs::Renderer::~Renderer()
{
	if (_ddraw)
	{
		delete _ddraw;
		_ddraw = nullptr;
	}
	if (_renderBuffer)
	{
		delete[] _renderBuffer;
		_renderBuffer = nullptr;
	}
	if (_swapBuffer)
	{
		delete[] _swapBuffer;
		_swapBuffer = nullptr;
	}
}

void hs::Renderer::DrawScene()
{
	_ddraw->BeginDraw();
	_ddraw->Clear();
	//write draw code here.
	_ddraw->DrawBitmap(0, 0, _width, _height, _renderBuffer);

	// end code.
	_ddraw->EndDraw();
	_ddraw->Blt();
}

void hs::Renderer::Line(Vec2i v0, Vec2i v1, const Color& color)
{
	FlipBuffer();
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
			Set(y, x, color);
		else
			Set(x, y, color);

		ds += slope;
		if (ds > 0.5f)
		{
			y += dy > 0.0f ? 1 : -1;
			ds -= 1.0f;
		}
	}
	FlipBuffer();
}

void hs::Renderer::Triangle(Vec3f v0, Vec3f v1, Vec3f v2, const Color& color)
{
	FlipBuffer();
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
			Set(p.x, p.y, color);
		}
	}
	FlipBuffer();
}
