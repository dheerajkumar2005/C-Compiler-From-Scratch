bool bb;

float foo(int x, bool y, float z)
{

    // return 10.;
    // return y ? 10. : 20.;
    // return bb ? 10. : z * 4.;
    // return (bb && (x > 1)) ? 10. : z * 4.;
    return (y && bb) || (x > 1 && z < 10.) ? 10. : z * 4.;
}

int foobar(int x, bool y, float z)
{
    return 69;
}

void bar(int x, bool y, float z)
{
    int a;
    a = 10;
}

void main()
{
    float f;
    int x;

    f = foo(1, 1 < 2, 20.);
    f = foo(1 + x, x < 2, f + 20.24985);
    bar(1 + x, x < 2, f + 20.24985);
    x = foobar(1 + x, x < 2, f + 20.24985);
}
