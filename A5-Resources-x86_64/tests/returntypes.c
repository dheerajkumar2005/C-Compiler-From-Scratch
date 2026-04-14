int foo()
{
    if (1 < 2)
    {
        return 10.;
    }
}

void main()
{
    int a;
    {
        a = foo();
    }
    a = foo();
}
