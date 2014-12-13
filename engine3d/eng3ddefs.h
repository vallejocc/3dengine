#ifndef __ENG3DDEFS_H__
#define __ENG3DDEFS_H__

//types

typedef unsigned char      UC8;
typedef unsigned short     US16;
typedef unsigned long      UL32;
typedef unsigned __int64   UQ64;
typedef signed char        SC8;
typedef signed short       SS16;
typedef signed long        SL32;
typedef signed __int64     SQ64;
typedef UC8                WABOOL;
typedef unsigned char *    MEMBLOCK;
#ifndef _BUILDING_64_
typedef unsigned long      MEMSZ;
#else
typedef unsigned __int64   MEMSZ;
#endif

//errors

typedef int ENG3DERR;

#define ENG3DERR_NO_ERROR 0
#define ENG3DERR_INITIALIZATION_ERROR -1
#define ENG3DERR_MEMORY_ERROR -2

//lists

typedef struct _ENG3D_LIST_ENTRY {
   struct _ENG3D_LIST_ENTRY *Flink;
   struct _ENG3D_LIST_ENTRY *Blink;
} ENG3D_LIST_ENTRY, *PENG3D_LIST_ENTRY;

#define Eng3dPostInsertEntry(a,b) \
  {(b)->Flink=(a)->Flink; \
   (b)->Blink=(a); \
   (a)->Flink=(b); \
   (b)->Flink->Blink=(b);}
  
#define Eng3dPreInsertEntry(a,b) \
  {(b)->Blink=(a)->Blink; \
   (b)->Flink=(a); \
   (a)->Blink=(b); \
   (b)->Blink->Flink=(b);}

#define Eng3dExtractEntry(a) \
  {(a)->Blink->Flink=(a)->Flink; \
   (a)->Flink->Blink=(a)->Blink;}

#define Eng3dInitList(a) \
  {(a)->Flink=(a); \
  (a)->Blink=(a);}

#define Eng3dIsListEmpty(a) \
  ((a)->Flink==(a))

#define ENG3D_FIELD_OFFSET(type,field) \
  ((UL32)(MEMSZ)&(((type *)0)->field))

#define ENG3D_BASE_FROM_FIELD(type, field, fieldptr) \
( ( type * ) ( ( (UC8*) fieldptr )-ENG3D_FIELD_OFFSET( type, field ) ) )

#endif