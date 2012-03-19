#include "main.h"

//--// declarations

//--// globals
static Elm_Gengrid_Item_Class gic;
static Evas_Object *status_grid;

//--// callbacks
static char *
_grid_label_get(void        *data,
                Evas_Object *obj __UNUSED__,
                const char  *part __UNUSED__)
{
   char buf[256];

   snprintf(buf, sizeof(buf), "Page");
   return strdup(buf);
}

static Evas_Object *
_grid_content_get(void        *data,
		  Evas_Object *obj,
		  const char  *part)
{
  Evas_Object *lay, *new;
  Elegance_Tool *tool = data;

  if (!strcmp(part, "elm.swallow.icon"))
  {
    printf("content_get\n");
    if (!tool)
    {
      printf("tool = %p\n", tool);
      lay = elm_layout_add(design_win);
      evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
      elm_layout_theme_set(lay, "layout", "application", "add_in_object");

      new = elm_win_inwin_add(design_win);
      elm_object_part_content_set(lay, "elm.swallow.add_in_object", new);
      elm_object_style_set(new, "elegance");
      elm_win_inwin_activate(new);

      // fixme
      //project->pages = eina_list_append(project->pages, page);
    }
    else
    {
      printf("tool = %p\n", tool);
      lay = elm_layout_add(design_win);
      evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
      elm_layout_theme_set(lay, "layout", "application", "add_in_object");

      new = tool->function_add(design_win);
      elm_object_part_content_set(lay,
				  "elm.swallow.add_in_object", new);
      edje_object_color_class_set(elm_layout_edje_get(lay), "elegance_1",
				  rand()%256, rand()%256, rand()%256, 30,
				  0, 0, 0, 0,
				  0, 0, 0, 0);
      // fixme
      //project->pages = eina_list_append(project->pages, page);
    }
    evas_object_show(lay);
    return lay;
  }
  return NULL;
}

//--// public routines
Evas_Object *
status_add(Evas_Object *win)
{
  Evas_Object *sc, *grid;
  // add the scroller

  status_grid = grid = elm_gengrid_add(win);
  elm_gengrid_horizontal_set(grid, EINA_TRUE);
  elm_gengrid_item_size_set(grid, 100, 100);
  evas_object_size_hint_weight_set(grid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(grid, EVAS_HINT_FILL, EVAS_HINT_FILL);

  gic.item_style = "default";
  gic.func.text_get = _grid_label_get;
  gic.func.content_get = _grid_content_get;
  gic.func.state_get = NULL;
  gic.func.del = NULL;

  evas_object_show(grid);

  return grid;
}

void
status_refresh(void)
{

}
