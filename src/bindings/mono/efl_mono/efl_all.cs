#pragma warning disable 1591

using System;
using System.Runtime.InteropServices;
using System.Threading;

using static Efl.UnsafeNativeMethods;

namespace Efl {

static class UnsafeNativeMethods {
    [DllImport(Efl.Libs.Ecore)] public static extern void ecore_init();
    [DllImport(Efl.Libs.Ecore)] public static extern void ecore_shutdown();
    [DllImport(Efl.Libs.Evas)] public static extern void evas_init();
    [DllImport(Efl.Libs.Evas)] public static extern void evas_shutdown();
    [DllImport(Efl.Libs.Elementary)] public static extern int elm_init(int argc, IntPtr argv);
    [DllImport(Efl.Libs.Elementary)] public static extern void elm_policy_set(int policy, int policy_detail);
    [DllImport(Efl.Libs.Elementary)] public static extern void elm_shutdown();
    [DllImport(Efl.Libs.Elementary)] public static extern void elm_run();
    [DllImport(Efl.Libs.Elementary)] public static extern void elm_exit();
}

public enum Components {
    Basic,
    Ui
}

public static class All {
    private static bool InitializedUi = false;

    public static void Init(Components components=Components.Basic) {
        Eina.Config.Init();
        efl.eo.Config.Init();
        ecore_init();
        evas_init();
        Eldbus.Config.Init();

        if (components == Components.Ui) {
            Efl.Ui.Config.Init();
            InitializedUi = true;
        }
    }

    /// <summary>Shutdowns all EFL subsystems.</summary>
    public static void Shutdown() {
        // Try to cleanup everything before actually shutting down.
        System.GC.Collect();
        System.GC.WaitForPendingFinalizers();

        if (InitializedUi)
            Efl.Ui.Config.Shutdown();
        Eldbus.Config.Shutdown();
        evas_shutdown();
        ecore_shutdown();
        efl.eo.Config.Shutdown();
        Eina.Config.Shutdown();
    }
}

// Placeholder. Will move to elm_config.cs later
namespace Ui {

public static class Config {
    public static void Init() {
        // TODO Support elm command line arguments
#if WIN32 // Not a native define, we define it in our build system
        // Ecore_Win32 uses OleInitialize, which requires single thread apartments
        if (System.Threading.Thread.CurrentThread.GetApartmentState() != ApartmentState.STA)
            throw new InvalidOperationException("UI Applications require STAThreadAttribute in Main()");
#endif
        elm_init(0, IntPtr.Zero);

        elm_policy_set((int)Elm.Policy.Quit, (int)Elm.Policy_Quit.Last_window_hidden);
    }
    public static void Shutdown() {
        elm_shutdown();
    }

    public static void Run() {
        elm_run();
    }

    public static void Exit() {
        elm_exit();
    }
}

}

}
