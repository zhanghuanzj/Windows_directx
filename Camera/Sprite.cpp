#include "Sprite.h"

Sprite::Sprite()
{
	frame = 0;
	columns = 1;
	width = height = 0;
	scaling = 1.0f;
	rotation = 0.0f;
	startframe = endframe = 0;
	direction = 1;
	starttime = delay = 0;
	velx = vely = 0.0f;
	color = D3DCOLOR_XRGB(255,255,255);
	istouched = false;
}

Sprite::~Sprite()
{

}

RECT Sprite::getRect()const
{
	RECT rect;
	rect.left = (long)x;
	rect.top = (long)y;
	rect.right = (long)x + width * scaling;
	rect.bottom = (long)y + height * scaling;
	return rect;
}

bool Sprite::collision(Sprite& sprite2)const
{
	RECT rect1 = getRect();
	RECT rect2 = sprite2.getRect();
	RECT rect;
	//未碰撞过且相撞了
	if (!sprite2.istouched&&IntersectRect(&rect,&rect1,&rect2))
	{
		sprite2.istouched = true;
		return true;
	}
	else
	{
		//碰撞了还处于碰撞状态则无效,不处于碰撞过状态且无碰撞则将碰撞过恢复
		if (!IntersectRect(&rect,&rect1,&rect2))
		{
			sprite2.istouched = false;
		}
		return false;
	}
	
}

bool Sprite::collision_R(Sprite& sprite2)const
{
	RECT rect1 = getRect();
	RECT rect2 = sprite2.getRect();
	double radius1 = width>height?width:height;
	double radius2 = sprite2.width>sprite2.height?sprite2.width:sprite2.height;
	radius1 = radius1*scaling/2;
	radius2 = radius2*sprite2.scaling/2;
	double x1 = x+width*scaling/2;
	double y1 = y+height*scaling/2;
	double x2 = sprite2.x+sprite2.width*sprite2.scaling/2;
	double y2 = sprite2.y+sprite2.height*sprite2.scaling/2;
	double distance = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
	
	//未碰撞过且相撞了
	if (!sprite2.istouched&&(distance<radius1+radius2))
	{
		sprite2.istouched = true;
		return true;
	}
	else
	{
		//碰撞了还处于碰撞状态则无效,不处于碰撞过状态且无碰撞则将碰撞过恢复
		if (!(distance<radius1+radius2))
		{
			sprite2.istouched = false;
		}
		return false;
	}
}