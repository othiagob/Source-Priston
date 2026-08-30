
#include "DXVertexDeclaration.h"
#include "Def.h"

namespace DX
{

	VertexDeclaration::VertexDeclaration(LPDIRECT3DVERTEXDECLARATION9 lpVertexDeclaration) : lpVertexDeclaration(lpVertexDeclaration)
	{
		ADDREF(lpVertexDeclaration);
	}

	VertexDeclaration::~VertexDeclaration()
	{
		RELEASE(lpVertexDeclaration);
	}

}