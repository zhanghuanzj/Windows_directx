#pragma once
#ifndef SAFE_DELETE					
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }      
#endif    

#ifndef SAFE_RELEASE			
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }  
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif

#ifndef WORLD_MATRIX
#define WORLD_MATRIX "WorldMatrix"
#endif

#ifndef VIEW_MATRIX
#define VIEW_MATRIX "ViewMatrix"
#endif

#ifndef PROJ_MATRIX
#define PROJ_MATRIX "ProjMatrix"
#endif

#ifndef TEXTURE
#define TEXTURE "ColorTexture"
#endif

#ifndef MATERIAL
#define MATERIAL "Material"
#endif

#ifndef LIGHTDIFFUSE
#define LIGHTDIFFUSE "LightDiffuse"
#endif

#ifndef LIGHTAMBIENT
#define LIGHTAMBIENT "LightAmbient"
#endif
