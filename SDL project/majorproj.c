#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <unistd.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

//Game Constants
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define MOVING_SPEED 8
#define VELOCITY_X  7 
#define VELOCITY_Y  7 
#define ENEMY_SIZE  100
#define ZOMBIE_VELOCITY 5 
#define WITCH_VELOCITY  4 
#define BOSS_VELOCITY  3 
#define MAX_ENEMIES  10 
#define boss_probability  10 
#define witch_probability  30 
#define zombie_probability  60 
#define ZOMBIE_HP  200
#define ZOMBIE_DMG  0.2 
#define WITCH_HP  400
#define WITCH_DMG   1
#define BOSS_HP   600 
#define BOSS_DMG   2 
#define BOX_SIZE   30 
#define DOOR_HEIGHT 30 
#define DOOR_WIDTH 80 
#define DOOR2_HEIGHT 80 
#define DOOR2_WIDTH 30 
#define ZOMBIE_REWARD 100 
#define WITCH_REWARD 200
#define BOSS_REWARD 300


#define MAX_NUMBER_OF_TEXT_BOXES 8
#define SPACING_BETWEEN_TEXT_BOXES 36
#define LOCATION_ADJUSTMENT 120
#define MAX_INPUT_LENGTH 15

#define MAX_BULLETS 30
#define BULLET_SIZE 10
#define BULLET_SPEED 10
#define PLAYER_SIZE 100
#define MENU_AUDIO_PATH "menutheme.mp3"
#define GAME_AUDIO_PATH "game.mp3"
const int BULLET_DMG=10;

typedef struct {
    char *options[MAX_NUMBER_OF_TEXT_BOXES];
    char imageFilePath[50];
    bool networkFunctionality;
} Menu;

typedef struct {
    float x, y;
    float dx, dy;
    int active;
} Bullet;

Bullet bullets[MAX_BULLETS] = {0};
int ammo = 100;
int lastDirectionX = 0;
int lastDirectionY = 1;

typedef struct{
    int R,G,B;
    float Value;
    int Max;
    SDL_Texture *heart;
    SDL_Rect box;
}Health;

typedef struct{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;
    int up;
    int down;
    int right;
    int left;
    int fire;
    int GO;
    int menuState;
    int goBackToMenu;
    int isPaused;
    int quit;
    SDL_Texture *bg;
    TTF_Font *font;
    SDL_Color white;
}App;

typedef struct{
    // For Both Player And Enemies
    int x,y;
    int dx;
    Health health;
    int CurrentSprite;
    int lastDirectionX;
    int lastDirectionY;
    SDL_Texture *ch[4];
    int points;
    //Enemy Specific
    char type;
    int hp;
    int speed;
    float dmg;
    int reward;
    int isAlive;
}Entity;

typedef struct{
    Entity Player;
    Entity Zombie;
    Entity Witch;
    Entity Boss;
}EntityType;

typedef struct{
  int x;
  int y;
  int show;
}TreasureBox;

App app;
Entity player1;
Entity enemies[MAX_ENEMIES];
TreasureBox box;
int enemyCount = 0;
int counter_x=0;
int counter_y=0;

int done = 0; // Checks If For Quitting Condition


void Initialize(void);
void RenderGraphics(Uint32 *lastSpawnTime);
int ProcessEvent(void);
void KeyDownEvent(SDL_KeyboardEvent *event);
void KeyUpEvent(SDL_KeyboardEvent *event);
void BackgroundCreator(void);
void PlayerControl(void);
void PlayerSpriteCreator(void);
void HealthSystem(void);
void GameOverEvent(void);
void RenderEnemySprites(void);
void RenderTreasureBox(void);
void generateEnemy(int *x, int *y, char* type, int *hp, float *dmg, int *speed, int *reward);
void moveEnemy(int player1_x, int player1_y,Entity *enemy, int *enemy_x, int *enemy_y, int *speed, int enemyCount, int currentEnemyIndex);
void EnemyToPlayerDamage(void);
int checkCollision(SDL_Rect A, SDL_Rect B);
void GameOverSystem(void);
int isWalkingSoundPlaying(void);
void treasureDrop();
void generateTreasureBox(void);
void boxClaimAndCheckCollision(void);
// SDL_Texture* loadTexture(const char* path);
void renderDoors(Uint32 *lastSpawnTime);
void checkDoorCollision(void);
int checkCollision(SDL_Rect A, SDL_Rect B);
void manageMenu(void);
static int displayOptions(Menu *menu,int *done);
static void handleMenuEntry(int *scene,int *done);
static void destroyBoxTextures(Menu *menu, SDL_Texture *textBoxTexture[]);
static void prepareTextBoxesToBeShown(Menu *menu, SDL_Rect textBoxRectangle[], SDL_Texture *textBoxTexture[]);
static void handlePauseOption(int *scene);
void RenderBullets();
void Shoot();
void UpdateBullets();
void UpdateEnemies(Entity enemies[]);
void ScreenShake(int *x,int *y);

// Function To generate random numbers with an upper bound
int randomnumgen(int max){
    srand(time(NULL));
    return rand() % max;
}

// Function for printing errors 
void Error(const char *msg){
    perror(msg);
    exit(1);
}

int backgroundPosX = 0;
int backgroundPosY = 0;
int bgPosX =0;
int bgPosY=0;

float shakeDuration = 0.0f;
float shakeMagnitude = 3.0f; // Adjust this value to control the intensity of the shake
float shakeTime = 0.5f; // Duration of the shake in seconds

// int sockfd,clientsock;
// struct sockaddr_in HostAddr;
// socklen_t joinSockLen;

int main(int argc, char* argv[]){

    Uint32 lastSpawnTime = SDL_GetTicks();
    Uint32 lastboxSpawnTime = SDL_GetTicks();
    Uint32 spawnInterval = 5000;
    Uint32 TreasureBoxInterval = 10000;

    Initialize(); // Initializes Everything
    // initializeAudio();

    while(!done){
        if(app.menuState){
           manageMenu();
        }
        else{
            //changeThemeSong();
            done = ProcessEvent(); // Processes Inputs
            PlayerControl(); // Conditions For Moving
            GameOverSystem();
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastSpawnTime >= spawnInterval && enemyCount < MAX_ENEMIES) {
                generateEnemy(&enemies[enemyCount].x, &enemies[enemyCount].y, &enemies[enemyCount].type, &enemies[enemyCount].hp, &enemies[enemyCount].dmg,&enemies[enemyCount].speed,&enemies[enemyCount].reward);
                enemyCount++;
                lastSpawnTime = currentTime;
            }
            if (currentTime - lastboxSpawnTime >= TreasureBoxInterval) {
            generateTreasureBox();
            lastboxSpawnTime = currentTime;
            box.show=1;
        }
            UpdateBullets();
            UpdateEnemies(enemies);
            EnemyToPlayerDamage();
            checkDoorCollision();
            boxClaimAndCheckCollision(); 
            RenderGraphics(&lastSpawnTime); //Draws on screen
        }
    }
}

//MAIN FUNCTIONS

void Initialize(void){

    //Initial Spawn Coordinates
    player1.x = SCREEN_WIDTH/2;
    player1.y = SCREEN_HEIGHT/2;
    player1.dx = MOVING_SPEED; // Moving speed
    player1.health.G = 255;
    player1.health.R = 0;
    player1.health.B = 0;
    player1.health.Max = 100;
    player1.health.Value = player1.health.Max;
    player1.CurrentSprite = 0;
    player1.points = 0;
    app.GO = 0;
    app.menuState = 1;
    app.goBackToMenu = 0;
    app.isPaused = 0;

    for(int i=0;i<MAX_ENEMIES;i++){
      enemies[i].isAlive=1;
    }

    int windowFlag,rendererFlag; // Flags

    windowFlag = 0;
    rendererFlag = SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC;

    // Initialize SDL_ttf
    TTF_Init();

    // Load a font
    app.font = TTF_OpenFont("resources/font.ttf", 24);  // Load a 24pt font

    // Check for errors
    if (!app.font) {
        printf("Error loading font: %s\n", TTF_GetError());
        // Handle error
    }

    TTF_SetFontStyle(app.font, TTF_STYLE_BOLD);

    // Set default color
    app.white.r = 0;
    app.white.g = 0;
    app.white.b = 0;
    app.white.a = 255;

    //Initializing SDL2
    if(SDL_Init(SDL_INIT_VIDEO)<0){

        printf("Couldn't Initialize SDL2.");
        exit(1);
    }
    //Initializing Images
    if(IMG_Init(IMG_INIT_PNG)!=IMG_INIT_PNG){

        printf("Couldn't Initialize SDL2_IMG.");
        exit(1);
    }
    //Creating window
    app.window = SDL_CreateWindow("GameWindow",
    SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,
    SCREEN_WIDTH,SCREEN_HEIGHT,
    windowFlag);

    if(!app.window){
        printf("Couldn't Create A Window.");
        exit(1);
    }
    //creating renderer
    app.renderer = SDL_CreateRenderer(app.window,
    -1,
    rendererFlag);

    if(!app.renderer){
        printf("Couldn't Create A Renderer.");
        exit(1);
    }

}

void RenderGraphics(Uint32 *lastSpawnTime){

    if(!app.GO){
        SDL_RenderClear(app.renderer);
        BackgroundCreator();
        PlayerSpriteCreator();
        RenderBullets();
        for (int i = 0; i < enemyCount; ++i) {
            moveEnemy(player1.x, player1.y,enemies, &enemies[i].x, &enemies[i].y, &enemies[i].speed,enemyCount,i);
        }
        RenderTreasureBox();
        RenderEnemySprites();
        HealthSystem();
        renderDoors(lastSpawnTime);
        SDL_RenderPresent(app.renderer);
        //SDL_Delay(16);
    }
    else{
        GameOverEvent();
    }

    SDL_Delay(16);
}

int ProcessEvent(void){

    SDL_Event event;
    int done = 0;

        while(SDL_PollEvent(&event)){
            switch(event.type){

                case SDL_QUIT:
                    done = 1;
                    break;

                case SDL_WINDOWEVENT_CLOSE:
                    if(app.window){
                        SDL_DestroyWindow(app.window);
                        app.window = NULL;
                        done = 1;
                    }
                    break;

                case SDL_KEYDOWN:
                    KeyDownEvent(&event.key);
                    switch(event.key.keysym.sym){
                        
                        case SDLK_1:
                            player1.CurrentSprite = 0;
                            break;
                        
                        case SDLK_2:
                            player1.CurrentSprite = 1;
                            break;

                        case SDLK_3:
                            player1.CurrentSprite = 2;
                            break;

                        case SDLK_4:
                            if(player1.health.Value<=0){
                                GameOverEvent();
                            }
                            else{
                                player1.health.Value -= 10;
                            }
                            break;

                        case SDLK_SPACE:
                            Shoot();
                            break;
                        case SDLK_ESCAPE:
                            app.menuState = 1; // Sets the state MenuState to true
                            break;

                        default:
                            break;
                    }
                    break;

                case SDL_KEYUP:
                    KeyUpEvent(&event.key);
                    break;
               }
        }

        return done;
}

void KeyDownEvent(SDL_KeyboardEvent *event){

    if(event->repeat == 0){

        if(event->keysym.scancode == SDL_SCANCODE_W){
            app.up = 1;
            lastDirectionX = 0;
            lastDirectionY = -1;
        }

        if(event->keysym.scancode == SDL_SCANCODE_S){
            app.down = 1;
            lastDirectionX = 0;
            lastDirectionY = 1;
        }

        if(event->keysym.scancode == SDL_SCANCODE_A){
            app.left = 1;
            lastDirectionX = -1;
            lastDirectionY = 0;
        }

        if(event->keysym.scancode == SDL_SCANCODE_D){
            app.right = 1;
            lastDirectionX = 1;
            lastDirectionY = 0;
        }

        if(event->keysym.scancode == SDL_SCANCODE_SPACE){
            app.fire = 1;
        }
    }
}

void KeyUpEvent(SDL_KeyboardEvent *event){

    if(event->repeat == 0){

        if(event->keysym.scancode == SDL_SCANCODE_W){
            app.up = 0;
        }

        if(event->keysym.scancode == SDL_SCANCODE_S){
            app.down = 0;
        }

        if(event->keysym.scancode == SDL_SCANCODE_A){
            app.left = 0;
        }

        if(event->keysym.scancode == SDL_SCANCODE_D){
            app.right = 0;
        }

        if(event->keysym.scancode == SDL_SCANCODE_SPACE){
            app.fire = 0;
        }
    }
}



void BackgroundCreator(){
    SDL_Surface* map;
    if(counter_x==0){
        if(counter_y==0){
            map=IMG_Load("resources/mainimg.png");
        }
        else if(counter_y==1){
            map=IMG_Load("resources/bottom.png");
        }
        else{
            map=IMG_Load("resources/top.png");
        }
    }
    else if(counter_x==-1){
        map=IMG_Load("resources/left.png");
    }
    else if(counter_x==1){
        map=IMG_Load("resources/right.png");
    }

    //SDL_Surface *map = IMG_Load("bg.png");
    if(map == NULL){
        printf("Failed to load sprite image: %s\n", IMG_GetError());
        SDL_DestroyTexture(app.bg);
        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(app.window);
        IMG_Quit();
        SDL_Quit();
        exit(1);
    }
    app.bg = SDL_CreateTextureFromSurface(app.renderer,map);
    if(app.bg == NULL){
        printf("Failed to load sprite image: %s\n", IMG_GetError());
        SDL_DestroyTexture(app.bg);
        SDL_DestroyRenderer(app.renderer);
        SDL_DestroyWindow(app.window);
        IMG_Quit();
        SDL_Quit();
        exit(1);
    }
    SDL_FreeSurface(map);

    SDL_Rect bgRect = { bgPosX, bgPosY, SCREEN_WIDTH,SCREEN_HEIGHT}; // Adjust width and height accordingly
    SDL_RenderCopy(app.renderer, app.bg, NULL, &bgRect);
}

void PlayerSpriteCreator(void){

    char temp[100];
    // Assigning Diff sprites to each texture
    for(int i = 0;i<4;i++){
        sprintf(temp,"resources/c%d.png",i+1);
        player1.ch[i] = SDL_CreateTextureFromSurface(app.renderer,IMG_Load(temp));
        if(player1.ch[i] == NULL){
            printf("File 'resources/c%d.png',not found !\n",i);
            for(int j = 0;j<i;j++){
                SDL_DestroyTexture(player1.ch[j]);
            }
            exit(1);
        }
    }

    SDL_Rect PlayerHitBox = {player1.x,player1.y,100,100};
    SDL_RenderCopy(app.renderer,player1.ch[player1.CurrentSprite],NULL,&PlayerHitBox);
}

void PlayerControl(void){

    if(app.up){
        player1.y -= player1.dx;
    }

    if(app.down){
        player1.y += player1.dx;
    }

    if(app.right){
        player1.x += player1.dx;
    }

    if(app.left){
        player1.x -= player1.dx;
    }
}

void HealthSystem(void){

    if((75<player1.health.Value)&&(player1.health.Value<=100)){
        player1.health.G = 255;
        player1.health.R = 0;
        player1.health.B = 0;
    }
    else if((50<player1.health.Value)&&(player1.health.Value<=75)){
        player1.health.G = 255;
        player1.health.R = 255;
        player1.health.B = 0;
    }
    else if((25<player1.health.Value)&&(player1.health.Value<=50)){
        player1.health.G = 165;
        player1.health.R = 255;
        player1.health.B = 0;
    }
    else if((0<=player1.health.Value)&&(player1.health.Value<=25)){
        player1.health.G = 0;
        player1.health.R = 255;
        player1.health.B = 0;
    }

    SDL_Surface *surface = IMG_Load("resources/heart.png");
    player1.health.box.x = 0;
    player1.health.box.y = SCREEN_HEIGHT-65;
    player1.health.box.w = 100;
    player1.health.box.h = 50;
    player1.health.heart = SDL_CreateTextureFromSurface(app.renderer,surface);
    
    
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
    SDL_Rect HealthBarBorder = {SCREEN_WIDTH/15-50,SCREEN_HEIGHT-50,200,20};
    SDL_RenderFillRect(app.renderer,&HealthBarBorder);

    SDL_SetRenderDrawColor(app.renderer, player1.health.R, player1.health.G, player1.health.B, 255); 
    SDL_Rect HealthBar = {SCREEN_WIDTH/15-50+10,SCREEN_HEIGHT-50+10,player1.health.Value*2-2*10,15-2*10};
    SDL_RenderFillRect(app.renderer,&HealthBar);

    SDL_RenderCopy(app.renderer,player1.health.heart,NULL,&player1.health.box);

}

void GameOverEvent(void){
    SDL_Delay(1000);
    //app.GO = 1;
    SDL_Texture *GameOver;

    SDL_Rect GOBOX = {0,0,1280,720};

    SDL_Surface *GOSprite = IMG_Load("resources/gameover.png");

    GameOver = SDL_CreateTextureFromSurface(app.renderer,GOSprite);

    SDL_RenderCopy(app.renderer,GameOver,NULL,&GOBOX);
    SDL_RenderPresent(app.renderer);
    SDL_Delay(10000);
    SDL_DestroyWindow(app.window);
    SDL_DestroyRenderer(app.renderer);
}

void RenderEnemySprites(void){
    // Rendering
        for (int i = 0; i < enemyCount; ++i) {
            SDL_Texture *temp;
            if(enemies[i].type=='z'){ 
                SDL_Texture *Zombie = SDL_CreateTextureFromSurface(app.renderer,IMG_Load("resources/en1.png"));
                temp = Zombie;
            } else if(enemies[i].type=='w'){
               SDL_Texture *Witch = SDL_CreateTextureFromSurface(app.renderer,IMG_Load("resources/en2.png"));
               temp = Witch;
            } else if(enemies[i].type=='b'){
                SDL_Texture *Boss = SDL_CreateTextureFromSurface(app.renderer,IMG_Load("resources/en3.png"));
                temp = Boss;
            } 
            if(enemies[i].isAlive==1){
                SDL_Rect enemyRect = {enemies[i].x, enemies[i].y, ENEMY_SIZE,ENEMY_SIZE};
                SDL_RenderCopy(app.renderer,temp,NULL,&enemyRect);
            }
            else{
                enemies[i].dmg = 0;
            }
            

        }
}

void generateEnemy(int *x, int *y, char* type, int *hp, float *dmg, int *speed, int *reward) {
    int randomno = rand() % 101; // Generate a random number between 0 and 100
    int spawnside=rand()%4+1;
    if(spawnside==1){
        *x =0;
        *y = rand() % (SCREEN_HEIGHT - ENEMY_SIZE);
    }
    else if(spawnside==2){
        *x = rand() % (SCREEN_WIDTH - ENEMY_SIZE);
        *y = 0;
    }
    else if(spawnside==3){
        *x = SCREEN_WIDTH;
        *y = rand() % (SCREEN_HEIGHT - ENEMY_SIZE);
    }
    else{
        *x = rand() % (SCREEN_WIDTH - ENEMY_SIZE);
        *y =SCREEN_HEIGHT;
    }
    


    if (randomno <= zombie_probability) {
        *type = 'z';
        *hp = ZOMBIE_HP;
        *dmg = ZOMBIE_DMG;
        *reward=ZOMBIE_REWARD;
        *speed=ZOMBIE_VELOCITY;
    } else if (randomno <= (zombie_probability + witch_probability)) {
        *type = 'w';
        *hp = WITCH_HP;
        *dmg = WITCH_DMG;
        *reward=WITCH_REWARD;
        *speed=WITCH_VELOCITY;
    } else {
        *type = 'b';
        *hp = BOSS_HP;
        *dmg = BOSS_DMG;
        *reward=BOSS_REWARD;
        *speed=BOSS_VELOCITY;
    }
}

void moveEnemy(int player1_x,int player1_y,Entity *enemy, int *enemy_x, int *enemy_y, int *speed, int enemyCount, int currentEnemyIndex) {
    // Move towards the player
    int dx1=player1_x - *enemy_x;
    int dy1=player1_y-*enemy_y;

  int dist1=(dx1*dx1)+(dy1*dy1);
  int direction_x, direction_y;

    direction_x = (dx1 > 0) ? 1 : -1;
    direction_y = (dy1 > 0) ? 1 : -1;
    
    *enemy_x += *speed * direction_x;
    *enemy_y += *speed * direction_y;

    // // Check collision with player
    // SDL_Rect enemyRect = {*enemy_x, *enemy_y, ENEMY_SIZE, ENEMY_SIZE};
    // SDL_Rect playerRect = {player2.x, player2.y, PLAYER_SIZE, PLAYER_SIZE}; // Assuming player size is 64x64

    // if(checkCollision(enemyRect, playerRect)){
    //     player2.health.Value -= enemies[currentEnemyIndex].dmg;
    //     *speed = 1; // Stops the enemy
    // }
    // else{
    //     *speed = enemies[currentEnemyIndex].speed;
    // }
}

void EnemyToPlayerDamage(void){
    SDL_Rect playerRect = {player1.x, player1.y, PLAYER_SIZE, PLAYER_SIZE}; // Assuming player size is 64x64

    for(int i = 0; i < MAX_ENEMIES; i++){
        SDL_Rect enemyRect = {enemies[i].x, enemies[i].y, ENEMY_SIZE, ENEMY_SIZE};

        if(checkCollision(playerRect, enemyRect)){
            ScreenShake(&bgPosX,&bgPosY);
            if(player1.health.Value - enemies[i].dmg <= 0){
                player1.health.Value = 0; // Ensure health doesn't go negative
            } else {
                player1.health.Value -= enemies[i].dmg;
            }
        }
    }
}

int checkCollision(SDL_Rect A, SDL_Rect B) {
    // The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    // Calculate the sides of rectangle A
    leftA = A.x;
    rightA = A.x + A.w;
    topA = A.y;
    bottomA = A.y + A.h;

    // Calculate the sides of rectangle B
    leftB = B.x;
    rightB = B.x + B.w;
    topB = B.y;
    bottomB = B.y + B.h;

    // Check collision
    if (bottomA <= topB) return 0; // No collision
    if (topA >= bottomB) return 0; // No collision
    if (rightA <= leftB) return 0; // No collision
    if (leftA >= rightB) return 0; // No collision

    // Collision detected
    return 1;
}

void GameOverSystem(void){
    if(player1.health.Value <= 0){
        SDL_Rect PlayerHitBox = {player1.x,player1.y,100,100};
        SDL_RenderCopy(app.renderer,player1.ch[3],NULL,&PlayerHitBox);
        SDL_RenderPresent(app.renderer);
        printf("DeD :(\n");
        app.GO = 1;
    }
}

void treasureDrop(){
    int n=randomnumgen(3);
    if(n==0){
        if(player1.health.Value- player1.health.Value>=5){
            player1.health.Value+=5;
        }else{
            player1.health.Value+=(player1.health.Value- player1.health.Value);
        }
    }else if(n==1){
        player1.health.Value-=2;
    }else{
        ammo+=100;
    }
    return;
}

void generateTreasureBox() {
    box.x = rand() % (SCREEN_WIDTH - ENEMY_SIZE);
    box.y = rand() % (SCREEN_HEIGHT - ENEMY_SIZE);
    return;
}

void RenderTreasureBox(){
   if(box.show==1){
            SDL_Texture* tr = SDL_CreateTextureFromSurface(app.renderer, IMG_Load("resources/treasurebox.png"));
            SDL_Rect TreasureBox = {box.x, box.y, 40, 40};
            SDL_RenderCopy(app.renderer, tr, NULL, &TreasureBox);         
        }
}

void boxClaimAndCheckCollision(void){
const Uint8* keystates=SDL_GetKeyboardState(NULL);
    if (abs(player1.x - box.x) <= 80 && abs(player1.y - box.y) <= 80 && box.show==1) {
    SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255); 
    SDL_Rect option = {box.x+ 5, box.y - 20, 20, 20};
    SDL_RenderFillRect(app.renderer, &option);
    
    if (keystates[SDL_SCANCODE_F]) {
        treasureDrop();
        box.show = 0;
    }

    if (player1.x <= box.x + BOX_SIZE && player1.x + 64 >= box.x &&
    player1.y<= box.y + BOX_SIZE && player1.y +64 >= box.y) {
    if (keystates[SDL_SCANCODE_W] && player1.y > box.y- 64)
        player1.y = box.y + BOX_SIZE;
    if (keystates[SDL_SCANCODE_A] && player1.x > box.x - 64)
        player1.x = box.x + BOX_SIZE;
    if (keystates[SDL_SCANCODE_S] && player1.y < box.y + BOX_SIZE)
        player1.y = box.y - 64;
    if (keystates[SDL_SCANCODE_D] && player1.x< box.x + BOX_SIZE)
        player1.x = box.x - 64;
}
}
}

void renderDoors(Uint32 *lastSpawnTime){
    const Uint8 *keystates=SDL_GetKeyboardState(NULL);
     SDL_Texture* door_horizontal = SDL_CreateTextureFromSurface(app.renderer, IMG_Load("resources/door_horizontal.png"));
    SDL_Texture* door_vertical = SDL_CreateTextureFromSurface(app.renderer, IMG_Load("resources/door_vertical.png"));
    // Render horizontal doors
    if (counter_y != -1) {
        SDL_Rect horizdoor={(SCREEN_WIDTH-DOOR_WIDTH)/2,0,80,40};
        SDL_RenderCopy(app.renderer, door_horizontal, NULL, &horizdoor);
    }

    // Render vertical doors
    if (counter_x != -1) {
         SDL_Rect vertdoor={0,(SCREEN_HEIGHT-DOOR2_HEIGHT)/2,40,80};
        SDL_RenderCopy(app.renderer, door_vertical, NULL, &vertdoor);
    }
    if (counter_x != 1) {
         SDL_Rect vertdoor={SCREEN_WIDTH-DOOR2_WIDTH,(SCREEN_HEIGHT-DOOR2_HEIGHT)/2,40,80};
        SDL_RenderCopy(app.renderer,door_vertical , NULL, &vertdoor);
    }
    if (counter_y != 1) {
         SDL_Rect horizdoor={(SCREEN_WIDTH-DOOR_WIDTH)/2,SCREEN_HEIGHT-DOOR_HEIGHT,80,40};
        SDL_RenderCopy(app.renderer, door_horizontal, NULL, &horizdoor);
    }

    // Render interactable doors
    // Door 1 (top)
    if (player1.x <= 70 && player1.y >= (SCREEN_HEIGHT - DOOR2_HEIGHT) / 2 - 64 && player1.y <= (SCREEN_HEIGHT + DOOR2_HEIGHT + 64) / 2 && counter_x != -1) {
        SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 255);
        SDL_Rect d1 = {30, (SCREEN_HEIGHT - 20) / 2, 20, 20};
        SDL_RenderFillRect(app.renderer, &d1);
        if (keystates[SDL_SCANCODE_G] && player1.points >= 1000) {
            player1.points -= 1000;
            counter_x = (counter_x - 1);
            counter_y = 0;
            BackgroundCreator();
            player1.x = SCREEN_WIDTH - DOOR2_WIDTH - 64 - 60;
            enemyCount = 0;
            box.show = 0;
            *lastSpawnTime = SDL_GetTicks();
        }
    }

    // Door 2 (left)
    if (player1.y <= 70 && player1.x >= (SCREEN_WIDTH - DOOR_WIDTH) / 2 - 64 && player1.x <= (SCREEN_WIDTH + DOOR_WIDTH + 64) / 2 && counter_y != -1) {
        SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 255);
        SDL_Rect d2 = {(SCREEN_WIDTH - 20) / 2, DOOR_HEIGHT, 20, 20};
        SDL_RenderFillRect(app.renderer, &d2);
        if (keystates[SDL_SCANCODE_G] && player1.points >= 1000) {
            player1.points -= 1000;
            counter_y = (counter_y - 1);
            counter_x = 0;
            BackgroundCreator();
            player1.y = SCREEN_HEIGHT - DOOR_WIDTH - 64 - 20;
            enemyCount = 0;
            box.show = 0;
            *lastSpawnTime = SDL_GetTicks();
        }
    }

    // Door 3 (right)
    if (player1.x >= SCREEN_WIDTH - 2 * DOOR2_WIDTH - 70 && player1.y >= (SCREEN_HEIGHT - DOOR2_HEIGHT) / 2 - 64 && player1.y <= (SCREEN_HEIGHT + DOOR2_HEIGHT + 64) / 2 && counter_x != 1) {
        SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 255);
        SDL_Rect d3 = {SCREEN_WIDTH - DOOR2_WIDTH - 20, (SCREEN_HEIGHT - 20) / 2, 20, 20};
        SDL_RenderFillRect(app.renderer, &d3);
        if (keystates[SDL_SCANCODE_G] && player1.points >= 1000) {
            player1.points -= 1000;
            counter_x = (counter_x + 1);
             counter_y = 0;
            BackgroundCreator();
            player1.x = DOOR2_WIDTH + 64 + 10;
            enemyCount = 0;
            box.show = 0;
            *lastSpawnTime = SDL_GetTicks();
        }
    }

    // Door 4 (bottom)
    if (player1.y >= SCREEN_HEIGHT - 2 * DOOR_HEIGHT - 70 && player1.x >= (SCREEN_WIDTH - DOOR_WIDTH) / 2 - 64 && player1.x <= (SCREEN_WIDTH + DOOR_WIDTH + 64) / 2 && counter_y != 1) {
        SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 255);
        SDL_Rect d4 = {(SCREEN_WIDTH - 20) / 2, (SCREEN_HEIGHT - DOOR_HEIGHT - 20), 20, 20};
        SDL_RenderFillRect(app.renderer, &d4);
        if (keystates[SDL_SCANCODE_G] && player1.points >= 1000) {
            player1.points -= 1000;
            counter_y = (counter_y + 1);
            counter_x = 0;
            BackgroundCreator();
            player1.y = DOOR2_WIDTH + 64 + 10;
            enemyCount = 0;
            box.show = 0;
            *lastSpawnTime = SDL_GetTicks();
        }
    }

}

void checkDoorCollision(){
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
// Check collision with door1
if (player1.x <= DOOR2_WIDTH && player1.y + PLAYER_SIZE >= (SCREEN_HEIGHT - DOOR2_HEIGHT) / 2 &&
    player1.y <= (SCREEN_HEIGHT + DOOR2_HEIGHT) / 2) {
    if (keystates[SDL_SCANCODE_W])
        player1.y = (SCREEN_HEIGHT + DOOR2_HEIGHT) / 2;
    if (keystates[SDL_SCANCODE_A])
       player1.x = DOOR2_WIDTH;
    if (keystates[SDL_SCANCODE_S])
        player1.y = (SCREEN_HEIGHT - DOOR2_HEIGHT) / 2 - PLAYER_SIZE;
}

// Check collision with door2
if (player1.y <= DOOR_HEIGHT && player1.x + PLAYER_SIZE >= (SCREEN_WIDTH - DOOR_WIDTH) / 2 &&
    player1.x <= (SCREEN_WIDTH + DOOR_WIDTH) / 2) {
    if (keystates[SDL_SCANCODE_W])
        player1.y = DOOR_HEIGHT;
    if (keystates[SDL_SCANCODE_A])
        player1.x = (SCREEN_WIDTH + DOOR_WIDTH) / 2;
    if (keystates[SDL_SCANCODE_D])
        player1.x = (SCREEN_WIDTH - DOOR_WIDTH) / 2-PLAYER_SIZE;
}

// Check collision with door3
if (player1.x >= SCREEN_WIDTH - DOOR2_WIDTH-PLAYER_SIZE && player1.y + PLAYER_SIZE >= (SCREEN_HEIGHT - DOOR2_HEIGHT) / 2 && player1.y <= (SCREEN_HEIGHT + DOOR2_HEIGHT) / 2) {
    if (keystates[SDL_SCANCODE_W])
        player1.y = (SCREEN_HEIGHT + DOOR2_HEIGHT) / 2;
    if (keystates[SDL_SCANCODE_S])
        player1.y = (SCREEN_HEIGHT - DOOR2_HEIGHT) / 2 - PLAYER_SIZE;
    if (keystates[SDL_SCANCODE_D])
        player1.x = SCREEN_WIDTH - DOOR2_WIDTH -PLAYER_SIZE;
}

// Check collision with door4
if (player1.y >= SCREEN_HEIGHT - DOOR_HEIGHT-PLAYER_SIZE && player1.x + PLAYER_SIZE >= (SCREEN_WIDTH - DOOR_WIDTH) / 2 && player1.x <= (SCREEN_WIDTH + DOOR_WIDTH) / 2+PLAYER_SIZE) {
    if (keystates[SDL_SCANCODE_A])
        player1.x = (SCREEN_WIDTH + DOOR_WIDTH) / 2;
    if (keystates[SDL_SCANCODE_S])
        player1.y = SCREEN_HEIGHT - DOOR_HEIGHT-PLAYER_SIZE;
    if (keystates[SDL_SCANCODE_D])
        player1.x = (SCREEN_WIDTH - DOOR_WIDTH) / 2-PLAYER_SIZE;
}
}

void manageMenu(void){
    int scene = 0;
    while(app.menuState){
        if(app.isPaused){
           handlePauseOption(&scene);
        }
        else if(scene == 0){
            handleMenuEntry(&scene,&done);
        }
        else if(scene == 1){
            app.menuState = 0;
        }
        else if(scene == 2){
           //app.menuState = 0;
        //    handleGameOver(&scene);
        }
        else if(scene == 3){
            done = 0;
        }
    }
}

static int displayOptions(Menu *menu,int *done) {
    SDL_Rect textBoxRectangle[MAX_NUMBER_OF_TEXT_BOXES+1];
    SDL_Texture *textBoxTexture[MAX_NUMBER_OF_TEXT_BOXES+1];
    SDL_Texture *backgroundTexture = IMG_LoadTexture(app.renderer, menu->imageFilePath);
    SDL_Point mousePosition;

    prepareTextBoxesToBeShown(menu, textBoxRectangle, textBoxTexture);

    int selectedOption = -1;
    while (selectedOption == -1) {
        while (SDL_PollEvent(&app.event)) {
            switch (app.event.type) {
                case SDL_QUIT:
                    app.menuState = 0;
                    *done = 1;
                    selectedOption = -2;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    SDL_GetMouseState(&mousePosition.x, &mousePosition.y);
                    for (int i = 0; strcmp(menu->options[i], "\0") != 0; i++) {
                        if (SDL_PointInRect(&mousePosition, &textBoxRectangle[i])) {
                            selectedOption = i;
                        }
                    }
                    break;
                default:
                    break;
            }
        }

        SDL_RenderCopy(app.renderer, backgroundTexture, NULL, NULL);
        for (int i = 0; strcmp(menu->options[i], "\0") != 0; i++) {
            SDL_RenderCopy(app.renderer, textBoxTexture[i], NULL, &textBoxRectangle[i]);
        }
        SDL_RenderPresent(app.renderer);
    }

    SDL_DestroyTexture(backgroundTexture);
    destroyBoxTextures(menu, textBoxTexture);

    return selectedOption;
}

static void prepareTextBoxesToBeShown(Menu *menu, SDL_Rect textBoxRectangle[], SDL_Texture *textBoxTexture[]) {
    SDL_Surface *textBoxSurface[MAX_NUMBER_OF_TEXT_BOXES+1];
    int largestTextBoxWidth = 0;
    int accumulatedTextBoxHeight = 0;

    for (int i = 0; strcmp(menu->options[i], "\0") != 0; i++) {
        textBoxSurface[i] = TTF_RenderText_Solid(app.font, menu->options[i], app.white);
        textBoxRectangle[i].y = accumulatedTextBoxHeight;
        textBoxRectangle[i].w = textBoxSurface[i]->w;
        textBoxRectangle[i].h = textBoxSurface[i]->h;
        accumulatedTextBoxHeight += textBoxRectangle[i].h + SPACING_BETWEEN_TEXT_BOXES;
        if (textBoxRectangle[i].w > largestTextBoxWidth) {
            largestTextBoxWidth = textBoxRectangle[i].w;
        }
    }
    
    int menuX = (SCREEN_WIDTH / 2) - (largestTextBoxWidth / 2);
    int menuY = (SCREEN_HEIGHT / 2) - LOCATION_ADJUSTMENT;
    for (int i = 0; strcmp(menu->options[i], "\0") != 0; i++) {
        textBoxRectangle[i].x = menuX + (largestTextBoxWidth - textBoxRectangle[i].w) / 2;
        textBoxRectangle[i].y = menuY + textBoxRectangle[i].y;
    }

    for (int i = 0; strcmp(menu->options[i], "\0") != 0; i++) {
        textBoxTexture[i] = SDL_CreateTextureFromSurface(app.renderer, textBoxSurface[i]);
        SDL_FreeSurface(textBoxSurface[i]);
    }
}

static void destroyBoxTextures(Menu *menu, SDL_Texture *textBoxTexture[]) {
    for (int i = 0; strcmp(menu->options[i], "\0") != 0; i++) {
    SDL_DestroyTexture(textBoxTexture[i]);
    }
}

static void handleMenuEntry(int *scene,int *done) {
    Menu menu = {{"Play!", "Quit", "", "", "\0"}, {"resources/start_menu.png"}, false};
    int selectedBox = displayOptions(&menu,done);
    switch (selectedBox) {
        case 0:
            *scene = 1;
            break;
        case 1:
            app.menuState = 0;
            *done = 1;
            *scene = 2;
            break;
        case 2:
            break;
        case 3:
            break;
        default:
            break;
    }
}

static void handlePauseOption(int *scene) {
    Menu menu = {{"Resume!", "Quit", "", "", "\0"}, {"resources/pause_menu.png"}, 0};
    int selectedBox = displayOptions(&menu,&done);
    switch (selectedBox) {
        case 0:
            app.menuState = 0;
            app.isPaused = 0;
            break;
        case 1:
            app.menuState = 0;
            app.isPaused = 0;
            done = 1;
            break;
        // case 2:
        //     app.menuState = 0;
        //     app.isPaused = 0;
        //     done = 1;
        //     break;
        // case 3:
        //     app.menuState = 0;
        //     app.isPaused = 0;
        //     done = 1;
        //     break;
        default:
            break;
    }
}

void Shoot() {
    if (ammo > 0) {
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (!bullets[i].active) {
                bullets[i].active = 1;
                bullets[i].x = player1.x + PLAYER_SIZE / 2 ;
                bullets[i].y = player1.y + PLAYER_SIZE / 2 ;
                bullets[i].dx = lastDirectionX * BULLET_SPEED;
                bullets[i].dy = lastDirectionY * BULLET_SPEED;
                ammo--;
                break;
            }
        }
    }
}

int points = 0;
void UpdateBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            bullets[i].x += bullets[i].dx;
            bullets[i].y += bullets[i].dy;

            if (bullets[i].y < 0 || bullets[i].y > SCREEN_HEIGHT || bullets[i].x < 0 || bullets[i].x > SCREEN_WIDTH) {
                bullets[i].active = 0;
            }
        }
    }
}

void UpdateEnemies(Entity enemies[]) {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        SDL_Rect enemyRect = {enemies[i].x, enemies[i].y, ENEMY_SIZE, ENEMY_SIZE};
        if (enemies[i].hp) {
            for (int j = 0; j < MAX_BULLETS; j++) {
                SDL_Rect bulletRect = {bullets[j].x, bullets[j].y, BULLET_SIZE, BULLET_SIZE}; // Assuming player size is 64x64
                if (bullets[j].active && checkCollision(enemyRect,bulletRect)) {
                    enemies[i].hp -= 25; // Decrease enemy health on hit
                    bullets[j].active = 0; // Deactivate bullet on hit
                    if (enemies[i].hp <= 0) {
                        enemies[i].hp = 0;
                        enemies[i].isAlive = 0;
                        bullets[j].active = 0;
                        if(enemies[i].type=='z'){
                            player1.points+=ZOMBIE_REWARD;
                        }else if(enemies[i].type=='w'){
                            player1.points+=WITCH_REWARD;
                        }else{
                            player1.points+=BOSS_REWARD;
                        }

                    }
                }
            }
        }
    }
}
 

void RenderBullets() {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            SDL_Rect bulletRect = {bullets[i].x, bullets[i].y, BULLET_SIZE, BULLET_SIZE};
            SDL_SetRenderDrawColor(app.renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(app.renderer, &bulletRect);
        }
    }
}

void ScreenShake(int *x,int *y){
    int shakeOffsetX = (rand() % (int)(2 * shakeMagnitude)) - shakeMagnitude;
    int shakeOffsetY = (rand() % (int)(2 * shakeMagnitude)) - shakeMagnitude;
    *x = backgroundPosX + shakeOffsetX;
    *y = backgroundPosY + shakeOffsetY;
}
