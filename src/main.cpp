#include<simplecpp>
#include<vector>
#include "utils.cpp"

//******high processing demands
//******can do better by less checking every time
//******and more intelligent checking near the mirror
//******reflection should be more at the mirror
//******limit on noSquares
//******sometimes dosent start
// full game with little optimization + spherical mirrors + cubical mirrors
//******doesn't handle reflection on both at same time ie corners


double vectorangle(double x1, double y1, double x2, double y2) {
    double cosineofangle, angle;
    if ((x2 - x1) != 0) {
        cosineofangle = ((x2 - x1) / sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
        angle = arccosine(cosineofangle);
        if (y2 - y1 >= 0)return angle;
        if (y2 - y1 < 0)return -angle;
    } else {
        if (y2 - y1 > 0) return 90;
        if (y2 - y1 < 0) return -90;
    }
    return 0;
}

//(the order of input matters) OUTPUT -pi to pi
double lineangle(double x1, double y1, double x2, double y2) {
    if ((x2 - x1) == 0)return 90;

    double tangentofangle, angle;
    tangentofangle = ((y2 - y1) / (x2 - x1));
    angle = arctangent(tangentofangle);
    return angle;


}
//OUTPUT -pi/2 to pi/2

class LightPlay {
    int n; //no of vertices including the initial point
    double **c; //coordinate of leading point
    vector<Line> lightRays;

    double dx, dy;//translators

    int noLines;
    Line *lineMirrors;
    double M[200][2][2]; //mirrors end points data******100 at max

    int noCircles;
    Circle *circleMirrors;

    double S[100][2];
    int noSquares;

    Circle target;
    double tarc[2];

    int pass;
    double scale;

    void assigntoheap() {

        c = new double *[n];
        for (int i = 0; i < n; i++) {
            c[i] = new double[2];
        }
        return;
    }

    void removefromheap() {
        for (int i = 0; i < n; i++) { delete[]c[i]; }
        delete[]c;
        return;
    }

    void resetthesysduringcollision() {
        double swc[n][2];
        for (int i = 0; i < n; i++) {
            swc[i][0] = c[i][0];
            swc[i][1] = c[i][1];
        }
        //coordiantes copied
        removefromheap();
        n = n + 1;
        assigntoheap();
        for (int i = 0; i < n - 1; i++) {
            c[i][0] = swc[i][0];
            c[i][1] = swc[i][1];
        }
        c[n - 1][0] = c[n - 2][0];
        c[n - 1][1] = c[n - 2][1];

        return;
    }

    void ontarget() {
        Line lin(c[n - 2][0], c[n - 2][1], c[n - 1][0], c[n - 1][1]);
        lin.setColor(COLOR(255, 255, 255));
        lightRays.push_back(lin);
        target.setColor(COLOR(153, 0, 0));
        wait(2);
    }

    void renderCircles() {
        circleMirrors = new Circle[noCircles];
        int p;
        double x1, y1;
        for (int i = 0; i < noCircles; i++) {
            p = getClick();
            S[i][0] = x1 = p / 65536;
            S[i][1] = y1 = p % 65536;
            {
                Circle source(x1, y1, 75);
                circleMirrors[i] = source;
                circleMirrors[i].setColor(COLOR(160, 160, 160));
                circleMirrors[i].setFill(1);
            }
        }
    }

    void renderSquares() {
        int p;
        int corr = 0;
        double x1, y1;
        for (int i = 0; i < noSquares; i++) {

            p = getClick();
            x1 = p / 65536;
            y1 = p % 65536;

            {
                Rectangle source(x1, y1, 50, 50);
                source.setColor(COLOR(160, 160, 160));
                source.imprint();
            }
            M[noLines + i + corr][0][0] = x1 + 25;
            M[noLines + i + corr][0][1] = y1 + 25;
            M[noLines + i + corr][1][0] = x1 - 25;
            M[noLines + i + corr][1][1] = y1 + 25;

            M[noLines + i + 1 + corr][0][0] = x1 - 25;
            M[noLines + i + 1 + corr][0][1] = y1 + 25;
            M[noLines + i + 1 + corr][1][0] = x1 - 25;
            M[noLines + i + 1 + corr][1][1] = y1 - 25;

            M[noLines + i + 2 + corr][0][0] = x1 - 25;
            M[noLines + i + 2 + corr][0][1] = y1 - 25;
            M[noLines + i + 2 + corr][1][0] = x1 + 25;
            M[noLines + i + 2 + corr][1][1] = y1 - 25;

            M[noLines + i + 3 + corr][0][0] = x1 + 25;
            M[noLines + i + 3 + corr][0][1] = y1 - 25;
            M[noLines + i + 3 + corr][1][0] = x1 + 25;
            M[noLines + i + 3 + corr][1][1] = y1 + 25;
            corr = corr + 3;
        }

        p = getClick();
        tarc[0] = x1 = p / 65536;
        tarc[1] = y1 = p % 65536;
        {
            Circle c(x1, y1, 50);
            target = c;
            target.setColor(COLOR(0, 204, 0));
            target.setFill(1);
        }
    }

    void renderLines() {
        lineMirrors = new Line[noLines];
        int p;
        double x1, y1, x2, y2;
        for (int i = 0; i < noLines; i++) {
            p = getClick();
            M[i][0][0] = x1 = p / 65536;
            M[i][0][1] = y1 = p % 65536;

            p = getClick();
            M[i][1][0] = x2 = p / 65536;
            M[i][1][1] = y2 = p % 65536;

            {
                Line source(x1, y1, x2, y2);
                lineMirrors[i] = source;
                lineMirrors[i].setColor(COLOR(160, 160, 160));
            }
        }
    }

    bool on_mirror() {
        for (int i = 0; i < noLines + noSquares * 16; i++) {
            double a, b, x1, y1, x2, y2;
            a = c[n - 1][0];
            b = c[n - 1][1];
            x1 = M[i][0][0];
            y1 = M[i][0][1];
            x2 = M[i][1][0];
            y2 = M[i][1][1];


            double lenght = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
            double a1 = sqrt((a - x1) * (a - x1) + (b - y1) * (b - y1)), b1 = sqrt(
                    (a - x2) * (a - x2) + (b - y2) * (b - y2));

            double sumOfparts = a1 + b1;

            if (abs(lenght - sumOfparts) <= 0.5) {
                pass = i;
                return true;
            }

            if (abs(lenght - sumOfparts) < scale) { scale = abs(lenght - sumOfparts); }
        }

        if (scale < 1) { scale = 1; }

        return false;
    }

    bool on_sphere() {
        double a, b, x1, y1;
        a = c[n - 1][0];
        b = c[n - 1][1];
        for (int i = 0; i < noCircles; i++) {
            x1 = S[i][0];
            y1 = S[i][1];

            double lenght = sqrt((x1 - a) * (x1 - a) + (y1 - b) * (y1 - b));
            if (lenght <= 75.5) {
                pass = i;
                return true;
            }

            if (lenght < scale) { scale = lenght; }
        }

        if (scale < 1) { scale = 1; }

        return false;
    }

    void collision(int m) {

        double o, a, b;
        a = vectorangle(c[n - 2][0], c[n - 2][1], c[n - 1][0], c[n - 1][1]);
        o = lineangle(M[m][0][0], M[m][0][1], M[m][1][0], M[m][1][1]);
        b = 2 * o - a;
        dx = cosine(b);
        dy = sine(b);
        resetthesysduringcollision();
        Line lin(c[n - 3][0], c[n - 3][1], c[n - 2][0], c[n - 2][1]);
        lin.setColor(COLOR(255, 255, 255));
        lightRays.push_back(lin);
        c[n - 1][0] = c[n - 1][0] + 2 * dx;
        c[n - 1][1] = c[n - 1][1] + 2 * dy;
        return;
    }

    void Scollision(int m) {
        Line lin(c[n - 2][0], c[n - 2][1], c[n - 1][0], c[n - 1][1]);
        lin.setColor(COLOR(255, 255, 255));
        lightRays.push_back(lin);
        double o1, o, a, b;
        a = vectorangle(c[n - 2][0], c[n - 2][1], c[n - 1][0], c[n - 1][1]);
        o1 = lineangle(c[n - 1][0], c[n - 1][1], S[m][0], S[m][1]);
        if (o1 > 0) { o = o1 - 90; }
        else { o = o1 + 90; }
        b = 2 * o - a;
        dx = cosine(b);
        dy = sine(b);
        resetthesysduringcollision();
        c[n - 1][0] = c[n - 1][0] + 2 * dx;
        c[n - 1][1] = c[n - 1][1] + 2 * dy;
        //Circle flag(c[n-1][0],c[n-1][1],10);
        return;

    }

public:

    LightPlay(int noLines, int noCircles, int noSquares) :
            noLines(noLines),
            noCircles(noCircles),
            noSquares(noSquares) {
        renderLines();
        renderCircles();
        renderSquares();
        Vector2d click;

        n = 2;
        assigntoheap();

        int p;

        p = getClick();
        double x1, y1;
        x1 = p / 65536;
        y1 = p % 65536;
        c[1][0] = c[0][0] = x1;
        c[1][1] = c[0][1] = y1;

        // Light source
        Circle refer(c[0][0], c[1][1], 75);
        refer.setColor(COLOR(242, 17, 17)).imprint();

        refer.reset(c[0][0], c[1][1], 50);
        refer.setColor(COLOR(242, 187, 17)).imprint();

        refer.reset(c[0][0], c[1][1], 25);
        refer.setColor(COLOR(242, 137, 17)).imprint();

        refer.reset(c[0][0], c[1][1], 2);
        refer.setColor(COLOR(24, 137, 17)).imprint();


        int checker = 0;
        scale = 1;
        while (true) {
            registerClick(&click);
            if (Vector2d(click.x - x1, click.y - y1).length() <= 75) { break; }
        }

        bool exit;
        while (true) {
            exit = false;

            double o;
            o = vectorangle(x1, y1, click.x, click.y);
            dx = cosine(o);
            dy = sine(o);

            //initiator launched


            while (true) {
                while (true) {
                    //cout<<"i";

                    c[n - 1][0] = c[n - 1][0] + dx * scale;
                    c[n - 1][1] = c[n - 1][1] + dy * scale;
                    checker++;


                    if (sqrt((c[n - 1][0] - tarc[0]) * (c[n - 1][0] - tarc[0]) +
                             (c[n - 1][1] - tarc[1]) * (c[n - 1][1] - tarc[1])) <= 50) {
                        ontarget();
                        exit = true;
                        cout << "PASSED" << endl;
                        break;
                    }
                    if (on_mirror()) { collision(pass); }
                    if (on_sphere()) { Scollision(pass); }
                    if (c[n - 1][0] >= 1450 || c[n - 1][0] <= 50 || c[n - 1][1] >= 650 || c[n - 1][1] <= 50) {
                        break;
                    }

                }
                if (exit) { break; }
                if (c[n - 1][0] >= 1450 || c[n - 1][0] <= 50 || c[n - 1][1] >= 650 || c[n - 1][1] <= 50) {
                    Line lin(c[n - 2][0], c[n - 2][1], c[n - 1][0], c[n - 1][1]);
                    lin.setColor(COLOR(255, 255, 255));
                    lightRays.push_back(lin);
                    break;
                }

            }


            //travels in way checks any reflection
            //distance calculating fn and hence reftector
            //resetting the system
            //storing new point data into the corres variables
            //end the reflections and create lines
            if (exit) { break; }
            while (true) {
                registerClick(&click);
                if (Vector2d(click.x - x1, click.y - y1).length() <= 75) { break; }
            }

            {//resetting the system
                //Line zero(0,0,0,0);
                //for(size_t i=0;i<lightRays.size();i++){lightRays[i]=zero;}
                lightRays.resize(0);
                removefromheap();
                n = 2;
                assigntoheap();
                c[1][0] = c[0][0] = x1;
                c[1][1] = c[0][1] = y1;
            }


        }
        //cout<<checker;
    }

    ~LightPlay() { removefromheap(); }

};


int main() {

    int noLines, noCircles, noSquares;
    cout << "Spheres-space-Mirrors-space-Cubes-enter   ex: 1 1 1 then press enter" << endl;
    cin >> noLines >> noCircles >> noSquares;

    initCanvas("light", 1500, 700);

    Rectangle border(750, 350, 1500, 700);
    border.setColor(COLOR(65, 65, 65)).setFill();
    border.imprint();
    border.reset(750, 350, 1400, 600);
    border.setColor(COLOR(0, 0, 0)).setFill();
    border.imprint();



//define a light class rotate ,reflect until last non reflecting surface.

    LightPlay p = LightPlay(noLines, noCircles, noSquares);

}
