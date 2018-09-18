#ifndef EFL_UI_FOCUS_MANAGER_H
#define EFL_UI_FOCUS_MANAGER_H

#include <Eina.h>

typedef struct _Efl_Ui_Focus_Manager_Data Efl_Ui_Focus_Manager_Data;
struct _Efl_Ui_Focus_Manager_Data
{
   Eina_List *focused_list;
   Eina_Hash *focus_stack;
   Efl_Canvas_Object *current_focus;
};

void _efl_ui_focus_manager_handle(Efl_Ui_Focusable *obj);
Eina_Bool _efl_ui_focus_manager_focus(Efl_Ui_Focusable *obj);
Eina_Bool _efl_ui_focus_manager_unfocus(Efl_Ui_Focusable *obj);

#endif //EFL_UI_FOCUS_MANAGER_H
