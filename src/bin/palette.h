#ifndef PALETTE_H
# define PALETTE_H

//--// typedef
typedef Evas_Object *(*f_add)(void *parent); // elm adding function
typedef void (*f_prop)(void *content); // func which apply content's specificities
typedef void (*f_pack)(void *object, void *subobj,
		       int x, int y, int w, int h); // elm packing function

//--// structure
// set a special structure for tool
typedef struct  _Elegance_Tool Elegance_Tool;
struct		_Elegance_Tool
{
    int		position; //position in the palette
    int		type; // which tool list ?
    char	*type_name; // container, elm or evas object
    char	*name; // the tool object
    char	*icon_small; // path to the small icon of the tool
    char	*icon_big; // path to the big icon of the tool
    f_add	function_add; // adding-in function
    f_pack	function_pack; // function to pack in a container
    f_prop	function_prop; //function to set properties of a specific object
};

//--// function
Evas_Object *palette_add(Evas_Object *win); // create the palette part
void palette_refresh(void); // refresh the palette part
void view_refresh(Evas_Object *icon); // refresh the central view

//--// extern global
extern const Elegance_Tool elm_list[9]; // list of elementary's tool
extern const Elegance_Tool container_list[2]; // list of container tool

#endif
