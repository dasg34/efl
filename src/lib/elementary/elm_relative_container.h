#ifndef EFL_NOLEGACY_API_SUPPORT

EAPI Evas_Object                 *elm_relative_container_add(Evas_Object *parent);


EAPI void elm_relative_container_relation_left_set(Evas_Object *obj, Evas_Object *child, Evas_Object *base, double relative_position);

EAPI void elm_relative_container_relation_right_set(Evas_Object *obj, Evas_Object *child, Evas_Object *base, double relative_position);

EAPI void elm_relative_container_relation_top_set(Evas_Object *obj, Evas_Object *child, Evas_Object *base, double relative_position);

EAPI void elm_relative_container_relation_bottom_set(Evas_Object *obj, Evas_Object *child, Evas_Object *base, double relative_position);


#endif
