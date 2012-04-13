#include "main.h"
#ifndef ELM_LIB_QUICKLAUNCH

//--// declarations
static void
_design_cb_win_del(void        *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void        *event_info __UNUSED__);

//--// globals
Evas_Object *design_win = NULL;
Evas_Object *design_layout = NULL;

//--// callbacks
static void
_design_cb_win_del(void        *data __UNUSED__,
                   Evas_Object *obj __UNUSED__,
                   void        *event_info __UNUSED__)
{
  design_exit();
}

//--// public routines
void
design_win_new(void)
{
  Evas_Object *win, *bg, *lay;

  // create new window base and set title and hook delete request
  design_win = win = elm_win_add(NULL, "main", ELM_WIN_BASIC);
  elm_win_title_set(win, "Elegance");
  evas_object_smart_callback_add(win, "delete,request",
				 _design_cb_win_del, NULL);

  // add a default background to the window
  bg = elm_bg_add(win);
  evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  elm_win_resize_object_add(win, bg);
  evas_object_show(bg);

  // define the main layout and set the main theme of elegance
  design_layout = lay = elm_layout_add(win);
  evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  elm_layout_theme_set(lay, "layout", "application", "elegance_main");

  // +--------+--------------------------------+-----------+
  // |toolbar |snippets                        |tree       |
  // +--------+                                |           |
  // |palette |                                |           |
  // |        +--------------------------------+           |
  // |        |view           |__|             |           |
  // |        |                                |           |
  // |        |__                            __|           |
  // |        |__|                          |__|           |
  // |        |                                |           |
  // |        |                __              |           |
  // |        |               |  |             |           |
  // |        +--------------------------------+           |
  // |        |status                          |           |
  // +--------+--------------------------------+-----------+

  // add windows's elements
  // initialize the project tree structure
  serialize_init();

  // add panels and fill them
  panels_add();

  // add the working zone which is named view
  view_add();

  // set resizes and shows
  elm_win_resize_object_add(win, lay);
  evas_object_show(lay);
  evas_object_show(win);
  evas_object_size_hint_min_set(win, 800, 600);
  evas_object_resize(win, 1024, 800);
}

void
design_exit(void)
{
  elm_exit();
}
#endif
