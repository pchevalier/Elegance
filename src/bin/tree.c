#include "main.h"

//--// declarations

//--// globals
static Elm_Genlist_Item_Class itc;
static Evas_Object *tree_list;
static Elegance_Content *actual_selected;
static Evas_Object *tree_popup;

Eina_Bool popup_on = EINA_FALSE;

//--// callbacks
// others
// function which fill the genlist item with an entry
static Evas_Object *
display_property(Evas_Object *entry,
		 Elegance_Property *prop)
{
  Elm_Entry_Filter_Limit_Size filter_limit;
  Elm_Entry_Filter_Accept_Set filter_accept;

  entry = elm_entry_add(design_win);
  elm_entry_entry_append(entry, prop->data);
  elm_entry_single_line_set(entry, EINA_TRUE);
  evas_object_size_hint_weight_set(entry,
				     EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(entry,
				  EVAS_HINT_FILL, EVAS_HINT_FILL);
  filter_limit.max_char_count = 10;
  filter_accept.accepted = "0123456789";
  elm_entry_markup_filter_append(entry,
				 elm_entry_filter_limit_size,
				 &filter_limit);
  elm_entry_markup_filter_append(entry,
				 elm_entry_filter_accept_set,
				 &filter_accept);
  return entry;
}

// genlist's callbacks
static int
gl_cmp(const void *pa, const void *pb)
{
  const Elm_Object_Item *ia = pa, *ib = pb;
  Elegance_Property *a = elm_object_item_data_get(ia);
  Elegance_Property *b = elm_object_item_data_get(ib);

  return strcmp(a->name, b->name);
}

static char *
gl_text_get(void *data,
	    Evas_Object *obj __UNUSED__,
	    const char *part __UNUSED__)
{
  Elegance_Property *prop = data;
  char buf[256];

  snprintf(buf, sizeof(buf), "%s", prop->name);
  return strdup(buf);
}

static Evas_Object *
gl_content_get(void *data,
	       Evas_Object *obj __UNUSED__,
	       const char *part __UNUSED__)
{
  Evas_Object *entry = NULL;
  Elegance_Property *prop = data;

  if (!strcmp(part, "elm.swallow.end"))
  {
    return display_property(entry, prop);
  }

  return entry;
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

//hash map's callbacks
static Eina_Bool
hash_fn(const Eina_Hash *hash __UNUSED__,
	const void *key,
	void *data,
	void *fdata __UNUSED__)
{
  Elegance_Property *prop = malloc(sizeof(Elegance_Property));

  prop->name = key;
  prop->data = data;
  elm_genlist_item_sorted_insert(tree_list, &itc, prop, NULL,
				 ELM_GENLIST_ITEM_NONE, gl_cmp,
				 NULL, NULL);
  return EINA_TRUE;
}

// popup's callbacks
// callback to del the popup
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

// callback to define actions when an item is selected
static void
_popup_item_cb(void *data,
	       Evas_Object *obj,
	       void *event_info __UNUSED__)
{
  Elegance_Content *content = data;

  if (actual_selected != content)
  {
    elm_genlist_clear(tree_list);
    eina_hash_foreach(content->prop, hash_fn, NULL);
    actual_selected = content;
  }
  _dismissed(NULL, obj, NULL);
}

// others
// usefull function to fill the choising item popup with icon
static Elm_Object_Item *
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

//--// public routines
// fill the choice item popup
void
_show_its_properties_cb(void           *data,
                         Evas_Object    *obj,
                         const char     *emission,
                         const char     *source)
{
  Elegance_Content *content = data;
  Evas_Coord x,y;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  // check if the popup is already created.
  // if no popup --> create and fill it
  if (!popup_on)
  {
    ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
		 "if %p", content);

    // create the popup
    tree_popup = elm_popup_add(design_win);
    evas_object_size_hint_weight_set(tree_popup,
				     EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    item_new(content->name, content->tool.icon_small, content);
    evas_pointer_canvas_xy_get(evas_object_evas_get(obj), &x, &y);
    evas_object_move(tree_popup, x, y);
    evas_object_show(tree_popup);
    popup_on = EINA_TRUE;

    ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
		 "end if");
  }
  // else --> just fill it with more items
  else
  {
    ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
		 "else");
    item_new(content->name, content->tool.icon_small, content);
  }
}

// add the tree's genlist in Elegance to show item's properties
Evas_Object *
tree_add(Evas_Object *win)
{
  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  // initialize the current selected item
  actual_selected = NULL;

  // fill the genlist item class
  itc.item_style        = "default";
  itc.func.text_get     = gl_text_get;
  itc.func.content_get  = gl_content_get;
  itc.func.state_get    = gl_state_get;
  itc.func.del          = gl_del;

  // create the genlist into Elegance
  tree_list = elm_genlist_add(win);
  elm_genlist_select_mode_set(tree_list, ELM_OBJECT_SELECT_MODE_NONE);
  evas_object_size_hint_weight_set(tree_list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(tree_list, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_show(tree_list);

  return tree_list;
}
