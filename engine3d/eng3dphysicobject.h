#ifndef __ENG3DPHYSICOBJECT_H__
#define __ENG3DPHYSICOBJECT_H__

#include "eng3dsolidobject.h"
#include "eng3ddefs.h"

class C3DPhysicObject : public C3DSolidObject
{

public:

  C3DPhysicObject();
  ~C3DPhysicObject();
  void UpdateCenterOfMass();
  
  TVertex CenterOfMass;

private:

  ENG3D_LIST_ENTRY ZMass;
  ENG3D_LIST_ENTRY XMass;
  ENG3D_LIST_ENTRY YMass;
  unsigned int AddBoxesInsideToLists(C3DBox * Box);

protected:

  void VertexesChangesCallbackC3DPhysicObject(TVERTEXES_CHANGE_ID ChangeId,
                                             TVertex * ChangedVertex,
                                             void * ChangeParams);

  virtual void VertexesChangesCallback(TVERTEXES_CHANGE_ID ChangeId,
                                     TVertex * ChangedVertex,
                                     void * ChangeParams);
};

#endif