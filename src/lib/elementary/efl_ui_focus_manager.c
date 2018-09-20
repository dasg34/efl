#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>

#include "elm_priv.h"
#include "efl_ui_focus_manager_private.h"

#define MY_CLASS EFL_UI_FOCUS_MANAGER_CLASS

#define EFL_UI_FOCUS_MANAGER_DATA_GET(o, sd)                           \
  Efl_Ui_Focus_Manager_Data *sd = efl_data_scope_get(o, MY_CLASS);

#define EFL_UI_FOCUS_MANAGER_DATA_GET_OR_RETURN(o, sd, ...)            \
  Efl_Ui_Focus_Manager_Data *sd = efl_data_scope_get(o, MY_CLASS);     \
  if (!sd)                                                             \
    {                                                                  \
       return __VA_ARGS__;                                             \
    }


#define EFL_UI_FOCUS_MANAGER_GET_OR_RETURN(o, ptr, ...)                \
  Efl_Ui_Focus_Manager *ptr = _efl_ui_focus_manager_get(o);            \
  if (!ptr)                                                            \
    {                                                                  \
       return __VA_ARGS__;                                             \
    }

static Efl_Ui_Focus_Manager *active_manager = NULL;

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

static inline Efl_Ui_Focus_Manager *
_efl_ui_focus_manager_get(Efl_Ui_Focusable *obj)
{
   return _efl_ui_win_focus_manager_get(_efl_ui_focus_manager_focus_root_get(obj));
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
   if (manager != active_manager)
     return;

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
             o = eina_list_data_get(pd->focused_list);
             if (efl_ui_focusable_is(o))
               efl_ui_focusable_focus_set(o, EINA_TRUE);
             break;
          }
        efl_ui_focusable_focus_set(o, EINA_TRUE);
     }
}

void
_efl_ui_focus_manager_active_manager_set(Efl_Ui_Focus_Manager *manager)
{
   if (!manager) return;
   EINA_SAFETY_ON_FALSE_RETURN(efl_isa(manager, EFL_UI_FOCUS_MANAGER_CLASS));
   active_manager = manager;
}

void
_efl_ui_focus_manager_last_focus_restore(Efl_Ui_Focusable *obj)
{
   EFL_UI_FOCUS_MANAGER_GET_OR_RETURN(obj, manager);
   EFL_UI_FOCUS_MANAGER_DATA_GET(manager, pd);

   Efl_Canvas_Object *last_focus;

   if (pd->current_focus)
     return;

   last_focus = pd->last_focus;
   pd->last_focus = NULL;

   if (last_focus)
     {
        if (efl_isa(last_focus, EFL_UI_FOCUSABLE_INTERFACE))
          efl_ui_focusable_focus_set(last_focus, EINA_TRUE);
        else
          evas_object_focus_set(last_focus, EINA_TRUE);
     }

   if (!pd->current_focus)
     efl_ui_focusable_focus_set(obj, EINA_TRUE);
}

Eina_Bool
_efl_ui_focus_manager_focus(Efl_Ui_Focusable *obj)
{
   EINA_SAFETY_ON_FALSE_RETURN_VAL(efl_isa(obj, EFL_UI_FOCUSABLE_INTERFACE), EINA_FALSE);
   EFL_UI_FOCUS_MANAGER_GET_OR_RETURN(obj, manager, EINA_FALSE);
   EFL_UI_FOCUS_MANAGER_DATA_GET(manager, pd);

   Eina_List *l, *l_next;
   Efl_Ui_Focusable *focus_parent;

   if (!active_manager)
     {
        pd->last_focus = obj;
        return EINA_FALSE;
     }

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
   EFL_UI_FOCUS_MANAGER_GET_OR_RETURN(obj, manager, EINA_FALSE);
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
_efl_ui_focus_manager_focus_move(Eo *obj EINA_UNUSED, void *_pd EINA_UNUSED, Efl_Ui_Focus_Direction dir)
{
   EFL_UI_FOCUS_MANAGER_DATA_GET_OR_RETURN(active_manager, pd, EINA_FALSE);
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
   EFL_UI_FOCUS_MANAGER_DATA_GET_OR_RETURN(active_manager, pd);
   pd->current_focus = NULL;
   efl_ui_focusable_focus_set(eina_list_data_get(pd->focused_list), EINA_FALSE);
}

EOLIAN static Efl_Canvas_Object *
_efl_ui_focus_manager_current_focus_get(const Eo *obj EINA_UNUSED, void *_pd EINA_UNUSED)
{
   EFL_UI_FOCUS_MANAGER_DATA_GET_OR_RETURN(active_manager, pd, NULL);
   return pd->current_focus;
}

EOLIAN static Efl_Canvas_Object *
_efl_ui_focus_manager_previous_focus_get(const Eo *obj EINA_UNUSED, void *_pd EINA_UNUSED)
{
   EFL_UI_FOCUS_MANAGER_DATA_GET_OR_RETURN(active_manager, pd, NULL);
   return pd->prev_focus;
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
