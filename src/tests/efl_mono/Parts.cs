#define CODE_ANALYSIS

#pragma warning disable 1591

using System;
using System.Diagnostics.CodeAnalysis;

namespace TestSuite {


[SuppressMessage("Gendarme.Rules.Portability", "DoNotHardcodePathsRule")]
public static class TestParts
{
    public static void basic_part_test()
    {
        Dummy.ITest_Object t = new Dummy.Test_Object();
        do_part_test(t);
    }

    private class Child : Dummy.Test_ObjectInherit
    {
        public Child() : base(null) {}
    }

    public static void inherited_part_test() {
        var t = new Child();
        do_part_test(t);
    }

    private static void do_part_test(Dummy.ITest_Object t)
    {
        var p1 = t.Part1;
        var p2 = t.Part2;
        Test.Assert(p1 is Dummy.Test_Object);
        Test.AssertEquals("part1", p1.GetName());
        Test.Assert(p2 is Dummy.Test_Object);
        Test.AssertEquals("part2", p2.GetName());
    }
}

}
