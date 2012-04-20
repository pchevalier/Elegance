#include "main.h"

//--// declarations

//--// globals
static Elm_Genlist_Item_Class itc;

//--// callbacks
// genlist's callback
static char *
gl_text_get(void *data __UNUSED__,
	    Evas_Object *obj __UNUSED__,
	    const char *part __UNUSED__)
{
  Elegance_Property *prop = data;

  return prop->name;
}

static Evas_Object *
gl_content_get(void *data __UNUSED__,
	       Evas_Object *obj __UNUSED__,
	       const char *part __UNUSED__)
{

  if (!strcmp(part, "elm.swallow.end"))
  {

  }
  return NULL;
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

  ELEGANCE_LOG("begin");

  itc.item_style     = "default";
  itc.func.text_get  = gl_text_get;
  itc.func.content_get  = gl_content_get;
  itc.func.state_get = gl_state_get;
  itc.func.del       = gl_del;

  list = elm_genlist_add(win);
  evas_object_size_hint_weight_set(list, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(list, EVAS_HINT_FILL, EVAS_HINT_FILL);
  evas_object_show(list);

  printf("tree finish\n");

  return list;
}
