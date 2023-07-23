#include "Mouse.h"

#include "OtterEngine/Common/MinimalWindows.h"

Mouse::MouseEvent::MouseEvent(Type type, Vector3Int pos) : m_type(type), m_position(pos) {
}

Mouse::MouseEvent* Mouse::ReadFirstEvent() {
	if (m_mouseEventBuffer.size() > 0) {
		Mouse::MouseEvent* mouseEvent = new Mouse::MouseEvent(m_mouseEventBuffer.front());
		m_mouseEventBuffer.pop();
		return mouseEvent;
	}
	return nullptr;
}

void Mouse::PopMouseEventBuffer() {
	while (m_mouseEventBuffer.size() > kInputBufferLimit) {
		m_mouseEventBuffer.pop();
	}
}

void Mouse::OnMouseMove(const Vector3Int& pos) {
	m_position.m_x = pos.m_x;
	m_position.m_y = pos.m_y;

	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::Move, pos));
	PopMouseEventBuffer();
}

void Mouse::OnMouseEnter() {
	m_inWindow = true;
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::Enter, Vector3Int()));
	PopMouseEventBuffer();
}

void Mouse::OnMouseLeave() {
	m_inWindow = false;
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::Leave, Vector3Int()));
	PopMouseEventBuffer();
}

void Mouse::OnLButtonDown(const Vector3Int& pos) {
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::LButtonDown, pos));
	PopMouseEventBuffer();
}

void Mouse::OnLButtonUp(const Vector3Int& pos) {
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::LButtonUp, pos));
	PopMouseEventBuffer();
}

void Mouse::OnRButtonDown(const Vector3Int& pos) {
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::RButtonDown, pos));
	PopMouseEventBuffer();
}

void Mouse::OnRButtonUp(const Vector3Int& pos) {
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::RButtonUp, pos));
	PopMouseEventBuffer();
}

void Mouse::OnMButtonDown(const Vector3Int& pos) {
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::MButtonDown, pos));
	PopMouseEventBuffer();
}

void Mouse::OnMButtonUp(const Vector3Int& pos) {
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::MButtonUp, pos));
	PopMouseEventBuffer();
}

void Mouse::OnWheelScrolled(const Vector3Int& pos, int delta) {
	m_wheelDelta += delta;
	while (m_wheelDelta >= WHEEL_DELTA) {
		m_wheelDelta -= WHEEL_DELTA;
		OnWheelUp(pos);
	}
	while (m_wheelDelta <= -WHEEL_DELTA) {
		m_wheelDelta += WHEEL_DELTA;
		OnWheelDown(pos);
	}
}

void Mouse::OnWheelUp(const Vector3Int& pos) {
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::WheelUp, pos));
	PopMouseEventBuffer();
}

void Mouse::OnWheelDown(const Vector3Int& pos) {
	m_mouseEventBuffer.push(Mouse::MouseEvent(Mouse::MouseEvent::Type::WheelDown, pos));
	PopMouseEventBuffer();
}
