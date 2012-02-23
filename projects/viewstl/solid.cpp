#include "solid.h"
#include <string.h>
#include <math.h>
#include <stdio.h>

#ifdef linux
#define strnicmp strncasecmp
#define _countof(arr) sizeof(arr)/sizeof(arr[0])
#endif

#define PI 3.14159265358979323846
#define FOV 30   /* Field of view for perspective mode */


CSolidBody::CSolidBody(void)
{
	static int gid = 0;
	m_id = gid++;
	Z_Depth = Big_Extent = 0;
	m_bSelected = false;
}

CSolidBody::~CSolidBody(void)
{

}

bool CSolidBody::load_stl_ascii(const char *path)
{
	FILE *fp = fopen(path, "r");
	if(fp == 0)
		return false;

	char oneline[255];
	char arg1[100], arg2[100];
	Facet facet = {0};

	while( !feof(fp) )
	{
		fgets(oneline, 255, fp);
		sscanf(oneline, "%s", arg1);

		if(0 == strcmp("facet", arg1))
		{
			sscanf(oneline, "%s %s %f %f %f", arg1, arg2, &facet.normal.x, &facet.normal.y, &facet.normal.z);
		}
		else if(0 == strcmp("vertex", arg1))
		{
			for(int i=0; i<3; i++)
			{
				Vertex &vertex = facet.vertex[i];
				if(i > 0)
					// first one already read
					fgets(oneline, 255, fp);
				sscanf(oneline, "%s %f %f %f", arg1, &vertex.x, &vertex.y, &vertex.z);
			}
		}
		else if(0 == strcmp("endfacet", arg1))
		{
			push_back( facet );
		}
	}
	fclose(fp);
	return true;
}

bool CSolidBody::load_stl_bin(const char *path)
{
	FILE *fp = fopen(path, "rb");
	if(fp == 0)
		return false;

	char header[80] = {0};
	char padding[2] = {0};
	int num_facets=0, count=0;
	Facet facet = {0};
	size_t read = fread(header, 80, 1, fp);
	read += fread(&num_facets, 1, 4, fp);
	while( !feof(fp) )
	{
		read += fread(&facet.normal.x, 4, 1, fp);
		read += fread(&facet.normal.y, 4, 1, fp);
		read += fread(&facet.normal.z, 4, 1, fp);
		for(int i=0; i<3; i++)
		{
			read += fread(&facet.vertex[i].x, 4, 1, fp);
			read += fread(&facet.vertex[i].y, 4, 1, fp);
			read += fread(&facet.vertex[i].z, 4, 1, fp);
		}
		read += fread(padding, 2, 1, fp);
		push_back( facet );
		count++;
	}
	fclose(fp);
	return true;
}

bool CSolidBody::is_stl_ascii(const char *path)
{
	FILE *fp = fopen(path, "r");
	if(fp == 0)
		return false;

	const char *keywords[] = {"solid", "facet", "normal", "outer", "loop", "vertex", "endloop", "endfacet", "endsolid"};
	char oneline[255];
	char tmp[100];
	bool result = false;
	fgets(oneline, 255, fp);
	sscanf(oneline, "%s", tmp);
	if(0 == strnicmp(tmp, "solid", sizeof("solid")))
	{
		tmp[0] = 0;
		fgets(oneline, 255, fp);
		sscanf(oneline, "%s", tmp);
		for(int i=0; i<_countof(keywords); i++)
		{
			if(0 == strnicmp(tmp, keywords[i], sizeof(keywords[i])))
			{
				result = true;
				break;
			}
		}
	}
	fclose(fp);
	return result;
}

bool CSolidBody::LoadSTL(const char *path) // ASCII
{
	bool result = false;

	if( is_stl_ascii( path ) )
		result = load_stl_ascii( path );
	else
		result = load_stl_bin( path );

	if(result)
		find_extents();

	return result;
}

void CSolidBody::find_extents()
{
	float LongerSide=0.f, ViewAngle=0.f;
	Vertex extent_pos={0}, extent_neg={0};
	//for(x=0; x<poly_count; x++)
	std::vector<Facet>::iterator it;
	for(it=begin(); it<end(); it++)
	{
		Facet *pfacet = &(*it);
		int i;
		for(i=0; i<3; i++)
		{
			if (pfacet->vertex[i].x > extent_pos.x)
				extent_pos.x = pfacet->vertex[i].x;
			if (pfacet->vertex[i].y > extent_pos.y)
				extent_pos.y = pfacet->vertex[i].y;
			if (pfacet->vertex[i].z > extent_pos.z)
				extent_pos.z = pfacet->vertex[i].z;
		}

		for(i=0; i<3; i++)
		{
			if (pfacet->vertex[i].x < extent_neg.x)
				extent_neg.x = pfacet->vertex[i].x;
			if (pfacet->vertex[i].y < extent_neg.y)
				extent_neg.y = pfacet->vertex[i].y;
			if (pfacet->vertex[i].z < extent_neg.z)
				extent_neg.z = pfacet->vertex[i].z;
		}
	}

	/* Do quick Z_Depth calculation while part resides in ++ quadrant */
	/* Convert Field of view to radians */
	ViewAngle = ((FOV / 2) * (PI / 180));
	if (extent_pos.x > extent_pos.y)
		LongerSide = extent_pos.x;
	if (extent_pos.x < extent_pos.y)
		LongerSide = extent_pos.y;
	/* Put the result where the main drawing function can see it */
	Z_Depth = ((LongerSide / 2) / tan(ViewAngle));
	Z_Depth = Z_Depth * -1;

	/* Do another calculation for clip planes */
	/* Take biggest part dimension and use it to size the planes */
	if ((extent_pos.x > extent_pos.y) && (extent_pos.x > extent_pos.z))
		Big_Extent = extent_pos.x;
	if ((extent_pos.y > extent_pos.x) && (extent_pos.y > extent_pos.z))
		Big_Extent = extent_pos.y;
	if ((extent_pos.z > extent_pos.y) && (extent_pos.z > extent_pos.x))
		Big_Extent = extent_pos.z;

	/*printf("extent_pos.x: %f\n", extent_pos.x);
	printf("extent_pos.y: %f\n", extent_pos.y);
	printf("extent_pos.z: %f\n", extent_pos.z);
	printf("extent_neg.x: %f\n", extent_neg.x);
	printf("extent_neg.y: %f\n", extent_neg.y);
	printf("extent_neg.z: %f\n", extent_neg.z);*/
	printf("Z_Depth: %f\n", Z_Depth);
	printf("Big_Extent: %f\n", Big_Extent);
}
