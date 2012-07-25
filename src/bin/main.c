#include "main.h"
#ifndef ELM_LIB_QUICKLAUNCH

// global used for the debugging mode
// it represents the domain log level
int elegance_log_dom = -1;

// main function wich launch Elegance
EAPI int
elm_main(int argc __UNUSED__,
         char **argv __UNUSED__)
{
  Elm_Theme *th; // theme of Elegance

  // define my own domain
# ifdef DEBUG
  elegance_log_dom = eina_log_domain_register("elegance",
					      EINA_COLOR_RED);
  eina_log_domain_level_set("elegance", EINA_LOG_LEVEL_DBG);
# endif

  // set the main theme
  th = elm_theme_default_get();
  elm_theme_overlay_add(th, PACKAGE_DATA_DIR"/themes/elegance.edj");
  elm_theme_full_flush();

  // create the design of elegance
  design_win_new();

  // launch elegance
  elm_run();

  // exit
  elm_shutdown();
  return 0;
}
#endif
ELM_MAIN()
