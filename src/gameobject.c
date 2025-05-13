#include "gameobject.h"

void init_material(Material* material) {
    set_color(material->ambient , 0.2f, 0.2f, 0.2f);
    set_color(material->diffuse , 0.8f, 0.8f, 0.8f);
    set_color(material->specular, 1.0f, 1.0f, 1.0f);
    
    material->shininess = 64; // 0 to 128
}

void set_material(const Material* material)
{
    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue
    };

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue
    };

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue
    };

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

GameObject* new_GameObject(const char* model_path, const char* texture_path, vec3 position, vec3 rotation, vec3 scale)
{
    printf("New game object: %s %s\n", model_path, texture_path);

    GameObject* game_object = (GameObject*)malloc(sizeof(GameObject));

    game_object->parent = NULL;

    if (model_path != NULL && texture_path != NULL) {
        Model* model = (Model*)malloc(sizeof(Model));
        load_model(model, model_path);
        game_object->model = model;
        game_object->texture_id = load_texture(texture_path);
        game_object->nodraw = false;
    } else {
        game_object->nodraw = true;
    }

    init_material(&(game_object->material));

    game_object->position = position;
    game_object->rotation = rotation;
    game_object->scale = scale;

    return game_object;
}

GameObject* new_GameObject_withModelAndTexture(Model* model, int texture_id, vec3 position, vec3 rotation, vec3 scale)
{
    printf("New game object with model and texture %f %f %f\n", position.x, position.y, position.z);

    GameObject* game_object = (GameObject*)malloc(sizeof(GameObject));

    game_object->parent = NULL;

    game_object->model = model;
    game_object->texture_id = texture_id;

    init_material(&(game_object->material));

    game_object->position = position;
    game_object->rotation = rotation;
    game_object->scale = scale;

    game_object->nodraw = false;

    return game_object;
}

void push_parent_transformations(const GameObject* game_object)
{
    if (game_object->parent != NULL)
    {
        push_parent_transformations(game_object->parent);
    }

    glPushMatrix();
    glTranslatef(game_object->position.x, game_object->position.y, game_object->position.z);
    glRotatef(game_object->rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(game_object->rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(game_object->rotation.z, 0.0f, 0.0f, 1.0f);
    glScalef(game_object->scale.x, game_object->scale.y, game_object->scale.z);
}

void pop_parent_transformations(const GameObject* game_object)
{
    glPopMatrix();
    if (game_object->parent != NULL)
    {
        pop_parent_transformations(game_object->parent);
    }
}

void draw_GameObject(const GameObject* game_object)
{
    if (game_object->nodraw)
    {
        return;
    }

    set_material(&(game_object->material));
    glBindTexture(GL_TEXTURE_2D, game_object->texture_id);

    if (game_object->parent != NULL)
    {
        push_parent_transformations(game_object->parent);
    }    
    glPushMatrix();
    {
        glTranslatef(game_object->position.x, game_object->position.y, game_object->position.z);
        glRotatef(game_object->rotation.x, 1.0f, 0.0f, 0.0f);
        glRotatef(game_object->rotation.y, 0.0f, 1.0f, 0.0f);
        glRotatef(game_object->rotation.z, 0.0f, 0.0f, 1.0f);
        glScalef(game_object->scale.x, game_object->scale.y, game_object->scale.z);
    }
    draw_model(game_object->model);
    glPopMatrix();
    if (game_object->parent != NULL)
    {
        pop_parent_transformations(game_object->parent);
    }
}

void destroy_GameObject(GameObject* game_object)
{
    free_model(game_object->model);
    free(game_object);
}