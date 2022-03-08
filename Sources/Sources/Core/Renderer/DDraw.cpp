#include "DDraw.h"
#include "DDraw.h"

#include <cmath>

DDraw::DDraw()
{
	memset(this, 0, sizeof(DDraw));
}

DDraw::~DDraw()
{
	CleanUp();
}

bool DDraw::Init(HWND hWnd)
{
	m_hWnd = hWnd;
	DDSURFACEDESC2 ddsd{};
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	if (DD_OK != DirectDrawCreate(nullptr, &m_pDDraw, nullptr))
	{
		MessageBox(m_hWnd, L"Fail to Create DirectDraw", L"Error", MB_OK);
		return false;
	}

	if (DD_OK != m_pDDraw->QueryInterface(IID_IDirectDraw7, (LPVOID*)&m_pDDraw7))
	{
		MessageBox(m_hWnd, L"Fail to Create DirectDraw7", L"Error", MB_OK);
		return false;
	}
	HRESULT hr = m_pDDraw7->SetCooperativeLevel(hWnd, DDSCL_NORMAL);

	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"Fail to Set Cooperative Level", L"Error", MB_OK);
		return false;
	}

	hr = m_pDDraw7->CreateSurface(&ddsd, &m_pDDSPrimary, nullptr);
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"Fail to Create Surface", L"Error", MB_OK);
		return false;
	}

	hr = m_pDDraw7->CreateClipper(0, &m_pDDClipper, nullptr);
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"Fail to Create Clipper", L"Error", MB_OK);
		return false;
	}

	m_pDDClipper->SetHWnd(0, m_hWnd);
	m_pDDSPrimary->SetClipper(m_pDDClipper);

	UpdateWindowPos();

	DWORD width = m_rcView.right - m_rcView.left;
	DWORD height = m_rcView.bottom - m_rcView.top;

	::SetCursorPos(m_rcView.left + width / 2, m_rcView.top + height / 2);

	if (!createBackBuffer(width, height))
	{
#ifdef _DEBUG
		__debugbreak();
#endif
		return false;
	}

	return true;
}

bool DDraw::createBackBuffer(DWORD width, DWORD height)
{
	DDSURFACEDESC2 ddsd{};
	ddsd.dwSize = sizeof(DDSURFACEDESC2);
	ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;

	HRESULT hr = m_pDDraw7->CreateSurface(&ddsd, &m_pDDSBack, nullptr);
	if (FAILED(hr))
	{
		MessageBox(m_hWnd, L"Fail to Create Surface", L"Error", MB_OK);
		return false;
	}

	m_pDDSBack->GetSurfaceDesc(&ddsd);
	m_width = ddsd.dwWidth;
	m_height = ddsd.dwHeight;

	return true;
}


bool DDraw::LockBackBuffer(char** ppBits, DWORD* pWidth, DWORD* pHeight, DWORD* pPitch)
{
	if (m_pDDSBack)
	{
		DDSURFACEDESC2 ddsd{};
		ddsd.dwSize = sizeof(DDSURFACEDESC2);

		m_pDDSBack->Lock(nullptr, &ddsd, DDLOCK_WAIT | DDLOCK_WRITEONLY, nullptr);

		*ppBits = (char*)ddsd.lpSurface;
		*pWidth = ddsd.dwWidth;
		*pHeight = ddsd.dwHeight;
		*pPitch = ddsd.lPitch;

		return true;
	}

	return false;
}

void DDraw::UnlockBackBuffer()
{
	if (m_pDDSBack)
		m_pDDSBack->Unlock(nullptr);
}

void DDraw::cleanupBackBuffer()
{
	if (m_pDDSBack)
	{
		m_pDDSBack->Release();
		m_pDDSBack = nullptr;
	}
}

bool DDraw::CalculateClipArea(Vec2i* pSrcStart, Vec2i* pDestStart, Vec2i* pDestSize, Vec2i* pPos, Vec2i* pImageSize)
{
	pDestStart->x = max(pPos->x, 0);
	pDestStart->y = max(pPos->y, 0);
	pDestStart->x = min(pDestStart->x, m_width);
	pDestStart->y = min(pDestStart->y, m_height);

	Vec2i destEnd = { max(pPos->x + pImageSize->x, 0), max(pPos->y + pImageSize->y, 0) };
	destEnd.x = min(destEnd.x, m_width);
	destEnd.y = min(destEnd.y, m_height);

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

bool DDraw::DrawBitmap(int startX, int startY, int width, int height, char* pBits)
{
#ifdef _DEBUG
	if (!m_pLockedBackBuffer)
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
	char* pDest = m_pLockedBackBuffer + (destStart.y * m_lockedBackBufferPitch) + (destStart.x * RGBA_SIZE);

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
		pDest += m_lockedBackBufferPitch;
	}

	return true;
}

bool DDraw::DrawBitmap(int width, int height, char* pBits)
{
	return DrawBitmap(m_rcView.left, m_rcView.top, width, height, pBits);
}

bool DDraw::BeginDraw()
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
	if (bufferWidth != m_width || bufferHeight != m_height)
	{
		__debugbreak();
	}
#endif
	m_pLockedBackBuffer = pBuffer;
	m_lockedBackBufferPitch = bufferPitch;

	return true;
}

void DDraw::EndDraw()
{
	UnlockBackBuffer();
	m_pLockedBackBuffer = nullptr;
	m_lockedBackBufferPitch = 0;
}

void DDraw::Blt()
{
	m_pDDSPrimary->Blt(&m_rcView, m_pDDSBack, nullptr, DDBLT_WAIT, nullptr);
}

void DDraw::Clear()
{
	if (!m_pLockedBackBuffer)
	{
#ifdef _DEBUG
		__debugbreak();
#endif
	}
	for (DWORD y = 0; y < m_height; y++)
	{
		memset(m_pLockedBackBuffer + y * m_lockedBackBufferPitch, 0, 4 * m_width);
	}
}

void DDraw::UpdateWindowSize()
{
	cleanupBackBuffer();

	UpdateWindowPos();

	DWORD width = m_rcView.right - m_rcView.left;
	DWORD height = m_rcView.bottom - m_rcView.top;

	createBackBuffer(width, height);

	if (BeginDraw())
	{
		Clear();
		EndDraw();
	}
}

void DDraw::UpdateWindowPos()
{
	GetClientRect(m_hWnd, &m_rcView);
	::ClientToScreen(m_hWnd, (POINT*)&m_rcView.left);
	::ClientToScreen(m_hWnd, (POINT*)&m_rcView.right);
}

void DDraw::CleanUp()
{
	if (m_pDDClipper)
	{
		m_pDDClipper->Release();
		m_pDDClipper = nullptr;
	}
	if (m_pDDSBack)
	{
		m_pDDSBack->Release();
		m_pDDSBack = nullptr;
	}
	if (m_pDDSPrimary)
	{
		m_pDDSPrimary->Release();
		m_pDDSPrimary = nullptr;
	}
	if (m_pDDraw7)
	{
		m_pDDraw7->Release();
		m_pDDraw7 = nullptr;
	}
}

DWORD DDraw::width() const
{
	return m_width;
}

DWORD DDraw::height() const
{
	return m_height;
}