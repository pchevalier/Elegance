#ifndef ELEGANCE_MAIN_H
# define ELEGANCE_MAIN_H

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

#define ELEGANCE_LOG_LEVEL -42

#define ELEGANCE_LOG(fmt, ...)			\
  EINA_LOG(EINA_LOG_DOMAIN_DEFAULT,		\
	   ELEGANCE_LOG_LEVEL,			\
	   fmt,					\
	   ## __VA_ARGS__)

#endif
