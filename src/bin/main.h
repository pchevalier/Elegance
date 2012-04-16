#ifndef ELEGANCE_MAIN_H
# define ELEGANCE_MAIN_H

# ifdef $DEBUG
#  define ELEGANCE_LOG(msg, ...)					\
  printf("\033[34m<\033[1;31mELEGANCE\033[34m:\033[33m%s"		\
	 "\033[34m:\033[33m%d\033[34m>\033[00m " msg"\n",		\
	  ## __VA_ARGS__)
# else
#  define ELEGANCE_LOG(msg, ...)
# endif /* !DEBUG */

# include <Elementary.h>
# include "config.h"
# include "palette.h"
# include "serialize.h"
# include "design.h"
# include "toolbar.h"
# include "snippets.h"
# include "status.h"
# include "panel.h"
# include "tree.h"
# include "view.h"
# include "dnd.h"

#endif
