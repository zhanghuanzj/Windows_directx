#ifndef SPRITE_H
#define SPRITE_H

#include "MyDirectX.h"

class Sprite
{
public:
	Sprite();
	~Sprite();
	RECT getRect()const;
	bool collision(Sprite& sprite2)const;
	bool collision_R(Sprite& sprite2)const;
	float x,y;
	int frame, columns;
	int width, height;
	float scaling, rotation;
	int startframe, endframe;
	int starttime, delay;
	int direction;
	float velx, vely;
	bool istouched;
	D3DCOLOR color;

private:

};


#endif
