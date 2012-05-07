#include "main.h"

//--// declarations

//--// globals
Evas_Object *view_layout = NULL;
Evas_Object *view_inwin = NULL;

//--// callbacks

//--// private routines
// recursive function for contents and their child
static Evas_Object *
view_reload_child(Elegance_Content *data)
{
  Evas_Object *new, *lay;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin - data: %s", data->name);

  data->lay = lay = elm_layout_add(design_win);
  elm_layout_theme_set(lay, "layout", "application", "add_in_object");
  evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(lay, EVAS_HINT_FILL, EVAS_HINT_FILL);
  data->obj = new = data->tool.function_add(design_win);
  elm_object_part_content_set(lay,
			      "elm.swallow.add_in_object", new);
  if (data->child)
  {
    Eina_List *l_subchild;
    Elegance_Content *data_subchild;
    Evas_Object *lay_sub;

    EINA_LIST_FOREACH(data->child, l_subchild, data_subchild)
    {
      lay_sub = view_reload_child(data_subchild);
      data->tool.function_pack(new, lay_sub,
			       data_subchild->prop->row,
			       data_subchild->prop->col,
			       data_subchild->prop->rowspan,
			       data_subchild->prop->colspan);
    }
  }
  evas_object_event_callback_add(new, EVAS_CALLBACK_MOUSE_DOWN,
				 _show_its_properties_cb, data);
  evas_object_show(lay);
  return lay;
}

//--// public routines
// clean working view
void
view_clean(Eina_List *list)
{
  Elegance_Content *content;
  Eina_List *contents;

  EINA_LIST_FOREACH(list, contents, content)
  {
    ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
		 "content->name: %s", content->name);

    if(content->child)
      view_clean(content->child);

    evas_object_del(content->obj);
    evas_object_del(content->lay);
  }
}

// function to reload a page into working view
void
view_reload(Eina_List *list)
{
  Elegance_Content *content;
  Eina_List *contents;

  EINA_LIST_FOREACH(list, contents, content)
  {
    Evas_Object *lay, *new;

    ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
		 "content->name: %s", content->name);

    content->lay = lay = elm_layout_add(design_win);
    evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_layout_theme_set(lay, "layout", "application", "add_in_object");
    content->obj = new = content->tool.function_add(design_win);

    // special part for inwin
    if (!strcmp(content->name, "inwin"))
    {
      elm_object_part_content_set(design_layout, "elm.swallow.view", lay);
      view_layout = lay;
      view_inwin = new;
      elm_object_style_set(new, "elegance");
      elm_win_inwin_activate(new);
      actual_content = content;
      dnd_target_register(view_layout);
    }
    elm_object_part_content_set(lay, "elm.swallow.add_in_object", new);

    evas_object_show(lay);
    evas_object_show(new);

    // run in recursion for child
    if(content->child)
    {
      Eina_List *l_subchild;
      Elegance_Content *data_subchild;
      Evas_Object *sublay;

      EINA_LIST_FOREACH(content->child, l_subchild, data_subchild)
      {
	sublay = view_reload_child(data_subchild);
	content->tool.function_pack(new, sublay,
				    actual_content->prop->row,
				    actual_content->prop->col,
				    actual_content->prop->rowspan,
				    actual_content->prop->colspan);
      }
    }
    evas_object_event_callback_add(new, EVAS_CALLBACK_MOUSE_DOWN,
				   _show_its_properties_cb, content);
  }
}

// init working view
void
view_add(void)
{
  Evas_Object *lay, *inwin;
  Elegance_Content *content;
  Elegance_Property *prop;
  Elegance_Tool tool = {"special", "special", "special", "special", "special",
			elm_win_inwin_add, elm_win_inwin_content_set };

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  // new content
  content = malloc(sizeof(Elegance_Content));
  content->name = strdup("inwin");

  // create the layout
  view_layout = lay = elm_layout_add(design_win);
  evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  // swallow it
  elm_object_part_content_set(design_layout, "elm.swallow.view", lay);
  elm_layout_theme_set(lay, "layout", "application", "add_in_object");

  // create inwin and put it into layout
  view_inwin = inwin = elm_win_inwin_add(design_win);
  elm_object_part_content_set(lay, "elm.swallow.add_in_object", inwin);
  elm_object_style_set(inwin, "elegance");
  elm_win_inwin_activate(inwin);

  // register layout for drag&drop
  dnd_target_register(view_layout);
  evas_object_show(lay);
  evas_object_show(inwin);

  // fill content's properties
  prop = malloc(sizeof(Elegance_Property));
  prop->name = strdup("inwin");
  prop->x = 0;
  prop->y = 0;
  prop->w = 800;
  prop->h = 600;
  prop->row = 0;
  prop->col = 0;
  prop->rowspan = 1;
  prop->colspan = 1;

  // fill content's informations and add to current page
  content->obj = inwin;
  content->lay = lay;
  content->tool = tool;
  content->child = NULL;
  content->prop = prop;
  actual_page->contents = eina_list_append(actual_page->contents,
					   content);

  evas_object_event_callback_add(inwin, EVAS_CALLBACK_MOUSE_DOWN,
				 _show_its_properties_cb, content);

  // focus this content
  actual_content = content;

  // refresh the status bar
  status_refresh();
}
