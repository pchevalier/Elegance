#ifndef TREE_H
# define TREE_H

extern Eina_Bool popup_on;
extern Evas_Object *tree_list;

Evas_Object *tree_add(Evas_Object *win);
void _show_its_properties_cb(void           *data,
			     Evas_Object    *obj,
			     const char     *emission,
			     const char     *source);

#endif
