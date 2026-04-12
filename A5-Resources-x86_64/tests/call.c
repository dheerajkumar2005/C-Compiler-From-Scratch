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
    int a;
    a = 10;
}

void main()
{
    float f;
    int x;

    f = foo(1 + x, x < 2, f + 20.24985);
    bar(1 + x, x < 2, f + 20.24985);
    x = foobar(1 + x, x < 2, f + 20.24985);
}
