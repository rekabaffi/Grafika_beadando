#include "scene.h"

#include "load.h"
#include "draw.h"

#include <math.h>

int help_image = 0;
bool show_help = false;

int game_over_image = 0;
bool show_game_over = false;

// 0-9 + : characters monospaced in a line
int number_atlas = 0;

void init_scene(Scene* scene, Camera* camera, SDL_Window* window)
{
    (void)window; // Suppress unused parameter warning

    help_image = load_texture("assets/textures/help.png");
    number_atlas = load_texture("assets/textures/numbers.png");
    game_over_image = load_texture("assets/textures/gameover.png");

    scene->camera = camera;
    scene->mockCamera = new_GameObject(NULL, NULL, new_vec3(0.0f, 0.0f, 0.0f), new_vec3(0.0f, 0.0f, 0.0f), new_vec3(1.0f, 1.0f, 1.0f));

    scene->game_objects_count = 255;
    scene->game_objects = (GameObject**)malloc(sizeof(GameObject*) * scene->game_objects_count);
    
    scene->colliders_count = 255;
    scene->colliders = (GameObject**)malloc(sizeof(GameObject*) * scene->colliders_count);

    int counter = 0;
    int col_counter = 0;

    // Base cube
    scene->game_objects[counter++] = new_GameObject(
        NULL, NULL
        , new_vec3(0.0f, 0.0f, 0.0f), new_vec3(0.0f, 0.0f, 0.0f), new_vec3(0.1f, 0.1f, 0.1f)
    );
    
    scene->game_objects[counter++] = new_GameObject(
        NULL, NULL
        , new_vec3(30.0f, 0.0f, 1.0f), new_vec3(90.0f, 0.0f, 0.0f), new_vec3(10.0f, 10.0f, 10.0f)
    );
    scene->game_objects[1]->parent = scene->game_objects[0];   

    // Camera Z follower
    scene->game_objects[counter++] = new_GameObject(
        NULL, NULL
        , new_vec3(0.0f, 0.0f, 0.0f), new_vec3(0.0f, 0.0f, 0.0f), new_vec3(1.0f, 1.0f, 1.0f)
    );

    // Cube in hand
    scene->game_objects[counter++] = new_GameObject(
        NULL, NULL
        , new_vec3(0.5f, -0.30f, -0.4f), new_vec3(90.0f, 0.0f, 0.0f), new_vec3(0.25f, 0.25f, 0.25f)
    );
    scene->game_objects[3]->parent = scene->game_objects[2];

    // Hand on cube
    scene->game_objects[counter++] = new_GameObject(
        "assets/models/hand.obj", "assets/textures/hand.png"
        , new_vec3(-2.0f, 0.0f, 0.0f), new_vec3(0.0f, -180.0f, 90.0f), new_vec3(3.0f, 3.0f, 3.0f)
    );
    scene->game_objects[4]->parent = scene->game_objects[3];

    // Ceiling
    scene->game_objects[counter++] = new_GameObject(
        "assets/models/ter.obj", "assets/textures/ter.jpg"
        , new_vec3(0.0f, 0.0f, -0.5f), new_vec3(90.0f, 0.0f, 0.0f), new_vec3(3.0f, 3.0f, 3.0f)
    );

    Model* model = (Model*)malloc(sizeof(Model));
    load_model(model, "assets/models/tulipfixed.obj");
    int texture = load_texture("assets/textures/virag.png");
    int texture2 = load_texture("assets/textures/virag2.png");
    int texture3 = load_texture("assets/textures/virag3.png");

    // Flowers
    for (int i = 0; i < 12; i++) {
        float rand_dir = (rand() % 360);
        vec3 forward = new_vec3(cos(degree_to_radian(rand_dir)), sin(degree_to_radian(rand_dir)), 0.0f);
        float distance = (rand() % 10) + 2;
        float rand_x = forward.x * distance;
        float rand_y = forward.y * distance;

        
        float rand_z = (rand() % 100) / -200.0f - 0.2f; 
        float rand_rot = (rand() % 360);

        int tex = 0;
        int rand_texture = rand() % 3;
        switch (rand_texture)
        {
        case 0:
            tex = texture;
            break;
        case 1:
            tex = texture2;
            break;
        case 2:
            tex = texture3;
            break;
        default:
            break;
        }

        scene->game_objects[counter++] = new_GameObject_withModelAndTexture(
            model, tex
            , new_vec3(rand_x, rand_y, rand_z), new_vec3(0.0f, 0.0f, rand_rot), new_vec3(0.05f, 0.05f, 0.05f)
        );
        scene->colliders[col_counter++] = scene->game_objects[counter - 1];
    }

    // Fill the rest with NULL
    for (int i = counter; i < scene->game_objects_count; i++) {
        scene->game_objects[i] = NULL;
    }

    for (int i = col_counter; i < scene->colliders_count; i++) {
        scene->colliders[i] = NULL;
    }
}


float time = 0.0f;
float base_head_brightness = 1.0f;
int score = 0;

void set_lighting(const Scene* scene) 
{
    // General ambient light for the whole scene
    float ambient_light[]  = { 5.0f, 5.0f, 5.0f , 1.0f }; // Base ambient brightness
    float diffuse_light[]  = { 5.5f, 5.5f, 5.5f , 1.0f };
    float specular_light[] = { 5.2f, 5.2f, 5.2f , 1.0f };
    float position[]       = { 0.0f, 15.0f, 0.0f, 1.0f }; // Bright light source at the top

    // Player-controlled head brightness
    float head_light[] = { base_head_brightness, base_head_brightness, base_head_brightness, 1.0f };
    float head_position[] = { scene->camera->position.x, scene->camera->position.y, scene->camera->position.z, 1.0f };

    // Enable general lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Set the base light at the top of the scene
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);

    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.02f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.005f);

    // Enable player-controlled head brightness
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, head_light);
    glLightfv(GL_LIGHT1, GL_POSITION, head_position);

    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.02f);
}

float game_timer = 63.0f;
float pick_timer = 10.0f;

void update_scene(Scene* scene, float elapsed_time)
{
    if (game_timer > 0.0f)
        game_timer -= elapsed_time;
    time += elapsed_time;
    pick_timer += elapsed_time;

    scene->mockCamera->position = scene->camera->position;
    scene->mockCamera->rotation = scene->camera->rotation;

    scene->game_objects[2]->position = scene->camera->position;
    scene->game_objects[2]->rotation.z = scene->camera->rotation.z;

    // Animate hand                      sin(Time * Speed + Offset) * Amplitude
    scene->game_objects[4]->rotation.z += sin(time * 3.0f + 3.2f) * 0.1f;
    scene->game_objects[4]->rotation.x += sin(time * 2.0f + 3.2f) * 0.05f;
    scene->game_objects[4]->position.y  = sin(time * 2.0f + 3.2f) * 0.05f;

    if (pick_timer < 1.2f) {
        scene->game_objects[3]->rotation.z = sin(pick_timer * 10.0f) * 50.0f;

        if (pick_timer > 1.1f) {
            // Check if a collider is in the way
            float angle = scene->camera->rotation.z;
            vec3 baseDir = new_vec3(cos(degree_to_radian(angle)), sin(degree_to_radian(angle)), 0.0f);
            vec3 direction = normalized(&baseDir);
            vec3 position = add(&scene->camera->position, &direction);

            for (int i = 0; i < scene->colliders_count; i++) {
                if (scene->colliders[i] == NULL) {
                    continue;
                }

                vec3 diff = subtract(&scene->colliders[i]->position, &position);
                float len = length(&diff);

                if (len < 1.7f) {
                    // If the collider is in the way, pick the flower
                    scene->colliders[i]->nodraw = true;
                    scene->colliders[i] = NULL;

                    score += 100;
                    pick_timer = 2.0f; // Prevents the animation from repeating
                    break;
                }
            }
        }
    } else {
        scene->game_objects[3]->rotation.z = 0.0f;
    }
}

void render_scene(const Scene* scene)
{
    set_lighting(scene);

    for (int i = 0; i < scene->game_objects_count; i++) {
        if (scene->game_objects[i] == NULL) {
            continue;
        }
        draw_GameObject(scene->game_objects[i]);
    }

    for (int i = 0; i < scene->colliders_count; i++) {
        // Check if the collider is NULL
        if (scene->colliders[i] == NULL) {
            continue;
        }

        // Check if player is colliding with the collider
        vec3 diff = subtract(&scene->mockCamera->position, &scene->colliders[i]->position);
        float len = length(&diff);

        vec3 norm = normalized(&diff);

        if (len < 1.7f) {
            float inside = 1.7f - len;
            vec3 move = multiply(&norm, inside);
            scene->camera->position = add(&scene->camera->position, &move);
            scene->camera->position.z = 1.0f;
        }
    }

    if (time > 63.0f)
        show_game_over = true;

    if (show_game_over) 
        draw_image_on_screen(game_over_image);  

    if (time > 68.0f)
        exit(0);

    if (show_help)
        draw_image_on_screen(help_image);

    int tenths = ((int)game_timer) / 10;
    draw_number(tenths                ,  0.05f, 1.7f, -3.0f, 0.05f);
    draw_number(((int)game_timer) % 10, -0.05f, 1.7f, -3.0f, 0.05f);

    // Score on 00000
    draw_number(score / 10000 % 10,  0.2f,  -1.7f, -3.0f, 0.05f);
    draw_number(score / 1000  % 10,  0.1f,  -1.7f, -3.0f, 0.05f);
    draw_number(score / 100   % 10,  0.0f,  -1.7f, -3.0f, 0.05f);
    draw_number(score / 10    % 10, -0.1f,  -1.7f, -3.0f, 0.05f);
    draw_number(score % 10        , -0.2f,  -1.7f, -3.0f, 0.05f);
}

void draw_image_on_screen(int texture) {
    glBindTexture(GL_TEXTURE_2D, texture);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-2.4,  1.8, -3);
    glTexCoord2f(1, 0);
    glVertex3f( 2.4,  1.8, -3);
    glTexCoord2f(1, 1);
    glVertex3f( 2.4, -1.8, -3);
    glTexCoord2f(0, 1);
    glVertex3f(-2.4, -1.8, -3);
    glEnd();

    glDisable(GL_BLEND);

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void draw_number(int number, float x, float y, float z, float scale) {
    (void)z; // Suppress unused parameter warning

    glBindTexture(GL_TEXTURE_2D, number_atlas);

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
    glTexCoord2f(number * 0.09f, 0);
    glVertex3f(-1.2f * scale - x,  1.8f * scale - y, -3);
    glTexCoord2f(number * 0.09f + 0.09f, 0);
    glVertex3f( 1.2f * scale - x,  1.8f * scale - y, -3);
    glTexCoord2f(number * 0.09f + 0.09f, 1);
    glVertex3f( 1.2f * scale - x, -1.8f * scale - y, -3);
    glTexCoord2f(number * 0.09f, 1);
    glVertex3f(-1.2f * scale - x, -1.8f * scale - y, -3);

    glEnd();

    glDisable(GL_BLEND);

    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void start_pick_animation()
{
    if (pick_timer > 2.0f)
        pick_timer = 0.0f;
}

void set_head_brightness(float brightness)
{
    // Clamp brightness to avoid invalid values
    if (brightness < 0.0f) brightness = 0.0f;
    if (brightness > 5.0f) brightness = 5.0f;

    base_head_brightness = brightness;
}

float get_head_brightness()
{
    return base_head_brightness;
}

void toggle_help()
{
    show_help = !show_help;
}