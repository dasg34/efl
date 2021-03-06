#ifdef HAVE_CONFIG_H
# include "elementary_config.h"
#endif

#include <Elementary.h>
#include "elm_priv.h"

#include "efl_page_transition_scroll.h"
#include "efl_page_transition.h"
#include "efl_ui_widget_pager.h"

#define MY_CLASS EFL_PAGE_TRANSITION_SCROLL_CLASS



static void
_page_info_deallocate(Efl_Page_Transition_Scroll_Data *pd)
{
   Page_Info *pi;

   EINA_LIST_FREE(pd->page_infos, pi)
     {
        if (pi->content)
          efl_pack_unpack(pi->obj, pi->content);

        efl_del(pi->obj);
        free(pi);
     }

   pd->head = NULL;
   pd->tail = NULL;
}

static void
_page_info_allocate(Efl_Page_Transition_Scroll_Data *pd,
                    Efl_Page_Transition_Data *spd)
{
   Page_Info *pi, *prev = NULL;
   int i;

   for (i = 0; i < pd->page_info_num; i++)
     {
        pi = calloc(1, sizeof(*pi));
        if (i == 0) pd->head = pi;
        else if (i == (pd->page_info_num - 1)) pd->tail = pi;
        pi->id = i;
        pi->pos = i - (pd->side_page_num + 1);
        pi->content_num = -1;
        pi->content = NULL;

        pi->obj = efl_add(EFL_UI_BOX_CLASS, spd->pager.obj);
        efl_canvas_group_member_add(spd->pager.group, pi->obj);
        pd->page_infos = eina_list_append(pd->page_infos, pi);

        if (prev)
          {
             pi->prev = prev;
             prev->next = pi;
          }
        prev = pi;
     }

   pd->head->prev = pd->tail;
   pd->tail->next = pd->head;
}

static void
_content_show(Efl_Page_Transition_Scroll_Data *pd,
              Efl_Page_Transition_Data *spd)
{
   Eina_List *list;
   Page_Info *pi;
   int tmp_id;
   Eo *tmp;
   int curr_page, cnt;

   curr_page = efl_ui_pager_current_page_get(spd->pager.obj);
   cnt = efl_content_count(spd->pager.obj);

   // at this point, the number of visible pages might have been changed,
   // so empty all boxes and refill them with the right contents.
   // FIXME make logic efficient: don't have to empty all and fill all the time
   EINA_LIST_FOREACH(pd->page_infos, list, pi)
     {
        if (pi->content)
          {
             efl_pack_unpack(pi->obj, pi->content);
             efl_canvas_object_clip_set(pi->content, pd->backclip);

             pi->content_num = -1;
             pi->content = NULL;
             pi->visible = EINA_FALSE;
          }
        efl_canvas_object_clip_set(pi->obj, pd->backclip);
     }

   EINA_LIST_FOREACH(pd->page_infos, list, pi)
     {
        if (pi->vis_page)
          {
             tmp_id = (curr_page + pi->pos + cnt) % cnt;

             if ((spd->loop == EFL_UI_PAGER_LOOP_DISABLED)
                 && ((pi->pos) * (tmp_id - curr_page) < 0)) continue;

             tmp = efl_pack_content_get(spd->pager.obj, tmp_id);
             if (tmp)
               {
                  efl_canvas_object_clip_set(pi->obj, pd->foreclip);

                  efl_pack(pi->obj, tmp);
                  efl_canvas_object_clip_set(tmp, pd->foreclip);

                  pi->content_num = tmp_id;
                  pi->content = tmp;
                  pi->visible = EINA_TRUE;
               }
          }
     }
}

//FIXME use ecore_job
static void
_page_info_geometry_change(Efl_Page_Transition_Scroll_Data *pd,
                           Efl_Page_Transition_Data *spd)
{
   Eina_List *list;
   Page_Info *pi;
   int content_w;

   content_w = spd->page_spec.sz.w
      + ((spd->page_spec.sz.w + spd->page_spec.padding) * pd->side_page_num * 2);

   if (content_w < spd->pager.w)
     {
        EINA_RECTANGLE_SET(&pd->viewport,
                           spd->pager.x + ((spd->pager.w - content_w) / 2),
                           spd->pager.y,
                           content_w,
                           spd->pager.h);
     }
   else
     {
        EINA_RECTANGLE_SET(&pd->viewport,
                           spd->pager.x,
                           spd->pager.y,
                           spd->pager.w,
                           spd->pager.h);
     }

   efl_gfx_entity_geometry_set(pd->foreclip, (Eina_Rect) pd->viewport);

   // this loop resets the geometry of each page based on the geometry of
   // the pager object, the page size, and the padding size.
   EINA_LIST_FOREACH(pd->page_infos, list, pi)
     {
        EINA_RECTANGLE_SET(&pi->geometry,
                           spd->pager.x + (spd->pager.w / 2)
                           + pi->pos * (spd->page_spec.sz.w + spd->page_spec.padding)
                           - (spd->page_spec.sz.w / 2),
                           spd->pager.y + (spd->pager.h / 2) - (spd->page_spec.sz.h / 2),
                           spd->page_spec.sz.w,
                           spd->page_spec.sz.h);

        if (eina_rectangles_intersect(&pi->geometry, &pd->viewport) &&
            ((pi->id != 0) && (pi->id != (pd->page_info_num - 1))))
          {
             pi->vis_page = EINA_TRUE;
             pi->visible = EINA_TRUE;
             efl_canvas_object_clip_set(pi->obj, pd->foreclip);
          }
        else
          {
             pi->vis_page = EINA_FALSE;
             pi->visible = EINA_FALSE;
             efl_canvas_object_clip_set(pi->obj, pd->backclip);
          }

        efl_gfx_entity_geometry_set(pi->obj, (Eina_Rect) pi->geometry);
     }

   if (efl_content_count(spd->pager.obj) > 0) _content_show(pd, spd);
}

static void
_resize_cb(void *data, const Efl_Event *ev)
{
   Efl_Page_Transition_Scroll *obj = data;
   EFL_PAGE_TRANSITION_SCROLL_DATA_GET(obj, pd);
   EFL_PAGE_TRANSITION_DATA_GET(obj, spd);

   Eina_Size2D sz;

   Efl_Ui_Pager *pager = ev->object;

   sz = efl_gfx_entity_size_get(pager);

   spd->pager.w = sz.w;
   spd->pager.h = sz.h;

   _page_info_geometry_change(pd, spd);
}

static void
_move_cb(void *data, const Efl_Event *ev)
{
   Efl_Page_Transition_Scroll *obj = data;
   EFL_PAGE_TRANSITION_SCROLL_DATA_GET(obj, pd);
   EFL_PAGE_TRANSITION_DATA_GET(obj, spd);

   Eina_Position2D pos;

   Efl_Ui_Pager *pager = ev->object;

   pos = efl_gfx_entity_position_get(pager);

   spd->pager.x = pos.x;
   spd->pager.y = pos.y;

   _page_info_geometry_change(pd, spd);
}

EOLIAN static void
_efl_page_transition_scroll_efl_page_transition_bind(Eo *obj,
                                                     Efl_Page_Transition_Scroll_Data *pd,
                                                     Eo *pager,
                                                     Efl_Canvas_Group *group)
{
   EFL_PAGE_TRANSITION_DATA_GET(obj, spd);

   if (spd->pager.obj == pager) return;

   if (spd->pager.obj)
     {
        efl_event_callback_del(spd->pager.group, EFL_GFX_ENTITY_EVENT_RESIZE, _resize_cb, obj);
        efl_event_callback_del(spd->pager.group, EFL_GFX_ENTITY_EVENT_MOVE, _move_cb, obj);

        _page_info_deallocate(pd);
        efl_del(pd->foreclip);
        efl_del(pd->backclip);
     }

   efl_page_transition_bind(efl_super(obj, MY_CLASS), pager, group);

   if (spd->pager.obj)
     {
        int cnt, i;
        Eo *item;

        efl_event_callback_add(spd->pager.group, EFL_GFX_ENTITY_EVENT_RESIZE, _resize_cb, obj);
        efl_event_callback_add(spd->pager.group, EFL_GFX_ENTITY_EVENT_MOVE, _move_cb, obj);

        pd->foreclip = efl_add(EFL_CANVAS_RECTANGLE_CLASS,
                               evas_object_evas_get(spd->pager.obj));
        evas_object_static_clip_set(pd->foreclip, EINA_TRUE);

        pd->backclip = efl_add(EFL_CANVAS_RECTANGLE_CLASS,
                               evas_object_evas_get(spd->pager.obj));
        evas_object_static_clip_set(pd->backclip, EINA_TRUE);
        efl_gfx_entity_visible_set(pd->backclip, EINA_FALSE);

        cnt = efl_content_count(spd->pager.obj);
        for (i = 0; i < cnt; i++)
          {
             item = efl_pack_content_get(spd->pager.obj, i);
             efl_canvas_object_clip_set(item, pd->backclip);
          }
        _page_info_allocate(pd, spd);
        _page_info_geometry_change(pd, spd);
     }
}

EOLIAN static void
_efl_page_transition_scroll_page_size_set(Eo *obj,
                                          Efl_Page_Transition_Scroll_Data *pd,
                                          Eina_Size2D sz)
{
   EFL_PAGE_TRANSITION_DATA_GET(obj, spd);

   efl_page_transition_page_size_set(efl_super(obj, MY_CLASS), sz);
   _page_info_geometry_change(pd, spd);
}

EOLIAN static void
_efl_page_transition_scroll_padding_size_set(Eo *obj,
                                             Efl_Page_Transition_Scroll_Data *pd,
                                             int padding)
{
   EFL_PAGE_TRANSITION_DATA_GET(obj, spd);

   efl_page_transition_padding_size_set(efl_super(obj, MY_CLASS), padding);
   _page_info_geometry_change(pd, spd);
}

EOLIAN static void
_efl_page_transition_scroll_update(Eo *obj,
                                   Efl_Page_Transition_Scroll_Data *pd,
                                   double pos)
{
   EFL_PAGE_TRANSITION_DATA_GET(obj, spd);

   double t;
   int tmp_id, curr_page, cnt;
   Eo *tmp;
   Eina_List *list;
   Page_Info *pi, *tpi;

   t = pos;
   if (t < 0) t *= (-1);

   curr_page = efl_ui_pager_current_page_get(spd->pager.obj);
   cnt = efl_content_count(spd->pager.obj);

   // while pages are scrolled,
   // 1. the geometry of each page needs to be changed
   // 2. if a page gets out of the viewport, it needs to be hidden
   // 3. if a page gets into the viewport, it needs to be shown
   EINA_LIST_FOREACH(pd->page_infos, list, pi)
     {
        if (pos < 0) // if scrolled right, each page takes next page's position
          tpi = pi->next;
        else // else if scrolled left, each page takes prev page's position
          tpi = pi->prev;

        EINA_RECTANGLE_SET(&pi->temp,
                           tpi->geometry.x * t + pi->geometry.x * (1 - t),
                           tpi->geometry.y,
                           tpi->geometry.w,
                           tpi->geometry.h);

        efl_gfx_entity_geometry_set(pi->obj, (Eina_Rect) pi->temp);

        if (!pi->vis_page && !tpi->vis_page) continue;

        if (!eina_rectangles_intersect(&pi->temp, &pd->viewport))
          {
             if (pi->content)
               {
                  efl_canvas_object_clip_set(pi->obj, pd->backclip);

                  efl_pack_unpack(pi->obj, pi->content);
                  efl_canvas_object_clip_set(pi->content, pd->backclip);

                  pi->content_num = -1;
                  pi->content = NULL;
                  pi->visible = EINA_FALSE;

               }
          }
        else
          {
             tmp_id = (curr_page + pi->pos + cnt) % cnt;
             if (pi->content_num != tmp_id)
               {
                  if (pi->content) //FIXME if the content num is the same, do nothing
                    {
                       efl_pack_unpack(pi->obj, pi->content);
                       efl_canvas_object_clip_set(pi->content, pd->backclip);

                       pi->content_num = -1;
                       pi->content = NULL;
                    }

                  if ((spd->loop == EFL_UI_PAGER_LOOP_DISABLED)
                      && ((pi->pos) * (tmp_id - curr_page) < 0)) continue;
                  tmp = efl_pack_content_get(spd->pager.obj, tmp_id);

                  if (tmp)
                    {
                       efl_canvas_object_clip_set(pi->obj, pd->foreclip);

                       efl_pack(pi->obj, tmp);
                       efl_canvas_object_clip_set(tmp, pd->foreclip);

                       pi->content_num = tmp_id;
                       pi->content = tmp;
                       pi->visible = EINA_TRUE;
                    }
               }
          }
     }
}

EOLIAN static void
_efl_page_transition_scroll_curr_page_change(Eo *obj EINA_UNUSED,
                                             Efl_Page_Transition_Scroll_Data *pd,
                                             double pos)
{
   Eina_List *list;
   Page_Info *pi, *target = NULL;

   // after the current page is changed, page infos need to be updated
   // with a new id based on the new geometry of the boxes.
   EINA_LIST_FOREACH(pd->page_infos, list, pi)
     {
        if (EINA_DBL_EQ(pos, 1.0))
          {
             pi->id = (pi->id - 1 + pd->page_info_num) % pd->page_info_num;
             target = pi->prev;
          }
        else if (EINA_DBL_EQ(pos, -1.0))
          {
             pi->id = (pi->id + 1) % pd->page_info_num;
             target = pi->next;
          }
        pi->pos = pi->id - (pd->side_page_num + 1);

        if (!target) return;

        EINA_RECTANGLE_SET(&pi->temp,
                           target->geometry.x,
                           target->geometry.y,
                           target->geometry.w,
                           target->geometry.h);

        if (eina_rectangles_intersect(&pi->temp, &pd->viewport) &&
            (pi->id != 0) && (pi->id != (pd->page_info_num - 1)))
          {
             pi->vis_page = EINA_TRUE;
             pi->visible = EINA_TRUE;
             efl_canvas_object_clip_set(pi->obj, pd->foreclip);
          }
        else
          {
             pi->vis_page = EINA_FALSE;
             pi->visible = EINA_FALSE;
             efl_canvas_object_clip_set(pi->obj, pd->backclip);
             if (pi->content)
               {
                  efl_pack_unpack(pi->obj, pi->content);
                  efl_canvas_object_clip_set(pi->content, pd->backclip);
               }
          }
     }

   if (EINA_DBL_EQ(pos, 1.0))
     {
        pd->head = pd->head->next;
        pd->tail = pd->tail->next;
     }
   else if (EINA_DBL_EQ(pos, -1.0))
     {
        pd->head = pd->head->prev;
        pd->tail = pd->tail->prev;
     }

   EINA_LIST_FOREACH(pd->page_infos, list, pi)
     {
        EINA_RECTANGLE_SET(&pi->geometry,
                           pi->temp.x,
                           pi->temp.y,
                           pi->temp.w,
                           pi->temp.h);
     }
}

EOLIAN static int
_efl_page_transition_scroll_side_page_num_get(const Eo *obj EINA_UNUSED,
                                              Efl_Page_Transition_Scroll_Data *pd)
{
   return pd->side_page_num;
}

static Page_Info *
_add_item(Efl_Page_Transition_Scroll_Data *pd, Efl_Page_Transition_Data *spd)
{
   Page_Info *pi;

   pi = calloc(1, sizeof(*pi));
   pi->obj = efl_add(EFL_UI_BOX_CLASS, spd->pager.obj);
   efl_canvas_group_member_add(spd->pager.group, pi->obj);
   pi->content_num = -1;
   pi->content = NULL;

   pi->next = pd->head;
   pd->head->prev = pi;
   pi->prev = pd->tail;
   pd->tail->next = pi;

   return pi;
}

static void
_remove_item(Page_Info *pi, Efl_Page_Transition_Scroll_Data *pd)
{
   efl_canvas_object_clip_set(pi->content, pd->backclip);
   efl_pack_unpack(pi->obj, pi->content);
   efl_del(pi->obj);
   pi->prev->next = pi->next;
   pi->next->prev = pi->prev;
   pd->page_infos = eina_list_remove(pd->page_infos, pi);
   free(pi);
}

EOLIAN static void
_efl_page_transition_scroll_side_page_num_set(Eo *obj,
                                              Efl_Page_Transition_Scroll_Data *pd,
                                              int side_page_num)
{
   EFL_PAGE_TRANSITION_DATA_GET(obj, spd);
   Page_Info *pi;
   int delta, i;

   if (pd->side_page_num == side_page_num) return;

   delta = side_page_num - pd->side_page_num;
   pd->side_page_num = side_page_num;
   pd->page_info_num = (side_page_num * 2) + 3;

   if (!spd->pager.obj) return;

   if (delta > 0)
     {
        // side_page_num is increased, so add boxes at both ends by the diff
        for (i = 0; i < delta; i++)
          {
             pi = _add_item(pd, spd);
             pd->page_infos = eina_list_prepend_relative(pd->page_infos, pi, pd->head);
             pd->head = pi;
             efl_gfx_stack_below(pi->obj, pi->next->obj);

             pi = _add_item(pd, spd);
             pd->page_infos = eina_list_append_relative(pd->page_infos, pi, pd->tail);
             pd->tail = pi;
             efl_gfx_stack_above(pi->obj, pi->prev->obj);
          }
     }
   else
     {
        // side_page_num is decreased, so remove boxes at both ends by the diff
        for (i = 0; i > delta; i--)
          {
             pi = pd->head;
             pd->head = pi->next;
             _remove_item(pi, pd);

             pi = pd->tail;
             pd->tail = pi->prev;
             _remove_item(pi, pd);
          }
     }

   pi = pd->head;
   for (i = 0; i < pd->page_info_num; i++)
     {
        pi->id = i;
        pi->pos = i - (pd->side_page_num + 1);
        pi = pi->next;
     }

   _page_info_geometry_change(pd, spd);
}

EOLIAN static void
_efl_page_transition_scroll_loop_set(Eo *obj,
                                     Efl_Page_Transition_Scroll_Data *pd,
                                     Efl_Ui_Pager_Loop loop)
{
   EFL_PAGE_TRANSITION_DATA_GET(obj, spd);

   if (spd->loop == loop) return;

   efl_page_transition_loop_set(efl_super(obj, MY_CLASS), loop);

   _content_show(pd, spd);
}

EOLIAN static Eo *
_efl_page_transition_scroll_efl_object_constructor(Eo *obj,
                                                   Efl_Page_Transition_Scroll_Data *pd)
{
   obj = efl_constructor(efl_super(obj, MY_CLASS));

   pd->side_page_num = 1;
   pd->page_info_num = pd->side_page_num * 2 + 3;

   return obj;
}

EOLIAN static void
_efl_page_transition_scroll_efl_object_invalidate(Eo *obj,
                                                  Efl_Page_Transition_Scroll_Data *pd)
{
   Page_Info *pi;

   EINA_LIST_FREE(pd->page_infos, pi)
     {
        efl_del(pi->obj);
        free(pi);
     }

   efl_invalidate(efl_super(obj, MY_CLASS));
}


#define EFL_PAGE_TRANSITION_SCROLL_EXTRA_OPS \
   EFL_OBJECT_OP_FUNC(efl_page_transition_update, \
                      _efl_page_transition_scroll_update), \
   EFL_OBJECT_OP_FUNC(efl_page_transition_curr_page_change, \
                      _efl_page_transition_scroll_curr_page_change), \
   EFL_OBJECT_OP_FUNC(efl_page_transition_page_size_set, \
                      _efl_page_transition_scroll_page_size_set), \
   EFL_OBJECT_OP_FUNC(efl_page_transition_padding_size_set, \
                      _efl_page_transition_scroll_padding_size_set), \
   EFL_OBJECT_OP_FUNC(efl_page_transition_loop_set, \
                      _efl_page_transition_scroll_loop_set)

#include "efl_page_transition_scroll.eo.c"
