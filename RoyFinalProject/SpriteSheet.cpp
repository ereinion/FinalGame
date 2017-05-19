//File: Sprite Sheet.h
//Fuunction: Fleshes out the functions to be used by the player
#include "SpriteSheet.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

Sprite::Sprite()
{
	image=NULL;
	imagehurt = NULL;
	good = NULL;
}
//Function: Constructs the sprite
//Parameters: none
//Return: none

Sprite::~Sprite()
{
	al_destroy_bitmap(image);
	al_destroy_bitmap(imagehurt);
	al_destroy_sample(good);
}
//Function: deconstructs the sprite
//Parameters: none
//Return: none

void Sprite::InitSprites(int width, int height)
{
	x = 800; 
	y = 96;
	//starting positions
	health = 250;
	points = 0;
	//starting overarching variables

	currFlower = 0;
	level1status = 0;
	//level 1 variables

	oracle1 = false;
	oracle2 = false;
	oracle3 = false;
	leyline = 0;
	level2mess = 0;
	//level 2 variables

	hint = 0;
	//level 3 variable

	maxFrame = 11;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 32;
	frameHeight = 32;
	animationColumns = 3;
	//sprite sheet data
	image = al_load_bitmap("character1.png");
	imagehurt = al_load_bitmap("character1sick.png");
	//load the two sprites
	good = al_load_sample("up.wav");
	//load the sound
}
//Function: Initializes the sprite sheet. Loads and stores it based on our selected player representation
//Parameters: the width and height of the screen
//Return: none

void Sprite::UpdateSprites(int width, int height, int dir)
{
	int oldx = x;
	int oldy = y;

	if (dir == 0){ //up
		y -= 2;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > 11 || curFrame <9)
				curFrame = 9;
		}
	}

	else if (dir == 1){ //down
		y += 2;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > 2)
				curFrame = 0;
			
		}
	}
	
	else if (dir == 2) //left key
	{
		x -= 2;
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > 5 || curFrame<3)
				curFrame = 3;
		}
	}

	else if(dir == 3){ //right key
		x += 2; 
		if (++frameCount > frameDelay)
		{
			frameCount = 0;
			if (++curFrame > 8 || curFrame <6)
				curFrame = 6;
		}
	} 

		
	//check for collide with foreground tiles
	if (dir == 0)
	{
		if (collided(x, y)) { //collision detection up
			x = oldx; 
			y = oldy + 2;
		}

	}

	else if (dir == 1)
	{
		if (collided(x, y + frameHeight)) { //collision detection down
			x = oldx; 
			y = oldy -2;
		}

	}

	else if (dir ==2)
	{ 
		if (collided(x , y + frameHeight)) { //collision detection to the left. Y value allows the illusion of depth
			x = oldx +2; 
			y= oldy;
		}

	}
	else if (dir ==3)
	{ 
		if (collided(x + frameWidth, y + frameHeight)) { //collision detection to the right. Y value allows the illusion of depth
			x = oldx -2; 
			y= oldy;
		}
	}
}
//Function: Based on the direction, loads a different portion of our sprite sheet. Also has collision detection
//Parameters: width and height of the screen. the direction variable
//Return: none

void Sprite::CollisionTriggerBlock()
{
	switch (triggerValue(x + frameWidth / 2, y + frameHeight + 5))
	{
	case 2:
		if (currFlower == 5)
		{
			points++;
			currFlower = 0;
			level1status = 0;
			al_play_sample(good, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		}	
		break;

	case 3:
		if (currFlower == 6)
		{
			points++;
			currFlower = 0;
			level1status = 0;
			al_play_sample(good, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
		break;
	case 4:
		if (currFlower == 7)
		{
			points++;
			currFlower = 0;
			level1status = 0;
			al_play_sample(good, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
		break;

	case 5:
		if (currFlower != 5)
			currFlower = 5;
		level1status = 1;
		break;
	case 6:
		if (currFlower != 6)
			currFlower = 6;
		level1status = 2;
		break;
	case 7:
		if (currFlower != 7)
			currFlower = 7;
		level1status = 3;
		break;

	case 10:
		if (oracle1  && oracle2 && oracle3)
		{
			points = 3;
			leyline = 4;
			level2mess = 0;
		}
		break;

	case 12:
		level2mess = 1;
		break;

	case 13:
		level2mess = 2;
		break;

	case 14:
		level2mess = 3;
		break;

	case 15:
		if (!oracle1)
		{
			oracle1=true;
			leyline++;
			al_play_sample(good, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
		break;
	case 16:
		if (!oracle2)
		{
			oracle2 = true;
			leyline++;
			al_play_sample(good, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
		break;
	case 17:
		if (!oracle3)
		{
			oracle3 = true;
			leyline++;
			al_play_sample(good, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		}
		break;

	case 20:
		health --;
		break;
	case 25:
		hint = 1;
		break;
	case 26:
		hint = 2;
		break;
	case 27:
		hint = 3;
		break;
	case 28:
		hint = 4;
		break;
	case 29:
		hint = 5;
		break;
	case 30:
		hint = 6;
		break;
	case 32:
		hint = 0;
		break;
	case 35:
		points = 3;
		hint = 0;
		break;
	}

}
//Function: Checks upon collision whether the collided block has user data, then performs a different task based on what data is present
//Parameters: none
//Return: none

void Sprite::DrawSprites(int xoffset, int yoffset)
{
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = (curFrame / animationColumns) * frameHeight;

	if (health>100)
		al_draw_bitmap_region(image, fx, fy, frameWidth,frameHeight, x-xoffset, y-yoffset, 0);
	else
		al_draw_bitmap_region(imagehurt, fx, fy, frameWidth, frameHeight, x - xoffset, y - yoffset, 0);
}
//Function: Uses the formula that has been around forever to display spritesheets. If the player's health is below 100 then shows a "sick" sprite
//Parameters: the x and y offset as ints
//Return: none

void Sprite::resetPos()
{
	x = 800; //starting positions
	y = 96;
	points = 0;
	health = 250;
}
//Function: Is called when levels are switched. Resets the player's position back to the initial, also resets points and hp per level
//Parameters: None
//Return: None

void Sprite::setLevel1Status(int status)
{
	level1status = status;
}
//Function: Sets the variable level1status for checking in the Trigger Block function
//Parameters: Int of the status we want to set
//Return: None

