#include "main.h"

//--// declarations

//--// globals
Elegance_Gengrid *status_grid = NULL;
static Evas_Object *status_popup = NULL;

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
_grid_state_get(void        *data __UNUSED__,
                Evas_Object *obj __UNUSED__,
                const char  *part __UNUSED__)
{
  return EINA_FALSE;
}

static void
_grid_del(void        *data __UNUSED__,
          Evas_Object *obj __UNUSED__)
{
}

// return layout with contents for refresh_layout
Evas_Object *
refresh_childs(Elegance_Content *data)
{
  Evas_Object *new, *lay;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin - data->name: %s", data->name);

  lay = elm_layout_add(design_win);
  elm_layout_theme_set(lay, "layout", "application", "add_in_object");
  evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(lay, EVAS_HINT_FILL, EVAS_HINT_FILL);
  new = data->tool.function_add(design_win);
  elm_object_part_content_set(lay,
			      "elm.swallow.add_in_object", new);

  if (data->child)
  {
    Eina_List *l_subchild;
    Elegance_Content *data_subchild;
    Evas_Object *lay_sub;

    EINA_LIST_FOREACH(data->child, l_subchild, data_subchild)
    {
      lay_sub = refresh_childs(data_subchild);

      data->tool.function_pack(new, lay_sub,
			       atoi(eina_hash_find(data_subchild->prop,
						   "col")),
			       atoi(eina_hash_find(data_subchild->prop,
						   "row")),
			       atoi(eina_hash_find(data_subchild->prop,
						   "colspan")),
			       atoi(eina_hash_find(data_subchild->prop,
						   "rowspan")));
    }
  }
  evas_object_show(lay);
  return lay;
}

// load page's items into a layout
static Evas_Object *
refresh_layout(Elegance_Gengrid_Item *item)
{
  Eina_List *l;
  Elegance_Content *data;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  EINA_LIST_FOREACH(item->page->contents, l, data)
  {
    Evas_Object *new = NULL;

    new = data->tool.function_add(design_win);
    elm_object_part_content_set(item->lay,
				"elm.swallow.add_in_object", new);
    if(!strcmp(data->name, "inwin"))
    {
      elm_object_style_set(new, "elegance");
      elm_win_inwin_activate(new);
      evas_object_show(new);
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
				 atoi(eina_hash_find(data_subchild->prop,
						     "col")),
				 atoi(eina_hash_find(data_subchild->prop,
						     "row")),
				 atoi(eina_hash_find(data_subchild->prop,
						     "colspan")),
				 atoi(eina_hash_find(data_subchild->prop,
						     "rowspan")));
      }
    }
  }
  return item->lay;
}

static void
_call_cancel_cb(void *data __UNUSED__,
		Evas_Object *obj __UNUSED__,
		void *event_info __UNUSED__)
{
  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  evas_object_del(status_popup);
}

// callback for adding a new page
static void
_call_new_page_cb(void *data __UNUSED__,
		  Evas *e __UNUSED__,
		  Evas_Object *obj __UNUSED__,
		  void *event_info __UNUSED__)
{
  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  _toolbar_new_page_cb(NULL, NULL, NULL);
}

// callback for reloading a page
static void
_call_reload_cb(void *data,
		Evas_Object *obj __UNUSED__,
		void *event_info __UNUSED__)
{
  Elegance_Gengrid_Item *item = data;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");
  view_clean(actual_page->contents);
  actual_page = item->page;

  view_reload(actual_page->contents);
  palette_refresh();
  evas_object_del(status_popup);;
}

// callback for changed string entry
static void
_label_changed(void *data,
	       Evas_Object *obj,
	       void *event_info __UNUSED__)
{
  Elegance_Gengrid_Item *item = data;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin %s", item->page->name);

  item->page->name = (char *)elm_object_text_get(obj);
  item->name = (char *)elm_object_text_get(obj);
  elm_gengrid_realized_items_update(status_grid->gengrid);

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "%s", item->page->name);
}

// callback to change item name
static void
_call_select_cb(void *data,
		Evas *e __UNUSED__,
		Evas_Object *obj __UNUSED__,
		void *event_info __UNUSED__)
{

  Elegance_Gengrid_Item *item = data;
  Evas_Object *btn1, *btn2, *table, *entry, *label;
  Evas_Coord x,y;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  if (item->page)
  {
    table = elm_table_add(design_win);
    elm_table_homogeneous_set(table, EINA_TRUE);

    label = elm_label_add(table);
    elm_object_text_set(label, "Page's name: ");
    evas_object_size_hint_fill_set(label, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(label, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_table_pack(table, label, 0, 0, 1, 1);
    evas_object_show(label);

    entry = elm_entry_add(table);
    elm_entry_entry_append(entry, item->page->name);
    elm_entry_single_line_set(entry, EINA_TRUE);
    evas_object_size_hint_fill_set(entry, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(entry, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_table_pack(table, entry, 1, 0, 2, 1);
    evas_object_show(entry);

    evas_object_smart_callback_add(entry,
				   "changed",
				   _label_changed, item);

    btn1 = elm_button_add(table);
    elm_object_text_set(btn1, "Save & Refresh");
    evas_object_size_hint_fill_set(btn1, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(btn1, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_table_pack(table, btn1, 0, 1, 2, 1);
    evas_object_show(btn1);

    btn2 = elm_button_add(table);
    elm_object_text_set(btn2, "Cancel");
    evas_object_size_hint_fill_set(btn2, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_size_hint_weight_set(btn2, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_table_pack(table, btn2, 2, 1, 1, 1);
    evas_object_show(btn2);

    status_popup = elm_ctxpopup_add(design_win);
    elm_object_content_set(status_popup, table);
    evas_object_size_hint_min_set(status_popup, 200, 100);

    evas_pointer_canvas_xy_get(evas_object_evas_get(design_win), &x, &y);
    evas_object_move(status_popup, x, y);
    evas_object_show(status_popup);

    evas_object_smart_callback_add(btn1, "clicked", _call_reload_cb, item);
    evas_object_smart_callback_add(btn2, "clicked", _call_cancel_cb, NULL);
  }
}

// add-in gengrid function
static Evas_Object *
_grid_content_get(void        *data,
		  Evas_Object *obj __UNUSED__,
		  const char  *part)
{
  Elegance_Gengrid_Item *item = data;

  if (!strcmp(part, "elm.swallow.icon"))
  {
    Evas_Object *lay;

    ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
		 "begin");

    lay = elm_layout_add(design_win);
    evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(lay, EVAS_HINT_FILL, EVAS_HINT_FILL);
    evas_object_show(lay);

    item->lay = lay;

    // special section for plus button
    if(!item->page)
    {
      Evas_Object *image;

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

    elm_layout_theme_set(lay, "layout", "application", "add_in_object");
    lay = refresh_layout(item);
    evas_object_event_callback_add(lay, EVAS_CALLBACK_MOUSE_UP,
				   _call_select_cb, item);
    return lay;
  }
  return NULL;
}

//--// public routines
// function to refresh status's gengrid
void
status_refresh(void)
{
  Elegance_Gengrid_Item *item, *item_plus;
  static Eina_Bool button_plus = EINA_FALSE;

  // call once to add a new page button in gengrid
  if(!button_plus)
  {
    ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
		 "button_plus");

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

  // add a new item if there is a new page
  if(status_grid->new_page)
  {
    ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
		 "new_page");

    item = malloc(sizeof(Elegance_Gengrid_Item));
    item->page = actual_page;
    item->lay = NULL;
    item->name = actual_page->name;
    status_grid->items = eina_list_append(status_grid->items,
					  item);

    item->it = elm_gengrid_item_insert_before(status_grid->gengrid,
					status_grid->gic, item,
					elm_gengrid_last_item_get(
					  status_grid->gengrid),
					NULL, NULL);

    status_grid->new_page = EINA_FALSE;
  }
  // refresh gengrid's items
  elm_gengrid_realized_items_update(status_grid->gengrid);
}

// init function for status gengrid
Evas_Object *
status_add(Evas_Object *win)
{
  Evas_Object *grid;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  // initialize contents' structure
  status_grid = malloc(sizeof(Elegance_Gengrid));
  status_grid->new_page = EINA_TRUE;
  status_grid->items = NULL;

  // create gengrid
  status_grid->gengrid = grid = elm_gengrid_add(win);
  elm_gengrid_horizontal_set(grid, EINA_TRUE);
  elm_gengrid_item_size_set(grid, 125, 125);
  evas_object_size_hint_weight_set(grid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(grid, EVAS_HINT_FILL, EVAS_HINT_FILL);
  elm_gengrid_align_set(grid, 0, 0.5);
  elm_gengrid_multi_select_set(grid, EINA_FALSE);
  elm_gengrid_select_mode_set(grid, ELM_OBJECT_SELECT_MODE_NONE);
  elm_gengrid_bounce_set(grid, EINA_TRUE, EINA_FALSE);

  // initialisation of the gengrid item class
  status_grid->gic = elm_gengrid_item_class_new();
  status_grid->gic->item_style = "default";
  status_grid->gic->func.text_get = _grid_label_get;
  status_grid->gic->func.content_get = _grid_content_get;
  status_grid->gic->func.state_get = _grid_state_get;
  status_grid->gic->func.del = _grid_del;

  evas_object_show(grid);
  return grid;
}

