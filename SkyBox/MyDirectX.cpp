#include "MyDirectX.h"
#include "DirectSound.h"


CSoundManager *dsound = NULL;
//Direct3D objects
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;
LPD3DXSPRITE spriteobj = NULL;

//DirectInput objects
LPDIRECTINPUT8 dinput;
LPDIRECTINPUTDEVICE8 dimouse;
LPDIRECTINPUTDEVICE8 dikeyBoard;
DIMOUSESTATE mouse_state;
XINPUT_GAMEPAD controllers[4];


//Direct3D initialization
bool Direct3D_Init(HWND hwnd,int width,int height,bool fullScreen)
{
	//设备接口初始化
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d==NULL)
	{
		MessageBox(hwnd,"Error initializing Direct3D","Error",MB_OK);
		return false;
	}
	
	D3DCAPS9 caps; int vp = 0;
	d3d->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps ) ;

	

	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; 
	//Direct3D表示方式参数设定
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp,sizeof(d3dpp));
	/*d3dpp.Windowed = !fullScreen;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = SCREENW;
	d3dpp.BackBufferHeight = SCREENH;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.EnableAutoDepthStencil=TRUE;
	d3dpp.AutoDepthStencilFormat=D3DFMT_D16;*/
	d3dpp.BackBufferWidth            = SCREENW;
	d3dpp.BackBufferHeight           = SCREENH;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 2;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = true;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;
	//创建Direct3D设备接口
	d3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,hwnd,vp,&d3dpp,&d3ddev);
	if (d3ddev==NULL)
	{
		MessageBox(hwnd,"Error creating Direct3D device","Error",MB_OK);
		return false;
	}

	//获取backbuffer&surface
	d3ddev->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&backbuffer);

	//精灵初始化
	D3DXCreateSprite(d3ddev,&spriteobj);
	
	return true;
}

//Direct shutDown
void Direct3D_ShutDown()
{
	if (d3ddev)
	{
		d3ddev->Release();
		d3ddev=NULL;
	}
	if (d3d)
	{
		d3d->Release();
		d3d=NULL;
	}
	if (spriteobj)
	{
		spriteobj->Release();
		spriteobj = NULL;
	}
}

//Draw surface to the backbuffer
void DrawSurface(LPDIRECT3DSURFACE9 dest,float x,float y,LPDIRECT3DSURFACE9 src)
{
	D3DSURFACE_DESC desc;
	src->GetDesc(&desc);

	RECT src_rect = {0,0,(long)desc.Width,(long)desc.Height};
	RECT dest_rect = {(long)x,(long)y,(long)x+desc.Width,(long)y+desc.Height};

	d3ddev->StretchRect(src,&src_rect,dest,&dest_rect,D3DTEXF_NONE);
}

//Draw the spriteframe to the texture
void DrawSpriteFrame2Texture(LPDIRECT3DTEXTURE9 texture,int destx,int dexty,int framenum,int frameW,int frameH,int columns)
{
	D3DXVECTOR3 position((float)destx,(float)dexty,0);
	D3DCOLOR white = D3DCOLOR_XRGB(255,255,255);
	RECT rect;
	rect.left = (framenum%columns)*frameW;
	rect.top = (framenum/columns)*frameH;
	rect.right = rect.left+frameW;
	rect.bottom = rect.top+frameH;
	spriteobj->Draw(texture,&rect,NULL,&position,white);
}

//Draw the spriteframe to the texture with transformation
void DrawSpriteTransform(LPDIRECT3DTEXTURE9 texture,int x,int y,int width,int height,int frame,int columns,float rotation,float scaling,D3DCOLOR color)
{
	D3DXVECTOR2 scale(scaling,scaling);
	D3DXVECTOR2 trans(x,y);
	D3DXVECTOR2 center((float)scaling*width/2,(float)scaling*height/2);
	D3DXMATRIX mat;
	D3DXMatrixTransformation2D(&mat,NULL,0,&scale,&center,rotation,&trans);
	spriteobj->SetTransform(&mat);

	int fx = (frame%columns)*width;
	int fy = (frame/columns)*height;
	RECT rect = {fx,fy,fx+width,fy+height};
	spriteobj->Draw(texture,&rect,NULL,NULL,color);
}

//Update the next frame of the sprite
void UpdateNextFrameNumber(int &frameNumber,int startFrame,int endFrame,int direction,int &startTime,int delay)
{
	if ((int)GetTickCount()>startTime+delay)
	{
		startTime = GetTickCount();
		frameNumber += direction;
		if (frameNumber>endFrame)
		{
			frameNumber = startFrame;
		}
		if (frameNumber<startFrame)
		{
			frameNumber = endFrame;
		}
	}
}

//print the string using the font given
void PrintFont(LPD3DXFONT font,int x,int y,string text,D3DCOLOR color)
{
	RECT rect = {x,y,0,0};
	font->DrawText(NULL,text.c_str(),text.length(),&rect,DT_CALCRECT,color);
	//font->DrawText(spriteobj,text.c_str(),text.length(),&rect,DT_LEFT,color);
}

//Load an image to the surface and return it
LPDIRECT3DSURFACE9 LoadSurface(string fileName)
{
	LPDIRECT3DSURFACE9 image = NULL;

	//get the image information
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(fileName.c_str(),&info)                    ;
	if (result!=D3D_OK)
	{
		return NULL;
	}

	//Create surface
	result = d3ddev->CreateOffscreenPlainSurface(
		info.Width,
		info.Height,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&image,
		NULL);
	if (result!=D3D_OK)
	{
		return NULL;
	}

	//Load the image to the surface
	result = D3DXLoadSurfaceFromFile(
		image,
		NULL,
		NULL,
		fileName.c_str(),
		NULL,
		D3DX_DEFAULT,
		D3DCOLOR_XRGB(0,0,0),
		NULL);
	if (result!=D3D_OK)
	{
		return NULL;
	}

	return image;
}

//Load an image to the texture and return it
LPDIRECT3DTEXTURE9 LoadTexture(string fileName,D3DCOLOR transColor)
{
	LPDIRECT3DTEXTURE9 texture = NULL;
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(fileName.c_str(),&info);
	if (result!=D3D_OK)
	{
		return NULL;
	}
	//create the new texture by loading a bitmap image file
	D3DXCreateTextureFromFileEx( 
		d3ddev,                //Direct3D device object
		fileName.c_str(),      //bitmap filename
		info.Width,            //bitmap image width
		info.Height,           //bitmap image height
		1,                     //mip-map levels (1 for no chain)
		D3DPOOL_DEFAULT,       //the type of surface (standard)
		D3DFMT_UNKNOWN,        //surface format (default)
		D3DPOOL_DEFAULT,       //memory class for the texture
		D3DX_DEFAULT,          //image filter
		D3DX_DEFAULT,          //mip filter
		transColor,            //color key for transparency
		&info,                 //bitmap file info (from loaded file)
		NULL,                  //color palette
		&texture );            //destination texture

	//make sure the bitmap textre was loaded correctly
	if (result != D3D_OK) return NULL;

	return texture;

}

//Make the font with the fontName given and the size
LPD3DXFONT MakeFont(string fontName,int size)
{
	//first just define it
	LPD3DXFONT font = NULL;
	//set the description
	D3DXFONT_DESC desc = {
		size,
		0,
		0,
		0,
		false,
		DEFAULT_CHARSET,
		OUT_TT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_PITCH,
		""
	};
	strcpy(desc.FaceName,fontName.c_str());
	//create the font
	D3DXCreateFontIndirect(d3ddev,&desc,&font);
	return font;
}

D3DXVECTOR2 GetBitmapSize(string fileName)
{
	D3DXIMAGE_INFO info;
	D3DXVECTOR2 size = D3DXVECTOR2(0.0f,0.0f);
	HRESULT result = D3DXGetImageInfoFromFile(fileName.c_str(),&info);
	if (result!=D3D_OK)
	{
		size = D3DXVECTOR2((float)info.Width,(float)info.Height);
	}
	else
	{
		size = D3DXVECTOR2((float)info.Width,(float)info.Height);
	}
	return size;
}


// Returns true if controller is plugged in
bool XInput_Controller_Found()
{
	XINPUT_CAPABILITIES caps;
	ZeroMemory(&caps, sizeof(XINPUT_CAPABILITIES));
	XInputGetCapabilities(0, XINPUT_FLAG_GAMEPAD, &caps);
	if (caps.Type != 0) return false;

	return true;
}

// Vibrates the controller
void XInput_Vibrate(int contNum, int amount)
{
	XINPUT_VIBRATION vibration;
	ZeroMemory( &vibration, sizeof(XINPUT_VIBRATION) );
	vibration.wLeftMotorSpeed = amount;
	vibration.wRightMotorSpeed = amount; 
	XInputSetState( contNum, &vibration );
}


bool DirectSound_Init(HWND hwnd)
{
	//create DirectSound manager object
	dsound = new CSoundManager();

	//initialize DirectSound
	HRESULT result;
	result = dsound->Initialize(hwnd, DSSCL_PRIORITY);
	if (result != DS_OK) return false;

	//set the primary buffer format
	result = dsound->SetPrimaryBufferFormat(2, 22050, 16);
	if (result != DS_OK) return false;

	//return success
	return true;
}

void DirectSound_ShutDown()
{
	if (dsound) delete dsound;
}

CSound *LoadSound(string filename)
{
	HRESULT result;

	//create local reference to wave data
	CSound *wave = NULL;

	//attempt to load the wave file
	char s[255];
	sprintf(s, "%s", filename.c_str());
	result = dsound->Create(&wave, s);
	if (result != DS_OK) wave = NULL;

	//return the wave
	return wave;
}

void PlaySound(CSound *sound)
{
	sound->Play();
}

void LoopSound(CSound *sound)
{
	sound->Play(0, DSBPLAY_LOOPING);
}

void StopSound(CSound *sound)
{
	sound->Stop();
}




/**
 *  Mesh loading and rendering code
 **/

void SetCamera(float posx, float posy, float posz, float lookx, float looky, float lookz)
{
    float fov = D3DX_PI / 4.0;
    float aspectRatio = SCREENW / SCREENH;
    float nearRange = 1.0;
    float farRange = 2000.0;
    D3DXVECTOR3 updir = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    D3DXVECTOR3 position = D3DXVECTOR3(posx, posy, posz);
    D3DXVECTOR3 target = D3DXVECTOR3(lookx, looky, lookz);

    //set the perspective 
    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, fov, aspectRatio, nearRange, farRange);
    d3ddev->SetTransform(D3DTS_PROJECTION, &matProj);

    //set up the camera view matrix
    D3DXMATRIX matView;
    D3DXMatrixLookAtLH(&matView, &position, &target, &updir);
    d3ddev->SetTransform(D3DTS_VIEW, &matView);
}


void SplitPath(const string& inputPath, string* pathOnly, string* filenameOnly)
{
    string fullPath( inputPath );
    replace( fullPath.begin(), fullPath.end(), '\\', '/');
    string::size_type lastSlashPos = fullPath.find_last_of('/');

    // check for there being no path element in the input
    if (lastSlashPos == string::npos)
    {
        *pathOnly="";
        *filenameOnly = fullPath;
    }
    else {
        if (pathOnly) {
            *pathOnly = fullPath.substr(0, lastSlashPos);
        }
        if (filenameOnly)
        {
            *filenameOnly = fullPath.substr( 
                lastSlashPos + 1,
                fullPath.size() - lastSlashPos - 1 );
        }
    }
}

bool DoesFileExist(const string &filename)
{
    return (_access(filename.c_str(), 0) != -1);
}

bool FindFile(string *filename)
{
    if (!filename) return false;

    //look for file using original filename and path
    if (DoesFileExist(*filename)) return true;

    //since the file was not found, try removing the path
    string pathOnly;
    string filenameOnly;
    SplitPath(*filename,&pathOnly,&filenameOnly);

    //is file found in current folder, without the path?
    if (DoesFileExist(filenameOnly))
    {
        *filename=filenameOnly;
        return true;
    }

    //not found
    return false;
}

MODEL *LoadModel(string filename)
{
    MODEL *model = (MODEL*)malloc(sizeof(MODEL));
    LPD3DXBUFFER matbuffer;
    HRESULT result;

    //load mesh from the specified file
    result = D3DXLoadMeshFromX(
        filename.c_str(),               //filename
        D3DXMESH_SYSTEMMEM,     //mesh options
        d3ddev,                 //Direct3D device
        NULL,                   //adjacency buffer
        &matbuffer,             //material buffer
        NULL,                   //special effects
        &model->material_count, //number of materials
        &model->mesh);          //resulting mesh

    if (result != D3D_OK)
    {
        MessageBox(0, "Error loading model file", APPTITLE.c_str(), 0);
        return NULL;
    }

    //extract material properties and texture names from material buffer
    LPD3DXMATERIAL d3dxMaterials = (LPD3DXMATERIAL)matbuffer->GetBufferPointer();
    model->materials = new D3DMATERIAL9[model->material_count];
    model->textures  = new LPDIRECT3DTEXTURE9[model->material_count];

    //create the materials and textures
    for(DWORD i=0; i<model->material_count; i++)
    {
        //grab the material
        model->materials[i] = d3dxMaterials[i].MatD3D;

        //set ambient color for material 
        model->materials[i].Ambient = model->materials[i].Diffuse;

        model->textures[i] = NULL;
        if (d3dxMaterials[i].pTextureFilename != NULL) 
        {
            string filename = d3dxMaterials[i].pTextureFilename;
            if( FindFile(&filename) )
            {
                result = D3DXCreateTextureFromFile(d3ddev, filename.c_str(), &model->textures[i]);
                if (result != D3D_OK) 
                {
                    MessageBox(NULL, "Could not find texture file", "Error", MB_OK);
                    return false;
                }
            }
        }
    }

    //done using material buffer
    matbuffer->Release();

    return model;
}

void DeleteModel(MODEL *model)
{
    //remove materials from memory
    if( model->materials != NULL ) 
        delete[] model->materials;

    //remove textures from memory
    if (model->textures != NULL)
    {
        for( DWORD i = 0; i < model->material_count; i++)
        {
            if (model->textures[i] != NULL)
                model->textures[i]->Release();
        }
        delete[] model->textures;
    }
    
    //remove mesh from memory
    if (model->mesh != NULL)
        model->mesh->Release();

    //remove model struct from memory
    if (model != NULL)
        free(model);
    
}


void DrawModel(MODEL *model)
{
    //any materials in this mesh?
    if (model->material_count == 0) 
    {
        model->mesh->DrawSubset(0);
    }
    else {
        //draw each mesh subset
        for( DWORD i=0; i < model->material_count; i++ )
        {
            // Set the material and texture for this subset
            d3ddev->SetMaterial( &model->materials[i] );

            if (model->textures[i]) 
            {
                if (model->textures[i]->GetType() == D3DRTYPE_TEXTURE) 
                {
                    D3DSURFACE_DESC desc;
                    model->textures[i]->GetLevelDesc(0, &desc);
                    if (desc.Width > 0) 
                    {
                        d3ddev->SetTexture( 0, model->textures[i] );
                    }
                }
            }
            
            // Draw the mesh subset
            model->mesh->DrawSubset( i );
        }
    }
}
