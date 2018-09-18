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
_efl_ui_focus_manager_focus_move(Eo *obj EINA_UNUSED, void *_pd EINA_UNUSED, Efl_Ui_Focus_Direction dir EINA_UNUSED)
{
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

EOLIAN static Efl_Canvas_Object *
_efl_ui_focus_manager_previous_focus_get(const Eo *obj EINA_UNUSED, void *_pd EINA_UNUSED)
{
   return NULL;
}

#include "efl_ui_focus_manager.eo.c"
