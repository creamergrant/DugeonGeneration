#include "Graph.h"

void Graph::AddEdge(int id, int id2)
{
	Vertex& v1 = FindVertex(id);
	Vertex& v2 = FindVertex(id2);

	float dist = v1.m_point.p.Distance(v2.m_point.p);
	v1.m_dist = dist;
	v2.m_dist = dist;

	v1.m_edges.emplace_back(dist, v2);
	v2.m_edges.emplace_back(dist, v1);

	GraphEdge e(v1.m_id, v2.m_id, dist);
	m_edgeList[dist] = e;
}

int Graph::CountNodes()
{
	return m_nodeList.size();
}

Vertex& Graph::FindVertex(int id)
{
	for (Vertex& v : m_nodeList)
	{
		if (v.m_id == id)
			return v;
	}
}

std::list<std::pair<float, Vertex&>> Graph::m_adj(int id)
{
	return FindVertex(id).m_edges;
}

void Graph::Pain()
{
	UnionFind uf(CountNodes());

	std::vector<GraphEdge> edgesLeft;

	for (auto [key, value] : m_edgeList)
	{
		edgesLeft.push_back(value);
	}

	for (auto [weight, edge] : m_edgeList)
	{
		int a = edge.id1;
		int b = edge.id2;

		if (uf.Find(a) != uf.Find(b))
		{
			m_mstList.push_back(edge);

			edgesLeft.erase(std::remove(edgesLeft.begin(), edgesLeft.end(), edge), edgesLeft.end());

			uf.Union(a, b);
		}
	}
	int amtToadd = m_edgeList.size() * .125f;

	for (int i = 0; i < amtToadd; i++)
	{
		m_mstList.push_back(edgesLeft[i]);
	}
	
}

void Graph::OptimizeHalls()
{
	std::map<int, std::pair<float, GraphEdge>> shortedges;

	for (const GraphEdge& e : m_mstList)
	{
		int a = e.id1;
		int b = e.id2;

		if (a > b)
		{
			std::swap(a, b);
		}
		
	}
}

float Graph::FindEdge(GraphEdge e)
{
	for (const auto& p : m_edgeList)
	{
		if (p.second == e)
		{
			return p.first;
		}
	}
}