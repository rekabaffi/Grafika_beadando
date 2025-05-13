#include "texture.h"

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

GLuint load_texture(const char* filename) 
{
    SDL_Surface* surface;
    GLuint texture_name;

    surface = IMG_Load(filename);
    if (!surface) {
        printf("IMG_Load: %s\n", IMG_GetError());
    }

    SDL_Surface* converted_surface = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA32, 0);
    if (!converted_surface) {
        printf("SDL_ConvertSurfaceFormat: %s\n", SDL_GetError());
    }

    glGenTextures(1, &texture_name);

    glBindTexture(GL_TEXTURE_2D, texture_name);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, converted_surface->w, converted_surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, converted_surface->pixels);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Free the surfaces
    SDL_FreeSurface(converted_surface);
    SDL_FreeSurface(surface);

    return texture_name;
}