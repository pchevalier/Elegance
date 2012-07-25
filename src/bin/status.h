#ifndef STATUS_H_
# define STATUS_H_

//--// structures
// structure of gengrid's items
typedef struct _Elegance_Gengrid_Item Elegance_Gengrid_Item;
struct	       _Elegance_Gengrid_Item
{
    char		*name; // item's name
    Evas_Object		*lay; // item's layout
    Elegance_Page	*page; // associated page
    Elm_Object_Item	*it; // associated item
};

// structure of the status' gengrid
typedef struct _Elegance_Gengrid Elegance_Gengrid;
struct	       _Elegance_Gengrid
{
    Eina_List			*items; // list of items (page)
    Evas_Object			*gengrid; // gengrid evas object
    Eina_Bool			new_page; // there is a new page ?
    Eina_Bool			button_plus; // add the plus button ?
    Elm_Gengrid_Item_Class	*gic; // gengrid item class
};

//--// functions
Evas_Object *status_add(Evas_Object *win); // create the status bar
void status_refresh(void); // refresh status' contents
void status_clean(void); // clean the status bar

//--// extern global
extern Elegance_Gengrid *status_grid; // status' gengrid

#endif
