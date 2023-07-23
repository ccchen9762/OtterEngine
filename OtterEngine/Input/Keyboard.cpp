#include "Keyboard.h"

Keyboard::KeyEvent::KeyEvent(Type type, unsigned char keyCode) :
	m_type(type), m_keyCode(keyCode) {
}

Keyboard::KeyEvent* Keyboard::ReadFirstEvent() {
	if (m_keyEventBuffer.size() > 0) {
		Keyboard::KeyEvent* keyEvent = new Keyboard::KeyEvent(m_keyEventBuffer.front());
		m_keyEventBuffer.pop();
		return keyEvent;
	}
	return nullptr;
}

char* Keyboard::ReadFirstChar() {
	if (m_charBuffer.size() > 0) {
		char* keyEvent = new char(m_charBuffer.front());
		m_charBuffer.pop();
		return keyEvent;
	}
	return nullptr;
}

void Keyboard::PopKeyEventBuffer() {
	while (m_keyEventBuffer.size() > kInputBufferLimit) {
		m_keyEventBuffer.pop();
	}
}

void Keyboard::PopCharBuffer() {
	while (m_charBuffer.size() > kInputBufferLimit) {
		m_charBuffer.pop();
	}
}

void Keyboard::OnKeyDown(unsigned char keyCode) {
	m_keyState[keyCode] = true;
	m_keyEventBuffer.push(Keyboard::KeyEvent(Keyboard::KeyEvent::Type::KeyDown, keyCode));
	PopKeyEventBuffer();
}

void Keyboard::OnKeyUp(unsigned char keyCode) {
	m_keyState[keyCode] = false;
	m_keyEventBuffer.push(Keyboard::KeyEvent(Keyboard::KeyEvent::Type::KeyUp, keyCode));
	PopKeyEventBuffer();
}

void Keyboard::OnCharEntered(unsigned char character) {
	m_charBuffer.push(character);
	PopCharBuffer();
}
