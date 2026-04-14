bool bb;

float foo()
{

    return 10.;
    // return y ? 10. : 20.;
    // return bb ? 10. : z * 4.;
    // return (bb && (x > 1)) ? 10. : z * 4.;
    // return (y && bb) || (x > 1 && z < 10.) ? 10. : z * 4.;
}

void main()
{
    float f;
    int x;

    f = foo();
}
