#pragma once


//#include "solid.h"

#include <stdio.h>
#include <vector>

struct Vertex
{
	float x, y, z;
};

struct Facet
{
	Vertex normal;
	Vertex vertex[3];
};


class CSolidBody : public std::vector<Facet>
{
public:
	CSolidBody(void);
	~CSolidBody(void);

	bool LoadSTL(const char *path);
	int id() { return m_id; }
	void select(bool select) { m_bSelected = select; }
	bool selected() { return m_bSelected; }
//	float GetZDepth() { return Z_Depth; }
//	float GetExtent() { return Big_Extent; }

private:
	bool is_stl_ascii(const char *path);
	bool load_stl_ascii(const char *path);
	bool load_stl_bin(const char *path);
	void find_extents();

private:
	int m_id;
	bool m_bSelected;
	float Z_Depth, Big_Extent;
};
