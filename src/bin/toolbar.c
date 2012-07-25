#include "main.h"

//--// declarations
static Evas_Object *fs_win = NULL;

//--// callbacks
// file browser's callbacks
static void
_file_chosen_save(void *data,
		  Evas_Object *obj __UNUSED__,
		  void *event_info)
{
  Evas_Object *win = data;
  char *file = event_info;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin -- %s", file);
  evas_object_del(win);
  serialize_project(file);
}

static void
_file_chosen_open(void *data,
		  Evas_Object *obj __UNUSED__,
		  void *event_info)
{
  Evas_Object *win = data;
  char *file = event_info;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin -- %s", file);
  evas_object_del(win);
  unserialize_project(file);
}

// callback called when the file browser is needed
static Evas_Object *
create_fs()
{
  Evas_Object *fs = NULL, *lay = NULL;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  fs_win = elm_win_inwin_add(design_win);

  // create file selector
  fs = elm_fileselector_add(fs_win);
  elm_fileselector_path_set(fs, "/home");
  evas_object_size_hint_weight_set(fs, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(fs, EVAS_HINT_FILL, EVAS_HINT_FILL);
  elm_object_text_set(fs, "File");
  elm_fileselector_is_save_set(fs, EINA_TRUE);
  evas_object_show(fs);

  lay = elm_layout_add(fs_win);
  evas_object_size_hint_weight_set(lay, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
  evas_object_size_hint_align_set(lay, EVAS_HINT_FILL, EVAS_HINT_FILL);
  elm_layout_theme_set(lay, "layout", "application", "titlebar");
  elm_object_part_content_set(lay, "elm.swallow.content", fs);
  elm_object_part_text_set(lay, "elm.text", "Save");
  evas_object_show(lay);

  evas_object_size_hint_min_set(fs_win, 800, 600);
  elm_win_inwin_content_set(fs_win, lay);
  elm_win_inwin_activate(fs_win);

  return fs;
}

//callback for saving function
static void
_toolbar_save_cb(void *data __UNUSED__,
	       Evas_Object *obj __UNUSED__,
	       void *event_info __UNUSED__)
{
  Evas_Object *fs = NULL;

  fs = create_fs();
  evas_object_smart_callback_add(fs,
				 "done",
  				 _file_chosen_save, fs_win);
}

//callback for opening function
static void
_toolbar_open_cb(void *data __UNUSED__,
	       Evas_Object *obj __UNUSED__,
	       void *event_info __UNUSED__)
{
  Evas_Object *fs = NULL;

  fs = create_fs();
  evas_object_smart_callback_add(fs,
				 "done",
  				 _file_chosen_open, fs_win);
}

// callback for new page
void
_toolbar_new_page_cb(void *data __UNUSED__,
		     Evas_Object *obj __UNUSED__,
		     void *event_info __UNUSED__)
{
  Elegance_Page *page;


  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  // clean view and add new item to status's gengrid
  status_grid->new_page = EINA_TRUE;
  popup_on = EINA_FALSE;
  view_clean(actual_page->contents);

  // add a new page to actual_project
  page = malloc(sizeof(Elegance_Page));
  page->name = strdup("Page sup");
  page->contents = NULL;
  page->hide_contents = EINA_TRUE;
  actual_project->pages = eina_list_append(actual_project->pages,
					   page);
  // focus this new page
  actual_page = page;

  // create view again
  view_add();

  // refresh palette to hide contents
  palette_refresh();
}

// callback to create a new project
void
_toolbar_new_project_cb(void *data __UNUSED__,
			Evas_Object *obj __UNUSED__,
			void *event_info __UNUSED__)
{
  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "new project");
}

//--// public routines
// init toolbar
Evas_Object *
toolbar_add(Evas_Object *win)
{
  Evas_Object *tb, *menu;
  Elm_Object_Item *tb_it;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

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
  elm_toolbar_item_append(tb, "default", "Open", _toolbar_open_cb, NULL);
  elm_toolbar_item_append(tb, "default", "Save", _toolbar_save_cb, NULL);

  return tb;
}
