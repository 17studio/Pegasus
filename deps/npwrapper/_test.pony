use "ponytest"

actor Main is TestList
  new create(env: Env) =>
    PonyTest(env, this)

  new make() =>
    None

  fun tag tests(test: PonyTest) =>
    test(_TestAdd)
    test(_TestSub)

class _TestAdd is UnitTest
  fun name(): String => "addition"

  fun apply(h: TestHelper) =>
    h.assert_eq[U32](4, 1+3)

class _TestSub is UnitTest
  fun name(): String => "subtraction"

  fun apply(h: TestHelper) =>
    h.assert_eq[U32](2, 4-2)

class iso _TestPyArrayObject is UnitTest
  fun name(): String => "pyarrobject"

  fun apply(h: TestHelper) =>
    None