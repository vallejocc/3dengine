#ifndef __ENG3DAOUTDRAWWORLD_H__
#define __ENG3DAOUTDRAWWORLD_H__

#include "eng3dworld.h"
#include "eng3dopengl.h"

class C3DAutodrawWorld : public C3DOpenglGlut, public C3DWorld
{

public:

  ENG3DERR AddObjectToScene(C3DObject * Object);
  ENG3DERR AddAllWorldObjectsToScene();
};

#endif