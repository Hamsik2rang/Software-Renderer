#include "DDraw.h"
#include "DDraw.h"


hs::DDraw::DDraw()
{
	memset(this, 0, sizeof(DDraw));
}

hs::DDraw::~DDraw()
{
	CleanUp();
}

bool hs::DDraw::Init(HWND hWnd)
{
	_hWnd = hWnd;
	DDSURFACEDESC2 ddsd{};
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	if (DD_OK != DirectDrawCreate(nullptr, &_pDDraw, nullptr))
	{
		MessageBox(_hWnd, L"Fail to Create DirectDraw", L"Error", MB_OK);
		return false;
	}

	if (DD_OK != _pDDraw->QueryInterface(IID_IDirectDraw7, (LPVOID*)&_pDDraw7))
	{
		MessageBox(_hWnd, L"Fail to Create DirectDraw7", L"Error", MB_OK);
		return false;
	}
	HRESULT hr = _pDDraw7->SetCooperativeLevel(hWnd, DDSCL_NORMAL);

	if (FAILED(hr))
	{
		MessageBox(_hWnd, L"Fail to Set Cooperative Level", L"Error", MB_OK);
		return false;
	}

	hr = _pDDraw7->CreateSurface(&ddsd, &_pDDSPrimary, nullptr);
	if (FAILED(hr))
	{
		MessageBox(_hWnd, L"Fail to Create Surface", L"Error", MB_OK);
		return false;
	}

	hr = _pDDraw7->CreateClipper(0, &_pDDClipper, nullptr);
	if (FAILED(hr))
	{
		MessageBox(_hWnd, L"Fail to Create Clipper", L"Error", MB_OK);
		return false;
	}

	_pDDClipper->SetHWnd(0, _hWnd);
	_pDDSPrimary->SetClipper(_pDDClipper);

	UpdateWindowPos();

	DWORD width = _rcView.right - _rcView.left;
	DWORD height = _rcView.bottom - _rcView.top;

	if (!CreateBackBuffer(width, height))
	{
#ifdef _DEBUG
		__debugbreak();
#endif
		return false;
	}

	return true;
}

bool hs::DDraw::CreateBackBuffer(DWORD width, DWORD height)
{
	DDSURFACEDESC2 ddsd{};
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;

	HRESULT hr = _pDDraw7->CreateSurface(&ddsd, &_pDDSBack, nullptr);
	if (FAILED(hr))
	{
		MessageBox(_hWnd, L"Fail to Create Surface", L"Error", MB_OK);
		return false;
	}

	_pDDSBack->GetSurfaceDesc(&ddsd);
	_width = ddsd.dwWidth;
	_height = ddsd.dwHeight;

	return true;
}


bool hs::DDraw::LockBackBuffer(char** ppBits, DWORD* pWidth, DWORD* pHeight, DWORD* pPitch)
{
	if (_pDDSBack)
	{
		DDSURFACEDESC2 ddsd{};
		ddsd.dwSize = sizeof(DDSURFACEDESC2);

		_pDDSBack->Lock(nullptr, &ddsd, DDLOCK_WAIT | DDLOCK_WRITEONLY, nullptr);

		*ppBits = (char*)ddsd.lpSurface;
		*pWidth = ddsd.dwWidth;
		*pHeight = ddsd.dwHeight;
		*pPitch = ddsd.lPitch;

		return true;
	}

	return false;
}

void hs::DDraw::UnlockBackBuffer()
{
	if (_pDDSBack)
		_pDDSBack->Unlock(nullptr);
}

void hs::DDraw::CleanUpBackBuffer()
{
	if (_pDDSBack)
	{
		_pDDSBack->Release();
		_pDDSBack = nullptr;
	}
}

bool hs::DDraw::CalculateClipArea(Vec2i* pSrcStart, Vec2i* pDestStart, Vec2i* pDestSize, Vec2i* pPos, Vec2i* pImageSize)
{
	pDestStart->x = max(pPos->x, 0);
	pDestStart->y = max(pPos->y, 0);
	pDestStart->x = min(pDestStart->x, _width);
	pDestStart->y = min(pDestStart->y, _height);

	Vec2i destEnd = { max(pPos->x + pImageSize->x, 0), max(pPos->y + pImageSize->y, 0) };
	destEnd.x = min(destEnd.x, _width);
	destEnd.y = min(destEnd.y, _height);

	int width = destEnd.x - pDestStart->x;
	int height = destEnd.y - pDestStart->y;
	if (width < 0 || height < 0)
		return false;

	pSrcStart->x = pDestStart->x - pPos->x;
	pSrcStart->y = pDestStart->y - pPos->y;

	pDestSize->x = width;
	pDestSize->y = height;

	return true;
}

bool hs::DDraw::DrawBitmap(int startX, int startY, int width, int height, char* pBits)
{
#ifdef _DEBUG
	if (!_pLockedBackBuffer)
		__debugbreak();
#endif
	Vec2i srcStart;
	Vec2i destStart;

	Vec2i pos{ startX, startY };
	Vec2i imageSize{ width, height };
	Vec2i destSize;

	if (!CalculateClipArea(&srcStart, &destStart, &destSize, &pos, &imageSize))
	{
		return false;
	}

	const DWORD RGBA_SIZE = 4;
	char* pSrc = pBits + (srcStart.x + srcStart.y * width) * RGBA_SIZE;
	char* pDest = _pLockedBackBuffer + (destStart.y * _lockedBackBufferPitch) + (destStart.x * RGBA_SIZE);

	for (int y = 0; y < destSize.y; y++)
	{
		for (int x = 0; x < destSize.x; x++)
		{
			*(DWORD*)pDest = *(DWORD*)pSrc;
			pSrc += RGBA_SIZE;
			pDest += RGBA_SIZE;
		}
		pSrc -= (destSize.x * RGBA_SIZE);
		pSrc += (width * RGBA_SIZE);
		pDest -= (destSize.x * RGBA_SIZE);
		pDest += _lockedBackBufferPitch;
	}

	return true;
}

bool hs::DDraw::DrawBitmap(int width, int height, char* pBits)
{
	return DrawBitmap(_rcView.left, _rcView.top, width, height, pBits);
}

bool hs::DDraw::BeginDraw()
{
	char* pBuffer = nullptr;
	DWORD bufferWidth = 0;
	DWORD bufferHeight = 0;
	DWORD bufferPitch = 0;

	if (!LockBackBuffer(&pBuffer, &bufferWidth, &bufferHeight, &bufferPitch))
	{
#ifdef _DEBUG
		__debugbreak();
#endif
		return false;
	}

#ifdef _DEBUG
	if (bufferWidth != _width || bufferHeight != _height)
	{
		__debugbreak();
	}
#endif
	_pLockedBackBuffer = pBuffer;
	_lockedBackBufferPitch = bufferPitch;

	return true;
}

void hs::DDraw::EndDraw()
{
	UnlockBackBuffer();
	_pLockedBackBuffer = nullptr;
	_lockedBackBufferPitch = 0;
}

void hs::DDraw::Blt()
{
	_pDDSPrimary->Blt(&_rcView, _pDDSBack, nullptr, DDBLT_WAIT, nullptr);
}

void hs::DDraw::Clear()
{
	if (!_pLockedBackBuffer)
	{
#ifdef _DEBUG
		__debugbreak();
#endif
	}
	for (DWORD y = 0; y < _height; y++)
	{
		memset(_pLockedBackBuffer + y * _lockedBackBufferPitch, 0, 4 * _width);
	}
}

void hs::DDraw::UpdateWindowSize()
{
	CleanUpBackBuffer();

	UpdateWindowPos();

	DWORD width = _rcView.right - _rcView.left;
	DWORD height = _rcView.bottom - _rcView.top;

	CreateBackBuffer(width, height);

	if (BeginDraw())
	{
		Clear();
		EndDraw();
	}
}

void hs::DDraw::UpdateWindowPos()
{
	GetClientRect(_hWnd, &_rcView);
	ClientToScreen(_hWnd, (POINT*)&_rcView.left);
	ClientToScreen(_hWnd, (POINT*)&_rcView.right);
}

void hs::DDraw::CleanUp()
{
	if (_pDDClipper)
	{
		_pDDClipper->Release();
		_pDDClipper = nullptr;
	}
	if (_pDDSBack)
	{
		_pDDSBack->Release();
		_pDDSBack = nullptr;
	}
	if (_pDDSPrimary)
	{
		_pDDSPrimary->Release();
		_pDDSPrimary = nullptr;
	}
	if (_pDDraw7)
	{
		_pDDraw7->Release();
		_pDDraw7 = nullptr;
	}
}

DWORD hs::DDraw::width() const
{
	return _width;
}

DWORD hs::DDraw::height() const
{
	return _height;
}