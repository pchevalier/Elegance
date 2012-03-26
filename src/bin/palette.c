#include "main.h"

//--// declarations

//--// globals
static int cols = 4;
static int num = 0;
static Elm_Genlist_Item_Class itc;
static Elm_Genlist_Item_Class itp;
static Evas_Object *palette_list;

// special list for container tools
static const Elegance_Tool container_list[2] = {
  { "container", "box", "Box", "box-s.png", "box-b.png",
    elm_box_add, elm_box_pack_end },
  { "container", "table", "Table", "table-s.png", "table-b.png",
    elm_table_add, elm_table_pack },
};

// list for all elm tools
static const Elegance_Tool elm_list[5] = {
  { "elementary", "icon", "Icon", "icon-s.png", "icon-b.png",
    elm_icon_add, NULL },
  { "elementary", "button", "Button", "button-s.png", "button-b.png",
    elm_button_add, NULL },
  { "elementary", "clock", "Clock", "clock-s.png", "clock-b.png",
    elm_clock_add, NULL },
  { "elementary", "video", "Video", "video-s.png", "video-b.png",
    elm_video_add, NULL },
  { "elementary", "bubble", "Bubble", "bubble-s.png", "bubble-b.png",
    elm_bubble_add, NULL },
};

// list for evas object tools
static const Elegance_Tool evas_list[3] = {
  { "evas", "rect", "Rectangle", "rect-s.png", "rect-b.png",
    evas_object_rectangle_add, NULL },
  { "evas", "text", "Text", "text-s.png", "text-b.png",
    evas_object_text_add, NULL },
  { "evas", "textblock", "Text Block", "textblock-s.png", "textblock-b.png",
    evas_object_textblock_add, NULL },
};

//--// callbacks
// genlist's callback
static char *
gl_text_get(void *data,
		  Evas_Object *obj __UNUSED__,
		  const char *part __UNUSED__)
{
  char buf[256];
  Elegance_Tool *d = data;

  snprintf(buf, sizeof(buf), "%s", d->label);
  return strdup(buf);
}

static Evas_Object *
gl_content_get(void *data,
	       Evas_Object *obj __UNUSED__,
	       const char *part __UNUSED__)
{
  Evas_Object *o = NULL;
  Elegance_Tool *d = data;

  if (!strcmp(part, "elm.swallow.end"))
  {
    o = dnd_add(design_win);
    dnd_action_dir_set(o, DND_RIGHT);
    dnd_icon_set(o, d->icon_small);
    dnd_icon_type_set(o, d->name);
    dnd_icon_drag_set(o, d->icon_big);
    evas_object_size_hint_align_set(o, -1, -1);
    evas_object_show(o);
    evas_object_pointer_mode_set(o, EVAS_OBJECT_POINTER_MODE_NOGRAB);
  }
  return o;
}

Eina_Bool
gl_state_get(void *data __UNUSED__,
	     Evas_Object *obj __UNUSED__,
	     const char *part __UNUSED__)
{
   return EINA_FALSE;
}

void gl_del(void *data __UNUSED__,
	    Evas_Object *obj __UNUSED__)
{
}

char *
gl8_text_get(void *data,
	     Evas_Object *obj __UNUSED__,
	     const char *part __UNUSED__)
{
  char buf[256];
  Elegance_Tool *d = data;

  if (data == container_list)
  {
    snprintf(buf, sizeof(buf), "%s", "container");
    return strdup(buf);
  }
  if (data == elm_list)
  {
    snprintf(buf, sizeof(buf), "%s", "elementary");
    return strdup(buf);
  }
  if (data == evas_list)
  {
    snprintf(buf, sizeof(buf), "%s", "evas");
    return strdup(buf);
  }
}

//--// private routines

// init function to create the palette's genlist
static Evas_Object *
init_palette_genlist(Evas_Object *win)
{
  Evas_Object *list;
  Elm_Object_Item *gli = NULL, *git = NULL;
  int i = 0;

  itc.item_style     = "default";
  itc.func.text_get  = gl_text_get;
  itc.func.content_get  = gl_content_get;
  itc.func.state_get = gl_state_get;
  itc.func.del       = gl_del;

  itp.item_style     = "group_index";
  itp.func.text_get  = gl8_text_get;
  itp.func.content_get  = NULL;
  itp.func.state_get = NULL;
  itp.func.del       = gl_del;

  list = elm_genlist_add(win);
  evas_object_pointer_mode_set(list, EVAS_OBJECT_POINTER_MODE_NOGRAB);
  evas_object_size_hint_weight_set(list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(list, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_show(list);

  // fixme to erase
  gli = elm_genlist_item_append(list, &itp, &container_list, NULL,
				ELM_GENLIST_ITEM_GROUP,
				NULL, NULL);
  for(i = 0; i < sizeof(container_list) / sizeof(container_list[0]); i++)
  {
    elm_genlist_item_append(list, &itc, &container_list[i], gli,
			    ELM_GENLIST_ITEM_NONE,
			    NULL, NULL);
  }
  return (list);
}

// function called when an object has to be added in a container
static void
add_in_container(const Elegance_Tool *list,
		 int i)
{
  Evas_Object *new, *lay;
  Elegance_Content *content;


  content = malloc(sizeof(Elegance_Content));
  content->name = strdup(list[i].name);

  lay = elm_layout_add(design_win);
  evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(lay, EVAS_HINT_FILL, EVAS_HINT_FILL);
  elm_layout_theme_set(lay, "layout", "application", "add_in_object");

  actual_content->tool.function_pack(actual_content->obj, lay,
				     NULL, NULL, NULL, NULL);

  if(!strcmp(list[i].type, "evas"))
    new = list[i].function_add(evas_object_evas_get(design_win));
  else
    new = list[i].function_add(design_win);
  elm_object_part_content_set(lay, "elm.swallow.add_in_object", new);
  edje_object_color_class_set(lay, "elegance_1",
			      rand()%256, rand()%256, rand()%256, 40,
			      0, 0, 0, 0,
			      0, 0, 0, 0);

  content->obj = new;
  content->lay = lay;
  content->child = NULL;
  content->tool = list[i];
  actual_content->child = eina_list_append(actual_content->child, content);

  dnd_target_register(lay);
  evas_object_show(lay);
  evas_object_show(new);
}

//--// public routines

// first adding function of palette that add containers into
Evas_Object *
palette_add(Evas_Object *win)
{
  Evas_Object *o;
  int i, x, y;

  // init genlist
  palette_list = o = init_palette_genlist(win);
  evas_object_show(o);

  return o;
}

// second function to add other tools
// after the adding of a container into the view
void
palette_add_contents(Evas_Object *win)
{
  int i;
  Elm_Object_Item *gli;
  static Eina_Bool done = EINA_FALSE;

  if (!done)
  {
    // add other tools into the palette's tab
    gli = elm_genlist_item_append(palette_list, &itp, &elm_list, NULL,
				  ELM_GENLIST_ITEM_GROUP,
				  NULL, NULL);
    for(i = 0; i < sizeof(elm_list) / sizeof(elm_list[0]); i++)
    {
      elm_genlist_item_append(palette_list, &itc, &elm_list[i], gli,
			      ELM_GENLIST_ITEM_NONE,
			      NULL, NULL);
    }

    gli = elm_genlist_item_append(palette_list, &itp, &evas_list, NULL,
				  ELM_GENLIST_ITEM_GROUP,
				  NULL, NULL);
    for(i = 0; i < sizeof(evas_list) / sizeof(evas_list[0]); i++)
    {
      elm_genlist_item_append(palette_list, &itc, &evas_list[i], gli,
			      ELM_GENLIST_ITEM_NONE,
			      NULL, NULL);
    }
    done = EINA_TRUE;
  }
}

// usefull function that detect if a container is added into the inwin (view)
void
palette_refresh(Evas_Object *icon,
		Evas_Coord x,
		Evas_Coord y)
{
  char *buf;
  int i = 0;

  buf = evas_object_data_get(icon, "--dnd_type");
  if (strcmp(actual_content->tool.name, "special"))
  {
    for (i = 0; i < sizeof(elm_list) / sizeof(elm_list[0]); i++)
    {
      if (!strcmp(buf, elm_list[i].name))
	return (add_in_container(elm_list, i));
    }
    for (i = 0; i < sizeof(evas_list) / sizeof(evas_list[0]); i++)
    {
      if (!strcmp(buf, evas_list[i].name))
	return (add_in_container(evas_list, i));
    }
    for (i = 0; i < sizeof(container_list) / sizeof(container_list[0]); i++)
    {
      if (!strcmp(buf, container_list[i].name))
	return (add_in_container(container_list, i));
    }
  }
  else
  {
    for (i = 0; i < sizeof(container_list) / sizeof(container_list[0]); i++)
    {
      if (!strcmp(buf, container_list[i].name))
      {
	Evas_Object *new, *lay;
	Elegance_Content *content;

	content = malloc(sizeof(Elegance_Content));
	content->name = buf;

	lay = elm_layout_add(design_win);
	evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_layout_theme_set(lay, "layout", "application", "add_in_object");
	actual_content->tool.function_pack(actual_content->obj, lay,
					   NULL, NULL, NULL, NULL);

	new = container_list[i].function_add(design_win);
	elm_object_part_content_set(lay,
				    "elm.swallow.add_in_object", new);
	edje_object_color_class_set(lay, "elegance_1",
				    rand()%256, rand()%256, rand()%256, 30,
				    0, 0, 0, 0,
				    0, 0, 0, 0);

	content->obj = new;
	content->lay = lay;
	content->tool = container_list[i];
	content->child = NULL;
	actual_content->child = eina_list_append(actual_content->child,
						 content);
	actual_content = content;

	dnd_target_register(new);
	dnd_target_unregister(view_inwin);
	evas_object_show(lay);
	evas_object_show(new);

	palette_add_contents(design_win);
      }
    }
  }
}
