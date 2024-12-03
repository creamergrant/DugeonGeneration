#pragma once

#include <list>
#include <vector>
#include "Vertex.h"
#include <map>

struct GraphEdge
{
	int id1 = 0;
	int id2 = 0;
	float weight = 0;

	GraphEdge() {}

	GraphEdge(int id1, int id2, float weight) :
		id1(id1),
		id2(id2),
		weight(weight)
	{
	}

	bool operator==(const GraphEdge& o) const
	{
		return (id1 == o.id1 && id2 == o.id2 && weight == o.weight);
	}
};

class UnionFind 
{
private:
	std::vector<int> m_parent;
	std::vector<int> m_rank;
public:
	UnionFind(int n)
	{
		n++;
		m_parent.resize(n);
		m_rank.resize(n, 0);
		for (int i = 0; i < n; ++i)
		{
			m_parent[i] = i;
		}
	}

	int Find(int x)
	{
		if (m_parent[x] != x)
		{
			m_parent[x] = Find(m_parent[x]);
		}
		return m_parent[x];
	}

	void Union(int x, int y)
	{
		int rx = Find(x);
		int ry = Find(y);

		if (rx != ry)
		{
			if (m_rank[rx] > m_rank[ry])
			{
				m_parent[ry] = rx;
			}
			else if (m_rank[rx] < m_rank[ry])
			{
				m_parent[rx] = ry;
			}
			else
			{
				m_parent[ry] = rx;
				m_rank[rx]++;
			}
		}
	}
};

class Graph
{
private:
	float FindEdge(GraphEdge e);
public:
	void AddEdge(int id, int id2);
	int CountNodes();
	Vertex& FindVertex(int id);

	std::list<std::pair<float, Vertex&>> m_adj(int id);
	std::vector<Vertex> m_nodeList;
	std::map<float, GraphEdge> m_edgeList;

	std::vector<GraphEdge> m_mstList;

	void Pain();

	void OptimizeHalls();
};