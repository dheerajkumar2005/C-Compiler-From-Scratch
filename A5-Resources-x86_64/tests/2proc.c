int zoo()
{
    int a, b, c;

    a = b > c ? b + c : b - c;

    if (b == c)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int main()
{
    int x, y, z;

    x = y > z ? y + z : y - z;

    if (y == z)
    {
        return x;
    }
    else
    {
        return y;
    }
}
