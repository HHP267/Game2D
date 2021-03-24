#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "entity.h"
#include "player.h"
#include "level.h"
#include "camera.h"
#include "enemy_walk.h"
#include "enemy_climb.h"
#include "enemy_drop.h"
#include "enemy_chase.h"
#include "enemy_rise.h"
#include "floor.h"
#include "transition.h"
#include "warp.h"
#include "hazard.h"
#include "collect.h"
#include "font.h"
#include "simple_logger.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
	Level *level;
	Font *font;
	TextLine scoreText;
	
	int levelType = 0;
	int levelLocation = 0;
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
	fontInit(10);
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
	font = fontLoad("fonts/8bit.ttf", 16);
	slog("font loaded");
	playerSpawn(vector2d(100, 100));
	transitionLocationSpawn(vector2d(300, 300));
	transitionLocationSpawn(vector2d(800, 600));

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

        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
		levelDraw(level);

		entity_manager_draw_entities();
            
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
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		if (playerEnteranceContact())
		{
			if (levelType == 0 && entity_manager_return_entity(0).position.x < 500)
			{
				slog("loading level1");
				entity_manager_free();
				levelFree(level);

				//re-initialize
				cameraSetDimensions(vector2d(1200, 720));
				cameraSetPosition(vector2d(0, 0));
				gf2d_sprite_init(1024);
				entity_manager_init(100);
				level = levelNew();

				SDL_ShowCursor(SDL_DISABLE);

				//restart level with a new json
				mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
				level = levelLoad("levels/level1.json");
				playerSpawn(vector2d(55, 55)); ///entity[0]
				transitionLocationSpawn(vector2d(15, 20)); //entity[1]
				walkerSpawn(vector2d(375, 500)); //entity[2]

				//create collectable
				collectCreate(vector2d(1100, 600)); //entity[3]


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
			else if (levelType == 0 && entity_manager_return_entity(0).position.x > 500)
			{
				slog("loading level2");
				entity_manager_free();
				levelFree(level);

				//re-initialize		
				cameraSetDimensions(vector2d(1200, 720));
				cameraSetPosition(vector2d(0, 0));
				gf2d_sprite_init(1024);
				entity_manager_init(100);
				level = levelNew();

				SDL_ShowCursor(SDL_DISABLE);

				//restart level with a new json
				mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
				level = levelLoad("levels/level2.json");
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
				level = levelNew();

				SDL_ShowCursor(SDL_DISABLE);

				//restart level with a new json
				mouse = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
				level = levelLoad("levels/overworld.json");
				playerSpawn(vector2d(100, 100)); //entity[0]
				transitionLocationSpawn(vector2d(300, 300)); //entity[1]
				transitionLocationSpawn(vector2d(800, 600));
 
				changeWorldState(levelType);
				levelType = 0;
				levelLocation = 0;

				slog("World State: %f", getWorldState());
			}
		}

		if (playerEnemyContact())
		{
			playerDie(entity_manager_return_entity(0));
			done = 1;
		}
		

        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
		//if (keys[SDL_SCANCODE_Q])slog("score %d", returnScore());

		gfc_line_sprintf(scoreText, "Previous Score: %f", prevScore);
		fontRender(font, scoreText, vector2d(200, 20), gfc_color8(255,0,0,255));

		

		gfc_line_sprintf(scoreText, "Score: %f", returnScore());
		fontRender(font, scoreText, vector2d(700, 20), gfc_color8(255, 0, 0, 255));

		gf2d_grahics_next_frame();
    }
	slog("------------END---------------");
    return 0;
}
/*eol@eof*/
