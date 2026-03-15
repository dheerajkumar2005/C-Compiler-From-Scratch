void main() {
    int a, b, c;
    float x, y;
    bool r;
    
    read x;
    y = -x;

    b = 5 * (a / 2) - 30;
    c = a > b ? a - b : a + b;
    print c * c + b;

    r = a < b || a != b;
    r = a > b && a == b;
}