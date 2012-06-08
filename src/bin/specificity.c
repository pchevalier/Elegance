#include "main.h"

//--// callbacks
static void
_loop_video_cb(void *data __UNUSED__,
	       Evas_Object *obj,
	       void *event_info __UNUSED__)
{
  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  emotion_object_position_set(obj, 0);
  emotion_object_play_set(obj, EINA_TRUE);
}


//--// public routines
// set specific options for bubble
void
apply_bubble_specificities(Elegance_Content *content)
{
  Evas_Object *icon1 = elm_icon_add(design_win);
  Evas_Object *icon2 = elm_icon_add(design_win);
  Elegance_Property prop[] = {
    { "label", "**LABEL**" },
    { "info", "**INFO**" },
    { "icon 1", PACKAGE_DATA_DIR"/tools/icon1.png" },
    { "icon 2", PACKAGE_DATA_DIR"/tools/icon1.png" },
    { NULL, NULL }
  };
  int j = 0;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  for (j = 0; prop[j].name != NULL; j++)
    if (!eina_hash_find(content->prop, prop[j].name))
      eina_hash_direct_add(content->prop, prop[j].name,
			   strdup(prop[j].data));

  elm_object_part_text_set(content->obj, NULL, eina_hash_find(content->prop,
							      "label"));
  elm_object_part_text_set(content->obj, "info", eina_hash_find(content->prop,
								"info"));

  elm_image_file_set(icon1, eina_hash_find(content->prop,
					  "icon 1"), NULL);
  elm_object_part_content_set(content->obj, NULL, icon1);

  elm_image_file_set(icon2, eina_hash_find(content->prop,
					  "icon 2"), NULL);
  elm_object_part_content_set(content->obj, "icon", icon2);
}

// set specific options for icon
void
apply_icon_specificities(Elegance_Content *content)
{
  Elegance_Property prop[] = {
    { "icon", PACKAGE_DATA_DIR"/tools/icon1.png" },
    { NULL, NULL }
  };
  int j = 0;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  for (j = 0; prop[j].name != NULL; j++)
    if (!eina_hash_find(content->prop, prop[j].name))
      eina_hash_add(content->prop, prop[j].name,
		    strdup(prop[j].data));

  elm_image_file_set(content->obj, eina_hash_find(content->prop,
						  "icon"), NULL);
}

// set specific options for video
void
apply_video_specificities(Elegance_Content *content)
{
  Elegance_Property prop[] = {
    { "file", PACKAGE_DATA_DIR"/tools/movie.avi" },
    { NULL, NULL }
  };
  int j = 0;
  Evas_Object *emotion_obj = elm_video_emotion_get(content->obj);

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  for (j = 0; prop[j].name != NULL; j++)
    if (!eina_hash_find(content->prop, prop[j].name))
      eina_hash_add(content->prop, prop[j].name,
		    strdup(prop[j].data));

  elm_video_file_set(content->obj, eina_hash_find(content->prop,
						  "file"));
  elm_video_play(content->obj);
  evas_object_smart_callback_add(emotion_obj,
				 "playback_finished",
				 _loop_video_cb, content);
}

// set specific options for button
void
apply_button_specificities(Elegance_Content *content)
{
  Elegance_Property prop[] = {
    { "label", "**LABEL**" },
    { "icon", PACKAGE_DATA_DIR"/tools/icon1.png" },
    { NULL, NULL }
  };
  int j = 0;
  Evas_Object *icon = elm_icon_add(design_win);

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  for (j = 0; prop[j].name != NULL; j++)
    if (!eina_hash_find(content->prop, prop[j].name))
      eina_hash_add(content->prop, prop[j].name,
		    strdup(prop[j].data));

  elm_object_part_text_set(content->obj, NULL, eina_hash_find(content->prop,
							      "label"));

  elm_image_file_set(icon, eina_hash_find(content->prop,
					  "icon"), NULL);
  evas_object_size_hint_max_set(icon, 50, 50);
  elm_object_part_content_set(content->obj, "icon", icon);
}

// set specific options for entry
void
apply_entry_specificities(Elegance_Content *content)
{
  Elegance_Property prop[] = {
    { "label", "**LABEL**" },
    { NULL, NULL }
  };
  int j = 0;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  for (j = 0; prop[j].name != NULL; j++)
    if (!eina_hash_find(content->prop, prop[j].name))
      eina_hash_add(content->prop, prop[j].name,
		    strdup(prop[j].data));

  elm_object_part_text_set(content->obj, NULL, eina_hash_find(content->prop,
							      "label"));
}

// set specific options for slider
void
apply_slider_specificities(Elegance_Content *content)
{
  Elegance_Property prop[] = {
    { "label", "**LABEL**" },
    { NULL, NULL }
  };
  int j = 0;

  ELEGANCE_LOG(EINA_LOG_LEVEL_DBG,
	       "begin");

  for (j = 0; prop[j].name != NULL; j++)
    if (!eina_hash_find(content->prop, prop[j].name))
      eina_hash_add(content->prop, prop[j].name,
		    strdup(prop[j].data));

  elm_object_part_text_set(content->obj, NULL, eina_hash_find(content->prop,
							      "label"));
}
