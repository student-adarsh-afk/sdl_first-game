#include <SDL.h>
#include <SDL_image.h>
#include "../include/entity.hpp"

entity::entity(vector2f p_pos, SDL_Texture* p_tex,int framewidth,int frameheight,int frames)
    : position(p_pos), tex(p_tex),frameWidth(framewidth),totalFrames(frames),groundY(p_pos.y) {
        currentFrame.x = 0;
        currentFrame.y = 0;
        currentFrame.w = framewidth;
        currentFrame.h = frameheight;

        int texW;
        SDL_QueryTexture(tex, NULL, NULL, &texW, NULL);
        framesPerRow = texW / frameWidth; 
}

SDL_Texture* entity::getTexture() {
    return tex;
}
SDL_Rect* entity::getCurrentFrame() {
    return &currentFrame;
}

void entity::updateAnimation(int framedelay) {
    frameCounter++;
    if (frameCounter >= frameDelay) {
        currentFrameIndex = (currentFrameIndex + 1) % totalFrames; // 0 to 19
        frameCounter = 0;
    }

    currentFrame.x = (currentFrameIndex % framesPerRow) * frameWidth;
    currentFrame.y = (currentFrameIndex / framesPerRow) * frameWidth;
}

//adding jump fn 
void entity::jump() {
    if (!isJumping) {
        velocityY = jumpPower;
        // position.x-=jumpPower-100;
        isJumping = true;
    } else if (canDoubleJump) {
        velocityY = jumpPower * 0.7f; // Adjust strength if needed
        canDoubleJump = false;
        position.x-=jumpPower-70;
    }
}

void entity::applyGravity() {
    velocityY += gravity;
    position.y += velocityY;

    if (position.y >= groundY) {
        position.y = groundY;
        isJumping = false;
        canDoubleJump = true; // Reset double jump
        velocityY = 0;
    }
}