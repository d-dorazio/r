#include <bits/stdc++.h>

#define O operator
#define R return

using I = int;
using D = double;

auto Mi = fminf, Ma = fmaxf;
auto Ab = fabsf, Sq = sqrtf;

struct V {
    D x,y,z;

    V(D v = 0) {
        x = y = z = v;
    }

    V(D a, D b, D c) {
        x = a;
        y = b;
        z = c;
    }

    D L() {
        R Sq(x*x + y*y + z*z);
    }

    V O+(V o) {
        R {x+o.x, y+o.y, z+o.z};
    }

    V O-(V o) {
        R {x-o.x, y-o.y, z-o.z};
    }

    V O/(V o) {
        R {x/o.x, y/o.y, z/o.z};
    }

    V O*(D s) {
        R {x*s, y*s, z*s};
    }

    V O*(V o) {
        R {y*o.z - z*o.y, z*o.x - x*o.z, x*o.y - y*o.x};
    }

    D dot(V o) {
        R x*o.x + y*o.y + z*o.z;
    }

    V O!() {
        R *this / V(L());
    }

};


D C(D v, D l, D h) { R Mi(h, Ma(l, v)); }   // clamp
D X(D l, D h, D t) { R (1-t)*l + t*h; }     // mix

D R1() {
    R D(rand()) / RAND_MAX;
}

D uni(D d, D b, D k) {
    D h = C(.5 + .5*(b-d)/k, 0, 1);
    R X(b, d, h) - k*h*(1-h);
}

D dif(D b, D d, D k) {
    D h = C(.5 - .5*(b+d)/k, 0, 1);
    R X(b, -d, h) + k*h*(1-h);
}

D sdbox(V p, V b) {
    R V(Ma(Ab(p.x)-b.x, 0),
        Ma(Ab(p.y)-b.y, 0),
        Ma(Ab(p.z)-b.z, 0)).L();
}

D S(V p, V& c) {
    D td = Mi(uni(dif(p.L() - .45, V(p.x,p.y,0).L() - .2, .1),    // sphere - segment running through Z
                  sdbox(p-V(.33, -.3, .3), V(.05,.5,.05)) - .1,
                  .15), // D
              Mi(sdbox(p-V(.9, -.7, .25), V(.05,.1,.05)) - .1,           // rounded box
                 V(p.x-.9, p.y+.3 - C(p.y+.3, 0, .6), p.z-.25).L() - .15   // rounded segment
                ) // ;
      ); // text distance

    p.y -= .7;
    D pd = p.dot({0, -1, 0}); // floor distance

    if (td < pd) {
        c = {.13, .54, .13};
        R td;
    }

    auto f = [](D v) { R Ab(fmod(v,1)) > .5; };
    c = (f(p.x) ^ f(p.z)) ? V(.82,.15,.15) : V(.96,.63,.1);

    R pd;
}

V M(V e, V r, I b, I& h) {
    V c, cc, n, l1 = !V(0,-1,.3);

    for (I i=0, hh=0; i<99; ++i,h=1) {
        D d = S(e, c), E=.01;

        if (d < E) {
            n = !V(S(e + V(E,0,0), cc) - d,
                   S(e + V(0,E,0), cc) - d,
                   S(e + V(0,0,E), cc) - d);

            if (b)
                M(e+n*E, n, 0, hh);

            R hh ? V() : c*n.dot(l1);
        }
        e = e + r*d;
    }

    h = 0;
    R {.92,.95,.95};
}

I main() {
    I w=512,
      h=w,
      S=9;    // samples

    V T(.4,0,0),       // look at
      E(-.5,0,1.3),    // eye
      Z = !(T - E),
      X = !(Z * V(0,1,0)),
      Y = !(Z * X);

    auto d = new char[w*h*3];

    for (I y=0,x,s,t; y<h; ++y) {
        for (x=0; x<w; ++x) {

            V c;
            for (s=0; s<S; ++s) {
                D xt = (x+R1())/w * 2 - 1,
                  yt = (h-y+R1())/h * 2 - 1;

                c = c + M(E, !(X*xt + Y*yt + Z), 1, t);
            }

            auto p = d + y*w*3 + x*3;
            p[0] = Sq(c.x/S)*255;
            p[1] = Sq(c.y/S)*255;
            p[2] = Sq(c.z/S)*255;
        }
    }

    printf("P6 %d %d 255\n", w, h);
    fwrite(d, 1, w*h*3, stdout);

    delete[] d;
    R 0;
}
