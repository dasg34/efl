#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_BETA
#include <Elementary.h>
#include "elm_suite.h"

#define TEST_FOCUS_NEXT(type, cmp)                                 \
   elm_object_focus_next(win, ELM_FOCUS_##type);                    \
   ck_assert_ptr_eq(elm_object_focused_object_get(win), cmp)

#define TEST_FOCUS_SET(obj, focus)                                  \
   elm_object_focus_set(obj, focus);                        \
   ck_assert_ptr_eq(elm_object_focused_object_get(win), obj);


static Evas_Object *win, *bx_main, *bx1, *bx2, *bx3;
static Evas_Object *btn, *btn1[2],*btn2[3], *btn3[4];

static Evas_Object *
test_button_create(Evas_Object *box, const char *text)
{
   Evas_Object *btn;

   btn = elm_button_add(box);
   elm_object_text_set(btn, text);
   evas_object_size_hint_weight_set(btn, 0.0, 0.0);
   evas_object_size_hint_align_set(btn, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(box, btn);
   evas_object_show(btn);

   return btn;
}

static Evas_Object *
test_box_create(Evas_Object *parent)
{
   Evas_Object *bx;

   bx = elm_box_add(parent);
   evas_object_size_hint_weight_set(bx, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   evas_object_size_hint_align_set(bx, EVAS_HINT_FILL, EVAS_HINT_FILL);
   elm_box_pack_end(parent, bx);
   evas_object_show(bx);

   return bx;
}

static void
focus_custom_view_create()
{
   Eina_List *custom_chain = NULL;

   win = win_add(NULL, "focus_custom", ELM_WIN_BASIC);
   ecore_evas_focus_device_set(ecore_evas_ecore_evas_get(evas_object_evas_get(win)), NULL, 0);

   bx_main = elm_box_add(win);
   evas_object_size_hint_weight_set(bx_main, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
   elm_win_resize_object_add(win, bx_main);
   evas_object_show(bx_main);

   btn = test_button_create(bx_main, "btn");

   bx1 = test_box_create(bx_main);
   bx2 = test_box_create(bx_main);
   bx3 = test_box_create(bx_main);

   btn1[0] = test_button_create(bx1, "btn1_1");
   btn1[1] = test_button_create(bx1, "btn1_2");

   btn2[0] = test_button_create(bx2, "btn2_1");
   btn2[1] = test_button_create(bx2, "btn2_2");
   btn2[2] = test_button_create(bx2, "btn2_3");

   btn3[0] = test_button_create(bx3, "btn3_1");
   btn3[1] = test_button_create(bx3, "btn3_2");
   btn3[2] = test_button_create(bx3, "btn3_3");
   btn3[3] = test_button_create(bx3, "btn3_4");

   custom_chain = eina_list_append(custom_chain, btn3[1]);
   custom_chain = eina_list_append(custom_chain, btn3[3]);
   custom_chain = eina_list_append(custom_chain, btn3[0]);
   elm_object_focus_custom_chain_set(bx3, custom_chain);
   custom_chain = NULL;

   custom_chain = eina_list_append(custom_chain, bx2);
   custom_chain = eina_list_append(custom_chain, btn);
   custom_chain = eina_list_append(custom_chain, bx3);
   elm_object_focus_custom_chain_set(bx_main, custom_chain);
   custom_chain = NULL;

   elm_object_focus_next_object_set(btn2[1], btn1[0], ELM_FOCUS_UP);
   elm_object_focus_next_object_set(btn3[3], btn3[2], ELM_FOCUS_UP);

   evas_object_resize(win, 300, 400);
   evas_object_show(win);
}

EFL_START_TEST (elm_focus_custom_focus)
{
   focus_custom_view_create();
   ecore_evas_focus_device_set(ecore_evas_ecore_evas_get(evas_object_evas_get(win)), NULL, 1);

   TEST_FOCUS_SET(btn2[0], EINA_TRUE);
   TEST_FOCUS_NEXT(NEXT, btn2[1]);
   TEST_FOCUS_NEXT(NEXT, btn2[2]);
   TEST_FOCUS_NEXT(NEXT, btn);
   TEST_FOCUS_NEXT(NEXT, btn3[1]);
   TEST_FOCUS_NEXT(NEXT, btn3[3]);
   TEST_FOCUS_NEXT(NEXT, btn3[0]);
   TEST_FOCUS_NEXT(NEXT, btn2[0]);

   TEST_FOCUS_SET(btn3[0], EINA_TRUE);
   TEST_FOCUS_NEXT(PREVIOUS, btn3[3]);
   TEST_FOCUS_NEXT(PREVIOUS, btn3[1]);
   TEST_FOCUS_NEXT(PREVIOUS, btn);
   TEST_FOCUS_NEXT(PREVIOUS, btn2[2]);
   TEST_FOCUS_NEXT(PREVIOUS, btn2[1]);
   TEST_FOCUS_NEXT(PREVIOUS, btn2[0]);
   TEST_FOCUS_NEXT(PREVIOUS, btn3[0]);

   TEST_FOCUS_SET(btn, EINA_TRUE);
   TEST_FOCUS_NEXT(DOWN, btn2[0]);
   TEST_FOCUS_NEXT(DOWN, btn2[1]);
   TEST_FOCUS_NEXT(DOWN, btn2[2]);
   TEST_FOCUS_NEXT(DOWN, btn3[0]);
   TEST_FOCUS_NEXT(DOWN, btn3[1]);
   TEST_FOCUS_NEXT(DOWN, btn3[3]);
   TEST_FOCUS_NEXT(DOWN, btn3[3]);

   TEST_FOCUS_SET(btn3[3], EINA_TRUE);
   TEST_FOCUS_NEXT(UP, btn3[2]);
   TEST_FOCUS_NEXT(UP, btn3[1]);
   TEST_FOCUS_NEXT(UP, btn3[0]);
   TEST_FOCUS_NEXT(UP, btn2[2]);
   TEST_FOCUS_NEXT(UP, btn2[1]);
   TEST_FOCUS_NEXT(UP, btn1[0]);
   TEST_FOCUS_NEXT(UP, btn);
   TEST_FOCUS_NEXT(UP, btn);
}
EFL_END_TEST

void elm_test_elm_focus(TCase *tc)
{
   tcase_add_test(tc, elm_focus_custom_focus);
}
