#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>
#include "elm_priv.h"

EAPI void
elm_object_focus_next_object_set(Evas_Object *obj, Evas_Object *next, Elm_Focus_Direction dir)
{
   efl_ui_focusable_focus_custom_object_set(obj, dir, next);
}

EAPI Evas_Object *
elm_object_focus_next_object_get(const Evas_Object *obj, Elm_Focus_Direction dir)
{
   return efl_ui_focusable_focus_custom_object_get(obj, dir);
}

EAPI void
elm_object_focus_custom_chain_set(Evas_Object *obj, Eina_List *objs)
{
   EINA_SAFETY_ON_NULL_RETURN(obj);
   elm_widget_focus_custom_chain_set(obj, objs);
}

EAPI void
elm_object_focus_custom_chain_unset(Evas_Object *obj)
{
   EINA_SAFETY_ON_NULL_RETURN(obj);
   elm_widget_focus_custom_chain_unset(obj);
}

EAPI const Eina_List *
elm_object_focus_custom_chain_get(const Evas_Object *obj)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(obj, NULL);
   return elm_widget_focus_custom_chain_get(obj);
}

EAPI void
elm_object_focus_custom_chain_append(Evas_Object *obj, Evas_Object *child, Evas_Object *relative_child)
{
   EINA_SAFETY_ON_NULL_RETURN(obj);
   elm_widget_focus_custom_chain_append(obj, child, relative_child);
}

EAPI void
elm_object_focus_custom_chain_prepend(Evas_Object *obj, Evas_Object *child, Evas_Object *relative_child)
{
   EINA_SAFETY_ON_NULL_RETURN(obj);
   elm_widget_focus_custom_chain_prepend(obj, child, relative_child);
}

EINA_DEPRECATED EAPI void
elm_object_focus_cycle(Evas_Object *obj, Elm_Focus_Direction dir)
{
   elm_object_focus_next(obj, dir);
}

EAPI void
elm_object_focus_next(Evas_Object *obj EINA_UNUSED, Elm_Focus_Direction dir)
{
   efl_ui_focus_manager_focus_move(EFL_UI_FOCUS_MANAGER_CLASS, dir);
}

EAPI Elm_Object_Item *
elm_object_focus_next_item_get(const Evas_Object  *obj,
                               Elm_Focus_Direction dir EINA_UNUSED)
{
   /* FOCUS-FIXME */
   return NULL;
}

EAPI void
elm_object_focus_next_item_set(Evas_Object     *obj,
                               Elm_Object_Item *next_item EINA_UNUSED,
                               Elm_Focus_Direction dir EINA_UNUSED)
{
   /* FOCUS-FIXME */
}

EAPI Evas_Object *
elm_object_focused_object_get(const Evas_Object *obj EINA_UNUSED)
{
   return efl_ui_focus_manager_current_focus_get(EFL_UI_FOCUS_MANAGER_CLASS);
}

EAPI Eina_Bool
elm_object_focus_get(const Evas_Object *obj)
{
   if (!elm_widget_is(obj))
     return evas_object_focus_get(obj);

   return efl_ui_focusable_focus_get(obj);
}

EAPI void
elm_object_focus_set(Evas_Object *obj, Eina_Bool focus)
{
   // ugly, but, special case for inlined windows
   if (efl_isa(obj, EFL_UI_WIN_CLASS))
     {
        Evas_Object *inlined = elm_win_inlined_image_object_get(obj);
        if (inlined)
          {
             evas_object_focus_set(inlined, focus);
             return;
          }
     }
   else if (elm_widget_is(obj))
     {
        efl_ui_focusable_focus_set(obj, focus);
     }
   else
     {
        evas_object_focus_set(obj, focus);
     }
}
