#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_actor.h"
#include "entity.h"
#include "player.h"
#include "level.h"
#include "camera.h"
#include "enemy_walk.h"
#include "enemy_climb.h"
#include "enemy_drop.h"
#include "enemy_chase.h"
#include "enemy_rise.h"
#include "waterfall.h"
#include "floor.h"
#include "transition.h"
#include "warp.h"
#include "destructable.h"
#include "hazard.h"
#include "collect.h"
#include "font.h"
#include "menuWindow.h"
#include "windows_common.h"
#include "gfc_audio.h"
#include "simple_logger.h"

static int done = 0;
static Window *_quit = NULL;
Window *test;


void clickCancel(void *data)
{
	_quit = NULL;
}
void clickExit(void *data)
{
	done = 1;
	_quit = NULL;
}
float randomLevel()
{
	float test;

	test = gfc_crandom();

	if (test < 0)test = 1;
	else if (test >= 0)test = 2;

	slog("%f", test);
	return test;
}



int main(int argc, char * argv[])
{
    /*variable declarations*/
    const Uint8 * keys;
	Level *level;
	//Font *font;
	TextLine scoreText;

	
	int levelType = 0;
	int levelLocation = 0;
	float destination = randomLevel();
	int physics = 0;
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
	cameraSetDimensions(vector2d(1200, 720));
	cameraSetPosition(vector2d(0, 0));
    gf2d_sprite_init(1024);
	gf2d_action_list_init(128);
	gfc_audio_init(256, 16, 4, 2, 1, 1);
	gfc_sound_init(16);
	gf2d_font_init("config/font.cfg");
	gf2d_windows_init(128);
	entity_manager_init(100);
	level = levelNew();

	
	FILE *fp;
	float prevScore;
	fp = fopen("scores.txt", "r");
	fscanf(fp, "%f", &prevScore);
	fclose(fp); 

    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	level = levelLoad("levels/overworld.json");
	Sound *music = gfc_sound_load("actors/speck.mp3", .5, 1);
	Sound *jump = gfc_sound_load("actors/jump.wav", .5, 2);
	Sound *lose = gfc_sound_load("actors/lose.wav", .5, 3);
	//bgMusic = Mix_LoadMUS("actors/speck.mp3");
	//Mix_PlayMusic(bgMusic, -1);
	gfc_sound_play(music, 3, 1, -1, -1);
	Entity *player = playerSpawn(vector2d(100, 100));
	transitionLocationSpawn(vector2d(1150, 400));
	Entity *rock = rockCreate(vector2d(500, 400));
	Entity *rock2 = rockCreate(vector2d(575, 400));
	Entity *plate = plateCreate(vector2d(0, 300));
	Entity *lock = lockCreate(vector2d(1100, 200));
	Entity *lock2 = lockCreate(vector2d(50, 200));
	lock2->flip.x = -1;

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
		entity_manager_update_entities();
		gf2d_windows_update_all();

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
		levelDraw(level);

		entity_manager_draw_entities();
		gf2d_windows_draw_all();
        //UI elements last
        gf2d_sprite_draw(
             mouse,
             vector2d(mx,my),
             NULL,
             NULL,
             NULL,
             NULL,
             &mouseColor,
            (int)mf);
        gf2d_graphics_next_frame();// render current draw frame and skip to the next frame

		//the loading of all the levels
		if (playerEnteranceContact())
		{
			if (levelType == 0 && destination == 1)
			{
				slog("loading level1");
				entity_manager_free();
				levelFree(level);

				//re-initialize
				cameraSetDimensions(vector2d(1200, 720));
				cameraSetPosition(vector2d(0, 0));
				gf2d_sprite_init(1024);
				entity_manager_init(100);
				gf2d_windows_init(128);
				level = levelNew();

				SDL_ShowCursor(SDL_DISABLE);

				//restart level with a new json
				mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
				level = levelLoad("levels/level1.json");
				gf2d_windows_draw_all();
				gfc_sound_play(music, 3, 1, -1, -1);
				playerSpawn(vector2d(55, 55)); ///entity[0]
				transitionLocationSpawn(vector2d(15, 20)); //entity[1]
				waterfallSpawn(vector2d(100, 200));
				walkerSpawn(vector2d(375, 500)); 

				//create collectable
				collectCreate(vector2d(1100, 600)); 

				for (int i = 100; i < 1200; i += 100)floorCreate(vector2d(i, 700)); //creates the bottom floor

				//creating platforms thorughout the level
				floorCreate(vector2d(0, 150)); //spawn plats
				floorCreate(vector2d(100, 150));

				floorCreate(vector2d(300, 550)); //plats with enemy
				floorCreate(vector2d(400, 550));
				floorCreate(vector2d(500, 550));
				floorCreate(vector2d(200, 350));

				floorCreate(vector2d(1050, 550)); //stuff surrounding above gem
				floorCreate(vector2d(950, 550));
				spikeCreate(vector2d(1000, 650));
				spikeCreate(vector2d(952, 650));

				dropperSpawn(vector2d(700, 300));
				riserSpawn(vector2d(800, 720));


				changeWorldState(levelType);
				levelType = 1;
				levelLocation = 1;

				slog("World State: %f", getWorldState());
			}
			else if (levelType == 0 && destination == 2)
			{
				slog("loading level2");
				entity_manager_free();
				levelFree(level);

				//re-initialize		
				cameraSetDimensions(vector2d(1200, 720));
				cameraSetPosition(vector2d(0, 0));
				gf2d_sprite_init(1024);
				entity_manager_init(100);
				gf2d_windows_init(128);
				level = levelNew();

				SDL_ShowCursor(SDL_DISABLE);

				//restart level with a new json
				mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
				level = levelLoad("levels/level2.json");
				gf2d_windows_draw_all();
				gfc_sound_play(music, 3, 1, -1, -1);
				playerSpawn(vector2d(55, 55)); ///entity[0]
				transitionLocationSpawn(vector2d(10, 20)); //entity[1] 
				climberSpawn(vector2d(800, 20)); //entity[2]
				
				warpPointSpawn(vector2d(1075, 100));
				warpPointSpawn(vector2d(50, 650));

				for (int i = 0; i < 1200; i += 100)floorCreate(vector2d(i, 700));
				chaserSpawn(vector2d(500,650));
				floorCreate(vector2d(0, 150)); //spawn platforms
				floorCreate(vector2d(100, 150));

				collectCreate(vector2d(950, 100));
				floorCreate(vector2d(1000, 175)); //gem & gem platforms
				floorCreate(vector2d(900, 175));
				floorCreate(vector2d(800, 175));
				

				floorCreate(vector2d(260, 475)); //level platforms
				floorCreate(vector2d(175, 250));
				floorCreate(vector2d(750, 550));
				floorCreate(vector2d(1050, 425));



				changeWorldState(levelType);
				levelType = 1;
				levelLocation = 2;

				slog("World State: %f", getWorldState());
			}
			else if (levelType == 1)
			{
				slog("loading overworld");
				entity_manager_free();
				levelFree(level);

				//re-initialize
				cameraSetDimensions(vector2d(1200, 720));
				cameraSetPosition(vector2d(0, 0));
				gf2d_sprite_init(1024);
				entity_manager_init(100);
				gf2d_windows_init(128);
				level = levelNew();

				SDL_ShowCursor(SDL_DISABLE);

				//restart level with a new json
				mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
				level = levelLoad("levels/overworld.json");
				gf2d_windows_draw_all();
				gfc_sound_play(music, 3, 1, -1, -1);
				Entity *player = playerSpawn(vector2d(100, 100));
				transitionLocationSpawn(vector2d(1150, 400));
				Entity *rock = rockCreate(vector2d(500, 400));
				Entity *rock2 = rockCreate(vector2d(575, 400));
				Entity *plate = plateCreate(vector2d(0, 300));
				Entity *lock = lockCreate(vector2d(1100, 200));
				Entity *lock2 = lockCreate(vector2d(50, 200));
				lock2->flip.x = -1;
 
				changeWorldState(levelType);	
				levelType = 0;
				levelLocation = 0;
				destination = randomLevel();

				slog("World State: %f", getWorldState());
			}
		}
		if (entityGrounded() && levelType == 0)
		{
			slog("loading traplevel");
			entity_manager_free();
			levelFree(level);

			//re-initialize		
			cameraSetDimensions(vector2d(1200, 720));
			cameraSetPosition(vector2d(0, 0));
			gf2d_sprite_init(1024);
			entity_manager_init(100);
			gf2d_windows_init(128);
			level = levelNew();

			SDL_ShowCursor(SDL_DISABLE);

			//restart level with a new json
			mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
			level = levelLoad("levels/level2.json");
			gf2d_windows_draw_all();
			gfc_sound_play(music, 3, 1, -1, -1);
			playerSpawn(vector2d(55, 55)); ///entity[0]
			transitionLocationSpawn(vector2d(1000, 650)); //entity[1] 
			waterfallSpawn(vector2d(600, 175));

			for (int i = 0; i < 1200; i += 100)floorCreate(vector2d(i, 700));
			chaserSpawn(vector2d(500, 650));
			dropperSpawn(vector2d(300, 200));
			floorCreate(vector2d(0, 150)); //spawn platforms
			floorCreate(vector2d(100, 150));

			spikeCreate(vector2d(1000, 505));
			floorCreate(vector2d(1000, 555));
			spikeCreate(vector2d(952, 505));
			floorCreate(vector2d(900, 555));
			spikeCreate(vector2d(900, 505));
			floorCreate(vector2d(800, 555));
			spikeCreate(vector2d(850, 505));

			changeWorldState(levelType);
			levelType = 1;
			levelLocation = 3;
		}


		//player die, window appears
		if (playerEnemyContact())
		{
			gfc_sound_play(lose, 3, 1, -1, -1);
			test = gf2d_window_load("config/testWindow.json");
			gf2d_window_set_position(test, vector2d(250, 250));
		}
		//jump sound effect plays
		if (keys[SDL_SCANCODE_UP] && levelType == 1 && entityGrounded())
		{
			gfc_sound_play(jump, 0, 1, -1, -1);
		}
		//ifs for when the player comes into contact with the Destructables
		if (playerDestructableContact(rock))
		{
			if (keys[SDL_SCANCODE_E])
			{
				entity_free(rock);
			}
			if (keys[SDL_SCANCODE_R])
			{
				rock->position.x -= 30;
			}
		}
		else if (playerDestructableContact(rock2))
		{
			if (keys[SDL_SCANCODE_E])
			{
				entity_free(rock2);
			}
			if (keys[SDL_SCANCODE_R])
			{
				rock->position.x -= 30;
			}

		}
		
		//unlock the door
		if (plateUnlock(rock, plate))
		{
			entity_free(lock);
		}
		
		/*
		if (keys[SDL_SCANCODE_ESCAPE] && _quit == NULL)
		{
			// exit condition
			_quit = window_yes_no("Quit?", clickExit, clickCancel, NULL, NULL);
			gf2d_window_set_position(_quit, vector2d(600, 300));
			//done = 1;
		}*/
		if (keys[SDL_SCANCODE_Q])
		{
			// exit condition
			done = 1;
		}

		slog("FPS: %f", gf2d_graphics_get_frames_per_second());
		gfc_line_sprintf(scoreText, "Previous Score: %f", prevScore);
		//fontRender(font, scoreText, vector2d(200, 20), gfc_color8(255,0,0,255));
		gf2d_font_draw_line_tag(scoreText, FT_Small, gfc_color(255,255,255,255), vector2d(200, 20));

		gfc_line_sprintf(scoreText, "Score: %f", returnScore());
		//fontRender(font, scoreText, vector2d(700, 20), gfc_color8(255, 0, 0, 255));
		gf2d_font_draw_line_tag(scoreText, FT_Small, gfc_color(255, 255, 255, 255), vector2d(700, 20));

		gf2d_graphics_next_frame();
    }
	slog("------------END---------------");
    return 0;
}
/*eol@eof*/
