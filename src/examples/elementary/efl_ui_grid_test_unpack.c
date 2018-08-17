// gcc -o efl_ui_grid_test_unpack efl_ui_grid_test_unpack.c `pkg-config --cflags --libs elementary`

#ifdef HAVE_CONFIG_H
#include "elementary_config.h"
#else
#define EFL_BETA_API_SUPPORT 1
#define EFL_EO_API_SUPPORT 1
#endif

#include <Elementary.h>
#include <Efl.h>
#include <stdio.h>

#define IMAX 100;

Eo *
EoGenerate(const Efl_Class *klass, Eo *parent, Efl_Ui_Dir dir)
{
   Eo* obj = efl_add(klass, parent);
   if (dir != EFL_UI_DIR_DEFAULT) efl_ui_direction_set(obj, dir);
   efl_gfx_size_hint_weight_set(obj, EFL_GFX_SIZE_HINT_EXPAND, EFL_GFX_SIZE_HINT_EXPAND);
   efl_gfx_size_hint_align_set(obj, EFL_GFX_SIZE_HINT_FILL, EFL_GFX_SIZE_HINT_FILL);
   return obj;
}

static void
_pack_btn_clicked(void *data, const Efl_Event *ev)
{
   Eo *grid = data;
   int r = 0, g = 0, b = 0, count;

   Eo *gitem = efl_add(EFL_UI_GRID_DEFAULT_ITEM_CLASS, grid);
   Eo *rect = evas_object_rectangle_add(evas_object_evas_get(grid));
   count = efl_content_count(grid);

   switch (count % 5)
        {
         case 0:
            r = 255;
            break;
         case 1:
            g = 255;
            break;
         case 2:
            b = 255;
            break;
         case 3:
            r = g = b = 255;
            break;
         case 4:
            r = g = b = 0;
            break;
      }
      efl_gfx_color_set(rect, r, g, b, 255);
      efl_content_set(gitem, rect);
      efl_text_set(gitem, "new item");
      // TEST#4 : Pack end
      efl_pack_end(grid, gitem);
}

static void
_unpack_btn_clicked(void *data, const Efl_Event *ev)
{
   Eo *grid = data;
   Eo *item = efl_pack_content_get(grid, 0);
   efl_pack_unpack(grid, item);
   efl_del(item);
}

static void
_upkall_btn_clicked(void *data, const Efl_Event *ev)
{
   Eo *grid = data;
   efl_pack_unpack_all(grid);
}

static void
_clear_btn_clicked(void *data, const Efl_Event *ev)
{
   Eo *grid = data;
   efl_pack_clear(grid);
}

EAPI_MAIN int
elm_main(int argc, char **argv)
{
   int itemmax = IMAX;
   int i = 0;
   if (argv[1]) itemmax = atoi(argv[1]);
   Eo *win, *box, *bbx;
   Eo *pbtn, *upbtn, *allbtn, *clrbtn;
   Eo *grid, *gitem;

   win =  efl_add(EFL_UI_WIN_CLASS, efl_main_loop_get(),
      efl_ui_win_type_set(efl_added, EFL_UI_WIN_BASIC),
      efl_text_set(efl_added, "Efl.Ui.Grid Unpack"),
      efl_ui_win_autodel_set(efl_added, EINA_TRUE));

   box = EoGenerate(EFL_UI_BOX_CLASS, win, EFL_UI_DIR_VERTICAL);
   elm_win_resize_object_add(win, box);

   // TEST#1 : Create Grid
   grid = EoGenerate(EFL_UI_GRID_CLASS, box, EFL_UI_DIR_VERTICAL);
   // TEST#2 : Set Item Default Size
   efl_ui_grid_item_size_set(grid, EINA_SIZE2D(100, 100)); // 4X4
   efl_pack_end(box, grid);

   for (i = 0; i < itemmax; i++)
   {
      int r = 0, g = 0, b = 0;
      // TEST#3 : Create Grid Item
      gitem = efl_add(EFL_UI_GRID_DEFAULT_ITEM_CLASS, grid);
      Eo *rect = evas_object_rectangle_add(evas_object_evas_get(win));
      switch (i % 5)
        {
         case 0:
            r = 255;
            break;
         case 1:
            g = 255;
            break;
         case 2:
            b = 255;
            break;
         case 3:
            r = g = b = 255;
            break;
         case 4:
            r = g = b = 0;
            break;
      }
      efl_gfx_color_set(rect, r, g, b, 255);
      efl_content_set(gitem, rect);
      // TEST#4 : Pack end
      efl_pack_end(grid, gitem);
   }

   bbx = EoGenerate(EFL_UI_BOX_CLASS, box, EFL_UI_DIR_HORIZONTAL);
   efl_gfx_size_hint_weight_set(bbx, EFL_GFX_SIZE_HINT_EXPAND, 0.05);
   efl_pack_end(box, bbx);

   pbtn = EoGenerate(EFL_UI_BUTTON_CLASS, bbx, EFL_UI_DIR_DEFAULT);
   efl_text_set(pbtn, "Pack Item");
   efl_gfx_size_hint_min_set(pbtn, EINA_SIZE2D(100, 25));
   // TEST#4 : Unpack Item
   efl_event_callback_add(pbtn, EFL_UI_EVENT_CLICKED, _pack_btn_clicked, grid);
   efl_pack_end(bbx, pbtn);

   upbtn = EoGenerate(EFL_UI_BUTTON_CLASS, bbx, EFL_UI_DIR_DEFAULT);
   efl_text_set(upbtn, "Unpack Item");
   efl_gfx_size_hint_min_set(upbtn, EINA_SIZE2D(100, 25));
   // TEST#4 : Unpack Item
   efl_event_callback_add(upbtn, EFL_UI_EVENT_CLICKED, _unpack_btn_clicked, grid);
   efl_pack_end(bbx, upbtn);

   allbtn = EoGenerate(EFL_UI_BUTTON_CLASS, bbx, EFL_UI_DIR_DEFAULT);
   efl_text_set(allbtn, "Unpack All");
   efl_gfx_size_hint_min_set(allbtn, EINA_SIZE2D(100, 25));
   // TEST#4 : Unpack All
   efl_event_callback_add(allbtn, EFL_UI_EVENT_CLICKED, _upkall_btn_clicked, grid);
   efl_pack_end(bbx, allbtn);

   clrbtn = EoGenerate(EFL_UI_BUTTON_CLASS, bbx, EFL_UI_DIR_DEFAULT);
   efl_text_set(clrbtn, "Clear");
   efl_gfx_size_hint_min_set(clrbtn, EINA_SIZE2D(100, 25));
   // TEST#4 : Unpack All
   efl_event_callback_add(clrbtn, EFL_UI_EVENT_CLICKED, _clear_btn_clicked, grid);
   efl_pack_end(bbx, clrbtn);

   efl_gfx_entity_size_set(win, EINA_SIZE2D(417, 600));

   elm_run();
   return 0;
}
ELM_MAIN()