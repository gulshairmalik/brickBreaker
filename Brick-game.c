#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <stdbool.h>

SDL_Texture* brickt2;

struct Screen{
	int xMIN,xMAX;
	int yMIN,yMAX;
	int backgx,backgy;
};

struct Ball{
    int x;
    int y;
    int h;
    int w;
    int velX;
    int velY;
    int count_live;
    int start_chek;
};

struct bat{
	int x,y;
	int h;
	int w;
	int xv;
};

struct Brick{
	int x,y;
	int h;
	int w;
	int visible;
};

struct media{
	SDL_Window* gWindow ;
	SDL_Renderer* render;
	SDL_Surface *screen;
    Mix_Music *bat;
    Mix_Music *brick;
    Mix_Music *menum;
    bool quit;
    int pause;
    int win_count;
    bool menu;
    int menu_up_down;
    int gameplay;
    int score;
    char score_str[30];
};

struct textures{

    SDL_Texture* brickt;
    SDL_Texture* batt;
    SDL_Texture* ballt;
    SDL_Texture* backg;
    SDL_Texture* lives;
    SDL_Texture* game_over;
    SDL_Texture* pause;
    SDL_Texture* youwin;
    SDL_Texture* scoret;
};


void initBrick(struct Brick *br, int x, int y){
	br->x=x;
	br->y=y;
	br->h = 25;
	br->w = 70;
	br->visible=1;
}

void initbat(struct bat* bt){
	bt->x=275;//320;
	bt->y=490;
	bt->xv=8;
	bt->h = 25;
	bt->w = 100;
}

void initBall(struct Ball *b,struct bat* bt){
	/*b->x=250;
	b->y=380;*/
	b->x=bt->x+35;
    b->y=bt->y-35;
	b->velX=0;
	b->velY=0;
	b->h=30;
	b->w=30;
	b->count_live=4;
	b->start_chek=1;
}

void initScreen(struct Screen* s,struct media* m){
    SDL_Init(SDL_INIT_EVERYTHING);

    Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048);
    m->bat = Mix_LoadMUS("batcollision.wav");
    m->brick = Mix_LoadMUS("brickcollision.wav");
    m->menum = Mix_LoadMUS("menu.wav");



    s->xMAX=650;s->yMAX=520;s->xMIN=0;s->yMIN=0;
    s->backgx = 0;
    s->backgy = 0;
    m->pause=1;
    m->win_count=0;
    m->menu=true;
    m->menu_up_down=1;
    m->gameplay=0;
    m->score=0;

    m->gWindow=NULL;

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"1")){
    }

    m->gWindow = SDL_CreateWindow( "BRICK BREAKER", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, s->xMAX,s->yMAX, SDL_WINDOW_SHOWN );
    //SDL_SetWindowFullscreen(m->gWindow,SDL_WINDOW_FULLSCREEN);
    m->screen = SDL_GetWindowSurface(m->gWindow);
    m->render = SDL_CreateRenderer(m->gWindow,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawColor(m->render, 0xFF, 0xFF, 0xFF, 255 );
}

void moveBall(struct Ball *b){
	b->x+=b->velX;
	b->y+=b->velY;
}

void CollisionDetection(struct Ball *b, struct Screen* s,struct media* m, struct bat* bt){
	//Ball Collision with Screen
	if(b->y+30>=s->yMAX){
		initbat(bt);
		b->x=bt->x+35;
        b->y=bt->y-35;
        b->velX=0;
        b->velY=0;
        b->start_chek=1;
        b->count_live--;

	}
	if(b->y<=s->yMIN){
        b->velY=-b->velY;
	}
	if((b->x+30>=s->xMAX)||(b->x<=s->xMIN)){

		b->velX=-b->velX;

	}
	//Ball Collison with Bat
	if((b->y+30>=bt->y ) && (b->x+30>=bt->x && b->x<=bt->x+100) ){
		b->velY=-b->velY;
	    Mix_PlayMusic(m->bat,1);
    }
	if((b->x+30==bt->x) && (b->y+30>=bt->y && b->y<=bt->y+25) || (b->x<=bt->x+100) && (b->y+30==bt->y && b->y<=bt->y+25) ){
		b->velX=-b->velX;
		Mix_PlayMusic(m->bat,1);
	}

}

void collision_brick(struct Brick *br,struct Ball *b,struct media* m) {

	if((b->y<=br->y+br->h && b->y+b->h>=br->y+br->h) && (b->x+b->w>br->x && b->x<br->x+br->w)&&br->visible>0){
		b->velY = -b->velY;
		br->visible--;
		Mix_PlayMusic(m->brick,1);
		m->win_count++;
		m->score+=10;
	}
	else if((b->y+b->h>=br->y && b->y<=br->y) && (b->x+b->w>br->x && b->x<br->x+br->w)&&br->visible>0){
		b->velY = -b->velY;
		br->visible--;
		Mix_PlayMusic(m->brick,1);
		m->win_count++;
		m->score+=10;
	}
	else if((b->x<=br->x+br->w && b->x+b->w>=br->x+br->w) && (b->y+b->h>br->y && b->y<br->y+br->h)&&br->visible>0){
		b->velX = -b->velX;
		br->visible--;
		Mix_PlayMusic(m->brick,1);
		m->win_count++;
		m->score+=10;
	}
	else if((b->x+b->w>=br->x && b->x<=br->x)&&(b->y+b->h>br->y&&b->y<br->y+br->h)&&br->visible>0){
		b->velX = -b->velX;
		br->visible--;
		Mix_PlayMusic(m->brick,1);
		m->win_count++;
		m->score+=10;
	}

	/*if(((b->y<=br->y+br->h&&b->y+b->h>=br->y) && (b->x>=br->x && b->x<=br->x+br->w) && br->visible==1)  || ((b->y+b->h==br->y ) && (b->x>=br->x && b->x<=br->x+br->w) && br->visible==1 )){
		b->velY = -b->velY;
		br->visible = 0;
		Mix_PlayMusic(m->brick,1);
		m->win_count++;
		m->score+=10;
    }
	if((br->visible==1 && (b->x+b->w==br->x) && (b->y>=br->y+br->h && b->y+b->h<=br->y)) || (br->visible==1 && (b->x==br->x+br->w) && (b->y>=br->y && b->y+b->h<=br->y)) ){
		b->velX=-b->velX;
		br->visible = 0;
		Mix_PlayMusic(m->brick,1);
	}*/
}
void colbrick(struct Brick *br,struct Ball *b,struct media* m){
    int llpk;
    for(llpk=0;llpk<30;llpk++){
        collision_brick(br+llpk,b,m);
    }
}


void drawbrick_single(struct media* m,struct Brick* br,struct textures* t){

    SDL_Rect brick_pos;

    if(br->visible==1){

        brick_pos.x = br->x;
        brick_pos.y = br->y;
        brick_pos.h = br->h;
        brick_pos.w = br->w;
        SDL_RenderCopy(m->render, t->brickt, NULL, &brick_pos);
    }
    if(br->visible==2){

        brick_pos.x = br->x;
        brick_pos.y = br->y;
        brick_pos.h = br->h;
        brick_pos.w = br->w;
        SDL_RenderCopy(m->render, brickt2, NULL, &brick_pos);
    }
}

void drawbrick_array(struct Brick* br,struct media* m,struct textures* t){

    int k=0;
    for(k=0;k<30;k++){
        drawbrick_single(m,br+k,t);
    }

}

void make_brick_visible(int x,struct Brick* br){
    int k=0;
    for(k=0;k<30;k++){
    (br+k)->visible = x;
    }
}

void draw_everything(struct media* m,struct Brick* br,struct Ball* b,struct bat* bt,struct Screen* s,struct textures* t){

    SDL_Rect recball,recbat,recscreen,reclive1,reclive2,reclive3,Message_rect,scorerect;


    recball.x = b->x;
    recball.y = b->y;
    recball.h = b->h;
    recball.w = b->w;

    recbat.x = bt->x;
    recbat.y = bt->y;
    recbat.h = bt->h;
    recbat.w = bt->w;

    recscreen.x = s->backgx;
    recscreen.y = s->backgy;
    recscreen.h = s->yMAX;
    recscreen.w = s->xMAX;

    reclive1.x=536;
    reclive2.x=568;
    reclive3.x=600;
    reclive1.y=reclive2.y=reclive3.y=5;
    reclive1.w=reclive2.w=reclive3.w=30;
    reclive1.h=reclive2.h=reclive3.h=30;

    Message_rect.x = 800;
    Message_rect.y = 5;
    Message_rect.w = 20;
    Message_rect.h = 20;

    scorerect.x=585;
    scorerect.y=385;
    scorerect.h=30;
    scorerect.w=30;

	SDL_RenderClear(m->render);
	SDL_RenderCopy(m->render, t->backg, NULL, NULL);
	SDL_RenderCopy(m->render, t->ballt, NULL, &(recball));
	SDL_RenderCopy(m->render, t->batt, NULL, &(recbat));
	SDL_RenderCopy(m->render, t->scoret, NULL, &(scorerect));
	if(b->count_live==1||b->count_live==2||b->count_live==3){SDL_RenderCopy(m->render, t->lives, NULL, &(reclive1));}
	if(b->count_live==2||b->count_live==3){SDL_RenderCopy(m->render, t->lives, NULL, &(reclive2));}
	if(b->count_live==3){SDL_RenderCopy(m->render, t->lives, NULL, &(reclive3));}
	if(b->count_live==0){
        SDL_RenderCopy(m->render,t->game_over,NULL,NULL);
        bt->xv=0;
        b->velX=0;
        b->velY=0;

	}
	if(m->pause==0){SDL_RenderCopy(m->render,t->pause,NULL,NULL);}
	if(m->win_count==30){
    int l_un_lives = b->count_live;
	initbat(bt);
	initBall(b,bt);
	b->count_live = l_un_lives;
	b->start_chek=1;
	m->win_count=31;
	make_brick_visible(2,br);
	}
	if(m->win_count == 91){
        SDL_RenderCopy(m->render,t->youwin,NULL,NULL);
        initbat(bt);
        initBall(b,bt);
        b->start_chek=1;
        b->count_live=0;
        m->win_count=0;
	}
	drawbrick_array(br,m,t);
	SDL_RenderPresent(m->render);

}

void creatsurfaces(struct media* m,struct textures* t){

    SDL_Surface* img1 = SDL_LoadBMP("1.bmp");
    SDL_Surface* img2 = SDL_LoadBMP("r3.bmp");
    SDL_Surface* img3 = SDL_LoadBMP("r1.bmp");
    SDL_Surface* img4 = SDL_LoadBMP("backg.bmp");
    SDL_Surface* img5 = SDL_LoadBMP("lives.bmp");
    SDL_Surface* img6 = SDL_LoadBMP("gameover.bmp");
    SDL_Surface* img7 = SDL_LoadBMP("Pause.bmp");
    SDL_Surface* img8 = SDL_LoadBMP("win.bmp");
    SDL_Surface* img9 = SDL_LoadBMP("r2.bmp");


    Uint32 colorkey = SDL_MapRGB(img1->format,0,0,0);
    SDL_SetColorKey(img1,1,colorkey);
    Uint32 colorkey2 = SDL_MapRGB(img2->format,255,255,255);
    SDL_SetColorKey(img2,1,colorkey2);
    //SDL_SetColorKey(img3,1,colorkey2);
    SDL_SetColorKey(img5,1,colorkey2);
    SDL_SetColorKey(img6,1,colorkey2);
    SDL_SetColorKey(img7,1,colorkey2);
    SDL_SetColorKey(img8,1,colorkey2);

    t->ballt = SDL_CreateTextureFromSurface(m->render, img1);
    t->batt = SDL_CreateTextureFromSurface(m->render, img2);
    t->brickt = SDL_CreateTextureFromSurface(m->render, img3);
    t->backg = SDL_CreateTextureFromSurface(m->render, img4);
    t->lives = SDL_CreateTextureFromSurface(m->render, img5);
    t->game_over = SDL_CreateTextureFromSurface(m->render, img6);
    t->pause = SDL_CreateTextureFromSurface(m->render, img7);
    t->youwin = SDL_CreateTextureFromSurface(m->render, img8);
    brickt2 = SDL_CreateTextureFromSurface(m->render,img9);

    SDL_FreeSurface(img1);
    SDL_FreeSurface(img2);
    SDL_FreeSurface(img3);
    SDL_FreeSurface(img4);
    SDL_FreeSurface(img5);
    SDL_FreeSurface(img6);
    SDL_FreeSurface(img7);
    SDL_FreeSurface(img8);
    SDL_FreeSurface(img9);
}

void Load_Score(struct media* m,struct textures* t){
	SDL_Color color1 = {208,0,0};
	TTF_Init();
    TTF_Font* font = TTF_OpenFont("font1.ttf",35);
	itoa(m->score,m->score_str,10);
    SDL_Surface* scoretext = TTF_RenderText_Blended(font,m->score_str,color1);
    t->scoret = SDL_CreateTextureFromSurface(m->render, scoretext);
    SDL_FreeSurface(scoretext);
    TTF_CloseFont(font);
    font = NULL;
}

void loadmenu(struct media* m ,struct textures* t){
	SDL_Surface* img9 = SDL_LoadBMP("Menu.bmp");
    SDL_Surface* img10 = SDL_LoadBMP("Menu1.bmp");

    SDL_Texture* menut = SDL_CreateTextureFromSurface(m->render, img9);
    SDL_Texture* menut1 = SDL_CreateTextureFromSurface(m->render, img10);

    SDL_FreeSurface(img9);
    SDL_FreeSurface(img10);

    SDL_RenderClear(m->render);
    if(m->menu_up_down==1){
    SDL_RenderCopy(m->render,menut,NULL,NULL);
    }
    else if(m->menu_up_down==0){
    SDL_RenderCopy(m->render,menut1,NULL,NULL);
    }
    SDL_RenderPresent(m->render);

    SDL_DestroyTexture(menut);
    SDL_DestroyTexture(menut1);
}

void Destroy_Everything(struct textures* t,struct media* m){
	SDL_DestroyTexture(t->backg);
    SDL_DestroyTexture(t->ballt);
	SDL_DestroyTexture(t->batt);
	SDL_DestroyTexture(t->brickt);
	SDL_DestroyTexture(t->game_over);
	SDL_DestroyTexture(t->lives);
	SDL_DestroyTexture(t->pause);
	SDL_DestroyTexture(t->youwin);
	SDL_DestroyTexture(t->scoret);
	Mix_CloseAudio();
	Mix_FreeMusic(m->bat);
	Mix_FreeMusic(m->brick);
	Mix_FreeMusic(m->menum);

	SDL_DestroyWindow(m->gWindow);
	SDL_DestroyRenderer(m->render);
    SDL_Quit();
}


void Event(struct bat* bt, struct Ball* b,struct media* m, struct Screen* s,struct Brick* br){
	SDL_Event e;
	while( SDL_PollEvent( &e ) != 0 )
    {
        //User requests quit
        if( e.type == SDL_QUIT ){
            m->quit = true;
            }
        // GAME PAUSE
		else if(e.type==SDL_KEYDOWN){

       	    if ( e.key.keysym.sym==SDLK_p && m->pause==1){
        	m->pause=0;
		    }
            else if ( e.key.keysym.sym==SDLK_p && m->pause==0){
        	m->pause=1;
		    }
		    else if(e.key.keysym.sym==SDLK_SPACE && b->start_chek==1&&b->x == bt->x+35&&b->y == bt->y-35){
            b->velX=4;
            b->velY=4;
            b->start_chek=0;
		    }
		    // Start Menu
		    else if(e.key.keysym.sym==SDLK_DOWN && (m->menu_up_down==1 && m->menu==true)){
            m->menu_up_down=0;
            Mix_PlayMusic(m->menum,1);
		    }
		    else if(e.key.keysym.sym==SDLK_UP && (m->menu_up_down==0 && m->menu==true)){
            m->menu_up_down=1;
            Mix_PlayMusic(m->menum,1);
		    }
		    else if(e.key.keysym.sym==SDLK_RETURN && (m->menu_up_down==1)){
            m->menu=false;
            m->gameplay=1;
		    }
		    else if(e.key.keysym.sym==SDLK_RETURN && (m->menu_up_down==0)){
            m->quit=true;
		    }
		    else if(e.key.keysym.sym==SDLK_ESCAPE && (m->menu==false && m->gameplay==1)){
            m->menu=true;
		    }
		    else if(e.key.keysym.sym==SDLK_ESCAPE && (m->menu==true && m->gameplay==1)){
            m->menu=false;
		    }
		    // Start Again
		    else if((e.key.keysym.sym==SDLK_LSHIFT || e.key.keysym.sym==SDLK_RSHIFT) && (b->count_live==0)){
                    b->count_live=3;
                    b->start_chek=1;
                    m->win_count=0;
                    initbat(bt);
                    b->x=bt->x+35;
					b->y=bt->y-35;
					m->score=0;
					make_brick_visible(1,br);

		    }
        }
    }

}

void movebat(struct bat* bt,struct Ball* b,struct Screen* s){
	const Uint8 *keyState;
	keyState = SDL_GetKeyboardState(NULL);
	if ( keyState[SDL_SCANCODE_RIGHT] && bt->x+110<=s->xMAX){
        	bt->x+=bt->xv;
        	if(b->start_chek==1){
            b->x = bt->x+35;
        }
		}
		 if ( keyState[SDL_SCANCODE_LEFT] && bt->x-10>=s->xMIN){
        	bt->x-=bt->xv;
            if(b->start_chek==1){
            b->x = bt->x+35;
        	}
		}
}


int main(int argc, char *argv[]) {
	struct Ball myBall;
	struct Screen myScreen;
	struct media* m;
	struct textures* t;
	struct bat bt;
	struct rectangles* r;

	struct Brick br[30];
	int x;
	int y;
	int i=0;
    for(y=40;y<240;y+=40){
    	for(x=20;x<620;x+=100){
    		initBrick(&br[i],x,y);
    		i++;
    		}
        }

	initBall(&myBall,&bt);
	initbat(&bt);
	initScreen(&myScreen,m);
	m->quit = false;
	creatsurfaces(m,t);

	while(!m->quit){
		if(m->menu==true){
		loadmenu(m,t);
	    }
		Event(&bt,&myBall,m,&myScreen,br);
		if(m->menu==false){
		if(m->pause==1){
		moveBall(&myBall);
		movebat(&bt,&myBall,&myScreen);
	    }

		draw_everything(m,&br[0],&myBall,&bt,&myScreen,t);
		CollisionDetection(&myBall,&myScreen,m,&bt);
		Load_Score(m,t);
		colbrick(br,&myBall,m);
	    }
    }
    Destroy_Everything(t,m);

return 0;
}
