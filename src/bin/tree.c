#include "main.h"

//--// declarations

//--// globals
static Elm_Genlist_Item_Class itc;

//--// callbacks
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
    elm_entry_entry_append(test_entry, content->tool.name);
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
  Evas_Object *list;
  Elegance_Content *test;
  Elegance_Tool tool_test = {"special", "special", "special", "special", "special",
			     elm_win_inwin_add, elm_win_inwin_content_set };

  ELEGANCE_LOG("begin");

  itc.item_style     = "default";
  itc.func.text_get  = gl_text_get;
  itc.func.content_get  = gl_content_get;
  itc.func.state_get = gl_state_get;
  itc.func.del       = gl_del;

  list = elm_genlist_add(win);
  elm_genlist_select_mode_set(list, ELM_OBJECT_SELECT_MODE_NONE);
  evas_object_size_hint_weight_set(list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(list, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_show(list);

  // to erase
  test = malloc(sizeof(Elegance_Content));
  test->name = strdup("test");
  test->obj = NULL;
  test->lay = NULL;
  test->tool = tool_test;
  test->child = NULL;

  elm_genlist_item_append(list, &itc, test, NULL,
			  ELM_GENLIST_ITEM_NONE,
			  NULL, NULL);

  return list;
}
