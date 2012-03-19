#include "main.h"

//--// declarations

//--// globals

typedef Evas_Object *(*panel_content_add)(Evas_Object *win);

typedef struct	_Panel_Builder	Panel_Builder;
struct		_Panel_Builder
{
    int                 box;
    const char		*name;
    panel_content_add	function_add;
    int			x_weight;
    int			y_weight;
    int			x_align;
    int			y_align;
};

static Evas_Object *boxes[4];

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

  o = build[i].function_add(win);

  evas_object_size_hint_weight_set(o, build[i].x_weight, build[i].y_weight);
  evas_object_size_hint_align_set(o, build[i].x_align, build[i].y_align);
  elm_box_pack_end(bx, o);
  evas_object_show(o);
}

//--// public routines
void
panels_add(void)
{
  int		i = 0;
  Evas_Object	*o;

  for (i = 0; i < sizeof (boxes) / sizeof (boxes[0]); i++)
  {
    boxes[i] = elm_box_add(design_win);
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

