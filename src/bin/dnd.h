#ifndef DND_H
# define DND_H

typedef enum
{
   DND_LEFT  = (1 << 0),
   DND_RIGHT = (1 << 1),
   DND_UP    = (1 << 2),
   DND_DOWN  = (1 << 3)
} Dnd_Dir;

Evas_Object *dnd_add(Evas_Object *parent);
void         dnd_icon_set(Evas_Object *obj, const char *icon);
void         dnd_icon_drag_set(Evas_Object *obj, const char *icon);
void         dnd_icon_type_set(Evas_Object *obj, const char *type);
void         dnd_action_dir_set(Evas_Object *obj, Dnd_Dir dir);
void         dnd_data_set(Evas_Object *obj, void *data);
void        *dnd_data_get(Evas_Object *obj);
void         dnd_target_register(Evas_Object *obj);
void         dnd_target_unregister(Evas_Object *obj);

#endif
