#include <iostream>
#include<vector>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "../include/renderWindow.hpp"
#include "../include/entity.hpp"
#include "../include/utils.hpp"
using namespace std;

//   g++ src\main.cpp -IC:\msys64\mingw64\include\SDL2 -LC:\msys64\mingw64\lib -lmingw32 -lSDL2main -lSDL2 -o sdl_app.exe
//.\sdl_app.exe

int main(int argc, char** args) {
			//
			if(SDL_Init(SDL_INIT_VIDEO) < 0) {
				cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
				return 1;
			}
			//
			if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
				cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
				SDL_Quit();
				return 1;
			}

			if (TTF_Init() == -1) {
			std::cout << "Failed to initialize TTF: " << TTF_GetError() << std::endl;
			return -1;
}

			//
			RenderWindow window("Game 1", 1280, 720);


			//loading texture 
			SDL_Texture* grassTexture=window.loadTexture("res/gfx/grass.png");
			SDL_Texture* playerTexture=window.loadTexture("res/gfx/player.png");
			SDL_Texture* sky_bg = window.loadTexture("res/gfx/sky.jpg");
			SDL_Texture*fireballTexture = window.loadTexture("res/gfx/attack2.png");
			//)

		int windowWidth = 1280;
		int windowHeight = 720;
		int tileSize = 32*6;

		vector<entity> ground;
		entity player(vector2f(100,windowHeight-2*tileSize),playerTexture,64,64,6);

		vector<entity> fireball;

		//fireball spawning system - CHANGE: Sequential spawning system
		Uint32 delayAfterFireball[] = {3000, 2000, 3000}; // CHANGE: delays after fireball reaches end
		int currentDelayIndex = 0; // CHANGE: Track which delay we're using
		Uint32 fireballEndTime = 0; // CHANGE: When last fireball reached the end
		bool canSpawnNext = true; // CHANGE: Control when to spawn next fireball

		//bg animation constants 
		float backgroundOffset =0.0f;
		float backgroundSpeed =1.0f;

		for(int x=0;x<windowWidth+tileSize;x +=tileSize){
			vector2f pos(x,(windowHeight-tileSize));
			ground.push_back(entity(pos,grassTexture,32,32));
		}


			//making game loop
			bool gameRunning = true;
			bool showPopup = false; // CHANGE: Moved outside the game loop so it persists
			bool gameOver = false; // CHANGE: Added game over state
			SDL_Event event;
			
			const float timestep=0.01f;
			float accumulator=0.0f;
			float currentTime=utils::hireTimeInSeconds();
			float cameraX = 0.0f;

		//
		


// main game loop
			while(gameRunning) {
				int startTick = SDL_GetTicks();
			
				float newTime=utils::hireTimeInSeconds();
				float frameTime=newTime-currentTime;
				currentTime= newTime;
				accumulator +=frameTime;
				// CHANGE: Removed the duplicate showPopup declaration that was resetting it each frame
				

				while(accumulator>=timestep){

					//event loop
				while (SDL_PollEvent(&event)) {
	if (event.type == SDL_QUIT) {
		gameRunning = false;
	}

	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_SPACE && !gameOver) { // CHANGE: Only allow jump if not game over
			player.jump();
		}
		// CHANGE: Moved P key handling inside the main keydown check to avoid duplicate code
		if (event.key.keysym.sym == SDLK_p) {
			showPopup = !showPopup; // toggle popup on 'P' key
		}
		// CHANGE: Add restart functionality when game over
		if (event.key.keysym.sym == SDLK_r && gameOver) {
			gameOver = false;
			fireball.clear(); // Clear all fireballs
			canSpawnNext = true; // Reset spawning
			fireballEndTime = 0;
			currentDelayIndex = 0;
			player.setPosition(vector2f(100, windowHeight - 2 * tileSize)); // Reset to original position
player.resetAnimation(); 
player.resetVelocity(); 

		}
	}
}

				// CHANGE: Only update game physics if not game over
				if (!gameOver) {
					// CHANGE: Move fireball physics inside timestep loop for consistent movement
					for (auto it = fireball.begin(); it != fireball.end(); ) {
						vector2f pos = it->getPosition();
						pos.x -= 150.0f * timestep;  // CHANGE: Reduced speed to 150 for smoother movement
						it->setPosition(pos);
						
						// CHANGE: Collision detection between fireball and player
						vector2f playerPos = player.getPosition();
			//detct collision 
			// Shrink hitbox margins
int margin = 15;

SDL_Rect fireballBox = {
	static_cast<int>(pos.x) + margin,
	static_cast<int>(pos.y) + margin,
	98 - 2 * margin,
	93 - 2 * margin
};

SDL_Rect playerBox = {
	static_cast<int>(playerPos.x) + margin,
	static_cast<int>(playerPos.y) + margin,
	64 - 2 * margin,
	64 - 2 * margin
};

// Collision check
if (SDL_HasIntersection(&fireballBox, &playerBox)) {
	std::cout << "Collided!" << std::endl;
	gameOver = true;
	break;
}
//ends collision 
						
						if (pos.x + 98 < 0) {
					it = fireball.erase(it);
					if (fireballEndTime == 0) {
				fireballEndTime = SDL_GetTicks();
				canSpawnNext = false;
	}
} else {
	++it;
}

					}
				}
			
				accumulator -=timestep;
				}
				const float alpha =accumulator/timestep;
				

				// Clear the window
				window.clear();

				//bg animation
				if(!gameOver){
						backgroundOffset -= backgroundSpeed;
				if (backgroundOffset <= -windowWidth)
				backgroundOffset = 0;
				}

				
				//animating sky
				SDL_Rect skyRect= {(int)backgroundOffset, 0, windowWidth, windowHeight};
				SDL_Rect skyRect2= {(int)backgroundOffset + windowWidth, 0, windowWidth, windowHeight};

				SDL_RenderCopy(window.getRenderer(), sky_bg, NULL, &skyRect);
				SDL_RenderCopy(window.getRenderer(), sky_bg, NULL, &skyRect2);


				// adding jump logi

				// Rendering starts 
				//animating player 
				if (!gameOver) { // CHANGE: Only update player animation if not game over
					player.applyGravity();
					player.updateAnimation();
				}
				window.render(player,4);

				//fireball spawning - CHANGE: Sequential spawning system
				if (!gameOver) { // CHANGE: Only spawn fireballs if not game over
					Uint32 currentTime = SDL_GetTicks();
					
					// CHANGE: Check if we can spawn next fireball (either first one or after delay)
					if (canSpawnNext || (fireballEndTime > 0 && currentTime - fireballEndTime > delayAfterFireball[currentDelayIndex])) {
						// CHANGE: Only spawn if no fireballs exist (ensures sequential spawning)
						if (fireball.empty()) {
							vector2f fireballPos(1350, player.getGroundY()+20);
							fireball.emplace_back(fireballPos, fireballTexture, 98,93, 9);
							
							canSpawnNext = false;
							fireballEndTime = 0;
						} else { 
							currentDelayIndex = (currentDelayIndex + 1) % 3; // cycle through delays
						}
					}
				}
			// CHANGE: Only handle fireball animation in render loop, movement is now in physics loop
			for (auto& fireballEntity : fireball) {
				fireballEntity.updateAnimation(9);
			}
			//render fireball
			if(!gameOver){ 
			for (auto& entity : fireball) {
		window.render(entity, 3);
	}}

	
				for (auto& entity : ground) {
					vector2f pos = entity.getPosition();
					if(!gameOver){ 
					pos.x -= 5.0f;  }
					entity.setPosition(pos);
					if (pos.x + 32*6 < 0) {  
					entity.setPosition(vector2f(pos.x+32*6*ground.size(), pos.y));  
					}
				window.render(entity, 6);
}

				if (showPopup) {
			SDL_Color white = {255, 255, 255};
			window.renderText("Hooooooo!", "res/fonts/Kenpixel.ttf", 32, white, 200, 150);
}

				// CHANGE: Display game over screen
				if (gameOver) {
					SDL_Color red = {255, 0, 0};
					SDL_Color white = {255, 255, 255};

					window.renderText("GAME OVER", "res/fonts/Kenpixel.ttf", 48, red, 500, 300);
					window.renderText("Press R to Restart", "res/fonts/Kenpixel.ttf", 24, white, 480, 380);
				}

// rendering ends 
				window.display();

				int frameTicks = SDL_GetTicks() - startTick;
				int refreshRate = window.getRefreshrate();
				if (refreshRate <= 0) refreshRate = 60;
				int frameDelay = 1000 / refreshRate;
				if (frameTicks < frameDelay) {
					SDL_Delay(frameDelay - frameTicks);
				}
			} // end while(gameRunning)

			window.cleanUp();
			return 0;
} // end main