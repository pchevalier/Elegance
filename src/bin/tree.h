#ifndef TREE_H
# define TREE_H

extern Eina_Bool popup_on;

Evas_Object *tree_add(Evas_Object *win);
void _show_its_properties_cb(void *data,
			     Evas *e,
			     Evas_Object *obj,
			     void *event_info);

#endif
