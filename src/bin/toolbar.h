#ifndef TOOLBAR_H
# define TOOLBAR_H

//--// functions
Evas_Object *toolbar_add(Evas_Object *win); // create Elegance's toolbar
void _toolbar_new_page_cb(void *data,
			  Evas_Object *obj,
			  void *event_info); // cb called when a new page is needed

#endif
