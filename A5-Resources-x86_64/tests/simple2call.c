bool bb;
string spl;

void bar(int x, float y, string s2)
{
}

void main()
{
    int x;
    float f;
    // bar("djo");
    bar(bb ? 1 + 2 : x, bb ? 10. * 20. : f, bb ? spl : "djo");
}
