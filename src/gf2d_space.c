#include "gf2d_space.h"
#include "gf2d_draw.h"
#include "simple_logger.h"
#include "gf2d_dynamic_body.h"
#include <stdlib.h>


Uint8 gf2d_body_shape_collide(Body *a,Shape *s,Vector2D *poc, Vector2D *normal);

void gf2d_free_shapes(void *data,void *context)
{
    Shape *shape;
    if (!data)return;
    shape = (Shape*)data;
    free(shape);
}

void gf2d_free_dynamic_bodies(void *data,void *context)
{
    if (!data)return;
    gf2d_dynamic_body_free((DynamicBody*)data);
}

void gf2d_space_free(Space *space)
{
    if (!space)return;
    
    //static shapes ARE owned by the space, so are deleted when the space goes away
    gf2d_list_foreach(space->staticShapes,gf2d_free_shapes,NULL);
    gf2d_list_foreach(space->dynamicBodyList,gf2d_free_dynamic_bodies,NULL);
    gf2d_list_delete(space->staticShapes);
    free(space);
}

Space *gf2d_space_new_full(
    int         precision,
    Rect        bounds,
    float       timeStep,
    Vector2D    gravity,
    float       dampening,
    float       slop)
{
    Space *space;
    space = gf2d_space_new();
    if (!space)return NULL;
    gf2d_rect_copy(space->bounds,bounds);
    vector2d_copy(space->gravity,gravity);
    space->timeStep = timeStep;
    space->precision = precision;
    space->dampening = dampening;
    space->slop = slop;
    return space;
}

Space *gf2d_space_new()
{
    Space *space;
    space = (Space *)malloc(sizeof(Space));
    if (!space)
    {
        slog("failed to allocate space for Space");
        return NULL;
    }
    memset(space,0,sizeof(Space));
    space->dynamicBodyList = gf2d_list_new();
    space->staticShapes = gf2d_list_new();
    return space;
}

void gf2d_space_add_static_shape(Space *space,Shape shape)
{
    Shape *newShape;
    if (!space)
    {
        slog("no space provided");
        return;
    }
    newShape = (Shape*)malloc(sizeof(shape));
    if (!newShape)
    {
        slog("failed to allocate new space for the shape");
        return;
    }
    memcpy(newShape,&shape,sizeof(Shape));
    space->staticShapes = gf2d_list_append(space->staticShapes,(void *)newShape);
}

void gf2d_space_remove_body(Space *space,Body *body)
{
    int i,count;
    DynamicBody *db = NULL;
    if (!space)
    {
        slog("no space provided");
        return;
    }
    if (!body)
    {
        slog("no body provided");
        return;
    }
    if (space->dynamicBodyList)
    {
        count = gf2d_list_get_count(space->dynamicBodyList);
        for (i = 0; i < count;i++)
        {
            db = (DynamicBody*)gf2d_list_get_nth(space->dynamicBodyList,i);
            if (!db)continue;
            if (db->body != body)continue;
            gf2d_dynamic_body_free(db);
            space->dynamicBodyList = gf2d_list_delete_nth(space->dynamicBodyList,i);
            break;
        }
    }
}

void gf2d_space_add_body(Space *space,Body *body)
{
    DynamicBody *db = NULL;
    if (!space)
    {
        slog("no space provided");
        return;
    }
    if (!body)
    {
        slog("no body provided");
        return;
    }
    db = gf2d_dynamic_body_new();
    if (!db)return;
    db->body = body;
    space->dynamicBodyList = gf2d_list_append(space->dynamicBodyList,(void *)db);
}

void gf2d_space_draw(Space *space,Vector2D offset)
{
    int i,count;
    SDL_Rect r;
    DynamicBody *db = NULL;
    if (!space)
    {
        slog("no space provided");
        return;
    }
    r = gf2d_rect_to_sdl_rect(space->bounds);
    vector2d_add(r,r,offset);    
    gf2d_draw_rect(r,vector4d(255,0,0,255));
    count = gf2d_list_get_count(space->dynamicBodyList);
    for (i = 0; i < count;i++)
    {
        db = (DynamicBody*)gf2d_list_get_nth(space->dynamicBodyList,i);
        if (!db)continue;
        gf2d_body_draw(db->body,offset);
    }
    count = gf2d_list_get_count(space->staticShapes);
    for (i = 0; i < count;i++)
    {
        gf2d_shape_draw(*(Shape *)gf2d_list_get_nth(space->staticShapes,i),gf2d_color8(0,255,0,255),offset);
    }
}

void gf2d_space_dynamic_bodies_step(Space *space,DynamicBody *db, float t)
{
    DynamicBody *other;
    int i,count;
    if (!space)return;
    vector2d_add(db->position,db->position,db->velocity);
    count = gf2d_list_get_count(space->dynamicBodyList);
    for (i = 0; i < count;i++)
    {
        other = (DynamicBody*)gf2d_list_get_nth(space->dynamicBodyList,i);
        if (!other)continue;
        if (other == db)continue;   // skip checking outself
        // check for layer compatibility
        if (gf2d_dynamic_body_collision_check(db,other))
        {
            slog("HIT");
        }
    }
}

void gf2d_space_step(Space *space,float t)
{
    DynamicBody *db = NULL;
    int i,count;
    if (!space)return;
    count = gf2d_list_get_count(space->dynamicBodyList);
    for (i = 0; i < count;i++)
    {
        db = (DynamicBody*)gf2d_list_get_nth(space->dynamicBodyList,i);
        if (!db)continue;
        gf2d_space_dynamic_bodies_step(space,db, t);
    }
}

void gf2d_space_reset_bodies(Space *space)
{
    int i,count;
    if (!space)return;
    count = gf2d_list_get_count(space->dynamicBodyList);
    for (i = 0; i < count;i++)
    {
        gf2d_dynamic_body_reset((DynamicBody*)gf2d_list_get_nth(space->dynamicBodyList,i),space->timeStep);
    }
}

void gf2d_space_update_bodies(Space *space)
{
    DynamicBody *db = NULL;
    int i,count;
    if (!space)return;
    count = gf2d_list_get_count(space->dynamicBodyList);
    for (i = 0; i < count;i++)
    {
        db = (DynamicBody*)gf2d_list_get_nth(space->dynamicBodyList,i);
        if (!db)continue;
        gf2d_dynamic_body_update(db);
    }
}

void gf2d_space_update(Space *space)
{
    float s;
    if (!space)return;
    gf2d_space_reset_bodies(space);
    // reset all body tracking
    for (s = 0; s <= 1; s += space->timeStep)
    {
        gf2d_space_step(space,s);
    }
    gf2d_space_update_bodies(space);
}



/*eol@eof*/