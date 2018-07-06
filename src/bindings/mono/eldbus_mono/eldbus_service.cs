#pragma warning disable 1591

using System;
using System.Runtime.InteropServices;

using static Eldbus.EldbusServiceNativeFunctions;

namespace Eldbus {

public static class EldbusServiceNativeFunctions
{
    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_service_interface_register(IntPtr conn, string path, IntPtr desc);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_service_interface_fallback_register(IntPtr conn, string path, IntPtr desc);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_service_interface_register2(IntPtr conn, string path, IntPtr desc);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_service_interface_fallback_register2(IntPtr conn, string path, IntPtr desc);

    [DllImport(Efl.Libs.Eldbus)] public static extern void
        eldbus_service_interface_unregister(IntPtr iface);

    [DllImport(Efl.Libs.Eldbus)] public static extern void
        eldbus_service_object_unregister(IntPtr iface);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_service_connection_get(IntPtr iface);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_service_object_path_get(IntPtr iface);

//     [DllImport(Efl.Libs.Eldbus)] [return: MarshalAs(UnmanagedType.U1)] public static extern bool
//         eldbus_service_signal_emit(IntPtr iface, uint signal_id, ...);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_service_signal_new(IntPtr iface, uint signal_id);

    [DllImport(Efl.Libs.Eldbus)] [return: MarshalAs(UnmanagedType.U1)] public static extern bool
        eldbus_service_signal_send(IntPtr iface, IntPtr signal_msg);

    [DllImport(Efl.Libs.Eldbus)] public static extern void
        eldbus_service_object_data_set(IntPtr iface, string key, IntPtr data);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_service_object_data_get(IntPtr iface, string key);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_service_object_data_del(IntPtr iface, string key);

    [DllImport(Efl.Libs.Eldbus)] [return: MarshalAs(UnmanagedType.U1)] public static extern bool
        eldbus_service_property_changed(IntPtr iface, string name);

    [DllImport(Efl.Libs.Eldbus)] [return: MarshalAs(UnmanagedType.U1)] public static extern bool
        eldbus_service_property_invalidate_set(IntPtr iface, string name, [MarshalAs(UnmanagedType.U1)] bool is_invalidate);

    [DllImport(Efl.Libs.Eldbus)] [return: MarshalAs(UnmanagedType.U1)] public static extern bool
        eldbus_service_object_manager_attach(IntPtr iface);

    [DllImport(Efl.Libs.Eldbus)] [return: MarshalAs(UnmanagedType.U1)] public static extern bool
        eldbus_service_object_manager_detach(IntPtr iface);
}

}

