#pragma once

#include <cstdint>
#include <wtypes.h>

enum class eInput
{
	LEFT = 0,
	UP,
	RIGHT,
	DOWN,
	A,B,C,D,E,
	F,G,H,I,J,
	K,L,M,N,O,
	P,Q,R,S,T,
	U,V,W,X,Y,
	Z,
	ESCAPE,
};

class InputManager
{
private:
	// For keyboard input
	uint32_t m_keyMap;
	// For mouse input
	int m_xDelta;
	int m_yDelta;
	DWORD m_lastCursorXPos;
	DWORD m_lastCursorYPos;

	// Screen infos
	DWORD m_width;
	DWORD m_height;

	InputManager();
	uint32_t Translate(DWORD wParam);
	static InputManager* s_pInstance;
public:
	static InputManager* GetInstance();
	~InputManager();

	void SetScreenSize(DWORD width, DWORD height);
	void SetCursorToCenter();
	
	void MouseMove();
	POINT GetMouseDelta();
	void KeyPress(DWORD wParam);
	void KeyRelease(DWORD wParam);
	bool IsPressed(eInput input);
};
