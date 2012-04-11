#include "main.h"

//--// declarations

//--// globals
static Evas_Coord dx, dy, dropx, dropy;
static Eina_List *targets = NULL;

static int
_dnd_sorter(const void *d1, const void *d2)
{
  Evas_Object *o1 = (Evas_Object *)d1;
  Evas_Object *o2 = (Evas_Object *)d2;
  Eina_List *o1l = NULL, *o2l = NULL, *l1, *l2, *list, *l;
  Evas_Object *common = NULL, *o;

  while (o1)
  {
    o1l = eina_list_prepend(o1l, o1);
    o1 = evas_object_smart_parent_get(o1);
  }
  while (o2)
  {
    o2l = eina_list_prepend(o2l, o2);
    o2 = evas_object_smart_parent_get(o2);
  }
  o1 = o1l->data;
  o2 = o2l->data;
  if (o1 != o2)
  {
    // different toplevels
    o = evas_object_bottom_get(evas_object_evas_get(o1));
    while (o)
    {
      if (o1 == o)
      {
	// o1 below o2
	eina_list_free(o1l);
	eina_list_free(o2l);
	return -1;
      }
      if (o2 == o)
      {
	// o2 below o1
	eina_list_free(o1l);
	eina_list_free(o2l);
	return 1;
      }
      o = evas_object_above_get(o);
    }
  }
  for (l1 = o1l, l2 = o2l;
       l1 && l2;
       l1 = l1->next, l2 = l2->next)
  {
    o1 = l1->data; o2 = l2->data;
    if (o1 != o2)
    {
      // children differ, thus parent is common
      list = evas_object_smart_members_get(common);
      for (l = list; l; l = l->next)
      {
	o = l->data;
	if (o1 == o)
	{
	  // o1 below o2
	  eina_list_free(list);
	  eina_list_free(o1l);
	  eina_list_free(o2l);
	  return -1;
	}
	if (o2 == o)
	{
	  // o2 below o1
	  eina_list_free(list);
	  eina_list_free(o1l);
	  eina_list_free(o2l);
	  return 1;
	}
      }
      eina_list_free(list);
      break;
    }
    common = o1;
  }
  eina_list_free(o1l);
  eina_list_free(o2l);
  return 0;
}

static Evas_Object *
_dnd_target_find(Evas_Coord x, Evas_Coord y)
{
  Eina_List *l, *in = NULL;
  Evas_Object *obj;

  EINA_LIST_FOREACH(targets, l, obj)
  {
    Evas_Coord ox, oy, ow, oh;

    evas_object_geometry_get(obj, &ox, &oy, &ow, &oh);
    if ((x >= ox) && (x < (ox + ow)) &&
	(y >= oy) && (y < (oy + oh)))
    {
      in = eina_list_sorted_insert(in, _dnd_sorter, obj);
    }
  }
  if (!in) return NULL;
  obj = in->data;
  eina_list_free(in);
  return obj;
}

static void
_dnd_handle(Evas_Object *obj, Evas_Coord x, Evas_Coord y)
{
  Evas_Object *dnd_icon = evas_object_data_get(obj, "--dnd_icon");
  Evas_Object *o;
  Evas_Coord w, h;

  if (!dnd_icon) return;
  dropx = x;
  dropy = y;
  evas_object_size_hint_min_get(dnd_icon, &w, &h);
  evas_object_move(dnd_icon, x - (w / 2), y - (h / 2));
  evas_object_resize(dnd_icon, w, h);
  o = _dnd_target_find(x, y);
}

static void
_dnd_action(Evas_Object *obj, Dnd_Dir dir, Evas_Coord x, Evas_Coord y)
{
  Evas_Object *dnd_icon = evas_object_data_get(obj, "--dnd_icon");

  if (dir != (Dnd_Dir)evas_object_data_get(obj, "--dnd_dir")) return;
  if (!dnd_icon)
  {
    char buf[PATH_MAX];

    dnd_icon = elm_icon_add(elm_object_top_widget_get(obj));
    elm_icon_no_scale_set(dnd_icon, 1);
    elm_icon_resizable_set(dnd_icon, EINA_FALSE, EINA_FALSE);
    evas_object_data_set(obj, "--dnd_icon", dnd_icon);
    snprintf(buf, sizeof(buf), "%s/tools/%s", PACKAGE_DATA_DIR,
	     (const char *)evas_object_data_get(obj, "--dnd_big_icon"));
    elm_icon_file_set(dnd_icon, buf, NULL);
    evas_object_show(dnd_icon);
    elm_object_scroll_freeze_push(obj);
  }
}

static void
find_and_refresh_top_actual_content(Eina_List *contents,
				    Evas_Coord x1,
				    Evas_Coord y1)
{
  Eina_List *l;
  Elegance_Content *data;
  Elegance_Content *new = NULL;

  printf("find_and_refresh_top_actual_content\n");

  if (!contents) return;
  EINA_LIST_FOREACH(contents, l, data)
  {
    if (!strcmp(data->tool.type, "container")||
	!strcmp(data->name, "inwin"))
    {
      Evas_Coord ox, oy, ow, oh;

      evas_object_geometry_get(data->obj, &ox, &oy, &ow, &oh);
      if (x1 >= ox && x1 <= ox+ow &&
	  y1 >= oy && y1 <= oy+oh)
      {
	actual_content = data;
	find_and_refresh_top_actual_content(data->child, x1, y1);
      }
    }
  }
}

static void
_dnd_finish(Evas_Object *obj, Evas_Coord x, Evas_Coord y)
{
  Evas_Object *o;
  Evas_Object *dnd_icon = evas_object_data_get(obj, "--dnd_icon");

  printf("_dnd_finish\n");

  elm_object_scroll_freeze_pop(obj);
  o = _dnd_target_find(x, y);
  find_and_refresh_top_actual_content(actual_page->contents, x, y);

  if (o != NULL)
  {
    view_refresh(obj, x, y);
    status_refresh();
  }

  evas_object_del(dnd_icon);
  evas_object_data_del(obj, "--dnd_icon");
}

//--// callbacks
static void
_dnd_mouse_down(void        *data,
                Evas        *evas __UNUSED__,
                Evas_Object *obj __UNUSED__,
                void        *event_info)
{
  Evas_Event_Mouse_Down *ev = event_info;
  if (ev->button != 1) return;
  evas_object_data_set(data, "--dnd_down", (void *)1);
  dx = ev->canvas.x;
  dy = ev->canvas.y;
}

static void
_dnd_mouse_up(void        *data,
              Evas        *evas __UNUSED__,
              Evas_Object *obj __UNUSED__,
              void        *event_info)
{
  Evas_Event_Mouse_Up *ev = event_info;
  if (ev->button != 1) return;
  evas_object_data_set(data, "--dnd_down", (void *)0);
  _dnd_finish(data, ev->canvas.x, ev->canvas.y);
}

static void
_dnd_mouse_move(void        *data,
                Evas        *evas __UNUSED__,
                Evas_Object *obj __UNUSED__,
                void        *event_info)
{
  Evas_Event_Mouse_Move *ev = event_info;
  Evas_Coord x, y, x1, y1, d, fs;

  if (!evas_object_data_get(data, "--dnd_down")) return;
  x1 = ev->cur.canvas.x;
  y1 = ev->cur.canvas.y;
  x = x1 - dx;
  y = y1 - dy;

  d = (x * x) + (y * y);
  fs = elm_config_finger_size_get();
  if (evas_object_data_get(data, "--dnd_icon"))
  {
    _dnd_handle(data, ev->cur.canvas.x, ev->cur.canvas.y);
    return;
  }
  if (d < (fs * fs)) return;
  if (abs(y) < (fs / 2))
  {
    if (x < 0)
      _dnd_action(data, DND_LEFT, ev->cur.canvas.x, ev->cur.canvas.y);
    else
      _dnd_action(data, DND_RIGHT, ev->cur.canvas.x, ev->cur.canvas.y);
  }
  else if (abs(x) < (fs / 2))
  {
    if (y < 0)
      _dnd_action(data, DND_UP, ev->cur.canvas.x, ev->cur.canvas.y);
    else
      _dnd_action(data, DND_DOWN, ev->cur.canvas.x, ev->cur.canvas.y);
  }
  _dnd_handle(data, ev->cur.canvas.x, ev->cur.canvas.y);
}

static void
_dnd_del(void        *data,
         Evas        *evas __UNUSED__,
         Evas_Object *obj __UNUSED__,
         void        *event_info __UNUSED__)
{
  Evas_Object *dnd_icon = evas_object_data_get(data, "--dnd_icon");
  if (dnd_icon)
  {
    evas_object_data_del(data, "--dnd_icon");
    evas_object_del(dnd_icon);
  }
}

static void
_target_del(void        *data __UNUSED__,
	    Evas        *evas __UNUSED__,
            Evas_Object *obj,
            void        *event_info __UNUSED__)
{
  dnd_target_unregister(obj);
}

//--// public routines
Evas_Object *
dnd_add(Evas_Object *parent)
{
  Evas_Object *dnd;

  dnd = elm_icon_add(parent);
  elm_icon_no_scale_set(dnd, 1);
  elm_icon_resizable_set(dnd, EINA_FALSE, EINA_FALSE);
  evas_object_pointer_mode_set(dnd, EVAS_OBJECT_POINTER_MODE_NOGRAB);
  evas_object_event_callback_add(dnd, EVAS_CALLBACK_MOUSE_DOWN,
				 _dnd_mouse_down, dnd);
  evas_object_event_callback_add(dnd, EVAS_CALLBACK_MOUSE_UP,
				 _dnd_mouse_up, dnd);
  evas_object_event_callback_add(dnd, EVAS_CALLBACK_MOUSE_MOVE,
				 _dnd_mouse_move, dnd);
  evas_object_event_callback_add(dnd, EVAS_CALLBACK_DEL,
				 _dnd_del, dnd);
  return dnd;
}

void
dnd_icon_set(Evas_Object *obj, const char *icon)
{
  char buf[PATH_MAX];

  snprintf(buf, sizeof(buf), "%s/tools/%s", PACKAGE_DATA_DIR, icon);
  elm_icon_file_set(obj, buf, NULL);
}

void
dnd_icon_type_set(Evas_Object *obj, const char *type)
{
  evas_object_data_set(obj, "--dnd_type", (void *)type);
}


void
dnd_icon_drag_set(Evas_Object *obj, const char *icon)
{
  evas_object_data_set(obj, "--dnd_big_icon", (void *)icon);
}

void
dnd_action_dir_set(Evas_Object *obj, Dnd_Dir dir)
{
  evas_object_data_set(obj, "--dnd_dir", (void *)dir);
}

void
dnd_data_set(Evas_Object *obj, void *data)
{
  evas_object_data_set(obj, "--dnd_data", data);
}

void *
dnd_data_get(Evas_Object *obj)
{
  return evas_object_data_get(obj, "--dnd_data");
}

void
dnd_target_register(Evas_Object *obj)
{
  printf("dnd_target_register\n");

  evas_object_event_callback_add(obj, EVAS_CALLBACK_DEL,
				 _target_del, NULL);
  targets = eina_list_append(targets, obj);
}

void
dnd_target_unregister(Evas_Object *obj)
{
  printf("dnd_target_unregister\n");

  evas_object_event_callback_del(obj, EVAS_CALLBACK_DEL,
				 _target_del);
  targets = eina_list_remove(targets, obj);
}
