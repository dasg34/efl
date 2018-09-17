#ifndef ELM_TEST_FOCUS_COMMON_H
#define ELM_TEST_FOCUS_COMMON_H

#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#define EFL_ACCESS_OBJECT_PROTECTED

#include <Elementary.h>
#include "elm_suite.h"
#include "elm_widget.h"
#include "focus_test.eo.h"

#define TEST_OBJ_NEW(name, x, y, w, h) \
  Efl_Ui_Focusable* name; \
  name = elm_focus_test_object_new("" #name "",x, y, w, h); \


Efl_Ui_Focusable* elm_focus_test_object_new(const char *name, int x, int y, int w, int h);

void elm_focus_test_setup_cross(Efl_Ui_Focusable **middle,
                                Efl_Ui_Focusable **south,
                                Efl_Ui_Focusable **north,
                                Efl_Ui_Focusable **east,
                                Efl_Ui_Focusable **west);

Efl_Ui_Focus_Manager *elm_focus_test_manager_new(Efl_Ui_Focusable **middle);
#endif