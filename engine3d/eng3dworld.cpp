#include "eng3dworld.h"

C3DWorld::C3DWorld()
{
  Eng3dInitList(&ObjectListHead);
}

C3DWorld::~C3DWorld()
{
  while(!Eng3dIsListEmpty(&ObjectListHead))
  {
    TObjectList * Temp = NULL;
    Temp = ENG3D_BASE_FROM_FIELD(TObjectList,Link,ObjectListHead.Flink);
    Eng3dExtractEntry(&Temp->Link);
    delete Temp;
  }
}

ENG3DERR C3DWorld::Initialize()
{
  return ENG3DERR_NO_ERROR;
}

ENG3DERR C3DWorld::AddObject(C3DObject * Object)
{
  TObjectList * ObjectNode = new TObjectList;

  //TODO: We must sort the object's list having in mind the distance to the camera (for transparencies).

  if(ObjectNode)
  {
    ENG3D_LIST_ENTRY * Temp;

    ObjectNode->Object = Object;

    if(Object->ObjectInfo3ds->bTransparent)
    {
      Eng3dPreInsertEntry(&ObjectListHead,&ObjectNode->Link);
    }
    else
    {
      Eng3dPostInsertEntry(&ObjectListHead,&ObjectNode->Link);
    }

    return ENG3DERR_NO_ERROR;
  }
  else
  {
    return ENG3DERR_MEMORY_ERROR;
  }
}

void C3DWorld::RemoveObject(C3DObject * Object)
{
  TObjectList * Temp = ENG3D_BASE_FROM_FIELD(TObjectList,Link,ObjectListHead.Flink);

  while((void*)Temp != (void*)&ObjectListHead)
  {
    if(Temp->Object==Object)
    {
      Eng3dExtractEntry(&Temp->Link);
      delete Temp;
      return;
    }

    Temp = ENG3D_BASE_FROM_FIELD(TObjectList,Link,Temp->Link.Flink);
  }
}