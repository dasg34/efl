#define ELM_RELATIVE_CONTAINER_CLASS elm_relative_container_class_get()

#define MY_LEGACY_CLASS ELM_RELATIVE_CONTAINER_CLASS
#define MY_CLASS_NAME_LEGACY "elm_relative_container"

EAPI Evas_Object *
elm_relative_container_add(Evas_Object *parent)
{
   EINA_SAFETY_ON_NULL_RETURN_VAL(parent, NULL);
   return elm_legacy_add(MY_CLASS, parent);
}

EAPI void
elm_relative_container_relation_left_set(Evas_Object *obj, Evas_Object *child, Evas_Object *base, double relative_position)
{
   efl_ui_relative_container_relation_left_set(obj, child, base, relative_position);
}

EAPI void
elm_relative_container_relation_right_set(Evas_Object *obj, Evas_Object *child, Evas_Object *base, double relative_position)
{
   efl_ui_relative_container_relation_right_set(obj, child, base, relative_position);
}

EAPI void
elm_relative_container_relation_top_set(Evas_Object *obj, Evas_Object *child, Evas_Object *base, double relative_position)
{
   efl_ui_relative_container_relation_top_set(obj, child, base, relative_position);
}

EAPI void
elm_relative_container_relation_bottom_set(Evas_Object *obj, Evas_Object *child, Evas_Object *base, double relative_position)
{
   efl_ui_relative_container_relation_bottom_set(obj, child, base, relative_position);
}

EOLIAN static Evas_Object *
_elm_relative_container_efl_object_constructor(Eo *obj, void *_pd EINA_UNUSED)
{
   obj = efl_constructor(efl_super(obj, MY_CLASS));
   efl_canvas_object_type_set(obj, MY_CLASS_NAME_LEGACY);
   efl_access_object_access_type_set(obj, EFL_ACCESS_TYPE_SKIPPED);
   efl_access_object_role_set(obj, EFL_ACCESS_ROLE_FILLER);

   return obj;
}

static Eina_Bool
_elm_relative_container_class_initializer(Efl_Class *klass)
{
   const Efl_Object_Ops *opsp = NULL;

   const Efl_Object_Property_Reflection_Ops *ropsp = NULL;

   EFL_OPS_DEFINE(ops,
      EFL_OBJECT_OP_FUNC(efl_constructor, _elm_relative_container_efl_object_constructor),
   );
   opsp = &ops;

   return efl_class_functions_set(klass, opsp, ropsp);
}

static const Efl_Class_Description _elm_relative_container_class_desc = {
   EO_VERSION,
   "Elm.Relative_Container",
   EFL_CLASS_TYPE_REGULAR,
   0,
   _elm_relative_container_class_initializer,
   NULL,
   NULL
};

EFL_DEFINE_CLASS(elm_relative_container_class_get, &_elm_relative_container_class_desc, EFL_UI_RELATIVE_CONTAINER_CLASS, EFL_UI_LEGACY_INTERFACE, NULL);
