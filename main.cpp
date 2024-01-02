#include"raylib.h"

struct AnimData{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data,int windowHeight){
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data,float deltaTime,int maxFrame){
    //update running time
    data.runningTime += deltaTime;
    if(data.runningTime >= data.updateTime){
        data.runningTime = 0.0;
        //update animation time
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if(data.frame > maxFrame){
            data.frame = 0;
        }
    }

    return data;
}

int main(){
    AnimData scarfy;
    //window dimensions
    const int WindowDimensions[2]={512,380};
    const int window_width{512};
    const int window_height{380};
    //initialize window
    InitWindow(WindowDimensions[0],WindowDimensions[1],"Dapper-Dasher");
    //initialize velocity
    int velocity{0};
    //acceleratio due to gravity(pixels/(sec*sec))
    const int gravity{1000};
    //jump velocity (pixels/sec)
    int jumpVel{-600};

    //charecter(sacrfy)
    Texture2D scarf = LoadTexture("textures/scarfy.png");
    //setting the frame of scarfy
    scarfy.rec.width = scarf.width/6;
    scarfy.rec.height = scarf.height;
    scarfy.rec.x = 0;
    scarfy.rec.y = 0;
    scarfy.runningTime = 0;
    scarfy.updateTime = 1.0/12.0;
    scarfy.frame = 0;

    scarfy.pos.x = window_width/2- scarfy.rec.width/2;
    scarfy.pos.y = window_height - scarfy.rec.height;

    //nebula(enemy in game)
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    

    const int sizeOfNebula{3};

    AnimData nebulae[3]{};

    for(int i=0;i<sizeOfNebula;i++){

            nebulae[i].frame = 0;
            nebulae[i].rec.height = nebula.height/8;
            nebulae[i].rec.width = nebula.width/8;
            nebulae[i].rec.x = 0.0;
            nebulae[i].rec.y = 0.0;
            nebulae[i].pos.y = window_height-nebulae[i].rec.height;
            nebulae[i].runningTime = 0;
            nebulae[i].pos.x = window_width + (300*i);
    }
    
    nebulae[0].updateTime = 1.0/12.0;
    nebulae[1].updateTime = 1.0/16.0;
    nebulae[2].updateTime = 1.0/20.0;

    float finishline{nebulae[sizeOfNebula-1].pos.x};
    //nebula velocity (pixels/sec)
    int nebVel{-200};

    bool isInAir{};
    SetTargetFPS(60);

    //for background
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};
    //midground
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};
    //foreground
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};
    //game logic
    while(WindowShouldClose() == false){
        BeginDrawing();
        ClearBackground(WHITE);

        const float  dT{GetFrameTime()};
        bgX -= 20 * dT;
        if(bgX <= -background.width * 2){
            bgX = 0.0;
        }
        mgX -= 40 * dT;
        if(mgX <= -midground.width * 2){
            mgX = 0.0;
        }
        fgX -= 80 * dT;
        if(fgX <= -foreground.width * 2){
            fgX = 0.0;
        }
        //draw backgound
        Vector2 bg1Pos;
        bg1Pos.x = bgX;
        bg1Pos.y = 0.0;
        DrawTextureEx(background,bg1Pos,0.0,2.0,WHITE);

        //draw background for multiple times
        Vector2 bg2Pos;
        bg2Pos.x = bgX + (background.width * 2);
        bg2Pos.y = 0.0;
        DrawTextureEx(background,bg2Pos,0.0,2.0,WHITE);

        //draw midgound
        Vector2 mg1Pos;
        mg1Pos.x = mgX;
        mg1Pos.y = 0.0;
        DrawTextureEx(midground,mg1Pos,0.0,2.0,WHITE);

        Vector2 mg2Pos;
        mg2Pos.x = mgX+(midground.width*2);
        mg2Pos.y = 0.0;
        DrawTextureEx(midground,mg2Pos,0.0,2.0,WHITE);
        
        //draw foreground
        Vector2 fg1Pos;
        fg1Pos.x = fgX;
        fg1Pos.y = 0.0;
        DrawTextureEx(foreground,fg1Pos,0.0,2.0,WHITE);

        Vector2 fg2Pos;
        fg2Pos.x = fgX+(foreground.width*2);
        fg2Pos.y = 0.0;
        DrawTextureEx(foreground,fg2Pos,0.0,2.0,WHITE);
        
        if(isOnGround(scarfy,WindowDimensions[1])){
            velocity = 0;
            isInAir = false;
        }else{
            //Applying gravity
            velocity += gravity * dT;
            isInAir = true;
        }



        if(IsKeyPressed(KEY_SPACE) && !isInAir){
            velocity+=jumpVel;
        }

        //update scarfy position
        scarfy.pos.y += velocity*dT;
        //update nebula Position
        for(int i=0;i<sizeOfNebula;i++){
            nebulae[i].pos.x += nebVel*dT;
        }
        //update nebula 2 Position
        
        scarfy.runningTime+=dT;
        if(!isInAir){
           scarfy = updateAnimData(scarfy,dT,5);
        }

        //for creating nebula's
        for(int i=0;i<sizeOfNebula;i++){
            
            nebulae[i] = updateAnimData(nebulae[i],dT,7);
        }

        bool collison{};
        for(AnimData nebula : nebulae){
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x+pad,
                nebula.pos.y+pad,
                nebula.rec.width-(pad*2),
                nebula.rec.height-(pad*2)
            };
            Rectangle scarfyRec{
                scarfy.pos.x,
                scarfy.pos.y,
                scarfy.rec.width,
                scarfy.rec.height
            };
            if(CheckCollisionRecs(nebRec,scarfyRec)){
                collison = true;
            }
        }
        //update finishline
        finishline +=nebVel*dT;

        if(collison){
            //end game
            DrawText("Game Over!",WindowDimensions[0]/4,WindowDimensions[1]/2,40,RED);
            scarfy.pos.x=finishline;
        }else if(scarfy.pos.x>finishline){
            //win the game
            DrawText("You Won",WindowDimensions[0]/3,WindowDimensions[1]/2,40,BLUE);
        }
        else{
            //Draw scarfy
            DrawTextureRec(scarf,scarfy.rec,scarfy.pos,WHITE);
            //Draw nebula 1
            for(int i=0;i<sizeOfNebula;i++){
                DrawTextureRec(nebula,nebulae[i].rec,nebulae[i].pos,WHITE);
            }
    
        }
    
        EndDrawing();
    }
    UnloadTexture(scarf);
    UnloadTexture(nebula);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    UnloadTexture(background);
    CloseWindow();
    return 0;
}