#ifndef SERIALIZE_H_
# define SERIALIZE_H_

typedef struct  _Elegance_Content Elegance_Content;
struct		_Elegance_Content
{
    char		*name;
    Elegance_Tool	tool;
    Evas_Object		*obj;
    Evas_Object		*lay;
    Eina_List		*child;
};

typedef struct  _Elegance_Page Elegance_Page;
struct		_Elegance_Page
{
    char		*name;
    Eina_List		*contents;
};

typedef struct  _Elegance_Project Elegance_Project;
struct		_Elegance_Project
{
    char		*name;
    Eina_List		*pages;
};

extern Elegance_Project *actual_project;
extern Elegance_Page *actual_page;
extern Elegance_Content *actual_content;

void serialize_init(void);
void serialize_print(void);

#endif
