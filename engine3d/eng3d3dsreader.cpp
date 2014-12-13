#include "eng3d3dsreader.h"
#include <stdio.h>
#include <string.h>

void C3D3dsReader::AnalyseMapChunk(FILE * f, MATERIAL_MAP * map,unsigned int chunklen)
{
  unsigned short subchunkid;
  unsigned int subchunklen;
  unsigned int endpos=ftell(f)+chunklen-6;

  while(1)
  {
    if(ftell(f)==endpos)
      break;

    if(!fread(&subchunkid,2,1,f) || 
    !fread(&subchunklen,4,1,f))
      break;

    switch(subchunkid)
    {
      case 0xA300: //material's mapfile. We find this id after a 0xA200 (material's diffuse)
      {  
        char c;
        unsigned int i=0;
        unsigned int curpostemp=ftell(f);
        
        do
        {
          i++;
          fread(&c,1,1,f);
        }while(c!='\0');
        
        map->filepath=new char[i];
        
        fseek(f,curpostemp,SEEK_SET);

        i=0;
        do
        {
          fread(&c,1,1,f);
          map->filepath[i]=c;
          i++;
        }while(c!='\0');

        fseek(f,curpostemp+subchunklen-6,SEEK_SET);
      }
      break;

      case 0xA351: //material's map options.
      fseek(f, subchunklen-6, SEEK_CUR);
      break;

      case 0xA353: //material's map filtering blur.
      fseek(f, subchunklen-6, SEEK_CUR);
      break;

      case 0xA354: //material's map uscale.
      fseek(f, subchunklen-6, SEEK_CUR);
      break;

      case 0xA356: //material's map vscale.
      fseek(f, subchunklen-6, SEEK_CUR);
      break;

      case 0xA358: //material's map uoffset.
      fseek(f, subchunklen-6, SEEK_CUR);
      break;

      case 0xA35A: //material's map voffset.
      fseek(f, subchunklen-6, SEEK_CUR);
      break;

      case 0xA35C: //material's map rotation.
      fseek(f, subchunklen-6, SEEK_CUR);
      break;

      default:
      fseek(f, subchunklen-6, SEEK_CUR);
      break;
    };
  }
}

C3D3dsReader::C3D3dsReader(char * Path3ds)
{
  unsigned short chunkid;
  unsigned int chunklen;

  Eng3dInitList(&MaterialsListHead);
  Eng3dInitList(&ObjectsListHead);

  FILE * f = fopen(Path3ds,"rb");

  if(!f)
    return;

  fseek(f,0,SEEK_SET);

  while(1)
  {
    if(!fread(&chunkid,2,1,f) || 
       !fread(&chunklen,4,1,f))
       break;

    switch(chunkid)
    {
      //----------------- MAIN3DS -----------------
			// Description: Main chunk, contains all the other chunks
			// Chunk ID: 4d4d 
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x4d4d: 
			break;    

			//----------------- EDIT3DS -----------------
			// Description: 3D Editor chunk, objects layout info 
			// Chunk ID: 3d3d (hex)
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
			case 0x3d3d:
			break;

			//----------------- MATERIALS -----------------
			// Description: Materials chunk
			// Chunk ID: afff (hex)
			// Chunk Lenght: 0 + sub chunks
			//-------------------------------------------
      case 0xafff:
      {
        unsigned int initpos;
        unsigned short subchunkid;
        unsigned int subchunklen;
        unsigned int temp;

        tMaterialInfo3ds * NewMaterial=new tMaterialInfo3ds;
        memset(NewMaterial,0,sizeof(tMaterialInfo3ds));
        Eng3dPostInsertEntry(&MaterialsListHead,&NewMaterial->Next);

        initpos=ftell(f);
        
        while(1)
        {
          if(!fread(&subchunkid,2,1,f) || 
          !fread(&subchunklen,4,1,f))
            break;
          
          switch(subchunkid)
          {
            case 0xA000: //material's name
            {  
              char c;
              unsigned int i=0;
              unsigned int curpostemp=ftell(f);
              
              do
              {
                i++;
                fread(&c,1,1,f);
              }while(c!='\0');
              
              NewMaterial->name=new char[i];
              
              fseek(f,curpostemp,SEEK_SET);

              i=0;
              do
              {
                fread(&c,1,1,f);
                NewMaterial->name[i]=c;
                i++;
              }while(c!='\0');

              fseek(f,curpostemp+subchunklen-6,SEEK_SET);
            }
            break;

            case 0xA010: //material's ambient (color)
            fseek(f,2+4,SEEK_CUR);
            fread(NewMaterial->AmbientColorRgba.rgba,1,3,f);
            fseek(f,subchunklen-6-9,SEEK_CUR);
            NewMaterial->AmbientColorOn=1;
            break;

            case 0xA020: //material's diffuse (color)
            fseek(f,2+4,SEEK_CUR);
            fread(NewMaterial->DiffuseColorRgba.rgba,1,3,f);
            fseek(f,subchunklen-6-9,SEEK_CUR);
            NewMaterial->DiffuseColorOn=1;
            break;

            case 0xA030: //material's specular (color)
            fseek(f,2+4,SEEK_CUR);
            fread(NewMaterial->SpecularColorRgba.rgba,1,3,f);
            fseek(f,subchunklen-6-9,SEEK_CUR);
            NewMaterial->SpecularColorOn=1;
            break;

            case 0xA040: //material's shininess (%)
            fseek(f,2+4,SEEK_CUR);
            fread(&NewMaterial->Shininess,1,2,f);
            fseek(f,subchunklen-6-8,SEEK_CUR);
            break;

            case 0xA041: //material's ShinStrength (%)
            fseek(f,2+4,SEEK_CUR);
            fread(&NewMaterial->ShinStrength,1,2,f);
            fseek(f,subchunklen-6-8,SEEK_CUR);
            break;

            case 0xA050: //material's transparency (%)
            fseek(f,2+4,SEEK_CUR);
            fread(&NewMaterial->Transparency,1,2,f);
            fseek(f,subchunklen-6-8,SEEK_CUR);
            break;

            case 0xA052: //material's transparency fall off (%)
            fseek(f,2+4,SEEK_CUR);
            fread(&NewMaterial->TransparencyFallOff,1,2,f);
            fseek(f,subchunklen-6-8,SEEK_CUR);
            break;

            case 0xA053: //material's reflect blur (%)
            fseek(f,2+4,SEEK_CUR);
            fread(&NewMaterial->ReflectBlur,1,2,f);
            fseek(f,subchunklen-6-8,SEEK_CUR);
            break;

            case 0xA080: //material's self-illum color (color)
            fseek(f,2+4,SEEK_CUR);
            fread(NewMaterial->SelfIllumColorRgba.rgba,1,3,f);
            fseek(f,subchunklen-6-9,SEEK_CUR);
            NewMaterial->SelfIllumColorOn=1;
            break;

            case 0xA084: //material's self-illum (%)
            fseek(f,2+4,SEEK_CUR);
            fread(&NewMaterial->SelfIllumPercent,1,2,f);
            fseek(f,subchunklen-6-8,SEEK_CUR);
            break;

            case 0xA200: //material's map
            NewMaterial->TextureMapOn=1;
            AnalyseMapChunk(f,&NewMaterial->TextureMap,subchunklen);
            break;          

            default:
			      fseek(f, subchunklen-6, SEEK_CUR);
            break;
          }

          if((temp=ftell(f))>=initpos+chunklen-6)
            break;
        }
      }    
      break;

			//----------------- OBJECT -----------------
			// Description: Object block, info for each object
			// Chunk ID: 4000 (hex)
			// Chunk Lenght: len(object name) + sub chunks
			//-------------------------------------------
      case 0x4000:
      {
        unsigned int initpos;
        unsigned short subchunkid;
        unsigned int subchunklen;
        unsigned int temp;
        unsigned int totalmaterials=0;
        
        {
          ENG3D_LIST_ENTRY * temp=NULL;
          for(temp=MaterialsListHead.Flink;temp!=&MaterialsListHead;temp=temp->Flink)
            totalmaterials++;
        }

        tObjectInfo3ds * NewObject=new tObjectInfo3ds;
        NewObject->bTransparent=0;
        NewObject->nMaterials=0;
        if(totalmaterials)
        {
          NewObject->Materials=new tMaterialInfo3ds*[totalmaterials];
          NewObject->nMaterialFaces=new unsigned short[totalmaterials];
          NewObject->MaterialFaces=new unsigned short*[totalmaterials];
        }
        else
        {
          NewObject->Materials=NULL;
          NewObject->MaterialFaces=NULL;
          NewObject->nMaterialFaces=NULL;
        }
        NewObject->FaceMaterial=NULL;
        NewObject->SmoothingGroups=NULL;
        NewObject->MapCoords=NULL;
        NewObject->name=NULL;
        NewObject->ObjectTriangles=NULL;
        NewObject->ObjectVertexes=NULL;
        NewObject->nObjectVertexes=0;
        NewObject->nObjectTriangles=0;
        Eng3dPostInsertEntry(&ObjectsListHead,&NewObject->Next);
        initpos=ftell(f);
        
        {
          char c;
          unsigned int i=0;
          unsigned int curpostemp=ftell(f);
          
          do
          {
            i++;
            fread(&c,1,1,f);
          }while(c!='\0');
          
          NewObject->name=new char[i];
          
          fseek(f,curpostemp,SEEK_SET);

          i=0;
          do
          {
            fread(&c,1,1,f);
            NewObject->name[i]=c;
            i++;
          }while(c!='\0');
        }

        while(1)
        {
          if(!fread(&subchunkid,2,1,f) || 
          !fread(&subchunklen,4,1,f))
            break;
          
          switch(subchunkid)
          {
            case 0x4100:
              break;

            case 0x4110: //object's vertexes
            {
              unsigned int i;
              unsigned short tempushort;
              fread(&tempushort,2,1,f);
              NewObject->nObjectVertexes=tempushort;
              NewObject->ObjectVertexes=new TVertex[NewObject->nObjectVertexes];
              for(i=0;i<NewObject->nObjectVertexes;i++)
              {
                fread(&NewObject->ObjectVertexes[i].x,sizeof(float),1,f);
                fread(&NewObject->ObjectVertexes[i].y,sizeof(float),1,f);
                fread(&NewObject->ObjectVertexes[i].z,sizeof(float),1,f);
              }
            }
            break;
            
            case 0x4120: //object's faces
            {
              unsigned int i;
              unsigned short tempushort;
              fread(&tempushort,2,1,f);
              NewObject->nObjectTriangles=tempushort;
              NewObject->ObjectTriangles=new TTriangle[NewObject->nObjectTriangles];
              for(i=0;i<NewObject->nObjectTriangles;i++)
              {
                fread(&tempushort,sizeof(short),1,f);NewObject->ObjectTriangles[i].a=tempushort;
                fread(&tempushort,sizeof(short),1,f);NewObject->ObjectTriangles[i].b=tempushort;
                fread(&tempushort,sizeof(short),1,f);NewObject->ObjectTriangles[i].c=tempushort;
                //fread(&tempushort,sizeof(short),1,f);NewObject->ObjectTriangles[i].FaceFlags=tempushort;
                fseek(f,sizeof(short),SEEK_CUR);
              }
            }
            break;

            case 0x4130: //object's material
            {
              char * MaterialName;

              {  
                char c;
                unsigned int i=0;
                unsigned int curpostemp=ftell(f);
                
                do
                {
                  i++;
                  fread(&c,1,1,f);
                }while(c!='\0');
                
                MaterialName=new char[i];
                
                fseek(f,curpostemp,SEEK_SET);

                i=0;
                do
                {
                  fread(&c,1,1,f);
                  MaterialName[i]=c;
                  i++;
                }while(c!='\0');
              }

              ENG3D_LIST_ENTRY * temp=NULL;

              for(temp=MaterialsListHead.Flink;temp!=&MaterialsListHead;temp=temp->Flink)
              {
                unsigned int i=0;
                unsigned int bDif=0;
                tMaterialInfo3ds * TempMaterial=ENG3D_BASE_FROM_FIELD(tMaterialInfo3ds,Next,temp);
                for(i=0;TempMaterial->name[i]&&MaterialName[i];i++)
                {
                  if(TempMaterial->name[i]!=MaterialName[i])
                  {
                    bDif=1;
                    break;
                  }
                }
                
                if(TempMaterial->name[i]!=MaterialName[i])
                  bDif=1;
                
                if(!bDif)
                {
                  fread(&(NewObject->nMaterialFaces[NewObject->nMaterials]=0),2,1,f);
                  NewObject->MaterialFaces[NewObject->nMaterials]=
                    new unsigned short[NewObject->nMaterialFaces[NewObject->nMaterials]];
                  for(i=0;i<NewObject->nMaterialFaces[NewObject->nMaterials];i++)
                    fread(&(NewObject->MaterialFaces[NewObject->nMaterials][i]=0),2,1,f);
                  NewObject->Materials[NewObject->nMaterials]=TempMaterial;
                  NewObject->nMaterials++;
                  if(TempMaterial->Transparency)
                    NewObject->bTransparent=1;
                  break;
                }
              }
              
              delete MaterialName;
            }

            break;

            case 0x4140: //object's uv
            {
              unsigned int i;
              unsigned short tempushort;
              fread(&tempushort,2,1,f);
              NewObject->nMapCoords=tempushort;
              NewObject->MapCoords=new TTextureMapCoord[NewObject->nMapCoords];
              for(i=0;i<NewObject->nMapCoords;i++)
              {
                fread(&NewObject->MapCoords[i].u,sizeof(float),1,f);
                fread(&NewObject->MapCoords[i].v,sizeof(float),1,f);
              }
            }
            break;

            case 0x4150: //object's smoothing groups
            {
              NewObject->SmoothingGroups=new unsigned int [NewObject->nObjectTriangles];
              unsigned int i=0;
              for(i=0;i<NewObject->nObjectTriangles;i++)
                fread(&NewObject->SmoothingGroups[i],4,1,f);
            }
            break;

            case 0x4160: //object's meshmatrix
            fseek(f, subchunklen-6, SEEK_CUR);
            break;

            case 0x4165: //object's default color
            fseek(f, subchunklen-6, SEEK_CUR);
            break;  

            default:
            fseek(f, subchunklen-6, SEEK_CUR);
            break;
          }

          if((temp=ftell(f))>=initpos+chunklen-6)
            break;
        }
      
        if(NewObject->nMaterialFaces && 
           NewObject->MaterialFaces && 
           NewObject->nMaterials &&
           NewObject->nObjectTriangles)
        {
          unsigned int i,j;
          
          NewObject->FaceMaterial = new unsigned int [NewObject->nObjectTriangles];

          //note here we are initializing the material for all faces to zero 
          //for avoiding problems, but if we dont find the material for a face 
          //in the 3ds file we should mark it in any way (0xffffffff or other thing)
          //and call opengl in the apropiated way
          for(i=0;i<NewObject->nObjectTriangles;i++)
            NewObject->FaceMaterial[i]=0;

          for(i=0;i<NewObject->nMaterials;i++)
          {
            for(j=0;j<NewObject->nMaterialFaces[i];j++)
            {
              NewObject->FaceMaterial[NewObject->MaterialFaces[i][j]]=i;
            }
          }
        }
      }   
      break;

			//----------- Skip unknow chunks ------------
			//We need to skip all the chunks that currently we don't use
			//We use the chunk lenght information to set the file pointer
			//to the same level next chunk
			//-------------------------------------------
			default:
			fseek(f, chunklen-6, SEEK_CUR);
      break;
    }
  }

  fclose(f);
}

C3D3dsReader::~C3D3dsReader()
{

}
