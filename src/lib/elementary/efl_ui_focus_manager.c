#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>

#include "elm_priv.h"
#include "efl_ui_focus_manager_private.h"

#define MY_CLASS EFL_UI_FOCUS_MANAGER_CLASS

#define EFL_UI_FOCUS_MANAGER_DATA_GET(o, sd)                             \
  Efl_Ui_Focus_Manager_Data *sd = efl_data_scope_get(o, MY_CLASS)


static Efl_Ui_Focus_Manager *manager;

static Efl_Ui_Focusable *
_efl_ui_focus_manager_focus_root_get(Efl_Ui_Focusable *obj)
{
   Efl_Ui_Focusable *o, *root = obj;
   for (;;)
     {
        o = efl_ui_focusable_focus_parent_get(root);
        if (!o) break;
        root = o;
     }
   return root;
}

Eina_Bool
_efl_ui_focus_manager_focus(Efl_Ui_Focus_Manager *obj EINA_UNUSED, Efl_Ui_Focusable *focus_obj EINA_UNUSED)
{
   return EINA_TRUE;
}

void
_efl_ui_focus_manager_unfocus(Efl_Ui_Focus_Manager *obj EINA_UNUSED, Efl_Ui_Focusable *focus_obj EINA_UNUSED)
{
   return;
}

EOLIAN static Eina_Bool
_efl_ui_focus_manager_focus_move(Eo *obj EINA_UNUSED, void *_pd EINA_UNUSED, Efl_Ui_Focus_Direction dir)
{
   EFL_UI_FOCUS_MANAGER_DATA_GET(manager, pd);
   Efl_Canvas_Object *cur, *o;
   Efl_Ui_Focusable *next_focus = NULL;
   Eina_List *l;

   cur = pd->current_focus;
   if (!cur) return EINA_FALSE;

   pd->prev_dir = dir;
   next_focus = efl_ui_focusable_focus_custom_object_get(cur, dir);
   if (!next_focus)
     {
        EINA_LIST_REVERSE_FOREACH(pd->focused_list, l, o)
          {
             next_focus = efl_ui_focusable_focus_next_get(o, cur, dir);
             if (next_focus) break;
             else if ((dir == EFL_UI_FOCUS_DIRECTION_PREVIOUS) ||
                      (dir == EFL_UI_FOCUS_DIRECTION_NEXT))
               cur = o;
          }
     }

   if (!next_focus)
     {
        Eina_List *list;
        Efl_Ui_Focusable *root_focus = eina_list_data_get(pd->focused_list);
        if (dir == EFL_UI_FOCUS_DIRECTION_NEXT)
          {
             list = efl_ui_focusable_child_list_get(root_focus);
             next_focus = eina_list_data_get(list);
             eina_list_free(list);
          }
        else if (dir == EFL_UI_FOCUS_DIRECTION_PREVIOUS)
          {
             while ((list = efl_ui_focusable_child_list_get(root_focus)))
               {
                  root_focus = eina_list_last_data_get(list);
                  eina_list_free(list);
               }
             next_focus = root_focus;
          }
     }

   if (next_focus)
     efl_ui_focusable_focus_set(next_focus, EINA_TRUE);
   pd->prev_dir = EFL_UI_FOCUS_DIRECTION_LAST;
   return EINA_TRUE;
}

EOLIAN static void
_efl_ui_focus_manager_focus_clear(Eo *obj EINA_UNUSED, void *_pd EINA_UNUSED)
{
   return;
}

EOLIAN static Efl_Canvas_Object *
_efl_ui_focus_manager_current_focus_get(const Eo *obj EINA_UNUSED, void *_pd EINA_UNUSED)
{
   EFL_UI_FOCUS_MANAGER_DATA_GET(manager, pd);
   return pd->current_focus;
}

EOLIAN static Efl_Canvas_Object *
_efl_ui_focus_manager_previous_focus_get(const Eo *obj EINA_UNUSED, void *_pd EINA_UNUSED)
{
   EFL_UI_FOCUS_MANAGER_DATA_GET(manager, pd);
   return pd->prev_focus;
}

#include "efl_ui_focus_manager.eo.c"
