//File: Sprite Sheet.h
//Function: Sets up the sprite sheet class for our maze runner
#ifndef SPRITEH
#define SPRITEH
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <iostream>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

using namespace std;

class Sprite
{
	friend int collided(int x, int y);
	//allows us to run collision detection from this class.
	friend int triggerValue( int x, int y );
	//allows us to check trigger values for events
public:
	Sprite();
	~Sprite();
	void InitSprites(int width, int height);
	void UpdateSprites(int width, int height, int dir); 
	void DrawSprites(int xoffset, int yoffset);
	void resetPos();
	void CollisionTriggerBlock();
	void setLevel1Status(int status);
	void setOracle();

	float getX() {return x;}
	float getY() {return y;}
	int getWidth() {return frameWidth;}
	int getHeight() {return frameHeight;}
	int getHealth(){ return health; }
	int getPoints(){ return points; }
	int getLevel1Status(){ return level1status; }
	int getOracle(){ return leyline; }
	int getLevel2mess(){ return level2mess; }
	int getHint(){ return hint; }
	//basic functions that return variables that need to be checked in Source.cpp
	void loseHealth(){ health--; }
	//basic function to dock a player's hp by 1

private:
	float x;
	float y;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationRows;
	//Sprite sheet variables

	int health;
	int points;
	//overarching game variables

	int currFlower;
	int level1status;
	// Level 1 game variables

	int leyline, level2mess;
	bool oracle1, oracle2, oracle3;
	// Level 2 game variables

	int hint;
	// Level 3 game variables

	ALLEGRO_BITMAP *image;
	ALLEGRO_BITMAP *imagehurt;
	//bitmap variables for the player

	ALLEGRO_SAMPLE *good;
	//sound variable when a task is completed
};
#endif