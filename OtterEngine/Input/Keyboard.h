#pragma once

#include <bitset>
#include <queue>

#include "OtterEngine/Common/Constants.h"

class Keyboard {

	friend class Window;

private:
	struct KeyEvent {
	public:
		enum class Type {KeyDown, KeyUp};

		KeyEvent(Type type, unsigned char keyCode);
		~KeyEvent() = default;

		Type getEventType() const { return m_type; }

	private:
		Type m_type;
		unsigned char m_keyCode;
	};

public:
	Keyboard() = default;
	~Keyboard() = default;

	// prevent copying
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator= (const Keyboard&) = delete;

	// check if buffers are empty
	bool KeyEventBufferEmpty() { return m_keyEventBuffer.empty(); }
	bool CharBufferEmpty() { return m_charBuffer.empty(); }

	bool IsKeyPressed(unsigned char keyCode) { return m_keyState[keyCode]; }

	KeyEvent* ReadFirstEvent();
	char* ReadFirstChar();

	// pop buffers until under kInputBufferLimit
	void PopKeyEventBuffer();
	void PopCharBuffer();

	// empty buffers
	void ClearKeyState() { m_keyState.reset(); }
	void ClearKeyEventBuffer() { std::queue<KeyEvent> q; std::swap(m_keyEventBuffer, q); }
	void ClearCharBuffer() { std::queue<char> q; std::swap(m_charBuffer, q); }

private:
	// called by Window instance
	void OnKeyDown(unsigned char keyCode);
	void OnKeyUp(unsigned char keyCode);
	void OnCharEntered(unsigned char character);

private:
	std::bitset<256> m_keyState;
	std::queue<KeyEvent> m_keyEventBuffer;
	std::queue<char> m_charBuffer;
};