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

  ELEGANCE_LOG("begin - data: %s", data->name);

  data->lay = lay = elm_layout_add(design_win);
  elm_layout_theme_set(lay, "layout", "application", "add_in_object");
  evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(lay, EVAS_HINT_FILL, EVAS_HINT_FILL);
  if(!strcmp(data->tool.type, "evas"))
    data->obj = new = data->tool.function_add(evas_object_evas_get(design_win));
  else
    data->obj = new = data->tool.function_add(design_win);
  elm_object_part_content_set(lay,
			      "elm.swallow.add_in_object", new);
  edje_object_color_class_set(lay, "elegance_1",
			      rand()%256, rand()%256, rand()%256, 30,
			      0, 0, 0, 0,
			      0, 0, 0, 0);
  if (data->child)
  {
    Eina_List *l_subchild;
    Elegance_Content *data_subchild;
    Evas_Object *lay_sub;

    EINA_LIST_FOREACH(data->child, l_subchild, data_subchild)
    {
      lay_sub = refresh_childs(data_subchild);
      data->tool.function_pack(new, lay_sub,
			       NULL, NULL, NULL, NULL);
    }
  }
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
    ELEGANCE_LOG("content->name: %s", content->name);

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

   ELEGANCE_LOG("content->name: %s", content->name);

    content->lay = lay = elm_layout_add(design_win);
    evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_layout_theme_set(lay, "layout", "application", "add_in_object");

    if(!strcmp(content->tool.type, "evas"))
      content->obj = new = content->tool.function_add(
	evas_object_evas_get(design_win));
    else
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
    edje_object_color_class_set(lay, "elegance_1",
				rand()%256, rand()%256, rand()%256, 40,
				0, 0, 0, 0,
				0, 0, 0, 0);

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
				    NULL, NULL, NULL, NULL);
      }
    }
  }
}

// init working view
void
view_add(void)
{
  Evas_Object *lay, *inwin;
  Elegance_Content *content;
  Elegance_Tool tool = {"special", "special", "special", "special", "special",
			elm_win_inwin_add, elm_win_inwin_content_set };

  ELEGANCE_LOG("begin");

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

  // fill content's informations and add to current page
  content->obj = inwin;
  content->lay = lay;
  content->tool = tool;
  content->child = NULL;
  actual_page->contents = eina_list_append(actual_page->contents,
					   content);
  // focus this content
  actual_content = content;

  // register layout for drag&drop
  dnd_target_register(view_layout);
  evas_object_show(lay);
  evas_object_show(inwin);

  // refresh the status bar
  status_refresh();
}
