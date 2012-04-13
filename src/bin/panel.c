#include "main.h"

//--// declarations

//--// globals
// pointer to elegance function for initialisation of a box
typedef Evas_Object *(*panel_content_add)(Evas_Object *win);

// struc for contents of boxes' window
typedef struct	_Panel_Builder	Panel_Builder;
struct		_Panel_Builder
{
    int                 box; // box number
    const char		*name; // name of the edje part
    panel_content_add	function_add; // elegance function to init box
    int			x_weight; // usefull argument for window disposition
    int			y_weight; // usefull argument for window disposition
    int			x_align; // usefull argument for window disposition
    int			y_align; // usefull argument for window disposition
};

  // +--------+--------------------------------+-----------+
  // |box 3   |box 0                           |box 1      |
  // |        |                                |           |
  // |        |                                |           |
  // |        +--------------------------------+           |
  // |        |               |__|             |           |
  // |        |                                |           |
  // |        |__                            __|           |
  // |        |__|                          |__|           |
  // |        |                                |           |
  // |        |                __              |           |
  // |        |               |  |             |           |
  // |        +--------------------------------+           |
  // |        |box 2                           |           |
  // +--------+--------------------------------+-----------+

// window boxes
static Evas_Object *boxes[4];

// contents of boxes' window
static const Panel_Builder build[5] = {
  { 0, "elm.swallow.top_panel", snippets_add,
    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND, EVAS_HINT_FILL, EVAS_HINT_FILL },
  { 1, "elm.swallow.right_panel", tree_add,
    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND, EVAS_HINT_FILL, EVAS_HINT_FILL },
  { 2, "elm.swallow.bottom_panel", status_add,
    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND, EVAS_HINT_FILL, EVAS_HINT_FILL },
  { 3, "elm.swallow.left_panel", toolbar_add,
    EVAS_HINT_EXPAND, 0, EVAS_HINT_FILL, 0 },
  { 3, "elm.swallow.left_panel", palette_add,
    EVAS_HINT_EXPAND, EVAS_HINT_EXPAND, EVAS_HINT_FILL, EVAS_HINT_FILL },
};

//--// callbacks

//--// private routines
static void
panel_add(Evas_Object		*win,
	  Evas_Object		*bx,
	  int			i)
{
  Evas_Object *o;

  // init content of the box
  o = build[i].function_add(win);

  // basic graphic functions
  evas_object_size_hint_weight_set(o, build[i].x_weight, build[i].y_weight);
  evas_object_size_hint_align_set(o, build[i].x_align, build[i].y_align);
  // add content to box
  elm_box_pack_end(bx, o);
  evas_object_show(o);
}

//--// public routines
// initialisation of panels' window
void
panels_add(void)
{
  int		i = 0;

  // for all boxes
  for (i = 0; i < sizeof (boxes) / sizeof (boxes[0]); i++)
  {
    // add a box in window
    boxes[i] = elm_box_add(design_win);
    // put it at the right place
    elm_object_part_content_set(design_layout,
				build[i].name,
				boxes[i]);
    evas_object_show(boxes[i]);
  }

  for (i = 0; i < sizeof (build) / sizeof (build[0]); i++)
  {
    panel_add(design_win, boxes[build[i].box], i);
  }
}

