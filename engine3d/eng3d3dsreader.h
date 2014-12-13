#ifndef __ENG3D3DSREADER_H__
#define __ENG3D3DSREADER_H__

#include "eng3ddefs.h"
#include "eng3dpolygon.h"
#include "eng3dvertex.h"
#include "eng3dtexture.h"
#include "eng3dcolor.h"
#include <stdio.h>

typedef struct
{
  char * filepath;

}MATERIAL_MAP;

typedef struct
{
  ENG3D_LIST_ENTRY Next;
  char * name;
  
  MATERIAL_MAP TextureMap;
  unsigned int TextureMapOn;

  union
  {
    struct
    {
      unsigned char r;
      unsigned char g;
      unsigned char b;
      unsigned char a;
    };
    unsigned char rgba[4];
  }DiffuseColorRgba;

  unsigned int DiffuseColorOn;

  union
  {
    struct
    {
      unsigned char r;
      unsigned char g;
      unsigned char b;
      unsigned char a;
    };
    unsigned char rgba[4];
  }AmbientColorRgba;

  unsigned int AmbientColorOn;

  union
  {
    struct
    {
      unsigned char r;
      unsigned char g;
      unsigned char b;
      unsigned char a;
    };
    unsigned char rgba[4];
  }SpecularColorRgba;

  unsigned int SpecularColorOn;

  union
  {
    struct
    {
      unsigned char r;
      unsigned char g;
      unsigned char b;
      unsigned char a;
    };
    unsigned char rgba[4];
  }SelfIllumColorRgba;

  unsigned int SelfIllumColorOn;

  short Transparency; //transparency. Opengl transparency is (100-thisone)/100.
  short TransparencyFallOff;
  short Shininess;
  short ShinStrength;
  short SelfIllumPercent;
  short ReflectBlur;

}tMaterialInfo3ds;

typedef struct
{
  ENG3D_LIST_ENTRY Next;
  char * name;
  TTriangle * ObjectTriangles;
  unsigned int nObjectTriangles;
  TVertex * ObjectVertexes;
  unsigned int nObjectVertexes;
  unsigned int * SmoothingGroups;
  unsigned int nMapCoords;
  TTextureMapCoord * MapCoords;
  unsigned int nMaterials;
  tMaterialInfo3ds ** Materials;
  unsigned short * nMaterialFaces;
  unsigned short ** MaterialFaces; //array of array of indexes in ObjectTriangles array (faces array)
  unsigned int * FaceMaterial;
  unsigned int bTransparent;
}tObjectInfo3ds;

class C3D3dsReader
{

public:

  ENG3D_LIST_ENTRY MaterialsListHead;
  ENG3D_LIST_ENTRY ObjectsListHead;

  C3D3dsReader(char * Path3ds);
  ~C3D3dsReader();

private:

  void AnalyseMapChunk(FILE * f, MATERIAL_MAP * map,unsigned int chunklen);

};

#endif