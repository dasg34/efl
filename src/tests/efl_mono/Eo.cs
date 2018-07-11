using System;
using System.Linq;

namespace TestSuite
{

class TestEo
{
    private class Derived : Dummy.Test_ObjectInherit
    {
    }

    //
    // Test cases:
    //
    public static void return_same_object()
    {
        Dummy.ITest_Object testing = new Dummy.Test_Object();
        Dummy.ITest_Object o1 = testing.ReturnObject();
        Test.Assert(o1.raw_handle != IntPtr.Zero);
        Test.Assert(o1.raw_handle == testing.raw_handle);
        Dummy.ITest_Object o2 = o1.ReturnObject();
        Test.Assert(o2.raw_handle != IntPtr.Zero);
        Test.Assert(o2.raw_handle == o1.raw_handle);
    }

    /* Commented out as adding the event listener seems to prevent it from being GC'd.
    public static void destructor_really_frees()
    {
       bool delEventCalled = false;
       {
           Dummy.ITest_Object obj = new Dummy.Test_Object();
           obj.DEL += (object sender, EventArgs e) => { delEventCalled = true; };
       }

       System.GC.WaitForPendingFinalizers();
       System.GC.Collect();
       System.GC.WaitForPendingFinalizers();
       System.GC.Collect();
       System.GC.WaitForPendingFinalizers();

       Test.Assert(delEventCalled, "DEL event not called");
    } */

    public static void dispose_really_frees()
    {
       bool delEventCalled = false;
       {
           Dummy.ITest_Object obj = new Dummy.Test_Object();
           obj.DelEvt += (object sender, EventArgs e) => { delEventCalled = true; };
           ((IDisposable)obj).Dispose();
       }

       Test.Assert(delEventCalled, "DEL event not called");
    }

    /* Commented out as adding the event listener seems to prevent it from being GC'd.
    public static void derived_destructor_really_frees()
    {
       bool delEventCalled = false;
       {
           Dummy.ITest_Object obj = new Derived();
           obj.DEL += (object sender, EventArgs e) => { delEventCalled = true; };
       }

       System.GC.WaitForPendingFinalizers();
       System.GC.Collect();
       System.GC.WaitForPendingFinalizers();
       System.GC.Collect();
       System.GC.WaitForPendingFinalizers();

       Test.Assert(delEventCalled, "DEL event not called");
    }

    public static void derived_dispose_really_frees()
    {
       bool delEventCalled = false;
       {
           Dummy.ITest_Object obj = new Derived();
           obj.DEL += (object sender, EventArgs e) => { delEventCalled = true; };
           ((IDisposable)obj).Dispose();
       }

       Test.Assert(delEventCalled, "DEL event not called");
    }
    */
}

class MyLoop : Efl.LoopInherit
{
    public MyLoop() : base(null) { }
}

class TestEoInherit
{
    public static void instantiate_inherited()
    {
        Efl.ILoop loop = new MyLoop();
        Test.Assert(loop.raw_handle != System.IntPtr.Zero);
    }
}

class TestEoNames
{
    public static void name_getset()
    {
        Dummy.ITest_Object obj = new Dummy.Test_Object();

        string name = "Dummy";
        obj.SetName(name);
        Test.AssertEquals(name, obj.GetName());
    }
}

class TestEoConstructingMethods
{
    public static void constructing_method()
    {
        bool called = false;
        string name = "Test object";
        Dummy.ITest_Object obj = new Dummy.Test_Object(null, (Dummy.ITest_Object a) => {
                called = true;
                Console.WriteLine("callback: obj raw_handle: {0:x}", a.raw_handle);
                a.SetName(name);
            });

        Test.Assert(called);
        Test.AssertEquals(name, obj.GetName());
    }

    private class Derived : Dummy.Test_ObjectInherit
    {
        public Derived(Dummy.ITest_Object parent = null,
                       Dummy.Test_ObjectInherit.ConstructingMethod cb = null) : base(parent, cb) {
        }
    }

    public static void constructing_method_inherit()
    {
        bool called = false;
        string name = "Another test object";
        Derived obj = new Derived(null, (Dummy.ITest_Object a) => {
                called = true;
                a.SetComment(name);
            });

        Test.Assert(called);
        Test.AssertEquals(name, obj.GetComment());
    }
}

class TestEoParent
{
    public static void basic_parent()
    {
        Dummy.ITest_Object parent = new Dummy.Test_Object(null);
        Dummy.ITest_Object child = new Dummy.Test_Object(parent);

        Test.AssertEquals(parent, child.GetParent());

        Dummy.ITest_Object parent_retrieved = Dummy.Test_Object.static_cast(child.GetParent());
        Test.AssertEquals(parent, parent_retrieved);
    }

    public static void parent_inherited_class()
    {
        Dummy.INumberwrapper parent = new Dummy.Numberwrapper(null);
        Dummy.ITest_Object child = new Dummy.Test_Object(parent);

        Test.AssertEquals(parent, child.GetParent());

        Dummy.INumberwrapper parent_retrieved = Dummy.Numberwrapper.static_cast(child.GetParent());
        Test.AssertEquals(parent, parent_retrieved);
    }

    private class Derived : Dummy.Test_ObjectInherit
    {
        public Derived(Dummy.ITest_Object parent = null) : base (parent)
        {
        }
    }

    public static void basic_parent_managed_inherit()
    {
        Dummy.ITest_Object parent = new Derived(null);
        Dummy.ITest_Object child = new Derived(parent);

        Test.AssertEquals(parent, child.GetParent());

        Dummy.ITest_Object parent_from_cast = Dummy.Test_Object.static_cast(child.GetParent());
        Test.AssertEquals(parent, parent_from_cast);
    }
}

class TestKlassMethods
{
    public static void basic_class_method()
    {
        int reference = 0xbeef;
        Dummy.Test_Object.SetKlassProp(reference);
        Test.AssertEquals(reference, Dummy.Test_Object.GetKlassProp());
    }

    public static void inherited_class_method()
    {
        int reference = 0xdead;
        Dummy.Child.SetKlassProp(reference);
        Test.AssertEquals(reference, Dummy.Child.GetKlassProp());
    }
}

class TestTypedefs
{
    public static void basic_typedef_test()
    {
        Dummy.ITest_Object obj = new Dummy.Test_Object();
        Dummy.MyInt input = 1900;
        Dummy.MyInt receiver;

        int ret = obj.BypassTypedef(input, out receiver);

        Test.AssertEquals((Dummy.MyInt)ret, input);
        Test.AssertEquals(receiver, input);

    }
}

class TestEoAccessors
{
    public static void basic_eo_accessors()
    {
        Dummy.ITest_Object obj = new Dummy.Test_Object();
        Eina.List<int> lst = new Eina.List<int>();
        lst.Append(4);
        lst.Append(3);
        lst.Append(2);
        lst.Append(5);
        Eina.Accessor<int> acc = obj.CloneAccessor(lst.GetAccessor());

        var zipped = acc.Zip(lst, (first, second) => new Tuple<int, int>(first, second));

        foreach(Tuple<int, int> pair in zipped)
        {
            Test.AssertEquals(pair.Item1, pair.Item2);
        }
    }
}

}
