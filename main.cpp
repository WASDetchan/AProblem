#include <bits/stdc++.h>

using namespace std;

enum side{
    CUBE,
    CUP,
    UNKNOWN
};

//#define P1 //0.515748
#define P2 //0.49965

#ifdef P1
const long double L = 23, //мм
m = 5.63, //г
v = 0.88e-3, // мм/мкс
H = -50, //мм
g = 10 * 1e3 / 1e12; //мм/мкс/мкс
#endif
#ifdef P2
const long double L = 23, //мм
m = 5.63, //г
v = 4.5e-3, // мм/мкс
H = -1000, //мм
g = 10 * 1e3 / 1e12; //мм/мкс/мкс
#endif


long int stepNum = 100000;
long double stepL = L / 2 / stepNum;

class Throw{
public:
    explicit Throw(long double _l){
        l = _l;
        omega = omegaK * l;
    }
    side calcSide(){
        while(S == UNKNOWN){
            step();
        }
        return S;
    }
private:
    const long double omegaK = 12 * v / L / L;

    const long double stepT = 1; // мкс

    const long double calcH = H + 2 * L;
    const long double calcT = (v + sqrt(v * v - 2 * g * calcH)) / g;

    long double time = calcT;
    long double angle = 0;

    long double l;
    long double omega;
    long double cy = 0;
    long double ly = 0;
    long double ry = 0;

    side S = UNKNOWN;

    void step(){
        time += stepT;
        calcAngle();
        calcY();
        checkLand();
    }

    void checkLand() {
        if(ly < H || ry < H){
            if(angle > M_PI / 2 && angle < M_PI * 3 / 2){
                S = CUP;
            }
            else{
                S = CUBE;
            }
        }
    }

    void calcAngle(){
        angle = omega * time;
        while(angle > 2 * M_PI) angle -= 2 * M_PI;
    }
    void calcCenterY(){
        cy = v * time - g * time * time / 2;
    }
    void calcLeftY(){
        ly = cy - L * sinl(angle) / 2;
    }
    void calcRightY(){
        ry = cy + L * sinl(angle) / 2;
    }
    void calcY(){
        calcCenterY();
        calcLeftY();
        calcRightY();
    }
};


void checkSide(long i, long double l, long &c, long &cm);

int main()
{
    long int c = 0, cm = 0;
    for(long int i = 0; i <= stepNum; i++){
        if(!(i % 1000)) cout << i / 100 << endl;
        long double l = i * stepL;
        if(l > L) {
            break;
        }
        checkSide(i, l, c, cm);
    }

    cout << endl << static_cast<long double>(c) / cm;

    return 0;
}

void checkSide(long i, long double l, long &c, long &cm) {
    cm = i + 1;
    auto t = Throw(l);
    side S = t.calcSide();
    c += S;
}
