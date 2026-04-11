float foo(int x, bool y, float z)
{
    return 10.;
}

void bar(int x, bool y, float z)
{
}

void main()
{
    float f;

    f = foo(1 + 2, 1 < 2, 10. + 20.);
    // bar(1 + 2, 1 < 2, 10. + 20.);
}
