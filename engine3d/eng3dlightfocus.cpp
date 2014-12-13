#include "eng3dlightfocus.h"
#include "eng3dvector.h"

CLightFocus::CLightFocus(float * _ambient,float * _diffuse,float * _specular,TVertex * _Orig, TVertex * _Dest,unsigned int _Type)
{
  ambient[0]=_ambient[0];ambient[1]=_ambient[1];ambient[2]=_ambient[2];ambient[3]=_ambient[3];
  diffuse[0]=_diffuse[0];diffuse[1]=_diffuse[1];diffuse[2]=_diffuse[2];diffuse[3]=_diffuse[3];
  specular[0]=_specular[0];specular[1]=_specular[1];specular[2]=_specular[2];specular[3]=_specular[3];
  Orig.x=_Orig->x;Orig.y=_Orig->y;Orig.z=_Orig->z;
  Dest.x=_Dest->x;Dest.y=_Dest->y;Dest.z=_Dest->z;
  TVertex temp;
  float mag;
  temp.x=Dest.x-Orig.x;temp.y=Dest.y-Orig.y;temp.z=Dest.z-Orig.z;
  v_normalize(temp,mag,temp);
  position[0]=temp.x;position[1]=temp.y;position[2]=temp.z;position[3]=_Type;
  Type=_Type;
  ApplyCam=1;
}

CLightFocus::~CLightFocus()
{

}
  
void CLightFocus::Update()
{
  TVertex temp;
  float mag;
  temp.x=Dest.x-Orig.x;temp.y=Dest.y-Orig.y;temp.z=Dest.z-Orig.z;
  v_normalize(temp,mag,temp);
  position[0]=temp.x;position[1]=temp.y;position[2]=temp.z;position[3]=Type;
}