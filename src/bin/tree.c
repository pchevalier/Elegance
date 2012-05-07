#include "main.h"

//--// declarations

//--// globals
static Elm_Genlist_Item_Class itc;
static Evas_Object *tree_list;
static Elegance_Content *actual_selected;
static Evas_Object *tree_popup;

Eina_Bool popup_on = EINA_FALSE;

//--// callbacks
static void
_dismissed(void *data __UNUSED__,
	   Evas_Object *obj,
	   void *event_info __UNUSED__)
{
  Evas_Object *popup_data = evas_object_data_get(obj, "im");

  if (popup_data)
    evas_object_del(popup_data);
  evas_object_del(obj);
  popup_on = EINA_FALSE;
}

static void
_popup_item_cb(void *data,
	       Evas_Object *obj,
	       void *event_info __UNUSED__)
{
  Elegance_Content *content = data;

  if (actual_selected != content)
  {
    elm_genlist_clear(tree_list);
    elm_genlist_item_append(tree_list, &itc, content, NULL,
  			    ELM_GENLIST_ITEM_NONE,
  			    NULL, NULL);
    actual_content = content;
  }
  _dismissed(NULL, obj, NULL);
  popup_on = EINA_FALSE;
}

Elm_Object_Item *
item_new(const char * label,
	 const char *icon,
	 Elegance_Content *content)
{
   Evas_Object *ic = elm_icon_add(tree_popup);
   char buf[PATH_MAX];

   snprintf(buf, sizeof(buf), "%s/tools/%s", PACKAGE_DATA_DIR, icon);
   elm_icon_file_set(ic, buf, NULL);
   elm_icon_resizable_set(ic, EINA_FALSE, EINA_FALSE);
   return elm_popup_item_append(tree_popup, label, ic,
				   _popup_item_cb, content);
}

void
_show_its_properties_cb(void *data,
			Evas *e __UNUSED__,
			Evas_Object *obj,
			void *event_info __UNUSED__)
{
  Elegance_Content *content = data;
  Evas_Coord x,y;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  if (!popup_on)
  {
    tree_popup = elm_popup_add(design_win);
    evas_object_size_hint_weight_set(tree_popup,
				     EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    item_new(content->name, content->tool.icon_small, content);
    evas_pointer_canvas_xy_get(evas_object_evas_get(obj), &x, &y);
    evas_object_move(tree_popup, x, y);
    evas_object_show(tree_popup);

    popup_on = EINA_TRUE;
  }
  else
    item_new(content->name, content->tool.icon_small, content);
}

// genlist's callback
static char *
gl_text_get(void *data,
	    Evas_Object *obj __UNUSED__,
	    const char *part __UNUSED__)
{
  Elegance_Content *content = data;
  char buf[256];

  snprintf(buf, sizeof(buf), "%s", content->name);
  return strdup(buf);
}

static Evas_Object *
gl_content_get(void *data,
	       Evas_Object *obj __UNUSED__,
	       const char *part __UNUSED__)
{
  Evas_Object *test_entry = NULL;
  Elegance_Content *content = data;

  if (!strcmp(part, "elm.swallow.end"))
  {
    Elm_Entry_Filter_Limit_Size filter_limit;
    Elm_Entry_Filter_Accept_Set filter_accept;

    test_entry = elm_entry_add(design_win);
    elm_entry_entry_append(test_entry, content->tool.icon_small);
    elm_entry_single_line_set(test_entry, EINA_TRUE);
    evas_object_size_hint_weight_set(test_entry,
				     EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    evas_object_size_hint_align_set(test_entry,
				    EVAS_HINT_FILL, EVAS_HINT_FILL);
    filter_limit.max_char_count = 10;
    filter_accept.accepted = "0123456789";
    elm_entry_markup_filter_append(test_entry,
				   elm_entry_filter_limit_size,
				   &filter_limit);
    elm_entry_markup_filter_append(test_entry,
				   elm_entry_filter_accept_set,
				   &filter_accept);
  }

  return test_entry;
}

static Eina_Bool
gl_state_get(void *data __UNUSED__,
	     Evas_Object *obj __UNUSED__,
	     const char *part __UNUSED__)
{
   return EINA_FALSE;
}

static void
gl_del(void *data __UNUSED__,
	    Evas_Object *obj __UNUSED__)
{
}

//--// public routines
Evas_Object *
tree_add(Evas_Object *win)
{
  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  actual_selected = NULL;

  itc.item_style        = "default";
  itc.func.text_get     = gl_text_get;
  itc.func.content_get  = gl_content_get;
  itc.func.state_get    = gl_state_get;
  itc.func.del          = gl_del;

  tree_list = elm_genlist_add(win);
  elm_genlist_select_mode_set(tree_list, ELM_OBJECT_SELECT_MODE_NONE);
  evas_object_size_hint_weight_set(tree_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(tree_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_show(tree_list);

  return tree_list;
}
