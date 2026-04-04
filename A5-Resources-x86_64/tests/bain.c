// Nested ternary operator with logical expressions
int value;

void main(){
    int a, b, c, y, z;
    bool ba, bb, bc;
    float fa, fb, fc;

    a = b + c;
    a = b - c;
    a = b * c;
    a = b / c;

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

    ba = bb && bc;
    ba = bb || bc;
    ba = !bb;
}