#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"

#include "model.h"
#include "gameobject.h"

#include <SDL2/SDL.h>

typedef struct Scene
{
    Camera* camera;
    GameObject* mockCamera;

    int game_objects_count;    // Number of game objects
    GameObject** game_objects; // Array of game objects

    int colliders_count;       // Number of colliders
    GameObject** colliders;    // Array of colliders
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene* scene, Camera* camera, SDL_Window* window);

/**
 * Set the lighting of the scene.
 */
void set_lighting(const Scene* scene);

/**
 * Set the current material.
 */
void set_material(const Material* material);

/**
 * Update the scene.
 */
void update_scene(Scene* scene, float elapsed_time);

/**
 * Render the scene objects.
 */
void render_scene(const Scene* scene);

/**
 * Start pick animation
 */
void start_pick_animation();

/**
 * Set the head brightness
 */
void set_head_brightness(float brightness);

/**
 * Get the head brightness
 */
float get_head_brightness();

/**
 * Toggle help
 */
void toggle_help();

void draw_image_on_screen(int texture);

void draw_number(int number, float x, float y, float z, float scale);

#endif /* SCENE_H */