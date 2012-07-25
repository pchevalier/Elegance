#ifndef TREE_H
# define TREE_H

//--// global extern
extern Eina_Bool popup_on; // boolean used for the popup
extern Evas_Object *tree_list; // the tree's genlist

// functions
Evas_Object *tree_add(Evas_Object *win); // build the tree part
void _show_its_properties_cb(void           *data,
			     Evas_Object    *obj,
			     const char     *emission,
			     const char     *source); // callback used to show prop

#endif
