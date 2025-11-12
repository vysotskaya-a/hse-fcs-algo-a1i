// montecarlo_experiment.cpp
#include <bits/stdc++.h>
using namespace std;

struct Circle { double x, y, r; };

bool inside_all(const vector<Circle>& C, double x, double y) {
    for (const auto &c : C) {
        double dx = x - c.x;
        double dy = y - c.y;
        if (dx*dx + dy*dy > c.r*c.r) return false;
    }
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Данные окружностей (по условию)
    vector<Circle> C = {
        {1.0, 1.0, 1.0},
        {1.5, 2.0, sqrt(5.0)/2.0},
        {2.0, 1.5, sqrt(5.0)/2.0}
    };

    // Точное значение площади
    const double S_exact = 0.25 * M_PI + 1.25 * asin(0.8) - 1.0;

    // Узкий прямоугольник (пересечение bounding boxes)
    double x_left_n = max({C[0].x - C[0].r, C[1].x - C[1].r, C[2].x - C[2].r});
    double x_right_n = min({C[0].x + C[0].r, C[1].x + C[1].r, C[2].x + C[2].r});
    double y_bottom_n = max({C[0].y - C[0].r, C[1].y - C[1].r, C[2].y - C[2].r});
    double y_top_n = min({C[0].y + C[0].r, C[1].y + C[1].r, C[2].y + C[2].r});

    // Широкий прямоугольник (охватывает все круги)
    double x_left_w = min({C[0].x - C[0].r, C[1].x - C[1].r, C[2].x - C[2].r});
    double x_right_w = max({C[0].x + C[0].r, C[1].x + C[1].r, C[2].x + C[2].r});
    double y_bottom_w = min({C[0].y - C[0].r, C[1].y - C[1].r, C[2].y - C[2].r});
    double y_top_w = max({C[0].y + C[0].r, C[1].y + C[1].r, C[2].y + C[2].r});

    // Файл с результатами
    ofstream fout("results.csv");
    fout << "rect_type,N,trial,estimate\n";

    // Фиксированный seed для воспроизводимости
    mt19937_64 gen(123456789);

    const int R = 30;   // число повторов
    const int N_min = 100;
    const int N_max = 100000;
    const int step = 500;

    for (int N = N_min; N <= N_max; N += step) {
        for (int rect_type = 0; rect_type < 2; ++rect_type) {
            double xl = (rect_type == 0 ? x_left_w : x_left_n);
            double xr = (rect_type == 0 ? x_right_w : x_right_n);
            double yb = (rect_type == 0 ? y_bottom_w : y_bottom_n);
            double yt = (rect_type == 0 ? y_top_w : y_top_n);
            double Srec = (xr - xl) * (yt - yb);

            uniform_real_distribution<double> ux(xl, xr);
            uniform_real_distribution<double> uy(yb, yt);

            for (int trial = 0; trial < R; ++trial) {
                int count_inside = 0;
                for (int i = 0; i < N; ++i) {
                    double x = ux(gen);
                    double y = uy(gen);
                    if (inside_all(C, x, y))
                        count_inside++;
                }
                double S_est = Srec * double(count_inside) / double(N);
                fout << (rect_type == 0 ? "wide" : "narrow") << "," << N << "," << trial << "," << S_est << "\n";
            }
        }
    }

    fout.close();

    cerr << "Эксперимент завершен.\n";
    cerr << "Результаты записаны в results.csv\n";
    cerr << "Точная площадь: " << S_exact << "\n";
}
