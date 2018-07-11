using System;

namespace TestSuite
{

class TestEinaError
{
    public static void basic_test()
    {
        Eina.Error.Clear();
        Test.AssertNotRaises<Efl.EflException>(Eina.Error.RaiseIfOccurred);
        Eina.Error.Set(Eina.Error.ENOENT);
        Test.AssertRaises<Efl.EflException>(Eina.Error.RaiseIfOccurred);
    }
}

class TestEolianError
{

    public static void global_eina_error()
    {
        Dummy.ITest_Object obj = new Dummy.Test_Object();
        Test.AssertRaises<Efl.EflException>(() => obj.RaisesEinaError());
    }

    class Child : Dummy.Test_ObjectInherit {
    }

    public static void global_eina_error_inherited()
    {
        Dummy.ITest_Object obj = new Child();
        Test.AssertRaises<Efl.EflException>(() => obj.RaisesEinaError());
    }

    class CustomException : Exception {
        public CustomException(string msg): base(msg) {}
    }

    class Overrider : Dummy.Test_ObjectInherit {
        public override void ChildrenRaiseError() {
            throw (new CustomException("Children error"));
        }
    }

    public static void exception_raised_from_inherited_virtual()
    {
        Dummy.ITest_Object obj = new Overrider();

        Test.AssertRaises<Efl.EflException>(obj.CallChildrenRaiseError);
    }

    // return eina_error
    public static void eina_error_return()
    {
        Dummy.ITest_Object obj = new Dummy.Test_Object();
        Eina.Error expected = 42;
        obj.SetErrorRet(expected);
        Eina.Error error = obj.ReturnsError();

        Test.AssertEquals(expected, error);

        expected = 0;
        obj.SetErrorRet(expected);
        error = obj.ReturnsError();

        Test.AssertEquals(expected, error);
    }

    class ReturnOverride : Dummy.Test_ObjectInherit {
        Eina.Error code;
        public override void SetErrorRet(Eina.Error err) {
            code = 2 * err;
        }
        public override Eina.Error ReturnsError()
        {
            return code;
        }
    }

    public static void eina_error_return_from_inherited_virtual()
    {
        Dummy.ITest_Object obj = new ReturnOverride();
        Eina.Error expected = 42;
        obj.SetErrorRet(expected);
        Eina.Error error = obj.ReturnsError();

        Test.AssertEquals(new Eina.Error(expected * 2), error);

        expected = 0;
        obj.SetErrorRet(expected);
        error = obj.ReturnsError();

        Test.AssertEquals(new Eina.Error(expected * 2), error);
    }

    // events
    class Listener
    {
        public bool called = false;
        public void callback(object sender, EventArgs e) {
            throw (new CustomException("Event exception"));
        }
        public void another_callback(object sender, EventArgs e) {}
    }

    public static void eina_error_event_raise_exception()
    {
        // An event whose managed delegate generates an exception
        // must set an eina_error so it can be reported back to
        // the managed code
        Dummy.ITest_Object obj = new Dummy.Test_Object();
        Listener listener = new Listener();
        obj.EvtWithIntEvt += listener.callback;

        Test.AssertRaises<Efl.EflException>(() => { obj.EmitEventWithInt(2); });
    }
}
}
