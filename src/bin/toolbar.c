#include "main.h"

//--// declarations

//--// globals

//--// callbacks

//--// public routines
void
_toolbar_new_page_cb(void *data,
		     Evas_Object *obj,
		     void *event_info)
{
  Elegance_Page *page;
  Elegance_Content *content;
  Elegance_Tool tool = {"special", "special", "special", "special", "special",
			elm_win_inwin_add, elm_win_inwin_content_set };
  Evas_Object *lay, *new;

  printf("_toolbar_new_page_cb\n");

  status_grid->new_page = EINA_TRUE;
  view_clean(actual_page->contents);

  page = malloc(sizeof(Elegance_Page));
  page->name = strdup("Page sup");
  page->contents = NULL;
  actual_project->pages = eina_list_append(actual_project->pages,
					   page);
  actual_page = page;

  view_add();

  palette_hide_contents();
}

void
_toolbar_new_project_cb(void *data,
			Evas_Object *obj,
			void *event_info)
{
  printf("new project\n");
}

Evas_Object *
toolbar_add(Evas_Object *win)
{
  Evas_Object *tb, *menu;
  Elm_Object_Item *tb_it;

  printf("toolbar_add\n");

  // add toolbar to main win
  tb = elm_toolbar_add(win);
  elm_toolbar_menu_parent_set(tb, win);

  // fill toolbar
  tb_it = elm_toolbar_item_append(tb, "default", "New", NULL, NULL);
  elm_toolbar_item_menu_set(tb_it, EINA_TRUE);
  menu = elm_toolbar_item_menu_get(tb_it);
  elm_menu_item_add(menu, NULL, "folder-new", "New Project",
		    _toolbar_new_project_cb, NULL);
  elm_menu_item_add(menu, NULL, "folder-new", "New Page",
		    _toolbar_new_page_cb, NULL);

  elm_toolbar_item_append(tb, "default", "Open", NULL, NULL);
  elm_toolbar_item_append(tb, "default", "Save", NULL, NULL);

  return tb;
}
