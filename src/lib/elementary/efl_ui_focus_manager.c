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

static Efl_Ui_Focusable *
_parent_focus(Efl_Ui_Focusable *obj)
{
   Efl_Ui_Focusable *parent = efl_ui_focusable_focus_parent_get(obj);
   if (!parent)
     return obj;

   if (efl_ui_widget_focus_allow_get(parent))
     {
        if (!efl_ui_focusable_focus_get(parent))
          efl_ui_focusable_focus_set(parent, EINA_TRUE);
        return parent;
     }
   return _parent_focus(parent);
}

static Eina_Bool
_parent_check(Efl_Ui_Focusable *obj)
{
   Efl_Ui_Focusable *parent = efl_ui_focusable_focus_parent_get(obj);

   while (parent)
     {
        if (!efl_ui_focusable_is(parent))
          return EINA_FALSE;
        parent = efl_ui_focusable_focus_parent_get(parent);
     }
   return EINA_TRUE;
}

void
_efl_ui_focus_manager_handle(Efl_Ui_Focusable *obj)
{
   EFL_UI_FOCUS_MANAGER_GET_OR_RETURN(obj, manager);
   EFL_UI_FOCUS_MANAGER_DATA_GET(manager, pd);

   Efl_Ui_Focusable *o = obj;
   efl_ui_focusable_focus_set(obj, EINA_FALSE);

   if (pd->current_focus && !_parent_check(pd->current_focus))
     {
        o = pd->current_focus;
        pd->current_focus = NULL;
     }

   while (!pd->current_focus)
     {
        o = eina_hash_set(pd->focus_stack, &o, NULL);
        if (!o)
          {
             o = _efl_ui_focus_manager_focus_root_get(obj);
             if (efl_ui_focusable_is(o))
               efl_ui_focusable_focus_set(o, EINA_TRUE);
             break;
          }
        efl_ui_focusable_focus_set(o, EINA_TRUE);
     }
}

Eina_Bool
_efl_ui_focus_manager_focus(Efl_Ui_Focusable *obj)
{
   EINA_SAFETY_ON_FALSE_RETURN_VAL(efl_isa(obj, EFL_UI_FOCUSABLE_INTERFACE), EINA_FALSE);
   EFL_UI_FOCUS_MANAGER_DATA_GET(manager, pd);

   Eina_List *l, *l_next;
   Efl_Ui_Focusable *focus_parent;

   if (!efl_ui_focusable_is(obj) || !_parent_check(obj))
     return EINA_FALSE;

   focus_parent = _parent_focus(obj);

   if (pd->current_focus)
     eina_hash_set(pd->focus_stack, &obj, pd->current_focus);

   if (efl_ui_focusable_focus_get(obj))
     {
        l = eina_list_data_find_list(pd->focused_list, obj);
        l_next = eina_list_next(l);
     }
   else
     {
        l = eina_list_data_find_list(pd->focused_list, focus_parent);
        l_next = eina_list_next(l);
        pd->focused_list = eina_list_append_relative_list(pd->focused_list,
                                                          obj, l);
     }
   pd->last_focus = pd->current_focus = obj;

   if (l_next)
     {
        Efl_Canvas_Object *o = eina_list_data_get(l_next);

        if (efl_isa(o, EFL_UI_FOCUSABLE_INTERFACE))
          efl_ui_focusable_focus_set(o, EINA_FALSE);
        else
          evas_object_focus_set(o, EINA_FALSE);
     }

   return EINA_TRUE;
}

Eina_Bool
_efl_ui_focus_manager_unfocus(Efl_Ui_Focusable *obj)
{
   EINA_SAFETY_ON_FALSE_RETURN_VAL(efl_isa(obj, EFL_UI_FOCUSABLE_INTERFACE), EINA_FALSE);
   EFL_UI_FOCUS_MANAGER_DATA_GET(manager, pd);

   Eina_List *l;

   l = eina_list_data_find_list(pd->focused_list, obj);
   if (!l)
     return EINA_FALSE;

   if (pd->current_focus == eina_list_last_data_get(pd->focused_list))
     {
        pd->current_focus = NULL;
        pd->focused_list = eina_list_remove(pd->focused_list, obj);
        _efl_ui_focus_manager_handle(obj);
        return EINA_TRUE;
     }

   for (;;)
     {
        Efl_Canvas_Object *o = eina_list_last_data_get(pd->focused_list);
        if (o == obj) break;
        efl_ui_focusable_focus_set(o, EINA_FALSE);
     }

   pd->focused_list = eina_list_remove_list(pd->focused_list, l);

   return EINA_TRUE;
}

EOLIAN static Eina_Bool
_efl_ui_focus_manager_focus_move(Eo *obj EINA_UNUSED, void *_pd EINA_UNUSED, Efl_Ui_Focusable *root, Efl_Ui_Focus_Direction dir)
{
   EFL_UI_FOCUS_MANAGER_DATA_GET(manager, pd);
   Efl_Canvas_Object *cur;
   Efl_Ui_Focusable *next_focus = NULL;

   cur = pd->current_focus;
   if (!cur) return EINA_FALSE;

   next_focus = efl_ui_focusable_focus_custom_object_get(cur, dir);
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
   EFL_UI_FOCUS_MANAGER_DATA_GET(manager, pd);
   pd->current_focus = NULL;
   efl_ui_focusable_focus_set(eina_list_data_get(pd->focused_list), EINA_FALSE);
}

EOLIAN static Efl_Canvas_Object *
_efl_ui_focus_manager_current_focus_get(const Eo *obj EINA_UNUSED, void *_pd EINA_UNUSED)
{
   EFL_UI_FOCUS_MANAGER_DATA_GET(manager, pd);
   return pd->current_focus;
}

EOLIAN static Eo *
_efl_ui_focus_manager_efl_object_constructor(Eo *obj, Efl_Ui_Focus_Manager_Data *pd)
{
   obj = efl_constructor(efl_super(obj, MY_CLASS));
   pd->focus_stack = eina_hash_pointer_new(NULL);
   return obj;
}

EOLIAN static void
_efl_ui_focus_manager_efl_object_destructor(Eo *obj, Efl_Ui_Focus_Manager_Data *pd)
{
   eina_hash_free(pd->focus_stack);
   eina_list_free(pd->focused_list);
   efl_destructor(efl_super(obj, MY_CLASS));
}

#include "efl_ui_focus_manager.eo.c"
