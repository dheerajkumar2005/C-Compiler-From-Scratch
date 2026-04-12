// Nested ternary operator with logical expressions
int value;

void main(){
    int a, b, c, y, z;
    bool ba, bb, bc;
    float fa, fb, fc;
    a = -a;
    a = b + c;
    a = b - c;
    a = b * c;
    a = b / c;

    fa = -fa;
    fa = fb + fc;
    fa = fb - fc;
    fa = fb * fc;
    fa = fb / fc;

    a = 10 + c;
    fa = 10.0 + fc;

    bb = y < z;
    bb = y <= z;
    bb = y > z;
    bb = y >= z;
    bb = y == z;
    bb = y != z;

    bb = fa < fb;
    bb = fa > fb;
    bb = fa <= fb;
    bb = fa >= fb;
    bb = fa == fb;
    bb = fa != fb;

    ba = bb && bc;
    ba = bb || bc;
    ba = !bb;
}