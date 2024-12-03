#pragma once

#include <GameDev2D.h>

class Room
{
private:
	GameDev2D::AABB m_box;
	int m_id = 0;

	GameDev2D::Color m_color = GameDev2D::ColorList::Gray;
	bool m_selected = false;

	bool m_active = true;
public:
	Room(GameDev2D::Vector2 center);
	Room(GameDev2D::AABB box);
	~Room();

	void Update(float delta);
	void Render(GameDev2D::BatchRenderer& renderer);

	float RoundM(float n, float m);

	GameDev2D::AABB& GetAABB();

	void SetId(int id) { m_id = id; }
	int GetId() { return m_id; }

	GameDev2D::Vector2 GetSize() { return m_box.GetDimensions(); }

	void SelectRoom();
	bool GetSelected() { return m_selected; }

	bool GetActive() { return m_active; }
	void SetActive(bool active) { m_active = active; }
};