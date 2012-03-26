#include "main.h"

//--// declarations
Elegance_Project *actual_project = NULL;
Elegance_Page *actual_page = NULL;
Elegance_Content *actual_content = NULL;

//--// globals
static int lvl;

//--// callbacks

//--// private routines
// function to print childs of a content
static void
serialize_print_childs(Elegance_Content *obj2)
{
  Elegance_Content *obj3;
  Eina_List *l3;

  EINA_LIST_FOREACH(obj2->child, l3, obj3)
  {
    printf("        %i Content named: %s - (%p) with %i childs\n",
	   lvl, obj3->name, obj3->obj, eina_list_count(obj3->child));
    if(obj3->child)
    {
      lvl++;
      serialize_print_childs(obj3);
      lvl--;
    }
  }
}

static void
serialize_print_contents(Elegance_Page *obj1)
{
  Elegance_Content *obj2;
  Eina_List *l2;

  EINA_LIST_FOREACH(obj1->contents, l2, obj2)
  {
    printf("        %i Content named: %s - (%p) with %i childs\n",
	   lvl, obj2->name, obj2->obj, eina_list_count(obj2->child));
    if(obj2->child)
    {
      lvl++;
      serialize_print_childs(obj2);
      lvl--;
    }
  }
}

//--// public routines
void
serialize_print(void)
{
  Eina_List *l1;
  Elegance_Page *obj1;

  if (!actual_project) return;

  lvl = 0;
  printf("\n--------- Project Structure of %s - %i pages ---------\n",
	 actual_project->name,
	 eina_list_count(actual_project->pages));
  EINA_LIST_FOREACH(actual_project->pages, l1, obj1)
  {
    printf("Page named : %s\nThis page contains :\n", obj1->name);
    serialize_print_contents(obj1);
  }
  printf("*********************************************************\n");
}

void
serialize_init(void)
{
  actual_project = malloc(sizeof(Elegance_Project));
  actual_project->name = strdup("project");
  actual_project->pages = NULL;

  actual_page = malloc(sizeof(Elegance_Page));
  actual_page->name = strdup("Page 1");
  actual_page->contents = NULL;
  actual_project->pages = eina_list_append(actual_project->pages,
					   actual_page);
}

