void main()
{
    int a, b, c, d, e;

    a = 1 > 2 ? b : c;
}

void main2()
{
    int a, b, c, d, e;

    a = 1 > 2 ? (3 > 4 ? b : c) : d;
}

void main3()
{
    int a, b, c, d, e;

    a = (1 > 2 ? (3 > 4 ? b : c) : (5 > 6 ? d : e));
}