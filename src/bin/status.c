#include "main.h"

//--// declarations

//--// globals
static Elegance_Gengrid *status_grid = NULL;

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
_grid_del(void        *data,
          Evas_Object *obj __UNUSED__)
{
}

static Evas_Object *
refresh_childs(Elegance_Content *data)
{
  Evas_Object *new, *lay;

  printf("        adding %s\n", data->name);
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
      printf("        packer: %s\n", data->tool.name);
      lay_sub = refresh_childs(data_subchild);
      data->tool.function_pack(new, lay_sub,
			       NULL, NULL, NULL, NULL);
    }
  }
  evas_object_show(lay);
  printf("refresh %s finish\n", data->name);
  return lay;
}

static Evas_Object *
refresh_layout(Elegance_Gengrid_Item *item)
{
  Eina_List *l;
  Elegance_Content *data;

  EINA_LIST_FOREACH(item->page->contents, l, data)
  {
    Evas_Object *new = NULL;

    printf("    adding %s\n", data->name);

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
      	printf("    data: %s\n", data->tool.name);
	lay = refresh_childs(data_subchild);
      	data->tool.function_pack(new, lay,
      				 NULL, NULL, NULL, NULL);
      }
    }
  }
  printf("refresh finish\n");
  return item->lay;
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

    lay = elm_layout_add(design_win);
    evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(lay, EVAS_HINT_FILL, EVAS_HINT_FILL);
    elm_layout_theme_set(lay, "layout", "application", "add_in_object");
    evas_object_show(lay);

    item->lay = lay;

    lay = refresh_layout(item);
    return lay;
  }
  return NULL;
}

//--// public routines
void
status_refresh(void)
{
  Elegance_Gengrid_Item *item = NULL;

  printf("status_refresh with %i items - ", status_grid->nb_items);

  if(status_grid->nb_items == 0)
  {
    printf("append\n");

    item = malloc(sizeof(Elegance_Gengrid_Item));
    item->page = actual_page;
    item->lay = NULL;
    item->name = actual_page->name;
    status_grid->items = eina_list_append(status_grid->items,
					  item);
    status_grid->nb_items++;

    elm_gengrid_item_append(status_grid->gengrid, status_grid->gic,
			    item, NULL, NULL);
  }
  else
  {
    printf("update\n");
    elm_gengrid_realized_items_update(status_grid->gengrid);
  }
  printf("\n\n");
}

Evas_Object *
status_add(Evas_Object *win)
{
  Evas_Object *sc, *grid;

  status_grid = malloc(sizeof(Elegance_Gengrid));
  status_grid->nb_items = 0;
  status_grid->items = NULL;

  status_grid->gengrid = grid = elm_gengrid_add(win);
  elm_gengrid_horizontal_set(grid, EINA_TRUE);
  elm_gengrid_item_size_set(grid, 120, 120);
  evas_object_size_hint_weight_set(grid, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(grid, EVAS_HINT_FILL, EVAS_HINT_FILL);
  elm_gengrid_align_set(grid, 0, 0.5);
  elm_gengrid_multi_select_set(grid, EINA_FALSE);

  status_grid->gic = elm_gengrid_item_class_new();
  status_grid->gic->item_style = "default";
  status_grid->gic->func.text_get = _grid_label_get;
  status_grid->gic->func.content_get = _grid_content_get;
  status_grid->gic->func.state_get = _grid_state_get;
  status_grid->gic->func.del = _grid_del;

  evas_object_show(grid);

  return grid;
}

