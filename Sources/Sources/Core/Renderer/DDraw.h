#pragma once

#include "../Math/Vector.hpp"

#include <ddraw.h>
#include <crtdbg.h>
#include <cassert>
#include <cmath>

class DDraw
{
private:
	LPDIRECTDRAW			m_pDDraw;
	LPDIRECTDRAW7			m_pDDraw7;
	LPDIRECTDRAWSURFACE7	m_pDDSPrimary;
	LPDIRECTDRAWSURFACE7	m_pDDSBack;
	LPDIRECTDRAWCLIPPER		m_pDDClipper;

	char*	m_pLockedBackBuffer;
	DWORD	m_lockedBackBufferPitch;
	DWORD	m_width;
	DWORD	m_height;
	HWND	m_hWnd;
	RECT	m_rcView;

	bool CreateBackBuffer(DWORD width, DWORD height);
	void CleanupBackBuffer();
public:
	DDraw();
	~DDraw();

	bool Init(HWND hwnd);
	bool LockBackBuffer(char** ppBits, DWORD* pWidth, DWORD* pHeight, DWORD* pPitch);
	void UnlockBackBuffer();

	bool CalculateClipArea(Vec2i* pSrcStart, Vec2i* pDestStart, Vec2i* pDestSize, const Vec2i* pPos, const Vec2i* pImageSize);
	bool DrawBitmap(int startX, int startY, int width, int height, char* pBits);
	bool DrawBitmap(int width, int height, char* pBits);

	bool BeginDraw();
	void EndDraw();
	void Blt();

	void Clear();
	void UpdateWindowSize();
	void UpdateWindowPos();
	void CleanUp();

	bool BeginGDI(HDC* pHdc);
	void DrawInfo(HDC hdc, WCHAR* text, DWORD len);
	void EndGDI(HDC hdc);
	// getter, setter
	DWORD GetWidth() const;
	DWORD GetHeight() const;
};
