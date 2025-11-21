#include"soccerPitch.h"

clsSoccerPitch::clsSoccerPitch(){

    _width = 2500;
    _height = 1000;
    _gameOn = true;


    //creating the two goals for two teams
    clsVector2d redTeamLeftPost(_width/8, _height/3);
    clsVector2d redTeamRightPost(_width/8, (_height*2)/3);  
    redGoal = new clsGoal(redTeamLeftPost, redTeamRightPost);

    clsVector2d blueTeamLeftPost( (_width*7)/8, (_height*2)/3 ); 
    clsVector2d blueTeamRightPost( (_width*7)/8, _height/3 );
    blueGoal = new clsGoal(blueTeamLeftPost, blueTeamRightPost);


    //the 4 determining game space
    clsVector2d topLeftPoint(0, 0);
    clsVector2d topRightPoint(_width, 0);
    clsVector2d bottomLeftPoint(0, _height);
    clsVector2d bottomRightPoint(_width, _height);
    
    //game space has 4 walls
    boundries = new clsWall[4];
    //top wall
    boundries[0].setWall(topLeftPoint, topRightPoint);
    //bottom wall
    boundries[1].setWall(bottomLeftPoint, bottomRightPoint);
    //right wall
    boundries[2].setWall(topRightPoint, bottomRightPoint);
    //left wall
    boundries[3].setWall(topLeftPoint, bottomLeftPoint);


    clsVector2d ballPos(_width/2, _height/2);
    ball = new clsSoccerBall(ballPos, 20);
}

double clsSoccerPitch::getWidth(){
    return _width;
}
double clsSoccerPitch::getHeight(){
    return _height;
}

void clsSoccerPitch::render()
    {
            // --- 1. Initialization ---
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            // cerr is used for error messages
            std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
            return;
        }

        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;

        // --- 2. Window and Renderer Creation ---
        // This creates both the window and its associated drawing context (renderer).
        if (SDL_CreateWindowAndRenderer(
                "Soccer AI Game",  // Window title
                _width,                   // Width
                _height,                  // Height
                0,           // Flags: 0 -> unresiable window
                &window,                        // Pointer to the created window
                &renderer                       // Pointer to the created renderer
            ) < 0) 
        {
            std::cerr << "Window/Renderer creation failed! SDL Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return;
        }

        // --- 3. The Main Game Loop ---
        bool IsRunning = true;
        SDL_Event Event;

        while (IsRunning) {
            
            // --- A. Event Polling (Check for user input) ---
            while (SDL_PollEvent(&Event)) {
                if (Event.type == SDL_EVENT_QUIT) {
                    IsRunning = false;
                }
            }

            // --- B. Rendering Steps (Drawing logic goes here) ---

            // Clear the screen to a background color (white)
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);

            // Set color for game elements (black)
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); 
    
            // draw 4 walls which determine the game space
            for(int i = 0; i < 4; ++i){
                boundries[i].render(renderer);
            }    
        
            // Draw the center Line
            SDL_RenderLine(renderer,_width/2 ,0 ,_width/2 , _height); 
            

            //draw the two goals
            redGoal->render(renderer);
            blueGoal->render(renderer);

            //draw the ball at the center of pitch
            ball->render(renderer);

            
            // Update the Screen
            SDL_RenderPresent(renderer);

            // Simple delay to cap frame rate (approx. 60 FPS)
            SDL_Delay(1000 / 60); 
        }

        // --- 4. Cleanup ---
        // Destroy the renderer first
        SDL_DestroyRenderer(renderer);
        // Destroy the window
        SDL_DestroyWindow(window);
        // Shut down all SDL subsystems
        SDL_Quit();
    }
