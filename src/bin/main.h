#ifndef ELEGANCE_MAIN_H
# define ELEGANCE_MAIN_H

# include <Elementary.h>
# include <Emotion.h>
# include "config.h"
# include "palette.h"
# include "serialize.h"
# include "design.h"
# include "toolbar.h"
# include "snippets.h"
# include "status.h"
# include "panel.h"
# include "tree.h"
# include "specificity.h"
# include "view.h"
# include "dnd.h"

extern int elegance_log_dom;

# ifdef DEBUG
#  define ELEGANCE_LOG(level, msg, ...)		\
  EINA_LOG(elegance_log_dom,			\
           level,				\
           msg,					\
           ## __VA_ARGS__)
# else
#  define ELEGANCE_LOG(msg, ...)
# endif /* !DEBUG */

#endif
