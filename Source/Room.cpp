#include "Room.h"
#include "Constants.h"

using namespace GameDev2D;

Room::Room(Vector2 center)
{
	Random r;
	
	float x = RoundM((float)r.RandomInt(c_cellSize * 1, c_cellSize * 12), c_cellSize * 2);
	float y = RoundM((float)r.RandomInt(c_cellSize * 1, c_cellSize * 12), c_cellSize * 2);

	m_box.lower = { center.x - x, center.y - y };
	m_box.upper = { center.x + x, center.y + y };
}

Room::Room(GameDev2D::AABB box) :
	m_box(box)
{
}

Room::~Room()
{
}

void Room::Update(float delta)
{
}

void Room::Render(GameDev2D::BatchRenderer& renderer)
{
	if (!m_active)return;
	Vector2 size = m_box.GetDimensions();
	for (int k = 0; k < size.y / c_cellSize; k++)
	{
		Vector2 offset;
		offset.y = m_box.lower.y + k * c_cellSize;
		for (int i = 0; i < size.x / c_cellSize; i++)
		{
			offset.x = m_box.lower.x + i * c_cellSize;
			renderer.RenderQuad(offset, {(float) c_cellSize, (float)c_cellSize }, m_color, ColorList::Blue, 1);
		}
	}
	renderer.RenderQuad(m_box.Center(), m_box.GetDimensions(), { .5f,.5f }, ColorList::Clear, ColorList::White,2);
}

float Room::RoundM(float n, float m)
{
	return floorf(((n + m - 1) / m)) * m;
}

AABB& Room::GetAABB()
{
	return m_box;
}

void Room::SelectRoom()
{
	m_color = ColorList::Red;
	m_selected = true;
}
