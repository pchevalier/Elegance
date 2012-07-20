#include "main.h"

//--// declarations
Elegance_Project *actual_project = NULL; // actual opened project
Elegance_Page *actual_page = NULL; // actual select page
Elegance_Content *actual_content = NULL; // actual select content

//--// globals
static int lvl;
static Eet_Data_Descriptor *_project_descriptor;
static Eet_Data_Descriptor *_page_descriptor;
static Eet_Data_Descriptor *_content_descriptor;
static Eet_Data_Descriptor *_tool_descriptor;

//--// callbacks
void
hash_table_data_free_cb(void *data)
{
  free(data);
}

//--// private routines
static void
_my_cache_descriptor_init(void)
{
  Eet_Data_Descriptor_Class eddc;

  EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Elegance_Project);
  _project_descriptor = eet_data_descriptor_file_new(&eddc);

   EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Elegance_Page);
   _page_descriptor = eet_data_descriptor_file_new(&eddc);

   EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Elegance_Content);
   _content_descriptor = eet_data_descriptor_file_new(&eddc);

   EET_EINA_FILE_DATA_DESCRIPTOR_CLASS_SET(&eddc, Elegance_Tool);
   _tool_descriptor = eet_data_descriptor_file_new(&eddc);

#define ADD_BASIC(member, eet_type)					\
   EET_DATA_DESCRIPTOR_ADD_BASIC					\
     (_project_descriptor, Elegance_Project, # member, member, eet_type)
   ADD_BASIC(name, EET_T_STRING);
#undef ADD_BASIC
   EET_DATA_DESCRIPTOR_ADD_LIST(_project_descriptor, Elegance_Project,
   				"pages", pages, _page_descriptor);

#define ADD_BASIC(member, eet_type)					\
   EET_DATA_DESCRIPTOR_ADD_BASIC					\
     (_page_descriptor, Elegance_Page, # member, member, eet_type)
   ADD_BASIC(name, EET_T_STRING);
   ADD_BASIC(hide_contents, EET_T_UCHAR);
#undef ADD_BASIC
   EET_DATA_DESCRIPTOR_ADD_LIST(_page_descriptor, Elegance_Page,
   				"contents", contents, _content_descriptor);

#define ADD_BASIC(member, eet_type) \
   EET_DATA_DESCRIPTOR_ADD_BASIC					\
     (_content_descriptor, Elegance_Content, # member, member, eet_type)
   ADD_BASIC(name, EET_T_STRING);
   ADD_BASIC(col, EET_T_UINT);
   ADD_BASIC(row, EET_T_UINT);
#undef ADD_BASIC
   EET_DATA_DESCRIPTOR_ADD_LIST(_content_descriptor, Elegance_Content,
   				"child", child, _content_descriptor);
   EET_DATA_DESCRIPTOR_ADD_HASH_STRING(_content_descriptor, Elegance_Content,
				       "prop", prop);
   EET_DATA_DESCRIPTOR_ADD_SUB(_content_descriptor, Elegance_Content,
			       "tool", tool, _tool_descriptor);

#define ADD_BASIC(member, eet_type) \
   EET_DATA_DESCRIPTOR_ADD_BASIC					\
     (_tool_descriptor, Elegance_Tool, # member, member, eet_type)
   ADD_BASIC(position, EET_T_UINT);
   ADD_BASIC(type, EET_T_UINT);
   ADD_BASIC(type_name, EET_T_STRING);
   ADD_BASIC(name, EET_T_STRING);
   ADD_BASIC(icon_small, EET_T_STRING);
   ADD_BASIC(icon_big, EET_T_STRING);
#undef ADD_BASIC

}

void
serialize_project(char *file)
{
  Eet_File		*ef = NULL;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  ef = eet_open(file, EET_FILE_MODE_WRITE);
  eet_data_write(ef, _project_descriptor, "project",
  		 actual_project, EINA_TRUE);
  eet_close(ef);
}

static void
rebuild_tools(Elegance_Content *content)
{

  Elegance_Content *c;
  Eina_List *l2;

  EINA_LIST_FOREACH(content->child, l2, c)
  {
    ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
		   "Rebuild: %s %i %i", c->name,
		   c->tool->type, c->tool->position);
    switch (c->tool->type)
    {
      case 0: c->tool->function_add = inwin_tool.function_add;
	c->tool->function_pack = inwin_tool.function_pack;
	c->tool->function_prop = inwin_tool.function_prop;
	break;
      case 1: c->tool->function_add =
	  container_list[c->tool->position].function_add;
	c->tool->function_pack =
	  container_list[c->tool->position].function_pack;
	c->tool->function_prop =
	  container_list[c->tool->position].function_prop;
	break;
      case 2: c->tool->function_add =
	  elm_list[c->tool->position].function_add;
	c->tool->function_pack =
	  elm_list[c->tool->position].function_pack;
	c->tool->function_prop =
	  elm_list[c->tool->position].function_prop;
	break;
    }
    if (c->child)
      rebuild_tools(c);
  }
}

void
unserialize_project(char *file)
{
  Eet_File *ef = eet_open(file, EET_FILE_MODE_READ);
  Eina_List *l1, *l2;
  Elegance_Page *page;
  Elegance_Content *content;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  view_clean(actual_page->contents);
  status_clean();
  if (!ef)
  {
    ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
		 "ERROR: could not open '%s' for read", file);
    return;
  }

  actual_project = eet_data_read(ef,
				 _project_descriptor,
				 "project");

  EINA_LIST_FOREACH(actual_project->pages, l1, page)
  {
    EINA_LIST_FOREACH(page->contents, l2, content)
    {
      ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
		   "Rebuild: %s %i %i", content->name,
		   content->tool->type, content->tool->position);
      switch (content->tool->type)
      {
	case 0:
	  content->tool->function_add = inwin_tool.function_add;
	  content->tool->function_pack = inwin_tool.function_pack;
	  content->tool->function_prop = inwin_tool.function_prop;
	  break;
	case 1: content->tool->function_add =
	    container_list[content->tool->position].function_add;
	  content->tool->function_pack =
	    container_list[content->tool->position].function_pack;
	  content->tool->function_prop =
	    container_list[content->tool->position].function_prop;
	  break;
	case 2: content->tool->function_add =
	    elm_list[content->tool->position].function_add;
	  content->tool->function_pack =
	    elm_list[content->tool->position].function_pack;
	  content->tool->function_prop =
	    elm_list[content->tool->position].function_prop;
	  break;
      }
      if (content->child)
	rebuild_tools(content);

      status_grid->new_page = EINA_TRUE;
      actual_page = page;
      status_refresh();
    }
  }

  actual_page =  eina_list_data_get(actual_project->pages);
  view_reload(actual_page->contents);
  palette_refresh();
  elm_genlist_clear(tree_list);
  eet_close(ef);
}

// function to print all childs of a content
static void
serialize_print_childs(Elegance_Content *obj2)
{
  Elegance_Content *obj3;
  Eina_List *l3;

  EINA_LIST_FOREACH(obj2->child, l3, obj3)
  {
    ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
		 "        %i Content named: %s - (%p) with %i childs",
		 lvl, obj3->name, obj3->obj, eina_list_count(obj3->child));
    if(obj3->child)
    {
      lvl++;
      // make recursion again
      serialize_print_childs(obj3);
      lvl--;
    }
  }
}

// private recursive function for printing all contents of a page
static void
serialize_print_contents(Elegance_Page *obj1)
{
  Elegance_Content *obj2;
  Eina_List *l2;

  EINA_LIST_FOREACH(obj1->contents, l2, obj2)
  {
    ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
		 "        %i Content named: %s - (%p) with %i childs",
		 lvl, obj2->name, obj2->obj, eina_list_count(obj2->child));
    if(obj2->child)
    {
      lvl++;
      // recursive function for childs
      serialize_print_childs(obj2);
      lvl--;
    }
  }
}

//--// public routines
// this function can print all elements of the actual project
void
serialize_print(void)
{
  Eina_List *l1;
  Elegance_Page *obj1;

  // check for errors
  if (!actual_project) return;

  // init lvl
  lvl = 0;
  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "--------- Project Structure of %s - %i pages ---------",
	       actual_project->name,
	       eina_list_count(actual_project->pages));
  // execute that to all page's project
  EINA_LIST_FOREACH(actual_project->pages, l1, obj1)
  {
    ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
		 "Page named : %s\nThis page contains :", obj1->name);
    // little recursive function for contents
    serialize_print_contents(obj1);
  }
  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "*********************************************************");
}

// init main structure for serialization (see serialize.h)
void
serialize_init(void)
{
  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  Elegance_Project *project;
  Elegance_Page *page;

  // new project
  project = malloc(sizeof(Elegance_Project));
  project->name = strdup("project");
  project->pages = NULL;

  // focus this project with a global pointer
  actual_project = project;

  // new page
  page = malloc(sizeof(Elegance_Page));
  page->name = strdup("Page 1");
  page->hide_contents = EINA_TRUE;
  page->contents = NULL;

  // add this page to the actual project
  actual_project->pages = eina_list_append(actual_project->pages,
					   page);
  // focus this page with a global pointer
  actual_page = page;
  _my_cache_descriptor_init();
}

