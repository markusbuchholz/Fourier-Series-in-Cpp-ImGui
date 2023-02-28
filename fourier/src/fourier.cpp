// Markus Buchholz, 2023

#include <stdafx.hpp>
#include "imgui_helper.hpp"
#include "ball.h"
#include <tuple>
#include <thread>
#include <chrono>
#include <vector>
#include <math.h>
#include <list>
#include <numeric>

//---------------------------------------------------------------
std::vector<Point> points = {
	{{200, 400}, 10}};

//----------- system dynamic parameters --------------------

float dt = M_PI / 720.0;
float P = 0.25;
float scale = 50.0;

//---------------------------------------------------------------

int main(int argc, char const *argv[])
{

	int w = 1280;
	int h = 720;
	std::string title = "Fourier series";
	initImgui(w, h, title);

	// simulation state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(89.0f / 255.0, 88.0f / 255.0, 87.0f / 255.0, 1.00f);
	ImVec4 ray_color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 ray_color2 = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);


	// Main loop
	int ii = 0;
	bool flag = true;
	std::list<ImVec2> wave_tail;
	std::list<int> test;


	while (!glfwWindowShouldClose(window) && flag == true)
	{
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();

		static int num_sig = 3;

		ImGuiWindowFlags window_flags = 0;
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(w, h), ImGuiCond_FirstUseEver);
		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

		ImGui::Begin("Simulation", nullptr, window_flags);
		ImDrawList *draw_list = ImGui::GetWindowDrawList();

		float stroke = 150.0;
		std::vector<int> N = {1, 3, 5};
		float r1 = 4.0 / (N[0] * M_PI) * scale;
		float r2 = 4.0 / (N[1] * M_PI) * scale;
		float r3 = 4.0 / (N[2] * M_PI) * scale;
		float size_line = 3;

		std::vector<ImVec2> circles;
		std::vector<ImVec2> dots_circles;
		std::vector<float> vec_r;

		float x = (float)points[0].position.x;
		float y = (float)points[0].position.y;

		std::vector<float> vec_w;

		for (int i = 0; i < num_sig; i++)
		{

			float prev_x = x;
			float prev_y = y;
			circles.push_back({prev_x, prev_y});

			int n = i * 2 + 1;

			float r = 4.0 / (n * M_PI) * scale;
			vec_r.push_back(r);

			// circle dot
			x += (float)std::cos(ii * dt * n / P) * r;
			y += (float)std::sin(ii * dt * n / P) * r;
			vec_w.push_back((float)std::sin(ii * dt * n / P) * r);
			dots_circles.push_back({x, y});
		}

		for (int jj = 0; jj < circles.size(); jj++)
		{

			draw_list->AddLine(circles[jj], dots_circles[jj], ImColor(ray_color), 2.0);
			draw_list->AddCircle(circles[jj], vec_r[jj], ImColor(ray_color));
		}

		ImVec2 wave_N = {(float)points[0].position.x + ii + stroke, (float)points[0].position.y + std::accumulate(vec_w.begin(), vec_w.end(),0)};

		draw_list->AddLine(dots_circles[dots_circles.size() - 1], wave_N, ImColor(ray_color), 2.0);

		wave_tail.push_front(wave_N);

		for (auto &it : wave_tail)
		{
			draw_list->AddCircleFilled(it, 1.0, IM_COL32(255, 0, 255, 255));
		}
		ii++;
		if (ii > 800)
		{
			wave_tail.erase(wave_tail.begin(), wave_tail.end());
			ii = 0;
		}
		ImGui::PushItemWidth(200);
		ImGui::SliderInt("Number of periods", &num_sig, 1, 20);
		ImGui::PopItemWidth();
		ImGui::End();


		// Rendering
		ImGui::Render();
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
	}

	termImgui();
	return 0;
}
