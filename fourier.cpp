// Markus Buchholz, 2023

// g++ fourier.cpp -o t -I/usr/include/python3.8 -lpython3.8

#include <iostream>
#include <tuple>
#include <vector>
#include <math.h>
#include <cmath>
#include <Eigen/Dense>

#include "matplotlibcpp.h"

namespace plt = matplotlibcpp;

//----------- system dynamic parameters --------------------

float dt = M_PI / 360.0;

float L = 2 * M_PI; // period
//---------------------------------------------------------------

float fourier(float t, int n)
{

    auto f = [=](float t)
    {
        return (4.0 / (n * M_PI)) * std::sin((n * M_PI * t / L));
    };

    return f(t);
}
//---------------------------------------------------------------

void plot2D(std::vector<std::vector<float>> time, std::vector<std::vector<float>> sig)
{

    plt::title("Fourier series ");
    plt::named_plot("n=3", time[0], sig[0]);
    plt::named_plot("n=11", time[0], sig[1]);
    plt::named_plot("n=21", time[0], sig[2]);
    plt::xlabel("pos X");
    plt::ylabel("pos Y");
    plt::legend();
    plt::xlabel("pos X");
    plt::ylabel("pos Y");
    plt::show();
}

//---------------------------------------------------------------

std::tuple<std::vector<std::vector<float>>, std::vector<std::vector<float>>> simulation()
{
    std::vector<std::vector<float>> time;
    std::vector<std::vector<float>> sig;
    std::vector<std::vector<int>> N = {{1, 3, 5}, {1, 3, 5, 7, 9, 11}, {1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21}};

    for (auto &nx : N)
    {
        std::vector<float> sig_nx;
        std::vector<float> time_nx;

        for (float t = 0.0; t < 2 * L; t = t + dt)
        {

            float sig_i = 0.0;
            for (auto &n : nx)
            {

                sig_i += fourier(t, n);
            }

            sig_nx.push_back(sig_i);
            time_nx.push_back(t);
        }
        sig.push_back(sig_nx);
        time.push_back(time_nx);
    }
    return std::make_tuple(time, sig);
}

int main()
{

    auto sim = simulation();

    plot2D(std::get<0>(sim), std::get<1>(sim));
}
