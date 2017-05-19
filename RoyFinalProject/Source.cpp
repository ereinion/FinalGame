//File: Source.cpp
//Author: Roy O'Dell
//Date: 6/16/16
//Function: Sets up the game rules, ending conditions, intro messages, game loop, displays the data & messages, plays background music and trigger music. 
//Input: Keyboard : Enter to get past the Intro. A and D toggle music. Arrow keys move the player. Esc exits the game once started
//Output: The game in an allegro display

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include "SpriteSheet.h"
#include "mappy_A5.h"
#include <iostream>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

using namespace std;

int collided(int x, int y);
int triggerValue( int x, int y ); 
void status(ALLEGRO_FONT *font, Sprite &Player);

int main(void)
{
	const int WIDTH = 600; 
	const int HEIGHT = 600;
	bool keys[] = {false, false, false, false};
	enum KEYS{UP, DOWN, LEFT, RIGHT};
	//Input Variables

	bool done = false;
	bool start = false;
	bool render = false;
	int avatarX = (WIDTH / 2) - 32;
	int gametime = 0;
	int level = 0;
	int scale = 1;
	//Game Loop Variables

	Sprite player;
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_SAMPLE *background, *winning, *losing;
	ALLEGRO_SAMPLE_ID sample1;
	//allegro variables for the game

	if(!al_init())										
		return -1;

	display = al_create_display(WIDTH, 800); //gives an area for game messages

	if (!al_install_audio())
		return -1;

	if (!al_init_acodec_addon())
		return -1;
	if (!al_reserve_samples(3))
		return -1;

	background = al_load_sample("Monody1.wav");
	winning = al_load_sample("victory.wav");
	losing = al_load_sample("over.wav");
	if (!background)
		exit(9);

	if(!display)										
		return -1;
	//General Allegro init stuff
	
	al_install_keyboard();
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	//all necessary addons

	player.InitSprites(WIDTH,HEIGHT);

	ALLEGRO_FONT *font = al_load_font("ff.ttf", 18, 0);
	ALLEGRO_FONT *introfont = al_load_font("ff.ttf", 12, 0);
	//Font for displaying messages in the game screen
	ALLEGRO_BITMAP *avatar = al_load_bitmap("avatar.png");
	int xOff = 0;
	int yOff = 0;

	if(MapLoad("Level1.fmp", 1))
		return -5;
	//Loads first map

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);

	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 50, ALLEGRO_ALIGN_LEFT, "WELCOME MORTAL");

	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 100, ALLEGRO_ALIGN_LEFT, "YOU MUST PASS THREE TESTS TO RELEASE ME");
	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 125, ALLEGRO_ALIGN_LEFT, "THE FIRST IS A TEST OF SPEED");
	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 150, ALLEGRO_ALIGN_LEFT, "COLLECT A FLOWER AND RETURN IT TO IT'S COLORED ORB");
	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 175, ALLEGRO_ALIGN_LEFT, "TRYING TO COLLECT A FLOWER WILL DROP ANY HELD");
	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 200, ALLEGRO_ALIGN_LEFT, "BE WARNED, EACH SECOND HERE DRAINS LIFE");

	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 300, ALLEGRO_ALIGN_LEFT, "THE SECOND IS A TEST OF MEMORY");
	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 325, ALLEGRO_ALIGN_LEFT, "THE THREE FLOWERS WILL GUIDE YOU TO VARIOUS LEYLINES");
	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 350, ALLEGRO_ALIGN_LEFT, "YOU MUST CROSS EVERY LEYLINE TO CONTINUE");
	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 375, ALLEGRO_ALIGN_LEFT, "RETURN TO THE MOUND WHEN ALL ARE CROSSED");
	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 400, ALLEGRO_ALIGN_LEFT, "BE WARNED, EACH SECOND HERE DRAINS LIFE");

	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 500, ALLEGRO_ALIGN_LEFT, "THE FINAL IS A TEST OF WIT");
	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 525, ALLEGRO_ALIGN_LEFT, "THE SAFE PATH WILL BE TOLD IN RIDDLES");
	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 550, ALLEGRO_ALIGN_LEFT, "STRAY FROM IT MORE THAN FOUR SECONDS AND YOU WILL DIE");

	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 650, ALLEGRO_ALIGN_LEFT, "ARROW KEYS MOVE YOU U,D,L,R");
	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 675, ALLEGRO_ALIGN_LEFT, "IF YOU ARE BRAVE ENOUGH, PRESS ENTER TO START");

	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 700, ALLEGRO_ALIGN_LEFT, "PRESS A TO ENABLE AUDIO");
	al_draw_textf(introfont, al_map_rgb(255, 255, 255), 0, 750, ALLEGRO_ALIGN_LEFT, "PRESS D TO DISABLE AUDIO");

	al_draw_bitmap(avatar, avatarX, 48, 0); 
	al_flip_display();
	//Intro Display messages, game rules and controls, as well as the 'Avatar' of the God who will scale based on progress

	while (!start)
	{

		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ENTER:
				start = true;
				break;

			}
		}
	}
	//Waits for the user to press the enter key before starting the game, gives time to read the rules

	MapDrawBG(xOff,yOff, 0, 0, WIDTH-1, HEIGHT-1);
	MapDrawFG(xOff,yOff, 0, 0, WIDTH-1, HEIGHT-1, 1);
	player.DrawSprites(0,0);
	//draws the fore and background if we wanted to use both. starts drawing the player.
	al_play_sample(background, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &sample1);

	al_flip_display();
	al_clear_to_color(al_map_rgb(0,0,0));

	while(!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			gametime++; //for displaying the gametime
			if (player.getHealth() <= 0)
			{
				al_play_sample(losing, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				al_draw_filled_rectangle(0, 600, 600, 800, al_map_rgb(0, 0, 0));
				al_draw_textf(introfont, al_map_rgb(255, 255, 255), WIDTH/4, 200, ALLEGRO_ALIGN_LEFT, "MORTAL, YOU HAVE PERISHED");
				al_draw_textf(introfont, al_map_rgb(255, 255, 255), WIDTH / 4, 250, ALLEGRO_ALIGN_LEFT, "YOU MANAGED TO COMPLETE");
				al_draw_textf(introfont, al_map_rgb(255, 255, 255), WIDTH / 4, 300, ALLEGRO_ALIGN_LEFT, "%i CHALLENGES", level);
				al_draw_textf(introfont, al_map_rgb(255, 255, 255), WIDTH / 4, 350, ALLEGRO_ALIGN_LEFT, "I WILL REVIVE YOU");
				al_draw_textf(introfont, al_map_rgb(255, 255, 255), WIDTH / 4, 400, ALLEGRO_ALIGN_LEFT, "GATHER YOUR STRENGTH");
				al_draw_textf(introfont, al_map_rgb(255, 255, 255), WIDTH / 4, 450, ALLEGRO_ALIGN_LEFT, "AND RETURN TO FREE ME!");
				al_flip_display();
				al_rest(10.0);
				
				done = true;
			}
			//gameover clause, shows the number of levels completed

			if (level == 0 || level == 1)
			{
				if (gametime / 60 == 1)
				{
					player.loseHealth();
					gametime = 0;
				}
			}
			//first two levels are timed off of hp

			render = true;

			if (keys[UP])
				player.UpdateSprites(WIDTH, HEIGHT, 0); //up=0
			else if (keys[DOWN])
				player.UpdateSprites(WIDTH, HEIGHT, 1); //down=1
			else if (keys[LEFT])
				player.UpdateSprites(WIDTH, HEIGHT, 2); //left=2
			else if (keys[RIGHT])
				player.UpdateSprites(WIDTH, HEIGHT, 3);//right=3
			else
				player.UpdateSprites(WIDTH, HEIGHT, 4);//just in case

			player.CollisionTriggerBlock(); //checks collision 60 times a second

			if (player.getPoints() == 3)
			{
				switch (level)
				{
				case 0:
					
					al_play_sample(winning, 1.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					player.setLevel1Status(0);
					level++;
					scale++;
					al_draw_filled_rectangle(0, 600, 600, 800, al_map_rgb(0, 0, 0));
					al_draw_textf(introfont, al_map_rgb(255, 255, 255), WIDTH / 4, 650, ALLEGRO_ALIGN_LEFT, "WELL DONE, I CAN FEEL MY POWER RETURNING");
					al_flip_display();
					al_rest(4.0);
					if (MapLoad("Level2.fmp", 1))
						return -5;
					player.resetPos();
					gametime = 0;

					break;
					//beat first level, display message and our winning sound, ups the scale of the avatar, loads next map

				case 1:
					
					al_play_sample(winning, 1.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					level++;
					scale++;
					al_draw_filled_rectangle(0, 600, 600, 800, al_map_rgb(0, 0, 0));
					al_draw_textf(introfont, al_map_rgb(255, 255, 255), WIDTH / 4, 650, ALLEGRO_ALIGN_LEFT, "WELL DONE, I AM STRONGER YET");
					al_flip_display();
					al_rest(4.0);
					if (MapLoad("Level3.fmp", 1))
						return -5;
					player.resetPos();
					gametime = 0;

					break;
					//beat second level display message and our winning sound, ups the scale of the avatar, loads next map

				case 2:
					al_stop_sample(&sample1);
					al_play_sample(winning, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
					scale++;
					al_draw_filled_rectangle(0, 600, 600, 800, al_map_rgb(0, 0, 0));
					al_draw_textf(introfont, al_map_rgb(255, 255, 255), WIDTH / 4, 650, ALLEGRO_ALIGN_LEFT, "MY POWER HAS BEEN RESTORED");
					al_draw_textf(introfont, al_map_rgb(255, 255, 255), WIDTH / 4, 700, ALLEGRO_ALIGN_LEFT, "NOW I WILL HELP SAVE YOUR WORLD");
					al_draw_scaled_bitmap(avatar, 0, 0, 32, 32, 800 - xOff, 800 - yOff, 32 * scale, 32 * scale, 0);
					al_flip_display();
					al_rest(10.0);
					done = true;
					break;
					//beat final level, displays the 'Avatar' at full form, waits 10 seconds then exits game loop
				}
			}
			
		
			

			render = true;

		}
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_A:
				al_play_sample(background, 0.3, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &sample1);
				break;
				//play sample
			case ALLEGRO_KEY_D:
				al_stop_sample(&sample1);
				break;
				//stop sample
			}
		}
		else if(ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch(ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			}
		}
		if(render && al_is_event_queue_empty(event_queue))
		{
			render = false;

			
			xOff = player.getX()+player.getWidth() - WIDTH/2 ;
			yOff = player.getY()+player.getHeight()   - HEIGHT/2 ;
			//update the map scroll position
			
			if (xOff < 0) xOff = 0;

			if (xOff > (mapwidth * mapblockwidth - WIDTH))
				xOff = mapwidth * mapblockwidth - WIDTH;
			if (yOff < 0) 
				yOff = 0;
			if (yOff > (mapheight * mapblockheight - HEIGHT)) 
				yOff = mapheight * mapblockheight - HEIGHT;
			//collision detection with edge of map, just in case
			
			MapDrawBG(xOff,yOff, 0, 0, WIDTH, HEIGHT);
			//Background first

			MapDrawFG(xOff,yOff, 0, 0, WIDTH, HEIGHT, 0);
			//then foreground

			player.DrawSprites(xOff, yOff);
			//redraws the player

			al_draw_scaled_bitmap(avatar, 0, 0, 32, 32, 800 - xOff, 800 - yOff, 32 * scale, 32 * scale, 0);
			//draws avatar in middle of the map during play

			status(font, player);			
			//updates the status

			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}
	MapFreeMem();
	al_destroy_font(font);
	al_destroy_font(introfont);
	al_destroy_bitmap(avatar);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_display(display);						
	al_destroy_sample(background);
	al_destroy_sample(winning);
	al_destroy_sample(losing);
	//destroy anything that can cause a memory leak
	return 0;
}



int collided(int x, int y)
{
	BLKSTR *blockdata;
	blockdata = MapGetBlock(x/mapblockwidth, y/mapblockheight);
	return blockdata->tl;
}
//Function: Returns collision data on blocks
//Paramters: x and y position of the block
//Returns: the int representative of the collision data

int triggerValue( int x, int y )
{

	BLKSTR* data;
	data = MapGetBlock( x/mapblockwidth, y/mapblockheight );
	return data->user1;
	
}
//Function: Returns user data on blocks
//Paramters: x and y position of the block
//Returns: the int value of the user data in the block

void status(ALLEGRO_FONT *font, Sprite &Player)
{
	al_draw_filled_rectangle(0, 600, 600, 800, al_map_rgb(0, 0, 0));
	al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 650, ALLEGRO_ALIGN_LEFT, "Health Remaining   %i", Player.getHealth());

	if (Player.getHealth() > 150)
	{
		al_draw_filled_rectangle(0, 700, Player.getHealth(), 720, al_map_rgb(0, 255, 0));
	}
	else if (Player.getHealth() > 50)
	{
		al_draw_filled_rectangle(0, 700, Player.getHealth(), 720, al_map_rgb(255, 255, 0));
	}
	else if (Player.getHealth() > 0)
	{
		al_draw_filled_rectangle(0, 700, Player.getHealth(), 720, al_map_rgb(255, 0, 0));
	}

	switch (Player.getLevel1Status())
	{
	case 1:
		al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 750, ALLEGRO_ALIGN_LEFT, "Currently Holding Blue Flower");
		break;
	case 2:
		al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 750, ALLEGRO_ALIGN_LEFT, "Currently Holding Yellow Flower");
		break;
	case 3:
		al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 750, ALLEGRO_ALIGN_LEFT, "Currently Holding Red Flower");
		break;
	}

	switch (Player.getOracle())
	{
	case 1:
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 650, ALLEGRO_ALIGN_LEFT, "One Leyline Found!");
		break;
	case 2:
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 650, ALLEGRO_ALIGN_LEFT, "Two Leylines Found!");
		break;
	case 3:
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 650, ALLEGRO_ALIGN_LEFT, "Final Leyline Found!");
		break;
	}

	switch (Player.getLevel2mess())
	{
	case 1:
		al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 750, ALLEGRO_ALIGN_LEFT, "Right from whence you came");
		break;
	case 2:
		al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 750, ALLEGRO_ALIGN_LEFT, "Look beyond your goal");
		break;
	case 3:
		al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 750, ALLEGRO_ALIGN_LEFT, "Where the Sun rises and ");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 0, 775, ALLEGRO_ALIGN_LEFT, "the name of the Pole Star");
		break;
	}

	switch (Player.getHint())
	{
	case 1:
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 650, ALLEGRO_ALIGN_LEFT, "7 Steps Down");
		break;
	case 2:
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 650, ALLEGRO_ALIGN_LEFT, "15 Steps Left");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 675, ALLEGRO_ALIGN_LEFT, "5 Steps Up");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 700, ALLEGRO_ALIGN_LEFT, "2 Steps Left");
		break;

	case 3:
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 650, ALLEGRO_ALIGN_LEFT, "7 Steps Down");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 675, ALLEGRO_ALIGN_LEFT, "1 Step Left");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 700, ALLEGRO_ALIGN_LEFT, "3 Steps Down");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 725, ALLEGRO_ALIGN_LEFT, "25 Steps Right");
		break;
	case 4:
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 650, ALLEGRO_ALIGN_LEFT, "5 Steps Down");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 675, ALLEGRO_ALIGN_LEFT, "4 Steps Right");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 700, ALLEGRO_ALIGN_LEFT, "5 Steps Up");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 725, ALLEGRO_ALIGN_LEFT, "3 Steps Right");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 750, ALLEGRO_ALIGN_LEFT, "11 Steps Down");
		break;
	case 5:
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 650, ALLEGRO_ALIGN_LEFT, "2 Steps Left");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 675, ALLEGRO_ALIGN_LEFT, "1 Step Up");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 700, ALLEGRO_ALIGN_LEFT, "1 Step Left");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 725, ALLEGRO_ALIGN_LEFT, "1 Step Up");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 750, ALLEGRO_ALIGN_LEFT, "5 Steps Left");
		break;
	case 6:
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 650, ALLEGRO_ALIGN_LEFT, "12 Steps Down");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 675, ALLEGRO_ALIGN_LEFT, "9 Steps Left");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 700, ALLEGRO_ALIGN_LEFT, "3 Steps Up");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 725, ALLEGRO_ALIGN_LEFT, "2 Steps Right");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 750, ALLEGRO_ALIGN_LEFT, "1 Step Up");
		al_draw_textf(font, al_map_rgb(255, 255, 255), 300, 775, ALLEGRO_ALIGN_LEFT, "1 Step Right then go Up!");
		break;

	}
}
//Function: Sets up the various game messages that will display during play.
//Paramters: A font to create the text and the player to check trigger conditions
//Returns: None
