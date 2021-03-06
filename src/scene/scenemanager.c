//hdr
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#ifndef SCENE_C
#define SCENE_C
#include "scene.c"
#undef SCENE_C
#endif

#ifndef HANDLEOBJECT_C
#define HANDLEOBJECT_C
#include "handleobject.c"
#undef HANDLEOBJECT_C
#endif

#ifndef STACK_C
#define STACK_C
#include "stack.c"
#undef STACK_C
#endif

struct 
{
    struct stack scene_stack;
    struct scene *active_scene;
} scene_manager;

void scene_manager_init();

void scene_manager_push(struct scene *p_s);

struct scene *scene_manager_pop();


void scene_manager_start();

void scene_manager_stop();

void scene_manager_event();

void scene_manager_click();

void scene_manager_update(double dt);

void scene_manager_draw(double dt);



void scene_manager_delete();

#endif

//src
#ifndef SCENEMANAGER_C

#include "mstd.h"

#define test_stack() if(scene_manager.scene_stack.size <= 0) return;
#define test_scene() if(scene_manager.active_scene == 0) return;

void scene_manager_init()
{
    stack_init(&scene_manager.scene_stack);
    scene_manager.active_scene = 0;
}

void scene_manager_push(struct scene *p_s)
{
    scene_dispatch_handle(p_s, H_STOP, 0);
    stack_push(&scene_manager.scene_stack, p_s);
    scene_manager.active_scene = p_s;
}

struct scene *scene_manager_pop()
{
    test_stack();

    struct scene *res_scene = stack_pop(&scene_manager.scene_stack);

    if(scene_manager.scene_stack.size > 0)
        scene_manager.active_scene = stack_top(&scene_manager.scene_stack);
    else
        scene_manager.active_scene = 0;

    return scene_manager.active_scene;
}

void scene_manager_start()
{
    test_scene();

    scene_dispatch_handle(scene_manager.active_scene, H_START, 0);
}

void scene_manager_stop()
{
    test_scene();

    scene_dispatch_handle(scene_manager.active_scene, H_STOP, 0);
}

void scene_manager_event()
{
    SDL_Event sdlevent;
    while(SDL_PollEvent(&sdlevent))
    {
        //handle click events
        if(sdlevent.type == SDL_MOUSEBUTTONDOWN)
        {
            test_scene();
            scene_manager_click();
        }

        //todo: handle quit event
        if(sdlevent.type==SDL_QUIT)
        {
            game_container.quit = true;
        }

        test_scene();
        scene_dispatch_handle(scene_manager.active_scene, H_EVENT, &sdlevent);
    }
}

void scene_manager_click()
{
    int x;
    int y;
    SDL_GetMouseState(&x, &y);

    struct click_data c_data;
    c_data.x = x;
    c_data.y = y;
    c_data.flag = 1;
    scene_dispatch_handle(scene_manager.active_scene, H_CLICK, &c_data);
}

void scene_manager_update(double dt)
{
    test_scene();

    scene_dispatch_handle(scene_manager.active_scene, H_UPDATE, &dt);     
}

void scene_manager_draw(double dt)
{
    test_scene();

    scene_dispatch_handle(scene_manager.active_scene, H_DRAW, &dt);
}

void scene_manager_delete()
{
    test_scene();

    free(scene_manager.active_scene);
}

#endif
