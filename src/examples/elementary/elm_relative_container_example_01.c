// gcc -o elm_relative_container_example_01 elm_relative_container_example_01.c `pkg-config --cflags --libs elementary`

#ifdef HAVE_CONFIG_H
#include "elementary_config.h"
#endif

#include <Elementary.h>

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   Eo *win, *layout;
   Eo *title, *title_left, *title_right, *bottom, *body_left, *body_right;
   Eo *side_left1, *side_left2, *side_right1, *side_right2;

   win = elm_win_util_standard_add("elm-relative-container", "Elm.RelativeContainer");
   elm_win_autodel_set(win, EINA_TRUE);
   elm_policy_set(ELM_POLICY_QUIT, ELM_POLICY_QUIT_LAST_WINDOW_CLOSED);
   evas_object_resize(win, 500, 300);
   evas_object_show(win);

   layout = elm_relative_container_add(win);
   evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
//   evas_object_size_hint_align_set(win, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(layout);
   elm_win_resize_object_add(win, layout);

   title_left = elm_button_add(layout);
   title_right = elm_button_add(layout);
   title = elm_button_add(layout);
   bottom = elm_button_add(layout);
   body_left = elm_button_add(layout);
   body_right = elm_button_add(layout);
   side_left1 = elm_button_add(layout);
   side_left2 = elm_button_add(layout);
   side_right1 = elm_button_add(layout);
   side_right2 = elm_button_add(layout);

   //title
   elm_object_text_set(title_left, "title left");
   evas_object_size_hint_min_set(title_left, 100, 50);
   evas_object_size_hint_align_set(title_left, 0, 0);
   evas_object_show(title_left);
   elm_relative_container_relation_right_set(layout, title_left, layout, 0.0);
   elm_relative_container_relation_bottom_set(layout, title_left, layout, 0.0);

   elm_object_text_set(title_right, "title right");
   evas_object_size_hint_min_set(title_right, 100, 50);
   evas_object_size_hint_align_set(title_right, 1.0, 0.0);
   evas_object_show(title_right);
   elm_relative_container_relation_left_set(layout, title_right, layout, 1.0);
   elm_relative_container_relation_bottom_set(layout, title_right, layout, 0.0);

   elm_object_text_set(title, "title");
   evas_object_size_hint_min_set(title, 100, 50);
   evas_object_size_hint_max_set(title, 200, 50);
   evas_object_size_hint_align_set(title, EVAS_HINT_FILL, 0.0);
   evas_object_show(title);
   elm_relative_container_relation_left_set(layout, title, title_left, 1.0);
   elm_relative_container_relation_right_set(layout, title, title_right, 0.0);
   elm_relative_container_relation_bottom_set(layout, title, layout, 0.0);


   //body
   elm_object_text_set(body_left, "body left");
   evas_object_size_hint_min_set(body_left, 50, 50);
   evas_object_size_hint_weight_set(body_left, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(body_left, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(body_left);
   elm_relative_container_relation_left_set(layout, body_left, title_left, 1.0);
   elm_relative_container_relation_right_set(layout, body_left, body_right, 0.0);
   elm_relative_container_relation_top_set(layout, body_left, title, 1.0);
   elm_relative_container_relation_bottom_set(layout, body_left, bottom, 0.0);

   elm_object_text_set(body_right, "body right");
   evas_object_size_hint_min_set(body_right, 50, 50);
   evas_object_size_hint_weight_set(body_right, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(body_right, EVAS_HINT_FILL, EVAS_HINT_FILL);
   evas_object_show(body_right);
   elm_relative_container_relation_left_set(layout, body_right, body_left, 1.0);
   elm_relative_container_relation_right_set(layout, body_right, title_right, 0.0);
   elm_relative_container_relation_top_set(layout, body_right, title, 1.0);
   elm_relative_container_relation_bottom_set(layout, body_right, bottom, 0.0);

   elm_object_text_set(side_left1, "left,top");
   evas_object_size_hint_min_set(side_left1, 100, 50);
   evas_object_show(side_left1);
   elm_relative_container_relation_left_set(layout, side_left1, layout, 0.0);
   elm_relative_container_relation_right_set(layout, side_left1, body_left, 0.0);
   elm_relative_container_relation_top_set(layout, side_left1, title_left, 1.0);
   elm_relative_container_relation_bottom_set(layout, side_left1, side_left2, 0.0);

   elm_object_text_set(side_left2, "left,bottom");
   evas_object_size_hint_min_set(side_left2, 100, 50);
   evas_object_show(side_left2);
   elm_relative_container_relation_left_set(layout, side_left2, layout, 0.0);
   elm_relative_container_relation_right_set(layout, side_left2, body_left, 0.0);
   elm_relative_container_relation_top_set(layout, side_left2, side_left1, 1.0);
   elm_relative_container_relation_bottom_set(layout, side_left2, bottom, 0.0);

   elm_object_text_set(side_right1, "right,top");
   evas_object_size_hint_min_set(side_right1, 100, 50);
   evas_object_show(side_right1);
   elm_relative_container_relation_left_set(layout, side_right1, body_right, 1.0);
   elm_relative_container_relation_right_set(layout, side_right1, layout, 1.0);
   elm_relative_container_relation_top_set(layout, side_right1, title_right, 1.0);
   elm_relative_container_relation_bottom_set(layout, side_right1, side_right2, 0.0);

   elm_object_text_set(side_right2, "right,bottom");
   evas_object_size_hint_min_set(side_right2, 100, 50);
   evas_object_show(side_right2);
   elm_relative_container_relation_left_set(layout, side_right2, body_right, 1.0);
   elm_relative_container_relation_right_set(layout, side_right2, layout, 1.0);
   elm_relative_container_relation_top_set(layout, side_right2, side_right1, 1.0);
   elm_relative_container_relation_bottom_set(layout, side_right2, bottom, 0.0);

   //bottom
   elm_object_text_set(bottom, "bottom");
   evas_object_size_hint_min_set(bottom, 100, 50);
   evas_object_size_hint_align_set(bottom, EVAS_HINT_FILL, 1.0);
   evas_object_show(bottom);
   elm_relative_container_relation_left_set(layout, bottom, layout, 0.0);
   elm_relative_container_relation_right_set(layout, bottom, layout, 1.0);
   elm_relative_container_relation_top_set(layout, bottom, layout, 1.0);

   elm_run();
   return 0;
}
ELM_MAIN()
