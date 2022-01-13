#include "Renderer.h"

void hs::Renderer::FlipBuffer()
{
	//TODO: Implement this.
}

void hs::Renderer::Set(int x, int y, const Color& color)
{
	memcpy(&_renderBuffer[(y * _width) + x], &color, 4);
}

hs::Renderer::Renderer(HWND hWnd, unsigned long width, unsigned long height)
	:_hWnd(hWnd), _width(width), _height(height)
{
	_ddraw = new DDraw;
	_ddraw->Init(_hWnd);
	_renderBuffer = new char[width * height * 4];

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
}

void hs::Renderer::DrawScene()
{
	_ddraw->BeginDraw();
	//TODO: write draw code here.
	_ddraw->DrawBitmap(0, 0, _width, _height, _renderBuffer);

	_ddraw->EndDraw();
}

void hs::Renderer::AddObject(Vec2i v0, Vec2i v1, const Color& color)
{
	bool steep = false;
	if (std::abs(v0.x - v1.x) < std::abs(v0.y - v1.y))
	{
		std::swap(v0.x, v0.y);
		std::swap(v1.x, v1.y);
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
}