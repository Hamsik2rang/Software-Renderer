#include "InputManager.h"


InputManager* InputManager::s_pInstance = nullptr;

InputManager::InputManager()
	:m_keyMap(0lu), m_width(0u), m_height(0u), m_xDelta(0), m_yDelta(0)
{}

InputManager::~InputManager()
{
	if (s_pInstance)
	{
		delete s_pInstance;
		s_pInstance = nullptr;
	}
}

InputManager* InputManager::GetInstance()
{
	if (nullptr == s_pInstance)
	{
		s_pInstance = new InputManager;
	}
	return s_pInstance;
}

uint64_t InputManager::Translate(WPARAM wParam)
{
	if (wParam < 'A')
	{
		if (wParam >= VK_LEFT && wParam <= VK_DOWN)
		{
			return (uint64_t)1u << (wParam - VK_LEFT);
		}
		//TODO: Implement this.
	}
	else if (wParam >= 'A' && wParam <= 'Z')
	{
		return (uint64_t)1u << (wParam - 'A' + 4);
	}
	return 0;
}

bool InputManager::IsPressed(eInput input)
{
	return m_keyMap & (uint64_t)1 << (uint64_t)input;
}

void InputManager::ShowCursor()
{
	::ShowCursor(true);
}

void InputManager::HideCursor()
{
	::ShowCursor(false);
}

void InputManager::MouseMove()
{
	// 스크린 크기가 지정되어 있지 않다면 리턴
	if (m_width == 0 || m_height == 0)
	{
		return;
	}
	POINT cursorPos;
	::GetCursorPos(&cursorPos);
	m_xDelta += cursorPos.x - m_lastCursorXPos;
	m_yDelta += cursorPos.y - m_lastCursorYPos;
}

POINT InputManager::GetMouseDelta()
{
	POINT delta{ m_xDelta, m_yDelta };
	return delta;
}

void InputManager::KeyPress(WPARAM wParam)
{
	m_keyMap |= Translate(wParam);
}

void InputManager::KeyRelease(WPARAM wParam)
{
	m_keyMap &= ~Translate(wParam);
}


void InputManager::SetScreenSize(DWORD width, DWORD height)
{
	m_width = width;
	m_height = height;
}

void InputManager::SetCursorToCenter()
{
	::SetCursorPos(m_width / 2, m_height / 2);
	m_lastCursorXPos = m_width / 2;
	m_lastCursorYPos = m_height / 2;
	m_xDelta = 0;
	m_yDelta = 0;
}