bool bb;
string spl;

int foo()
{
    return 10;
}

void bar(int x, float y, float z, int w, string s1, string s2)
{
}

void barr(int x)
{
}

void main()
{
    int a, b;

    // a = foo();
    bar(10, 10., 10., 10, "sombr", "djo");
    // bar(10, 10., 10., 10, "sombr", bb ? spl : "djo");
    // bar(a, 10., 20., a + b, "sombr", 1 > 2 ? spl : "djo");
}
