#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "renderWindow.hpp"
#include "entity.hpp"

RenderWindow::RenderWindow(const char*p_title,int p_w,int p_h)
    :window(NULL), renderer(NULL)
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, p_w, p_h, SDL_WINDOW_SHOWN);

        if(window == NULL) {
            std::cout<< "Could not create window: " << SDL_GetError() << std::endl;
            return;
        }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if(renderer == NULL) {
        std::cout<< "Could not create renderer: " << SDL_GetError() << std::endl;
        return;
    }
}


    SDL_Texture* RenderWindow::loadTexture(const char*filePath) {
        SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, filePath);
    if(texture == NULL) {   
        std::cout << "Could not load texture: " << SDL_GetError() << std::endl;
    }
    return texture;
}

int RenderWindow::getRefreshrate(){
    int displayIndex =SDL_GetWindowDisplayIndex(window);
    SDL_DisplayMode mode;
    SDL_GetDisplayMode(displayIndex,0,&mode);
    return mode.refresh_rate;

}

SDL_Renderer* RenderWindow::getRenderer() {
    return renderer;
}



void RenderWindow::cleanUp() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void RenderWindow::clear() {
    SDL_RenderClear(renderer);
}
int scale =6;

void RenderWindow::render(entity& p_entity,int scale) {
    SDL_Rect src;
    src.x = p_entity.getCurrentFrame()->x;
    src.y = p_entity.getCurrentFrame()->y;
    src.w = p_entity.getCurrentFrame()->w;
    src.h = p_entity.getCurrentFrame()->h;

    SDL_Rect dst;
    dst.x = p_entity.getPosition().x;
    dst.y = p_entity.getPosition().y;
    dst.w = p_entity.getCurrentFrame()->w*scale;
    dst.h = p_entity.getCurrentFrame()->h*scale;

    SDL_RenderCopy(renderer, p_entity.getTexture(), &src, &dst);
}

void RenderWindow::display() {
    SDL_RenderPresent(renderer);
}

void RenderWindow::renderUnscaled(entity& p_entity) {
        SDL_Rect src;
    src.x = p_entity.getCurrentFrame()->x;
    src.y = p_entity.getCurrentFrame()->y;
    src.w = p_entity.getCurrentFrame()->w;
    src.h = p_entity.getCurrentFrame()->h;

    SDL_Rect dst;
    dst.x = p_entity.getPosition().x;
    dst.y = p_entity.getPosition().y;
    dst.w = src.w;
    dst.h = src.h;
    SDL_RenderCopy(renderer, p_entity.getTexture(), &src, &dst);
}

void RenderWindow::renderText(const std::string& message, const std::string& fontPath, int fontSize, SDL_Color color, int x, int y)
{
    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) {
        std::cout << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Solid(font, message.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = surface->w;
    dest.h = surface->h;

    SDL_RenderCopy(renderer, texture, nullptr, &dest);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}


