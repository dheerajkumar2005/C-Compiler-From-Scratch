void f(int a, int b)
{
    int c;
    c = 5;

    print a + b + c;
}

int g(int x, int y)
{
    int z;
    z = x + y;
    return z;
}

void main()
{
    // f(g(10, 20), 10);
    g(10, 20);
}
