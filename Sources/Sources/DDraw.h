#pragma once

#include <ddraw.h>
#include <crtdbg.h>
#include <cassert>
#include <cmath>

#include "Vector.hpp"

namespace hs {
	class DDraw
	{
	private:
		LPDIRECTDRAW			_pDDraw;
		LPDIRECTDRAW7			_pDDraw7;
		LPDIRECTDRAWSURFACE7	_pDDSPrimary;
		LPDIRECTDRAWSURFACE7	_pDDSBack;
		LPDIRECTDRAWCLIPPER		_pDDClipper;

		char*					_pLockedBackBuffer;
		DWORD					_lockedBackBufferPitch;
		DWORD					_width;
		DWORD					_height;
		HWND					_hWnd;
		RECT					_rcView;

		bool CreateBackBuffer(DWORD width, DWORD height);
		void CleanUpBackBuffer();
	public:
		DDraw();
		~DDraw();

		bool Init(HWND hwnd);
		bool LockBackBuffer(char** ppBits, DWORD* pWidth, DWORD* pHeight, DWORD* pPitch);
		void UnlockBackBuffer();

		bool CalculateClipArea(Vec2i* srcStart, Vec2i* destStart, Vec2i* destSize, Vec2i* pos, Vec2i* imageSize);
		bool DrawBitmap(int startX, int startY, int width, int height, char* pBits);

		bool BeginDraw();
		void EndDraw();
		void Blt();

		void Clear();
		void UpdateWindowSize();
		void UpdateWindowPos();

		void CleanUp();
		
		// getter, setter
		DWORD width() const;
		DWORD height() const;
	};
}