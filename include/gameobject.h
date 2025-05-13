#ifndef GAMEOBJECT
#define GAMEOBJECT

#include "load.h"
#include "texture.h"
#include "draw.h"
#include "utils.h"

#include <GL/gl.h>
#include <stdlib.h>
#include <stdbool.h>

#define new_vec3(x, y, z) (vec3){x, y, z}
#define set_vec3(v, x, y, z) v.x = x; v.y = y; v.z = z

#define set_color(color, r, g, b) color.red = r; color.green = g; color.blue = b

typedef struct GameObject GameObject;

struct GameObject
{
    Model* model;
    Material material;
    int texture_id;

    vec3 position;
    vec3 rotation;
    vec3 scale;

    GameObject* parent;

    bool nodraw;
};

/**
 * Create a new game object.
 */
GameObject* new_GameObject(const char* model_path, const char* texture_path, vec3 position, vec3 rotation, vec3 scale);

/**
 * Create a new game object with model and texture.
 */
GameObject* new_GameObject_withModelAndTexture(Model* model, int texture_id, vec3 position, vec3 rotation, vec3 scale);

/**
 * Draw the game object.
 */
void draw_GameObject(const GameObject* game_object);

/**
 * Push parent transformations.
 */
void push_parent_transformations(const GameObject* game_object);

/**
 * Pop parent transformations.
 */
void pop_parent_transformations(const GameObject* game_object);

/**
 * Destroy the game object.
 */
void destroy_GameObject(GameObject* game_object);

#endif /* GAMEOBJECT */