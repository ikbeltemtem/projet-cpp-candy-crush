#include "game.h"
#define DELAY 500
#define IMAGE_RES 64 // Resolution of Shapes
#include <iostream>
#include "timer.h"

using namespace std;
Game::Game(int width,int height)
    {
        SCREEN_WIDTH = width;
        SCREEN_HEIGHT = height;
        moves=0;
        point=0;
        mtimer = timer::instance();
    }

SDL_Surface* Game::loadSurface(string path)
{
	//Load image at specified path
	SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
	}
	return loadedSurface;
}

bool Game::init()
{
	//Initialization flag
	bool success = true;


	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "Fruity-Crush", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}

	// Inti SDL_tt (TEXT Extenstion)
	if(TTF_Init()<0)
    {
        cout<<"Probelm Initing SDL_ttf"<<endl;
        success=false;
    }

    // Load Sound
    if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }

    //Initialize SDL_mixer (Sound Extension)
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
    {
        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }
	return success;
}

bool Game::loadMedia()
{
	//Loading success flag
	bool success = true;

    // Load the 5 Shapes
    shapes[GRID_TYPE_DIAMOND] =loadSurface( "./images/pink.bmp" );
    shapes[GRID_TYPE_DODE]=loadSurface( "./images/chees.bmp" );
    shapes[GRID_TYPE_HEXAGON]=loadSurface( "./images/green.bmp" );
    shapes[GRID_TYPE_SQUARE]=loadSurface( "./images/tomate.bmp" );
    shapes[GRID_TYPE_TRIANGLE]=loadSurface( "./images/chees1.bmp" );
    if( shapes[0] == NULL || shapes[1]==NULL || shapes[2]==NULL || shapes[3]==NULL|| shapes[4]==NULL )
	{
		printf( "Failed to load Basic Shapes!\n" );
		success = false;
	}
	background=loadSurface("./images/fi.bmp");
	if(background==NULL)
	{
	    cout<<"failed to load background"<<endl;
	    success=false;
	}

	bac=loadSurface("./images/bac.bmp");
	if(bac==NULL)
	{
	    cout<<"failed to load bac"<<endl;
	    success=false;
	}
   start=loadSurface("./images/start.bmp");
	if(bac==NULL)
	{
	    cout<<"failed to load start"<<endl;
	    success=false;
	}
	// Load Celebration Image

	point_shape=loadSurface( "./images/dd.bmp" );
	if(point_shape==NULL)
    {
        cout<<"Failed to load Point Image"<<endl;
        success=false;
    }

    // Load Font (Lazy)
    font = TTF_OpenFont("./images/futura.ttf",25);
    if( font == NULL )
    {
        cout<<"Failed To Load Font"<<endl;
        success=false;
    }
    font2 = TTF_OpenFont("./images/futura.ttf",35);
    if( font == NULL )
    {
        cout<<"Failed To Load Font"<<endl;
        success=false;
    }

    // Load Sound
    gMusic = Mix_LoadWAV( "./images/high.wav" );
    if( gMusic == NULL )
    {
        printf( "Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    return success;
}

void Game::close()
{
	//Deallocate surfaces
	map<int,SDL_Surface*>::iterator i;
	for(  i = shapes.begin(); i!=shapes.end(); ++i)
	{
		SDL_FreeSurface( shapes[i->first]);
		shapes[i->first] = NULL;
	}
	SDL_FreeSurface(textSurface);
	SDL_FreeSurface( point_shape);
    SDL_FreeSurface( background);
    SDL_FreeSurface( bac);
    SDL_FreeSurface( start);

	SDL_DestroyWindow( gWindow ); //Destroy window
	Mix_FreeChunk( gMusic ); // free sound
	gWindow = NULL;
	SDL_Quit(); //Quit SDL subsystems
}

void Game::animate_point(vector<int>&pos, Grid grid)
{
    gCurrentSurface = point_shape;
    SDL_Rect tmp;
    tmp.w = IMAGE_RES;
    tmp.h = IMAGE_RES;

    // First Box (0,3)
    tmp.y = pos[0]*IMAGE_RES+15;
    tmp.x = pos[3]*IMAGE_RES+15;
    SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, &tmp );
    // Middle Box (1,4)
    tmp.y = pos[1]*IMAGE_RES+15;
    tmp.x = pos[4]*IMAGE_RES+15;
    SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, &tmp );
    // Last Box (2,5)
    tmp.y = pos[2]*IMAGE_RES+15;
    tmp.x = pos[5]*IMAGE_RES+15;
    SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, &tmp );

    //Update the surface
	SDL_UpdateWindowSurface( gWindow );
	SDL_Delay(DELAY);
	// Restore Normal
	animate_grid(grid);
}

void Game::animate_grid(Grid grid)
{
    // Render the Grid on the Screen
    for(int i=0;i<GRID_SIZE;i++)
    {
        for(int j=0;j<GRID_SIZE;j++)
        {
            gCurrentSurface = shapes[grid.get_shape(i,j)];
            SDL_Rect tmp;
            tmp.x = j*IMAGE_RES+15; // Since X is Y in SDL
            tmp.y = i*IMAGE_RES+15;
            tmp.w = IMAGE_RES;
            tmp.h = IMAGE_RES;
            //Apply the current image
            SDL_BlitSurface( gCurrentSurface, NULL, gScreenSurface, &tmp );
        }
    }

    //Update the surface
    SDL_UpdateWindowSurface( gWindow );

}

void Game::ready_text(string text, int type)
{
    SDL_Rect tmp;
    SDL_Color foregroundColor = {149,130,126 };
    SDL_Color backgroundColor = {241,192,111};
    if(type==1) // Instruction
    {
        int posx = SCREEN_WIDTH/10, posy = SCREEN_HEIGHT/10;
        // Line 1
        text="";
        textSurface = TTF_RenderText_Shaded(font,text.c_str(),foregroundColor,backgroundColor);
        tmp.x = posx;
        tmp.y = posy;
        SDL_BlitSurface(textSurface, NULL, gScreenSurface, &tmp);
        // Line 2
        text="";
        textSurface = TTF_RenderText_Shaded(font,text.c_str(),foregroundColor, backgroundColor);
        tmp.x = posx;
        tmp.y = posy+40;

        SDL_BlitSurface(textSurface, NULL, gScreenSurface, &tmp);
        // Line 3
        text="";
        textSurface = TTF_RenderText_Shaded(font,text.c_str(),foregroundColor, backgroundColor);
        tmp.x = posx;
        tmp.y = posy+80;
        SDL_BlitSurface(textSurface, NULL, gScreenSurface, &tmp);
    }
    if(type==2)  // Normal In game Display
    {
    T = (int)mtimer->deltatime() / 1060;
        text=text + "   " + "Score: " + to_string(point) + " " + "Moves: " + to_string(moves);
        textSurface = TTF_RenderText_Shaded(font,text.c_str(),foregroundColor,backgroundColor);
        if(textSurface==NULL)
            cout<<"Problem Creating Text Surface"<<endl;
        //tmp.x = 0;
        tmp.x=SCREEN_HEIGHT-540;
        tmp.y = SCREEN_HEIGHT-70;

        SDL_BlitSurface(textSurface, NULL, gScreenSurface, &tmp);
    }
    if(type==3)
    {

        string ch;

        if ((T / 60) >= 1)
        {
            ch = to_string(T / 60) +"min"+ to_string(T % 60)+"sec";
        }
        else
            ch = to_string(T)+"sec";
        text=text+"you get "+to_string(point)+" points in " ;

        textSurface = TTF_RenderText_Blended(font2,text.c_str(),{0,0,0});
       tmp.x=SCREEN_HEIGHT-420;
        tmp.y = SCREEN_HEIGHT-460;
         SDL_BlitSurface(textSurface, NULL, gScreenSurface, &tmp);
         text=ch;

        textSurface = TTF_RenderText_Blended(font2,text.c_str(),{0,0,0});
       tmp.x=SCREEN_HEIGHT-310;
        tmp.y = SCREEN_HEIGHT-410;
        SDL_BlitSurface(textSurface, NULL, gScreenSurface, &tmp);
    }
    //Update the surface

    SDL_UpdateWindowSurface( gWindow );
}

int Game::game_main()
{
    Grid grid;          // Make object of Grid Class
    //int x0,y0,x1,y1;    // Click Location
    int x0,y0,x1,y1;

    bool clicked=false;
    SDL_Event e;        //Event handler
    bool quit = false, display_instructions=true;
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
		    // DISPLAY INSTRUCTIONS
		    SDL_BlitSurface(start,NULL,gScreenSurface,NULL);
		    ready_text("",1);
            while(display_instructions)
            {
                while( SDL_PollEvent( &e ) != 0 )
                {
                    if( e.type == SDL_QUIT )
						{
						    display_instructions=false;
						    quit = true;
						}
                    if(e.type == SDL_MOUSEBUTTONDOWN)
                        display_instructions = false;
                }
            }

			// MAIN GAME LOOP

			grid.load_grid();
			 SDL_BlitSurface(background,NULL,gScreenSurface,NULL);
			ready_text("READY",2);
			while( !quit /*&& moves<=5*/)
			{
			    mtimer->update();
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					// Cancel a Selection
					else if(e.button.button == SDL_BUTTON_RIGHT)
                    {
                        clicked=false;
                    }
                    // A Move is Requested
					else if(e.type == SDL_MOUSEBUTTONDOWN)
                    {

                        if(clicked==false)
                        {
                            SDL_GetMouseState( &y0, &x0); // SDL's X is Y
                            clicked=true;
                        }
                        else
                        {
                            // A MOVE HAS BEEN MADE [MAIN LOGIC]
                            SDL_GetMouseState( &y1, &x1);
                            // Coordinates to Indexes
                            x0/=IMAGE_RES;
                            y0/=IMAGE_RES;
                            x1/=IMAGE_RES;
                            y1/=IMAGE_RES;
                            // Check Validity
                            bool isvalid = grid.valid_move(x0,y0,x1,y1);
                            if(isvalid)
                            {
                               moves++;
                               // Check Grid For Match
                               vector<int>tmp(6,0);     // Store coordinates
                               int cascade=0;
                               while(grid.point_move(tmp))
                               {
                                   // A Match is Made
                                   Mix_PlayChannel( -1, gMusic, 0 ); // Play Sound
                                   animate_grid(grid);               // See result of Move
                                   SDL_Delay(DELAY/10);
                                   ready_text("WOW",2);           // Display "BRAVO!!"
                                   animate_point(tmp,grid);          // Display The Celebration
                                   grid.update_grid(tmp);            // Update Grid with new Elements
                                   SDL_Delay(DELAY/10);
                                   cascade++;
                               }
                               point+=(cascade==1)?10:20*cascade;    // Update Point
                               ready_text("READY",2);
                            }
                            clicked=false;
                        }
                    }if(moves==7){
                       SDL_BlitSurface(bac,NULL,gScreenSurface,NULL);
                       map<int,SDL_Surface*>::iterator i;
	for(  i = shapes.begin(); i!=shapes.end(); ++i)
	{
		SDL_FreeSurface( shapes[i->first]);
		shapes[i->first] = NULL;
	}
	ready_text("",3);

}
				}
                animate_grid(grid);
			}
		}
	}
	//Free resources and close SDL
	close();
	return 0;
}
