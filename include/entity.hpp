#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "math.hpp"

class entity{
    private:
        vector2f position; // Position of the entity
        SDL_Rect currentFrame; // Current frame of the entity
        SDL_Texture* tex; // Texture of the entity

        //animation controls 
    int frameWidth;
    int totalFrames;
    int frameCounter = 0;
    int frameDelay = 10; // Adjust to control animation speed
    int currentFrameIndex = 0;
    int framesPerRow;

        // adding jump and gravity
        float velocityY = 0;
        float velocityX = 0;
        bool isJumping = false;
        bool canDoubleJump = true;
        // float jumpVelocity =-10.0f;
        float gravity = 0.5f;
        float jumpPower = -12.0f;
        float groundY=500;

        
        

    public:
        entity(vector2f p_pos, SDL_Texture* p_texture,int framewidth,int frameheight,int frames=1);
        void setPosition(vector2f p_pos){
            position = p_pos;
            }

        vector2f& getPosition(){
            return position;
        };
        float getGroundY() const{
            return groundY;
        }


        SDL_Texture* getTexture() ;
        SDL_Rect* getCurrentFrame();
        
        void updateAnimation(int framedelay= 10);
        void jump();
        void applyGravity();

        // Reset animation-related states
void resetAnimation() {
    frameCounter = 0;
    currentFrameIndex = 0;
    currentFrame.x = 0;
    currentFrame.y = 0;
}

// Reset jumping and velocity states
void resetVelocity() {
    velocityY = 0;
    isJumping = false;
    canDoubleJump = true;
}





};