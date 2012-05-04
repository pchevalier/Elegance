#include "main.h"
#ifndef ELM_LIB_QUICKLAUNCH

int elegance_log_dom = -1;

EAPI int
elm_main(int argc __UNUSED__,
         char **argv __UNUSED__)
{
  Elm_Theme *th;

  // define my own domain
# ifdef DEBUG
  printf("debug\n");
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
  elm_shutdown();
  return 0;
}
#endif
ELM_MAIN()
