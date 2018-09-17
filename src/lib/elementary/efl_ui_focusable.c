#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>
#include "elm_priv.h"

typedef struct {
  Eina_Bool old_focus;
  Eina_Bool ongoing_prepare_call;
} Efl_Ui_Focusable_Data;

EOLIAN static void
_efl_ui_focusable_focus_set(Eo *obj, Efl_Ui_Focusable_Data *pd, Eina_Bool focus)
{
   if (pd->old_focus == focus) return;

   pd->old_focus = focus;
   efl_event_callback_call(obj, EFL_UI_FOCUSABLE_EVENT_FOCUS_CHANGED , (void*) (uintptr_t) focus);
}

EOLIAN static Eina_Bool
_efl_ui_focusable_focus_get(const Eo *obj EINA_UNUSED, Efl_Ui_Focusable_Data *pd)
{
   return pd->old_focus;
}

EOLIAN static void
_efl_ui_focusable_prepare_logical(Eo *obj, Efl_Ui_Focusable_Data *pd)
{
  if (pd->ongoing_prepare_call) return;

  pd->ongoing_prepare_call = EINA_TRUE;

  efl_ui_focusable_prepare_logical_none_recursive(obj);

  pd->ongoing_prepare_call = EINA_FALSE;
}



#include "efl_ui_focusable.eo.c"
