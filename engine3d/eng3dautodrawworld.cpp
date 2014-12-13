#include "eng3dautodrawworld.h"

ENG3DERR C3DAutodrawWorld::AddObjectToScene(C3DObject * Object)
{
  Object->UpdateWorldObjectVertexes();
  AddTrianglesToScene(Object);

  return ENG3DERR_NO_ERROR;
}

ENG3DERR C3DAutodrawWorld::AddAllWorldObjectsToScene()
{
  TObjectList * Temp = ENG3D_BASE_FROM_FIELD(TObjectList,Link,ObjectListHead.Flink);

  while((void*)Temp != (void*)&ObjectListHead)
  {
    AddObjectToScene(Temp->Object);
    Temp = ENG3D_BASE_FROM_FIELD(TObjectList,Link,Temp->Link.Flink);
  }
  
  return ENG3DERR_NO_ERROR;
}