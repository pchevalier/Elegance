#ifndef VIEW_H
# define VIEW_H

//--// function
void view_clean(Eina_List *list); // clean the view
void view_add(void); // create the view part
void view_reload(Eina_List *list); // reload the view

//--// extern global
extern Evas_Object *view_layout; // the view's layout
extern Evas_Object *view_inwin; // the view's inwin
extern const Elegance_Tool inwin_tool; // specific Elegance_Tool for view's inwin

#endif
