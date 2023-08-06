#include "Mouse.h"

#include "OtterEngine/Common/pch.h"

Mouse::MouseEvent::MouseEvent(Type type, Vector3Int pos) : m_type(type), m_position(pos) {
}

Mouse::MouseEvent Mouse::ReadFirstEvent() {
	if (m_mouseEventBuffer.size() > 0) {
		MouseEvent mouseEvent = m_mouseEventBuffer.front();
		m_mouseEventBuffer.pop();
		return mouseEvent;
	}

	return MouseEvent(MouseEvent::Type::Empty, Vector3Int());
}

void Mouse::PopMouseEventBuffer() {
	while (m_mouseEventBuffer.size() > kInputBufferLimit) {
		m_mouseEventBuffer.pop();
	}
}

void Mouse::OnMouseMove(const Vector3Int& pos) {
	m_position.x = pos.x;
	m_position.y = pos.y;

	m_mouseEventBuffer.push(MouseEvent(MouseEvent::Type::Move, pos));
	PopMouseEventBuffer();
}

void Mouse::OnMouseEnter() {
	m_inWindow = true;
	m_mouseEventBuffer.push(MouseEvent(MouseEvent::Type::Enter, Vector3Int()));
	PopMouseEventBuffer();
}

void Mouse::OnMouseLeave() {
	m_inWindow = false;
	m_mouseEventBuffer.push(MouseEvent(MouseEvent::Type::Leave, Vector3Int()));
	PopMouseEventBuffer();
}

void Mouse::OnLButtonDown(const Vector3Int& pos) {
	m_LButtonState = true;
	m_mouseEventBuffer.push(MouseEvent(MouseEvent::Type::LButtonDown, pos));
	PopMouseEventBuffer();
}

void Mouse::OnLButtonUp(const Vector3Int& pos) {
	m_LButtonState = false;
	m_mouseEventBuffer.push(MouseEvent(MouseEvent::Type::LButtonUp, pos));
	PopMouseEventBuffer();
}

void Mouse::OnMButtonDown(const Vector3Int& pos) {
	m_MButtonState = true;
	m_mouseEventBuffer.push(MouseEvent(MouseEvent::Type::MButtonDown, pos));
	PopMouseEventBuffer();
}

void Mouse::OnMButtonUp(const Vector3Int& pos) {
	m_MButtonState = false;
	m_mouseEventBuffer.push(MouseEvent(MouseEvent::Type::MButtonUp, pos));
	PopMouseEventBuffer();
}

void Mouse::OnRButtonDown(const Vector3Int& pos) {
	m_RButtonState = true;
	m_mouseEventBuffer.push(MouseEvent(MouseEvent::Type::RButtonDown, pos));
	PopMouseEventBuffer();
}

void Mouse::OnRButtonUp(const Vector3Int& pos) {
	m_RButtonState = false;
	m_mouseEventBuffer.push(MouseEvent(MouseEvent::Type::RButtonUp, pos));
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
	m_mouseEventBuffer.push(MouseEvent(MouseEvent::Type::WheelUp, pos));
	PopMouseEventBuffer();
}

void Mouse::OnWheelDown(const Vector3Int& pos) {
	m_mouseEventBuffer.push(MouseEvent(MouseEvent::Type::WheelDown, pos));
	PopMouseEventBuffer();
}
