#include "main.h"

//--// declarations

//--// globals

//--// callbacks

//--// public routines
Evas_Object *
tree_add(Evas_Object *win)
{
  Evas_Object *gl;

  // add a genlist into the right panel
  gl = elm_genlist_add(win);

  return gl;
}
