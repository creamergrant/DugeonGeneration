#include "Game.h"
#include "Room.h"
#include <set>
#include "../Graph/Graph.h"
#include <cmath>

namespace GameDev2D
{
	Game::Game()
	{
		//generate a bunch of rooms inside of a circle with different ids
		for (int i = 0; i < c_cellAmt; i++)
		{
			m_rooms[i] = new Room(GetRandomPointInCircle(m_center, m_radius));
			m_averageRoomSize += m_rooms[i]->GetSize().x;
			m_averageRoomSize += m_rooms[i]->GetSize().y;
			m_rooms[i]->SetId(i + 1);
		}
		m_averageRoomSize /= c_cellAmt*2;
		m_averageRoomSize *= 1.35f;

		int counter = 1;
		for (Room* r : m_rooms)
		{
			if (r->GetSize().x > m_averageRoomSize && r->GetSize().y > m_averageRoomSize)
			{
				r->SelectRoom();
				r->SetId(counter);
				counter++;
			}
		}

		m_graph = new Graph();
	}

	Game::~Game()
	{
		for (Room* ro : m_rooms)
		{
			delete ro;
		}
	}

	void Game::OnUpdate(float delta)
	{
		//seperate the rooms
		if (m_seperating)
		{
			int counter = 0;
			for (Room* r : m_rooms)
			{
				for (Room* r2 : m_rooms)
				{
					Vector2 r1Lower = r->GetAABB().lower;
					Vector2 r2Lower = r2->GetAABB().lower;
					if (r != r2)
					{
						r->GetAABB().ResolveCollision(r2->GetAABB());
						if (r1Lower != r->GetAABB().lower && r2Lower != r->GetAABB().lower)
						{
							counter++;
						}
					}
				}
			}
			if (counter == 0)
			{
				//once seperated do trianluation and minimum spanning tree
				m_seperating = false;
				std::vector<Point> points;
				for (Room* r : m_rooms)
				{
					if (r->GetSelected())
					{
						points.push_back({r->GetAABB().Center(), r->GetId()});
					}
				}
				m_edges = Trianglulation(points);
				for (auto p : points)
				{
					m_graph->m_nodeList.push_back(Vertex(p, 0));
				}
				for (auto e : m_edges)
				{
					m_graph->AddEdge(e.v1.id, e.v2.id);
				}

				m_graph->Pain();

				for (auto t : m_graph->m_mstList)
				{
					int tx = t.id1;
					int ty = t.id2;
					for (auto e : m_edges)
					{
						int ex = e.v1.id;
						int ey = e.v2.id;
						if ((tx == ex || tx == ey) && (ty == ex || ty == ey))
						{
							m_paths.push_back(e);
						}
					}
				}

				//connect the paths with straight lines

				for (Edge e : m_paths)
				{
					Vector2 n1 = e.v1.p;
					Vector2 n2 = e.v2.p;
					int id1 = e.v1.id;
					int id2 = e.v2.id;

					Vector2 mid = (n2 - n1) / 2 + n1;

					Room* r1 = FindRoomById(id1);
					Room* r2 = FindRoomById(id2);


					if (mid.x > r1->GetAABB().lower.x && mid.x < r1->GetAABB().upper.x && mid.x > r2->GetAABB().lower.x && mid.x < r2->GetAABB().upper.x)
					{
						m_halls.push_back(Edge(mid, { mid.x, r1->GetAABB().Center().y }));
						m_halls.push_back(Edge(mid, { mid.x, r2->GetAABB().Center().y }));
					}
					else if (mid.y > r1->GetAABB().lower.y && mid.y < r1->GetAABB().upper.y && mid.y > r2->GetAABB().lower.y && mid.y < r2->GetAABB().upper.y)
					{
						m_halls.push_back(Edge(mid, { r1->GetAABB().Center().x, mid.y }));
						m_halls.push_back(Edge(mid, { r2->GetAABB().Center().x, mid.y }));
					}
					else 
					{
						Vector2 l1;
						Vector2 l2;

						l1 = { r2->GetAABB().Center().x, r1->GetAABB().Center().y };
						l2 = { r1->GetAABB().Center().x, r2->GetAABB().Center().y };

						m_halls.push_back(Edge(r1->GetAABB().Center(), l1));
						m_halls.push_back(Edge(l1, r1->GetAABB().Center()));
						m_halls.push_back(Edge(r1->GetAABB().Center(), l2));
						m_halls.push_back(Edge(l2, r1->GetAABB().Center()));

						l1 = { r1->GetAABB().Center().x, r2->GetAABB().Center().y };
						l2 = { r2->GetAABB().Center().x, r1->GetAABB().Center().y };

						m_halls.push_back(Edge(r2->GetAABB().Center(), l1));
						m_halls.push_back(Edge(l1, r2->GetAABB().Center()));
						m_halls.push_back(Edge(r2->GetAABB().Center(), l2));
						m_halls.push_back(Edge(l2, r2->GetAABB().Center()));
					}

				}

				for (auto ro : m_rooms)
				{
					if(!ro->GetSelected())
						ro->SetActive(false);
				}

				for (auto ro : m_rooms)
				{
					if (!ro->GetSelected())
					{
						for (auto e : m_halls)
						{
							Vector2 low = ro->GetAABB().lower;
							Vector2 up = ro->GetAABB().upper;
							Edge edges[4] =
							{
								{low,{low.x,up.y}},
								{low,{up.x, low.y}},
								{up,{low.x,up.y}},
								{up,{up.x, low.y}}
							};
							for (int i = 0; i < 4; i++)
							{
								if (Intersects(edges[i], e))
								{
									ro->SetActive(true);
								}
							}
						}
					}
				}

				for (auto e : m_halls)
				{
					Vector2 lower = e.v1.p;
					Vector2 upper = e.v2.p;

					if (lower.x == upper.x)
					{
						lower.x -= (c_cellSize * 1.5f);
						upper.x += (c_cellSize * 1.5f);
					}
					if (lower.y == upper.y)
					{
						lower.y -= (c_cellSize * 1.5f);
						upper.y += (c_cellSize * 1.5f);
					}

					AABB temp(lower, upper);
					Vector2 size = temp.GetDimensions();
					for (int k = 0; k < size.y / c_cellSize; k++)
					{
						Vector2 offset;
						offset.y = temp.lower.y + k * c_cellSize;
						for (int i = 0; i < size.x / c_cellSize; i++)
						{
							offset.x = temp.lower.x + i * c_cellSize;
							AABB box(offset, offset + Vector2{c_cellSize, c_cellSize});
							bool contained = false;
							for (auto r : m_rooms)
							{
								if (r->GetActive())
								{
									if (r->GetAABB().Contains(box))
									{
										contained = true;
									}
								}
							}
							if (!contained)
								m_hallCells.push_back(box);
						}
					}
				}

			}
		}
		else
		{
			int i = 0;
		}
	}

	void Game::OnRender(BatchRenderer& batchRenderer)
	{
		batchRenderer.BeginScene();

		for (auto ro : m_rooms)
		{
			if(ro->GetActive())
				ro->Render(batchRenderer);
		}
		batchRenderer.RenderCircle(m_center, m_radius, ColorList::Clear, ColorList::Black, 2);

		for (Edge e : m_paths)
		{
			//e.Render(batchRenderer);
		}

		for (Edge h : m_halls)
		{
			h.Render(batchRenderer);
		}

		for (auto c : m_hallCells)
		{
			batchRenderer.RenderQuad(c.Center(), c.GetDimensions(), { .5f, .5f }, ColorList::Gray, ColorList::Black, 1);
		}

		batchRenderer.EndScene();
	}

	void Game::OnKeyEvent(KeyCode keyCode, KeyState keyState)
	{
		
	}

	void Game::OnMouseButtonEvent(MouseButton button, MouseButtonState state, float mouseX, float mouseY)
	{
		
	}

	void Game::OnMouseMovedEvent(float mouseX, float mouseY)
	{
		
	}

	std::vector<Edge> Game::Trianglulation(std::vector<Point>& points)
	{
		std::vector<Triangle> triangles;
		std::vector<Edge> edges;

		//big triangle that encompasses all points
		Point p1 = { -4000,-4000,0 };
		Point p2 = { 4000,-4000,0 };
		Point p3 = { 0,4000,0 };
		triangles.emplace_back(p1, p2, p3);

		for (const auto& p : points)
		{
			std::set<Edge> polygon;

			for (auto it = triangles.begin(); it != triangles.end();)
			{
				//if the current triangle contains a point
				if (it->ContainsPoint(p))
				{
					//add the new edges and erase the triangle
					polygon.insert(Edge(it->v1, it->v2));
					polygon.insert(Edge(it->v2, it->v3));
					polygon.insert(Edge(it->v3, it->v1));
					it = triangles.erase(it);
				}
				else
				{
					++it;
				}
			}

			//get rid of un wanted edges
			for (auto it = polygon.begin(); it != polygon.end();)
			{
				if (polygon.find(Edge(it->v2, it->v1)) != polygon.end())
				{
					polygon.erase(Edge(it->v2, it->v1));
					it = polygon.erase(it);
				}
				else
				{
					++it;
				}
			}

			//add the new triangle
			for (const Edge& edge : polygon)
			{
				triangles.emplace_back(edge.v1, edge.v2, p);
			}
		}

		//get all the edges
		std::vector<Edge> result;
		for (const Triangle& t : triangles)
		{
			if (!(t.v1 == p1 || t.v1 == p2 || t.v1 == p3 ||
				t.v2 == p1 || t.v2 == p2 || t.v2 == p3 ||
				t.v3 == p1 || t.v3 == p2 || t.v3 == p3))
			{
				result.emplace_back(t.v1, t.v2);
				result.emplace_back(t.v2, t.v3);
				result.emplace_back(t.v3, t.v1);
			}
		}

		return result;
	}

	Vector2 Game::GetRandomPointInCircle(Vector2 center, float radius)
	{
		Random rand;
		float r = radius * sqrtf(rand.RandomFloat(0,1));
		float theta = rand.RandomFloat(0, 1) * 2.0f * 3.14f;
		Vector2 point;
		point.x = center.x + r * cosf(theta);
		point.y = center.y + r * sinf(theta);
		point.x = RoundM(point.x, c_cellSize);
		point.y = RoundM(point.y, c_cellSize);
		return point;
	}

	float Game::RoundM(float n, float m)
	{
		return floorf(((n + m - 1) / m))* m;
	}

	Room* Game::FindRoomById(int id)
	{
		for (auto r : m_rooms)
		{
			if (r->GetId() == id && r->GetSelected())
				return r;
		}
		return nullptr;
	}

	bool Game::Intersects(Edge e1, Edge e2)
	{
		Vector2 a1 = e1.v1.p;
		Vector2 a2 = e1.v2.p;
		Vector2 b1 = e2.v1.p;
		Vector2 b2 = e2.v2.p;

		Vector2 b = a2 - a1;
		Vector2 d = b2 - b1;
		float bDotPerp = b.x * d.y - b.y * d.x;

		if (bDotPerp == 0)
			return false;

		Vector2 c = b1 - a1;
		float t = (c.x * d.y - c.y * d.x) / bDotPerp;

		if (t < 0 || t>1)
			return false;

		float u = (c.x * b.y - c.y * b.x) / bDotPerp;
		if (u < 0 || u > 1)
			return false;

		return true;
	}

}