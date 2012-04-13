#include "main.h"
#ifndef ELM_LIB_QUICKLAUNCH

EAPI int
elm_main(int argc __UNUSED__,
         char **argv __UNUSED__)
{
  Elm_Theme *th;

  // debug mode
  if (DEBUG)
    eina_log_level_set(ELEGANCE_LOG_LEVEL);

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
