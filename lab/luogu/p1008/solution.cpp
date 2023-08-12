#include <iostream>

using namespace std;

int main() {
    int i, j, k, q, w, e, r, t, y, u, p, o, a[9], m, n, v = 0;

    for (i = 1; i <= 3; i++) {
        for (j = 1; j <= 9; j++) {
            for (k = 1; k <= 9; k++) {
                q = i * 100 + j * 10 + k;

                w = q * 2;

                e = q * 3;

                r = w / 100;
                t = (w % 100) / 10;
                y = w % 10;

                u = e / 100;
                p = (e % 100) / 10;
                o = e % 10;

                a[0] = i;
                a[1] = j;
                a[2] = k;
                a[3] = r;
                a[4] = t;
                a[5] = y;
                a[6] = u;
                a[7] = p;
                a[8] = o;

                v = 0;
                for (m = 0; m <= 8; m++) {
                    for (n = m + 1; n <= 8; n++) {
                        if (a[m] == a[n]) {
                            v = 1;
                        }
                    }
                }

                if (v != 1 && e <= 999 && t != 0 && y != 0 && p != 0 &&
                    o != 0) {
                    cout << q << " " << w << " " << e << endl;
                }
            }
        }
    }

    return 0;
}
