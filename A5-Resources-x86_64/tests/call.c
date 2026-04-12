float foo(int x, bool y, float z)
{
    return 10.;
}

int foobar(int x, bool y, float z)
{
    return 69;
}

void bar(int x, bool y, float z)
{
}

void main()
{
    float f;
    int x;

    f = foo(1 + 2, 1 < 2, 10. + 20.);
    bar(1 + 2, 1 < 2, 10. + 20.);
    x = foobar(1 + 2, 1 < 2, 10. + 20.);
}
