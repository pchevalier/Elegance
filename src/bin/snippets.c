#include "main.h"

//--// declarations

//--// globals

//--// callbacks

//--// public routines
Evas_Object *
snippets_add(Evas_Object *win)
{
  Evas_Object *tb;

  // add toolbar to main win
  tb = elm_toolbar_add(win);
  elm_toolbar_menu_parent_set(tb, win);

  // FIXME: fill the toolbar
  elm_toolbar_item_append(tb, "default", "To Fill", NULL, NULL);

  return tb;
}
