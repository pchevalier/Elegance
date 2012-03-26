#ifndef STATUS_H_
# define STATUS_H_

typedef struct _Elegance_Gengrid_Item Elegance_Gengrid_Item;
struct	       _Elegance_Gengrid_Item
{
    char		*name;
    Evas_Object		*lay;
    Elegance_Page	*page;
};

typedef struct _Elegance_Gengrid Elegance_Gengrid;
struct	       _Elegance_Gengrid
{
  Eina_List			*items;
  Evas_Object			*gengrid;
  int				nb_items;
  Elm_Gengrid_Item_Class	*gic;
};

Evas_Object *status_add(Evas_Object *win);
void status_refresh(void);

#endif
