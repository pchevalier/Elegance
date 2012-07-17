#ifndef PALETTE_H
# define PALETTE_H

// typedef of adding function
typedef Evas_Object *(*f_add)(void *parent);
typedef void (*f_prop)(void *content);
typedef void (*f_pack)(void *object, void *subobj,
		       int x, int y, int w, int h);

// set a special structure for tool
typedef struct  _Elegance_Tool Elegance_Tool;
struct		_Elegance_Tool
{
    char	*type; // container, elm or evas object
    char	*name; // the tool object
    char	*icon_small; // path to the small icon of the tool
    char	*icon_big; // path to the big icon of the tool
    f_add	function_add; // adding-in function
    f_pack	function_pack; // function to pack in a container
    f_prop	function_prop; //function to set properties of a specific object
};

Evas_Object *palette_add(Evas_Object *win);
void palette_refresh(void);
void view_refresh(Evas_Object *icon);

#endif
