#pragma once
#include <d3d9.h>
#include <d3dx9.h>

class Texture
{
public:
	static Texture Instance(LPDIRECT3DDEVICE9 dev)
	{
		static Texture texture(dev);
		return texture;
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
private:
	LPDIRECT3DTEXTURE9 snow;
	LPDIRECT3DTEXTURE9 black;
	LPDIRECT3DTEXTURE9 orange;
	LPDIRECT3DTEXTURE9 tree;
	LPDIRECT3DTEXTURE9 box;

	Texture(LPDIRECT3DDEVICE9 dev)
	{
		D3DXCreateTextureFromFile(dev, "Textures\\snow.jpg", &snow);
		D3DXCreateTextureFromFile(dev, "Textures\\eye.jpg", &black);
		D3DXCreateTextureFromFile(dev, "Textures\\nose.jpg", &orange);
		D3DXCreateTextureFromFile(dev, "Textures\\tree.jpg", &tree);
		D3DXCreateTextureFromFile(dev, "Textures\\box.bmp", &box);
	}
};
