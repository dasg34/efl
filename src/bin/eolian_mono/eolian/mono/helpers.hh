#ifndef EOLIAN_MONO_HELPERS_HH
#define EOLIAN_MONO_HELPERS_HH

#include "grammar/klass_def.hpp"
#include "blacklist.hh"
#include "name_helpers.hh"

namespace eolian_mono {

namespace helpers {

/* General helpers, not related directly with generating strings (those go in the name_helpers.hh). */

namespace attributes = efl::eolian::grammar::attributes;

inline bool need_struct_conversion(attributes::regular_type_def const* regular)
{
   return regular && regular->is_struct() && !blacklist::is_struct_blacklisted(*regular);
}

inline bool need_struct_conversion(attributes::parameter_def const& param, attributes::regular_type_def const* regular)
{
   if (param.direction == attributes::parameter_direction::in && param.type.has_own)
     return false;

   return need_struct_conversion(regular);
}

inline bool need_struct_conversion_in_return(attributes::type_def const& ret_type, attributes::parameter_direction const& direction)
{
   auto regular = efl::eina::get<attributes::regular_type_def>(&ret_type.original_type);

   if (!regular->is_struct())
     return false;

   if (regular->is_struct() && (direction == attributes::parameter_direction::out || direction == attributes::parameter_direction::unknown))
     return false;

   if (ret_type.has_own)
     return false;

   return true;
}

inline bool need_pointer_conversion(attributes::regular_type_def const* regular)
{
   if (!regular)
     return false;

   if (regular->is_enum()
       || (regular->is_struct() && name_helpers::type_full_eolian_name(*regular) != "Eina.Binbuf")
      )
     return true;

   std::set<std::string> const types {
     "bool", "char"
     , "byte" , "short" , "int" , "long" , "llong" , "int8" , "int16" , "int32" , "int64" , "ssize"
     , "ubyte", "ushort", "uint", "ulong", "ullong", "uint8", "uint16", "uint32", "uint64", "size"
     , "ptrdiff"
     , "float", "double"
   };
   if (types.find(regular->base_type) != types.end())
     return true;

   return false;
}

// While klass_def has immediate_inherits, we need a way to get all interfaces inherited by an interface
// either directly or through another interface.
std::set<attributes::klass_name, attributes::compare_klass_name_by_name> interface_inherits(attributes::klass_def const& cls)
{
   std::set<attributes::klass_name, attributes::compare_klass_name_by_name> inherits;

   std::function<void(attributes::klass_name const&, int)> inherit_algo = 
       [&] (attributes::klass_name const& klass, int indent)
       {
          // TODO we could somehow cache klass_def instantiations
          attributes::klass_def c(get_klass(klass, cls.unit), cls.unit);
          /* EINA_LOG_ERR("%*s Checking parents of %s", indent + 1, " ", name_helpers::klass_full_concrete_or_interface_name(c).c_str()); */
          for(auto&& inherit : c.immediate_inherits)
            {
               /* EINA_LOG_ERR("%*s Testing interface %s", indent + 1, " ", name_helpers::klass_full_concrete_or_interface_name(inherit).c_str()); */
               switch(inherit.type)
                 {
                 case attributes::class_type::mixin:
                 case attributes::class_type::interface_:
                 /* case attributes::class_type::abstract_: */
                   /* EINA_LOG_ERR("%*s It is an C# interface. Adding to set of interfaces and checking its parents.", indent+1, " "); */
                   inherits.insert(inherit);
                   inherit_algo(inherit, indent+2);
                   break;
                 case attributes::class_type::regular:
                 case attributes::class_type::abstract_:
                   inherit_algo(inherit, indent+2);
                 default:
                   break;
                 }
            }
          /* EINA_LOG_ERR("%*s Returning from call to class %s", indent+1, " ", name_helpers::klass_full_concrete_or_interface_name(c).c_str()); */
       };

   inherit_algo(get_klass_name(cls), 0);


   return inherits;
}

// Returns the set of interfaces implemented by this type that haven't been implemented
// by a regular parent class.
std::set<attributes::klass_name, attributes::compare_klass_name_by_name> non_implemented_interfaces(attributes::klass_def const& cls)
{
   std::set<attributes::klass_name, attributes::compare_klass_name_by_name> implemented_interfaces;
   std::set<attributes::klass_name, attributes::compare_klass_name_by_name> interfaces;

   std::function<void(attributes::klass_name const&, int, bool)> inherit_algo = 
       [&] (attributes::klass_name const& klass, int indent, bool is_implemented)
       {
          // TODO we could somehow cache klass_def instantiations
          attributes::klass_def c(get_klass(klass, cls.unit), cls.unit);
          /* EINA_LOG_ERR("%*s Checking parents of %s", indent + 1, " ", name_helpers::klass_full_concrete_or_interface_name(c).c_str()); */
          for(auto&& inherit : c.immediate_inherits)
            {
               /* EINA_LOG_ERR("%*s Testing interface %s", indent + 1, " ", name_helpers::klass_full_concrete_or_interface_name(inherit).c_str()); */
               switch(inherit.type)
                 {
                 case attributes::class_type::mixin:
                 case attributes::class_type::interface_:
                   /* EINA_LOG_ERR("%*s It is an C# interface. Adding to set of interfaces and checking its parents.", indent+1, " "); */
                   interfaces.insert(inherit);
                   if (is_implemented)
                     implemented_interfaces.insert(inherit);
                   inherit_algo(inherit, indent+2, is_implemented);
                   break;
                 case attributes::class_type::abstract_:
                 case attributes::class_type::regular:
                   inherit_algo(inherit, indent+2, true);
                 default:
                   break;
                 }
            }
          /* EINA_LOG_ERR("%*s Returning from call to class %s", indent+1, " ", name_helpers::klass_full_concrete_or_interface_name(c).c_str()); */
       };

   inherit_algo(get_klass_name(cls), 0, false);

   for (auto&& inherit : implemented_interfaces)
     {
        /* EINA_LOG_ERR("Removing interface %s as it is alreayd implemented.", name_helpers::klass_full_concrete_or_interface_name(inherit).c_str()); */
        if (interfaces.find(inherit) != interfaces.end())
          interfaces.erase(inherit);
     }


   return interfaces;
}


/*
 * Determines whether this class has any regular ancestor or not
 */
bool has_regular_ancestor(attributes::klass_def const& cls)
{
   auto inherits = cls.inherits;
   std::function<bool(attributes::klass_name const&)> is_regular = 
       [&] (attributes::klass_name const& klass)
       {
          EINA_LOG_ERR("Ancestor %s is regular? %d", name_helpers::klass_full_concrete_or_interface_name(klass).c_str(),
                  klass.type == attributes::class_type::regular || klass.type == attributes::class_type::abstract_);
          return klass.type == attributes::class_type::regular || klass.type == attributes::class_type::abstract_;
       };

   return std::any_of(inherits.begin(), inherits.end(), is_regular);
}

} // namespace helpers

} // namespace eolian_mono

#endif
