#pragma once

#include <queue>

#include "OtterEngine/Common/constants.h"
#include "OtterEngine/Common/Math/MathUtils.h"

class Mouse {

	friend class Window;

public:
	struct MouseEvent {
	public:
		enum class Type {
			Empty,
			Move, Enter, Leave,
			LButtonDown, LButtonUp,
			RButtonDown, RButtonUp,
			MButtonDown, MButtonUp,
			WheelUp, WheelDown
		};

	public:
		MouseEvent(Type type, const Vector3Int& pos);
		~MouseEvent() = default;

		Type getEventType() const { return m_type; }
		const Vector3Int& getPosition() const { return m_position; }

	private:
		Type m_type;
		Vector3Int m_position;
	};

public:
	Mouse() = default;
	~Mouse() = default;

	// prevent copying
	Mouse(const Mouse&) = delete;
	Mouse& operator= (const Mouse&) = delete;

	bool IsInWindow() const { return m_inWindow; }

	bool IsLButtonPressed() { return m_LButtonState; }
	bool IsMButtonPressed() { return m_MButtonState; }
	bool IsRButtonPressed() { return m_RButtonState; }

	MouseEvent ReadFirstEvent();

	bool MouseEventBufferEmpty() const { return m_mouseEventBuffer.empty(); }
	void PopMouseEventBuffer();
	void ClearMouseEventBuffer() { std::queue<MouseEvent> q; std::swap(m_mouseEventBuffer, q); }

private:
	// called by Window instance
	void OnMouseMove(const Vector3Int& pos);
	void OnMouseEnter();
	void OnMouseLeave();
	void OnLButtonDown(const Vector3Int& pos);
	void OnLButtonUp(const Vector3Int& pos);
	void OnMButtonDown(const Vector3Int& pos);
	void OnMButtonUp(const Vector3Int& pos);
	void OnRButtonDown(const Vector3Int& pos);
	void OnRButtonUp(const Vector3Int& pos);
	void OnWheelScrolled(const Vector3Int& pos, int delta);
	void OnWheelUp(const Vector3Int& pos);
	void OnWheelDown(const Vector3Int& pos);

private:
	bool m_inWindow = false;
	bool m_LButtonState = false;
	bool m_MButtonState = false;
	bool m_RButtonState = false;
	Vector3Int m_position;
	int m_wheelDelta = 0;
	std::queue<MouseEvent> m_mouseEventBuffer;
};