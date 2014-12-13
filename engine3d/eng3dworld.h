#ifndef __ENG3DWORLD_H__
#define __ENG3DWORLD_H__

#include "eng3dsolidobject.h"
#include "eng3ddefs.h"

typedef struct
{
  ENG3D_LIST_ENTRY Link;
  C3DObject * Object;
} TObjectList;

class C3DWorld
{
public:

  C3DWorld();
  ~C3DWorld();

  ENG3DERR Initialize();
  ENG3DERR AddObject(C3DObject *);
  void RemoveObject(C3DObject *);

  ENG3D_LIST_ENTRY ObjectListHead;
};

#endif