#pragma once

#include <GameDev2D.h>
#include "Constants.h"
#include "Edge.h"
#include "Triangle.h"
#include "Point.h"

class Room;
class Graph;

namespace GameDev2D
{

	class Game
	{
	public:
		Game();
		~Game();

		//Write game logic in the Update method
		void OnUpdate(float delta);

		//Render game objects in the Draw method
		void OnRender(BatchRenderer& batchRenderer);

		//Input event methods
		void OnKeyEvent(KeyCode keyCode, KeyState keyState);
		void OnMouseButtonEvent(MouseButton button, MouseButtonState state, float mouseX, float mouseY);
		void OnMouseMovedEvent(float mouseX, float mouseY);

		std::vector<Edge> Trianglulation(std::vector<Point>& points);

	private:
		Room* m_rooms[c_cellAmt];

		float m_timer = 0;

		float m_radius = 300;
		Vector2 m_center = { 600,600 };

		bool m_seperating = true;
		int m_counter = 20;

		float m_averageRoomSize = 0;

		Vector2 GetRandomPointInCircle(Vector2 center, float radius);

		float RoundM(float n, float m);

		std::vector<Edge> m_edges;

		std::vector<Edge> m_paths;

		std::vector<Edge> m_halls;

		std::vector<AABB> m_hallCells;

		Graph* m_graph;

		Room* FindRoomById(int id);

		bool Intersects(Edge e1, Edge e2);

	};
}
