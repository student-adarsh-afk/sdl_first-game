#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "entity.hpp"
#include <SDL_ttf.h>



class RenderWindow {
    public:
    RenderWindow(const char*p_title,int p_w,int p_h);
    SDL_Texture*loadTexture(const char*filePath) ;
            void cleanUp();
            void clear();
            void display();
            void render(entity& entity,int scale);
            int  getRefreshrate();
            SDL_Renderer* getRenderer();
            void renderUnscaled(entity& entity); 
            void renderText( const std::string& message, const std::string& fontPath, int fontSize, SDL_Color color, int x, int y);
            
    private:
    SDL_Window*window;
    SDL_Renderer*renderer; 
};