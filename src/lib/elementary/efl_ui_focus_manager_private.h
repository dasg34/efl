#ifndef EFL_UI_FOCUS_MANAGER_H
#define EFL_UI_FOCUS_MANAGER_H

#include <Eina.h>

typedef struct _Efl_Ui_Focus_Manager_Data Efl_Ui_Focus_Manager_Data;
struct _Efl_Ui_Focus_Manager_Data
{
   Eina_List *focused_list;
   Efl_Canvas_Object *current_focus;
   Efl_Canvas_Object *prev_focus;
   Efl_Ui_Focus_Direction prev_dir;
};

Eina_Bool _efl_ui_focus_manager_focus(Efl_Ui_Focus_Manager *obj, Efl_Ui_Focusable *focus_obj);
void _efl_ui_focus_manager_unfocus(Efl_Ui_Focus_Manager *obj, Efl_Ui_Focusable *focus_obj);

#endif //EFL_UI_FOCUS_MANAGER_H
