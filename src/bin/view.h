#ifndef VIEW_H
# define VIEW_H

void view_clean(Eina_List *list);
void view_add(void);
Evas_Object *view_reload(Eina_List *list);

extern Evas_Object *view_layout;
extern Evas_Object *view_inwin;

#endif
