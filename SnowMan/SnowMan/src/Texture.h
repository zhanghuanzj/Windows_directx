#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include <string>
#include "CommonUtil.h"
using namespace std;

class Texture
{
public:
	static Texture* Instance(LPDIRECT3DDEVICE9 dev)
	{
		static Texture *pTexture = new Texture(dev);
		return pTexture;
	}

	LPDIRECT3DTEXTURE9 get_black()
	{
		return black;
	}

	LPDIRECT3DTEXTURE9 get_orange()
	{
		return orange;
	}

	LPDIRECT3DTEXTURE9 get_tree()
	{
		return tree;
	}

	LPDIRECT3DTEXTURE9 get_snow()
	{
		return snow;
	}

	LPDIRECT3DTEXTURE9 get_box()
	{
		return box;
	}

	LPDIRECT3DTEXTURE9 get_texture(const string name)
	{
		if (textures.find(name) == textures.end()) return nullptr;
		else return textures[name];
	}

private:
	LPDIRECT3DTEXTURE9 snow;
	LPDIRECT3DTEXTURE9 black;
	LPDIRECT3DTEXTURE9 orange;
	LPDIRECT3DTEXTURE9 tree;
	LPDIRECT3DTEXTURE9 box;
	map<string, LPDIRECT3DTEXTURE9> textures;
	Texture(LPDIRECT3DDEVICE9 dev)
	{
		D3DXCreateTextureFromFile(dev, "Textures\\snow.jpg", &snow);
		D3DXCreateTextureFromFile(dev, "Textures\\eye.jpg", &black);
		D3DXCreateTextureFromFile(dev, "Textures\\nose.jpg", &orange);
		D3DXCreateTextureFromFile(dev, "Textures\\tree.jpg", &tree);
		D3DXCreateTextureFromFile(dev, "Textures\\box.bmp", &box);
		string paths[14] = { "20131029040153281154.jpg",
							"20140218114649890764.jpg",
							"20140218114649890566.jpg",
							"20110707060413578463.jpg",
							"20110707060413578842.jpg",
							"20110707060413578979.JPG",
							"20110707060413578902.jpg",
							"20110707060413578338.jpg",
							"20110707060413578666.jpg",
							"20110707060413578732.jpg",
							"Map__11.bmp",
							"20110707060413578308.jpg",
							"20110707060413578660.jpg",
							"Map__88.bmp" };
		string dir = "Textures\\";
		for (int i = 0; i < 14; ++i)
		{
			LPDIRECT3DTEXTURE9 tex;
			string path = dir + paths[i];
			D3DXCreateTextureFromFile(dev, path.c_str(), &tex);
			textures[paths[i]] = tex;
		}
	}

public:
	~Texture()
	{
		SAFE_RELEASE(snow);
		SAFE_RELEASE(black);
		SAFE_RELEASE(orange);
		SAFE_RELEASE(tree);
		SAFE_RELEASE(box);
		for (auto it = textures.begin(); it != textures.end(); ++it)
		{
			SAFE_RELEASE(it->second);
		}
	}


};
