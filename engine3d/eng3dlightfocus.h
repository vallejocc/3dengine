#ifndef __ENG3DLIGHTFOCUS_H__
#define __ENG3DLIGHTFOCUS_H__

#include "eng3dvertex.h"

class CLightFocus
{

public:

  CLightFocus(float * _ambient,float * _diffuse,float * _specular,TVertex * _Orig, TVertex * _Dest,unsigned int _Type);
  ~CLightFocus();
  void Update();

  float ambient[4];
  float diffuse[4];
  float specular[4];
  float position[4];

  unsigned int Type; //1=puntual 0=Direccional
  
  TVertex Orig;
  TVertex Dest;

  unsigned int ApplyCam;
};


#endif