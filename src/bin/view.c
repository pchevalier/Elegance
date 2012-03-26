#include "main.h"

//--// declarations

//--// globals
Evas_Object *view_layout = NULL;
Evas_Object *view_inwin = NULL;

//--// callbacks

//--// public routines
void
view_add(void)
{
  Evas_Object *lay, *inwin;
  Elegance_Content *content;
  Elegance_Tool tool = {"special", "special", "special", "special", "special",
			elm_win_inwin_add, elm_win_inwin_content_set };

  content = malloc(sizeof(Elegance_Content));
  content->name = strdup("inwin");

  view_layout = lay = elm_layout_add(design_win);
  evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  elm_object_part_content_set(design_layout, "elm.swallow.view", lay);
  elm_layout_theme_set(lay, "layout", "application", "add_in_object");

  view_inwin = inwin = elm_win_inwin_add(design_win);
  elm_object_part_content_set(lay, "elm.swallow.add_in_object", inwin);
  elm_object_style_set(inwin, "elegance");
  elm_win_inwin_activate(inwin);

  content->obj = inwin;
  content->lay = lay;
  content->tool = tool;
  content->child = NULL;
  actual_page->contents = eina_list_append(actual_page->contents,
					   content);
  actual_content = content;

  dnd_target_register(view_inwin);
  evas_object_show(lay);
  evas_object_show(inwin);
  status_refresh();
}
