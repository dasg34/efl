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

static Efl_Ui_Focusable *
_efl_ui_focus_manager_move_next(Efl_Ui_Focusable *root, Efl_Ui_Focusable *cur, Eina_Bool next)
{
   Eina_List *list, *l, *ll;
   Efl_Ui_Focusable *next_focus = NULL, *o;

   list = efl_ui_focusable_child_list_get(root);
   if (list)
     {
        if (next)
          {
             next_focus = eina_list_data_get(list);
             EINA_LIST_FOREACH_SAFE(list, l, ll, o)
               {
                  if ((cur == o) && ll)
                    next_focus = eina_list_data_get(ll);
                  list = eina_list_remove_list(list, l);
               }
          }
        else
          {
             next_focus = eina_list_last_data_get(list);
             EINA_LIST_REVERSE_FOREACH_SAFE(list, l, ll, o)
               {
                  if ((cur == o) && ll)
                    next_focus = eina_list_data_get(ll);
                  list = eina_list_remove_list(list, l);
               }
          }
     }

   return next_focus;
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
_efl_ui_focus_manager_focus_move(Eo *obj EINA_UNUSED, void *_pd EINA_UNUSED, Efl_Ui_Focusable *root, Efl_Ui_Focus_Direction dir)
{
   EFL_UI_FOCUS_MANAGER_DATA_GET(manager, pd);
   Efl_Canvas_Object *cur;
   Efl_Ui_Focusable *next_focus = NULL;

   cur = pd->current_focus;
   if (!cur) return EINA_FALSE;

   next_focus = efl_ui_focusable_focus_custom_get(cur, dir);
   if (next_focus)
     goto end;

   switch (dir)
     {
      case EFL_UI_FOCUS_DIRECTION_UP:
      case EFL_UI_FOCUS_DIRECTION_DOWN:
      case EFL_UI_FOCUS_DIRECTION_LEFT:
      case EFL_UI_FOCUS_DIRECTION_RIGHT:
        next_focus = efl_ui_focusable_focus_next_get(root, cur, dir);
        break;
      case EFL_UI_FOCUS_DIRECTION_NEXT:
        if (!efl_isa(cur, EFL_UI_FOCUSABLE_INTERFACE))
          return EINA_FALSE;
        next_focus = _efl_ui_focus_manager_move_next(root, cur, EINA_TRUE);
        break;
      case EFL_UI_FOCUS_DIRECTION_PREVIOUS:
        if (!efl_isa(cur, EFL_UI_FOCUSABLE_INTERFACE))
          return EINA_FALSE;
        next_focus = _efl_ui_focus_manager_move_next(root, cur, EINA_FALSE);
        break;
      default:
        return EINA_FALSE;
     }

   if (!next_focus)
     return EINA_FALSE;

end:
   efl_ui_focusable_focus_set(next_focus, EINA_TRUE);
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
   return NULL;
}

#include "efl_ui_focus_manager.eo.c"
