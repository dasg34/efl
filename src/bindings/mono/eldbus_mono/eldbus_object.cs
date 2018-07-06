
#pragma warning disable 1591

using System.Runtime.InteropServices;

using static Eldbus.EldbusObjectNativeFunctions;

using IntPtr = System.IntPtr;

namespace Eldbus {


public static class EldbusObjectNativeFunctions
{
    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_object_get(IntPtr conn, string bus, string path);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_object_ref(IntPtr obj);

    [DllImport(Efl.Libs.Eldbus)] public static extern void
        eldbus_object_unref(IntPtr obj);

    [DllImport(Efl.Libs.Eldbus)] public static extern void
        eldbus_object_free_cb_add(IntPtr obj, IntPtr cb, IntPtr data);

    [DllImport(Efl.Libs.Eldbus)] public static extern void
        eldbus_object_free_cb_del(IntPtr obj, IntPtr cb, IntPtr data);

// typedef enum
// {
//    ELDBUS_OBJECT_EVENT_IFACE_ADDED = 0, /**< a parent path must have a ObjectManager interface */
//    ELDBUS_OBJECT_EVENT_IFACE_REMOVED, /**< a parent path must have a ObjectManager interface */
//    ELDBUS_OBJECT_EVENT_PROPERTY_CHANGED, /**< a property has changes */
//    ELDBUS_OBJECT_EVENT_PROPERTY_REMOVED, /**< a property was removed */
//    ELDBUS_OBJECT_EVENT_DEL,
//    ELDBUS_OBJECT_EVENT_LAST    /**< sentinel, not a real event type */
// } Eldbus_Object_Event_Type;
//
//     [DllImport(Efl.Libs.Eldbus)] public static extern void
//         eldbus_object_event_callback_add(IntPtr obj, Eldbus_Object_Event_Type type, IntPtr cb, IntPtr cb_data);
//
//     [DllImport(Efl.Libs.Eldbus)] public static extern void
//         eldbus_object_event_callback_del(IntPtr obj, Eldbus_Object_Event_Type type, IntPtr cb, IntPtr cb_data);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_object_connection_get(IntPtr obj);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_object_bus_name_get(IntPtr obj);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_object_path_get(IntPtr obj);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_object_send(IntPtr obj, IntPtr msg, IntPtr cb, IntPtr cb_data, double timeout);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_object_signal_handler_add(IntPtr obj, string _interface, string member, IntPtr cb, IntPtr cb_data);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_object_method_call_new(IntPtr obj, string _interface, string member);

    // FreeDesktop.Org Methods

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_object_peer_ping(IntPtr obj, IntPtr cb, IntPtr data);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_object_peer_machine_id_get(IntPtr obj, IntPtr cb, IntPtr data);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_object_introspect(IntPtr obj, IntPtr cb, IntPtr data);

    [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
        eldbus_object_managed_objects_get(IntPtr obj, IntPtr cb, IntPtr data);

//     [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
//         eldbus_object_manager_interfaces_added(IntPtr obj, Eldbus_Signal_Cb cb, IntPtr cb_data);
//
//     [DllImport(Efl.Libs.Eldbus)] public static extern IntPtr
//         eldbus_object_manager_interfaces_removed(IntPtr obj, Eldbus_Signal_Cb cb, IntPtr cb_data);
}


public class Object : System.IDisposable
{

    public IntPtr Handle {get;set;} = IntPtr.Zero;
    public bool Own {get;set;} = true;

    private void InitNew(IntPtr handle, bool own)
    {
        Handle = handle;
        Own = own;
        CheckHandle();
    }

    private void CheckHandle()
    {
        if (Handle == IntPtr.Zero)
        {
            Eldbus.Common.RaiseNullHandle();
        }
    }

    public Object(IntPtr handle, bool own)
    {
        InitNew(handle, own);
    }

    public Object(Eldbus.Connection conn, string bus, string path)
    {
        if (conn == null)
            throw new System.ArgumentNullException("conn");
        if (bus == null)
            throw new System.ArgumentNullException("bus");
        if (path == null)
            throw new System.ArgumentNullException("path");

        var handle = eldbus_object_get(conn.Handle, bus, path);

        if (handle == IntPtr.Zero)
        {
            Eina.Error.RaiseIfOccurred();
            throw new SEHException("Eldbus: could not get `Object' object from eldbus_object_get");
        }

        InitNew(handle, true);
    }

    ~Object()
    {
        Dispose(false);
    }

    protected virtual void Dispose(bool disposing)
    {
        IntPtr h = Handle;
        Handle = IntPtr.Zero;
        if (h == IntPtr.Zero)
            return;

        if (Own)
            eldbus_object_unref(h);
    }

    public void Dispose()
    {
        Dispose(true);
        System.GC.SuppressFinalize(this);
    }

    public void Free()
    {
        Dispose();
    }

    public IntPtr Release()
    {
        IntPtr h = Handle;
        Handle = IntPtr.Zero;
        return h;
    }

    public Eldbus.Connection GetConnection()
    {
        CheckHandle();
        var conn = eldbus_object_connection_get(Handle);

        if (conn == IntPtr.Zero)
        {
            Eina.Error.RaiseIfOccurred();
            throw new SEHException("Eldbus: could not get `Connection' object from eldbus_object_connection_get");
        }

        return new Eldbus.Connection(conn, false);
    }

    public string GetBusName()
    {
        CheckHandle();
        var ptr = eldbus_object_bus_name_get(Handle);
        return Marshal.PtrToStringAuto(ptr);
    }

    public string GetPath()
    {
        CheckHandle();
        var ptr = eldbus_object_path_get(Handle);
        return Marshal.PtrToStringAuto(ptr);
    }

    public void Ref()
    {
        CheckHandle();
        eldbus_object_ref(Handle);
    }

    public void Unref()
    {
        CheckHandle();
        eldbus_object_unref(Handle);
    }

    public Eldbus.Pending Send(Eldbus.Message msg, Eldbus.MessageDelegate dlgt = null, double timeout = -1)
    {
        CheckHandle();

        if (msg == null)
            throw new System.ArgumentNullException("msg");

        IntPtr cb_wrapper = dlgt == null ? IntPtr.Zero : Eldbus.Common.GetMessageCbWrapperPtr();
        IntPtr cb_data = dlgt == null ? IntPtr.Zero : Marshal.GetFunctionPointerForDelegate(dlgt);

        var pending_hdl = eldbus_object_send(Handle, msg.Handle, cb_wrapper, cb_data, timeout);

        if (pending_hdl == IntPtr.Zero)
        {
            Eina.Error.RaiseIfOccurred();
            throw new SEHException("Eldbus: could not get `Pending' object from eldbus_object_send");
        }

        return new Eldbus.Pending(pending_hdl, false);
    }

    public Eldbus.Message NewMethodCall(string _interface, string member)
    {
        CheckHandle();

        var hdl = eldbus_object_method_call_new(Handle, _interface, member);

        if (hdl == IntPtr.Zero)
        {
            Eina.Error.RaiseIfOccurred();
            throw new SEHException("Eldbus: could not get `Message' object from eldbus_object_method_call_new");
        }

        return new Eldbus.Message(hdl, false);
    }

    public Eldbus.Pending PeerPing(Eldbus.MessageDelegate dlgt = null)
    {
        CheckHandle();

        IntPtr cb_wrapper = dlgt == null ? IntPtr.Zero : Eldbus.Common.GetMessageCbWrapperPtr();
        IntPtr cb_data = dlgt == null ? IntPtr.Zero : Marshal.GetFunctionPointerForDelegate(dlgt);

        var pending_hdl = eldbus_object_peer_ping(Handle, cb_wrapper, cb_data);

        if (pending_hdl == IntPtr.Zero)
        {
            Eina.Error.RaiseIfOccurred();
            throw new SEHException("Eldbus: could not get `Pending' object from eldbus_object_peer_ping");
        }

        return new Eldbus.Pending(pending_hdl, false);
    }

    public Eldbus.Pending GetPeerMachineId(Eldbus.MessageDelegate dlgt = null)
    {
        CheckHandle();

        IntPtr cb_wrapper = dlgt == null ? IntPtr.Zero : Eldbus.Common.GetMessageCbWrapperPtr();
        IntPtr cb_data = dlgt == null ? IntPtr.Zero : Marshal.GetFunctionPointerForDelegate(dlgt);

        var pending_hdl = eldbus_object_peer_machine_id_get(Handle, cb_wrapper, cb_data);

        if (pending_hdl == IntPtr.Zero)
        {
            Eina.Error.RaiseIfOccurred();
            throw new SEHException("Eldbus: could not get `Pending' object from eldbus_object_peer_machine_id_get");
        }

        return new Eldbus.Pending(pending_hdl, false);
    }

    public Eldbus.Pending Introspect(Eldbus.MessageDelegate dlgt = null)
    {
        CheckHandle();

        IntPtr cb_wrapper = dlgt == null ? IntPtr.Zero : Eldbus.Common.GetMessageCbWrapperPtr();
        IntPtr cb_data = dlgt == null ? IntPtr.Zero : Marshal.GetFunctionPointerForDelegate(dlgt);

        var pending_hdl = eldbus_object_introspect(Handle, cb_wrapper, cb_data);

        if (pending_hdl == IntPtr.Zero)
        {
            Eina.Error.RaiseIfOccurred();
            throw new SEHException("Eldbus: could not get `Pending' object from eldbus_object_introspect");
        }

        return new Eldbus.Pending(pending_hdl, false);
    }

    public Eldbus.Pending GetManagedObjects(Eldbus.MessageDelegate dlgt = null)
    {
        CheckHandle();

        IntPtr cb_wrapper = dlgt == null ? IntPtr.Zero : Eldbus.Common.GetMessageCbWrapperPtr();
        IntPtr cb_data = dlgt == null ? IntPtr.Zero : Marshal.GetFunctionPointerForDelegate(dlgt);

        var pending_hdl = eldbus_object_managed_objects_get(Handle, cb_wrapper, cb_data);

        if (pending_hdl == IntPtr.Zero)
        {
            Eina.Error.RaiseIfOccurred();
            throw new SEHException("Eldbus: could not get `Pending' object from eldbus_object_managed_objects_get");
        }

        return new Eldbus.Pending(pending_hdl, false);
    }

}


}
