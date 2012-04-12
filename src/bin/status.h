#ifndef STATUS_H_
# define STATUS_H_

// structure of gengrid's items
typedef struct _Elegance_Gengrid_Item Elegance_Gengrid_Item;
struct	       _Elegance_Gengrid_Item
{
    char		*name; // item's name
    Evas_Object		*lay; // item's layout
    Elegance_Page	*page; // associated page
};

// structure of the status' gengrid
typedef struct _Elegance_Gengrid Elegance_Gengrid;
struct	       _Elegance_Gengrid
{
    Eina_List			*items; // list of items (page)
    Evas_Object			*gengrid; // gengrid evas object
    Eina_Bool			new_page; // there is a new page ?
    Elm_Gengrid_Item_Class	*gic; // gengrid item class
};

Evas_Object *status_add(Evas_Object *win);
void status_refresh(void);
Evas_Object *refresh_childs(Elegance_Content *data);

extern Elegance_Gengrid *status_grid; // status' gengrid

#endif
