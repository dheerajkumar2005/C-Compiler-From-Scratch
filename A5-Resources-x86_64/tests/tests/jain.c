// Nested ternary operator with logical expressions
int value;

void main()
{
    // IMPORTANT: Prints the stuff in v0
    
    int x, y, z;
    int a, b, c, d;
    bool b1, b2, b3;
    float p, q, r;
    
    print - (x * (b1 ? b : c));

    z = x + y;
    z = x - y;
    z = x * y;
    z = x / y;

    r = p + q;
    r = p - q;
    r = p * q;
    r = p / q;

    b3 = !b1;
    b3 = b1 && b2;
    b3 = b1 || b2;

    b1 = x < y;
    b1 = x <= y;
    b1 = x > y;
    b1 = x >= y;
    b1 = x == y;
    b1 = x != y;

    x = (a + b) * (c + d);

    b1 = p < q;
    b1 = p <= q;
    b1 = p > q;
    b1 = p >= q;
    b1 = p != q;
    b1 = p == q;
}