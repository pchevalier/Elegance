#include "main.h"

//--// declarations

//--// globals
Elegance_Gengrid *status_grid = NULL;

//--// callbacks
static char *
_grid_label_get(void        *data,
                Evas_Object *obj __UNUSED__,
                const char  *part __UNUSED__)
{
  Elegance_Gengrid_Item *item = data;

  return strdup(item->name);
}


static Eina_Bool
_grid_state_get(void        *data,
                Evas_Object *obj __UNUSED__,
                const char  *part __UNUSED__)
{
  return EINA_FALSE;
}

static void
_grid_del(void        *data,
          Evas_Object *obj __UNUSED__)
{
}

Evas_Object *
refresh_childs(Elegance_Content *data)
{
  Evas_Object *new, *lay;

  printf("refresh_childs -- %s\n", data->name);

  lay = elm_layout_add(design_win);
  elm_layout_theme_set(lay, "layout", "application", "add_in_object");
  evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(lay, EVAS_HINT_FILL, EVAS_HINT_FILL);
  if(!strcmp(data->tool.type, "evas"))
    new = data->tool.function_add(evas_object_evas_get(design_win));
  else
    new = data->tool.function_add(design_win);
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

static Evas_Object *
refresh_layout(Elegance_Gengrid_Item *item)
{
  Eina_List *l;
  Elegance_Content *data;

  printf("refresh_layout\n");

  EINA_LIST_FOREACH(item->page->contents, l, data)
  {
    Evas_Object *new = NULL;

    if(!strcmp(data->tool.type, "evas"))
      new = data->tool.function_add(evas_object_evas_get(design_win));
    else
      new = data->tool.function_add(design_win);
    elm_object_part_content_set(item->lay,
				"elm.swallow.add_in_object", new);
    edje_object_color_class_set(item->lay, "elegance_1",
				rand()%256, rand()%256, rand()%256, 30,
				0, 0, 0, 0,
				0, 0, 0, 0);
    if(!strcmp(data->tool.name, "special"))
    {
      elm_win_inwin_activate(new);
      elm_object_style_set(new, "elegance");
    }

    if(data->child)
    {
      Eina_List *l_subchild;
      Elegance_Content *data_subchild;
      Evas_Object *lay;

      EINA_LIST_FOREACH(data->child, l_subchild, data_subchild)
      {
	lay = refresh_childs(data_subchild);
      	data->tool.function_pack(new, lay,
      				 NULL, NULL, NULL, NULL);
      }
    }
  }
  return item->lay;
}

static void
_call_new_page_cb(void *data,
		  Evas *e,
		  Evas_Object *obj,
		  void *event_info)
{
  printf("_call_new_page_cb\n");

  _toolbar_new_page_cb(NULL, NULL, NULL);
}

static void
_call_reload_cb(void *data,
		Evas *e,
		Evas_Object *obj,
		void *event_info)
{
  Elegance_Gengrid_Item *item = data;
  Evas_Object *lay;

  printf("_call_reload_cb\n");

  view_clean(actual_page->contents);
  actual_page = item->page;

  view_reload(actual_page->contents);
}

static Evas_Object *
_grid_content_get(void        *data,
		  Evas_Object *obj,
		  const char  *part)
{
  Elegance_Gengrid_Item *item = data;

  if (!strcmp(part, "elm.swallow.icon"))
  {
    Evas_Object *lay;

    printf("_grid_content_get\n");

    lay = elm_layout_add(design_win);
    evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(lay, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(lay);

    item->lay = lay;

    if(!item->page)
    {
      Evas_Object *image;

      printf("_grid_content_get -- new_button\n");

      image = elm_image_add(lay);
      evas_object_size_hint_weight_set(image, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
      evas_object_size_hint_align_set(image, EVAS_HINT_FILL, EVAS_HINT_FILL);
      elm_image_file_set(image, PACKAGE_DATA_DIR"/themes/plus.png", NULL);
      evas_object_event_callback_add(image, EVAS_CALLBACK_MOUSE_UP,
				     _call_new_page_cb, NULL);
      evas_object_show(image);

      elm_gengrid_item_show(elm_gengrid_last_item_get(status_grid->gengrid),
			    ELM_GENGRID_ITEM_SCROLLTO_IN);

      return image;
    }

    printf("_grid_content_get -- new_page\n");

    elm_layout_theme_set(lay, "layout", "application", "add_in_object");
    lay = refresh_layout(item);
    evas_object_event_callback_add(lay, EVAS_CALLBACK_MOUSE_UP,
				   _call_reload_cb, item);
    return lay;
  }
  return NULL;
}

//--// public routines
void
status_refresh(void)
{
  Elegance_Gengrid_Item *item, *item_plus;
  static Eina_Bool button_plus = EINA_FALSE;

  if(!button_plus)
  {
    printf("status_refresh -- button_plus\n");

    item_plus = malloc(sizeof(Elegance_Gengrid_Item));
    item_plus->page = NULL;
    item_plus->lay = NULL;
    item_plus->name = strdup("New Page");
    status_grid->items = eina_list_append(status_grid->items,
					  item_plus);

    elm_gengrid_item_append(status_grid->gengrid, status_grid->gic,
			    item_plus, NULL, NULL);

    button_plus = EINA_TRUE;
  }

  if(status_grid->new_page)
  {
    printf("status_refresh -- new_page\n");

    item = malloc(sizeof(Elegance_Gengrid_Item));
    item->page = actual_page;
    item->lay = NULL;
    item->name = actual_page->name;
    status_grid->items = eina_list_append(status_grid->items,
					  item);

    elm_gengrid_item_insert_before(status_grid->gengrid, status_grid->gic, item,
				   elm_gengrid_last_item_get(status_grid->gengrid),
				   NULL, NULL);

    status_grid->new_page = EINA_FALSE;
  }
  elm_gengrid_realized_items_update(status_grid->gengrid);

  evas_object_show(status_grid->gengrid);

}

Evas_Object *
status_add(Evas_Object *win)
{
  Evas_Object *sc, *grid;

  printf("status_add\n");

  status_grid = malloc(sizeof(Elegance_Gengrid));
  status_grid->new_page = EINA_TRUE;
  status_grid->items = NULL;

  status_grid->gengrid = grid = elm_gengrid_add(win);
  elm_gengrid_horizontal_set(grid, EINA_TRUE);
  elm_gengrid_item_size_set(grid, 125, 125);
  evas_object_size_hint_weight_set(grid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(grid, EVAS_HINT_FILL, EVAS_HINT_FILL);
  elm_gengrid_align_set(grid, 0, 0.5);
  elm_gengrid_multi_select_set(grid, EINA_FALSE);
  elm_gengrid_select_mode_set(grid, ELM_OBJECT_SELECT_MODE_NONE);
  elm_gengrid_scroller_policy_set(grid, ELM_SCROLLER_POLICY_AUTO,
				  ELM_SCROLLER_POLICY_OFF);

  status_grid->gic = elm_gengrid_item_class_new();
  status_grid->gic->item_style = "default";
  status_grid->gic->func.text_get = _grid_label_get;
  status_grid->gic->func.content_get = _grid_content_get;
  status_grid->gic->func.state_get = _grid_state_get;
  status_grid->gic->func.del = _grid_del;

  evas_object_show(grid);

  return grid;
}

