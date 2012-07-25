#ifndef ELEGANCE_DESIGN_H
# define ELEGANCE_DESIGN_H

//--// functions
void design_win_new(void); // create, initialize Elegance
void design_exit   (void); // kill Elegance

//--// extern global
extern Evas_Object *design_win; // Elegance window
extern Evas_Object *design_layout; // Elegance main layout

#endif
