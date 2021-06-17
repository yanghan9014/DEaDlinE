#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <stdio.h>

#include "l_window.h"
#include "l_texture.h"
#include "l_renderer.h"
#include "l_mixer.h"
#include "l_mixerchunk.h"

#include "scene.h"
#include "gameplay_map.h"
#include "gameplay_maps.h"
#include "overlay.h"
#include "game_displays.h"

#include "timer.h"
#include "gamestate.h"

#include "game.h"
#include "character.h"

#include "screen.h"
#include "select.h"
#include "bit_counter.h"
#include "textbox.h"


//TODO: add fall back clock: Vsync is off when window is minimized
const int WINDOWED_WIDTH = 1280;
const int WINDOWED_HEIGHT = 720;

const int SMAIN_BUTTON_WIDTH  = 280;//
const int SMAIN_BUTTON_HEIGHT = 100;//

const int SMAP_SCHARACTER_BUTTON_WIDTH  = 347;//
const int SMAP_SCHARACTER_BUTTON_HEIGHT = 259;//

bool gIsGamePadMode = true;
//global, for use of Player
SDL_Joystick* gamePad[2];
//The music that will be played
LMixer gMixer;
//global font
TTF_Font* gFont = nullptr;
//The window we'll be rendering to
LWindow gWindow;
//The window renderer
LRenderer gRenderer;

character* cha_List[ CHARACTER_COUNT ];



//Starts up SDL and creates window
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK ) < 0 )
	{
		printf( "[init] SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "[init] Warning: Linear texture filtering not enabled!" );
		}

		//Switch back to keyboard mode when less than 2 are connected
		if( SDL_NumJoysticks() < 2 )
		{
			//unimplemented
			//printf( "[init] Warning: No joysticks connected! Keyboard mode on\n" );
			gIsGamePadMode = false;
		}
		else
		{
			printf( "[init] GamePad mode on\n" );
			//Load joystick
			gamePad[0] = SDL_JoystickOpen( 0 );
			gamePad[1] = SDL_JoystickOpen( 1 );
			if( gamePad[0] == nullptr )
			{
				printf( "[init] Warning: Unable to open game controller 1 ! SDL Error: %s\n", SDL_GetError() );
				if(gamePad[1] == nullptr )
				{
					printf( "[init] Warning: Unable to open game controller 2 ! SDL Error: %s\n", SDL_GetError() );
				}
			}
		}
	}
	//Initialize SDL_mixer
	if( !gMixer.init() )
	{
		printf( "[init] SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
		success = false;
	}
	if( TTF_Init() == -1 )
	{
		printf( "[init] SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	gFont = TTF_OpenFont( "fonts/AbhayaLibre-ExtraBold.ttf", 40 );
	if( gFont == nullptr )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	//Create window
	else if( !gWindow.init() )
	{
		printf( "[init] Window could not be created! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	//Create renderer for window
	else if( !gRenderer.createFromGWindow() )
	{
			printf( "[init] Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
	}

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if( !( IMG_Init( imgFlags ) & imgFlags ) )
	{
		printf( "[init] SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
		success = false;
	}

	cha_List[Sniper] =		new character(Sniper,
							"skin_examples/character1.png",
							30, 60, 4, 4, 18, 100 );

	cha_List[Assassin] =	new character(Assassin,
							"skin_examples/character2.png",
							30, 60, 10, 4, 20, 100 );

    cha_List[RedCloud] =    new character(RedCloud,
							"skin_examples/character3.png",
							30, 60, 7, 4, 30, 100 );

    cha_List[TA] =          new character(TA,
							"skin_examples/character4.png",
							30, 60, 7, 4, 20, 100 );

    cha_List[Yeet] =        new character(Yeet,
							"skin_examples/character5.png",
							30, 60, 14, 4, 20, 100 );

    cha_List[MrEdge] =	    new character(MrEdge,
							"skin_examples/character6.png",
							30, 60, 8, 4, 20, 100 );

	return success;
}

void close()
{
	for (int i = 0; i < 6; i++)
		delete cha_List[i];

	SDL_JoystickClose( gamePad[0] );
	SDL_JoystickClose( gamePad[1] );
	gamePad[0] = gamePad[1] = nullptr;

	gMixer.free();
	gRenderer.free();
	gWindow.free();
	TTF_CloseFont(gFont);

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	exit(0x0);//cheating; FIX: properly debug XD
}

void FadeOverlay(Scene& scene, SDL_Event& e, unsigned char alpha, unsigned char delta_alpha)
{
	Overlay black(&scene, "overlays/black.png", Overlay::STATIC | Overlay::STRETCH);
	black.setFade(alpha, delta_alpha);

	while (black.isFading())
	{
		while ( SDL_PollEvent( &e ) != 0 )
		{
			gWindow.handleEvent( e );
			scene.handleEvent( e );
		}
		scene.renderAll();
	}
}



int main( int argc, char* args[] )
{
	//State handler
	GameState gameState;
	//Event handler
	SDL_Event e;

	//Start up SDL and create window
	if( !init() )
	{
		printf( "[main] Failed to initialize!\n" );
		gameState = GameState::CLOSE;
	}
	else
	{
		gameState = GameState::INTRO;

		gRenderer.clear();//clear to blank while starting
		gRenderer.present();

		Scene introScene;

		Overlay background(&introScene, "overlays/intro_test.png");
		background.setFade(0, 1);

		gMixer.loadMus("audio/offvocal_digital_tattoo_intro.wav");
		gMixer.play(1);

		Timer timer;

		//Play intro on startup
		while ( gameState == GameState::INTRO )
		{
			while ( SDL_PollEvent( &e ) != 0 )
			{
				gameState.handleEvent( e );
				gWindow.handleEvent( e );
				introScene.handleEvent( e );
				gMixer.handleEvent( e );
			}

			if ( !gMixer.isPlaying() )//this state only last while the music does
			{
				gameState = GameState::START;
				printf( "\n[main] Entered GameState::MENU\n" );
			}

			if (timer.getElapsed() > 4000)//for black-out effect
			{
				background.removeFromScene();
			}
			introScene.renderAll();
		}

		if ( gMixer.isPlaying() && gameState != GameState::CLOSE )//this state only last while the music does
		{
			gMixer.fadeOut(100);
			LMixerChunk enter;
			enter.loadWav("audio/button_down.wav");
			enter.play();

			FadeOverlay(introScene, e, 0, 10);
		}
	}

	if ( gameState == GameState::CLOSE)//cheat
		close();

	//game-wide stuff that need to be out of the main state loop
	GameplayMapManager maps("map_files/maps.dat");

	select_screen mainSelect = select_screen();
	mainSelect.setSelection(1, -1);
	select_screen mapSelect = select_screen();
	select_screen charSelect = select_screen();
	charSelect.setSelection(2, 1);

	SDL_Color textColor = {0, 0, 0, 0};//TODO: remove
	{
	if (!mainSelect.loadFromFile("button_sprites/go_into_game_sprite.PNG"            ,
							"button_sprites/map_choose_sprite.PNG"              ,
							"button_sprites/character_choose_sprite.PNG"        ,
							"button_sprites/teaching_mode_sprite.PNG"           ,
							"button_sprites/game_setting_sprite.PNG"            ,
							"button_sprites/endgame_sprite.PNG"                 ,
							"button_sprites/smain_goback_sprite.PNG"            ,
							"overlays/select_screen.PNG"                         ,
							" ",textColor))
	{
		printf("smain fail\n");
	}
	else if(!mapSelect.loadFromFile ("button_sprites/map1_sprite.PNG"                    ,
						"button_sprites/map2_sprite.PNG"                    ,
						"button_sprites/map3_sprite.PNG"                    ,
						"button_sprites/map4_sprite.PNG"                    ,
						"button_sprites/map5_sprite.PNG"                    ,
						"button_sprites/map6_sprite.PNG"                    ,
						"button_sprites/smap_scharacter_goback_sprite.PNG"  ,
						"overlays/map_character_select_screen.PNG"           ,
						" ",textColor
						))
	{
		printf("smap fail\n");
	}
	else if(!charSelect.loadFromFile("button_sprites/character1_sprite.PNG"               ,
								"button_sprites/character2_sprite.PNG"                    ,
								"button_sprites/character3_sprite.PNG"                    ,
								"button_sprites/character4_sprite.PNG"                    ,
								"button_sprites/character5_sprite.PNG"                    ,
								"button_sprites/character6_sprite.PNG"                    ,
								"button_sprites/smap_scharacter_goback_sprite.PNG"  ,
								"overlays/map_character_select_screen.PNG"           ,
								" ",textColor
								))
	{
		printf("scharacter fail\n");
	}
	else
	{
		//printf("Finish screen file loading\n");
		//Set sprites
		mainSelect.setSpriteClips(SMAIN_BUTTON_HEIGHT, SMAIN_BUTTON_WIDTH);
		mapSelect.setSpriteClips(SMAP_SCHARACTER_BUTTON_HEIGHT, SMAP_SCHARACTER_BUTTON_WIDTH);
		charSelect.setSpriteClips(SMAP_SCHARACTER_BUTTON_HEIGHT, SMAP_SCHARACTER_BUTTON_WIDTH);

		mainSelect.setposition	(800, 80, 840, 180, 840, 260, 800, 360, 800, 460, 740, 560, 1080, 610);//TODO: read from file
		mapSelect.setposition	(74 , 78, 467, 78 , 860, 78 , 74 , 378, 467, 378, 860, 378, 1040, 550);
		charSelect.setposition	(74 , 78, 467, 78 , 860, 78 , 74 , 378, 467, 378, 860, 378, 1040, 550);
	}
	}

	while ( gameState != GameState::CLOSE )
	{
		if ( gameState == GameState::START )
		{
			gRenderer.clear();
			gRenderer.present();

			Scene startScene;

			Overlay background(&startScene, "overlays/loading_screen.png");

			TextBox text(&startScene, "- Press Enter to Start -", 500, 500);//FIX: align the position
			text.setFade(0, 3, true);

			Overlay white(&startScene, "overlays/white.png", Overlay::STRETCH);
			white.setFade(255, -2);


			gMixer.loadMus("audio/Asian_EDMTechno_Type_Beat_start.wav");
			gMixer.play();


			while ( gameState == GameState::START )
			{
				while ( SDL_PollEvent( &e ) != 0 )
				{
					gameState.handleEvent( e );
					gWindow.handleEvent( e );
					startScene.handleEvent( e );
				}
				startScene.renderAll();
			}

			LMixerChunk enter;
			enter.loadWav("audio/button_down.wav");
			if ( gameState == GameState::MENU )
			{
				enter.play();
			}

			gMixer.fadeOut();
			FadeOverlay(startScene, e, 0, 10);
		}
		else if ( gameState == GameState::MENU )
		{
			gRenderer.clear();
			gRenderer.present();

			Scene menuMainScene;
			Scene menuMapScene;
			Scene menuCharScene;
			Scene menuTutScene;

			mainSelect.setScene(&menuMainScene);
			mapSelect.setScene(&menuMapScene);
			charSelect.setScene(&menuCharScene);
			Overlay tutorial(&menuTutScene, "overlays/tutorial.png");

			mainSelect.getBitCounter().reset();
			mainSelect.update();
			mainSelect.setActive(true);
			mapSelect.setActive(true);
            charSelect.setActive(true);

			gMixer.loadMus("audio/wind_menu.wav");
			gMixer.fadeIn();


			Scene* menuScene = &menuMainScene;
			gameState.subscribeToSelection(&mainSelect);

			FadeOverlay(*menuScene, e, 255, -5);
			bool doFade = false;
			while ( gameState == GameState::MENU )
			{
				if (gameState == GameState::MAIN)
				{
					doFade = gameState.subscribeToSelection(&mainSelect);
					if (doFade)
					{
						FadeOverlay(*menuScene, e, 0, 10);
					}
					menuScene = &menuMainScene;
				}
				else if (gameState == GameState::MAP)
				{
					doFade = gameState.subscribeToSelection(&mapSelect);
					if (doFade)
					{
						FadeOverlay(*menuScene, e, 0, 10);
					}
					menuScene = &menuMapScene;
				}
				else if (gameState == GameState::CHARACTER)
				{
					doFade = gameState.subscribeToSelection(&charSelect);
					if (doFade)
					{
						FadeOverlay(*menuScene, e, 0, 10);
					}
					menuScene = &menuCharScene;
				}
				else if (gameState == GameState::TUTORIAL)
				{
					doFade = gameState.subscribeToSelection(nullptr);
					if (doFade)
					{
						FadeOverlay(*menuScene, e, 0, 10);
					}
					menuScene = &menuTutScene;
				}

				if (doFade)
				{
					FadeOverlay(*menuScene, e, 255, -10);
					doFade = false;
				}

				while ( SDL_PollEvent( &e ) != 0 )
				{
					gameState.handleEvent( e );
					gWindow.handleEvent( e );
					menuScene->handleEvent( e );
				}

				menuScene->renderAll();
			}

			mainSelect.setActive(false);
			mapSelect.setActive(false);
            charSelect.setActive(false);
			gameState.subscribeToSelection(nullptr);

			gMixer.fadeOut(500);
			FadeOverlay(*menuScene, e, 0, 5);
		}
		else if ( gameState == GameState::PLAY )
		{
			gRenderer.clear();
			gRenderer.present();

			Scene playScene;

			maps.loadMap(mapSelect.getSelection(1), &playScene);
			maps.pushMapToSceneAgain();

			game GameOn(maps.getMap(), &playScene, gIsGamePadMode, charSelect.getSelection(1), charSelect.getSelection(2), 0);

			GameDisplays display(&playScene);

			//Load music
			gMixer.loadMus("audio/offvocal_Izayoi_Setsugekka_play.wav");
			gMixer.play();

			while ( gameState == GameState::PLAY )
			{
				//handleInput
				while ( SDL_PollEvent( &e ) != 0 )
				{
					gameState.handleEvent( e );
					gWindow.handleEvent( e );
					playScene.handleEvent( e );
					GameOn.handleEvent();
				}

				if (gameState == GameState::READY)
				{
					GameOn.restart();
					display.start(GameOn.getPlayer(0), GameOn.getPlayer(1));
					gameState = GameState::FIGHT;
				}
				else if (gameState == GameState::FIGHT)
				{
					GameOn.UpdatePosition();
					GameOn.Check_Collision();
					GameOn.updateScene();

					display.update();
					playScene.renderAll();

					if (display.death())
					{
						if (display.finished())
							gameState = GameState::FINISH;
						else
							gameState = GameState::REST;

						GameOn.UpdatePosition();
					}
				}
				else if (gameState == GameState::REST)
				{
					display.update();
					playScene.renderAll();

					if (!display.doingTransition())
					{
						FadeOverlay(playScene, e, 0, 10);
						gameState = GameState::READY;
					}
				}
				else
				{
					display.update();
					playScene.renderAll();

					if (!display.doingTransition())
					{
						gameState = GameState::MAIN;
						gameState = GameState::MENU;
					}
				}
			}

			gMixer.fadeOut();
			FadeOverlay(playScene, e, 0, 10);
		}
		else if (gameState == GameState::EXIT)
		{
			Scene exitScene;

			Overlay exit(&exitScene, "overlays/closing_screen.png");

			FadeOverlay(exitScene, e, 255, -12);

			Timer t;
			while ( t.getElapsed() < 1500)
			{
				while ( SDL_PollEvent( &e ) != 0 )
				{
					gWindow.handleEvent( e );
					exitScene.handleEvent( e );
				}
				exitScene.renderAll();
			}

			FadeOverlay(exitScene, e, 0, 3);
			gameState = GameState::CLOSE;
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
