#define _USE_MATH_DEFINES
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <cmath>
#include <vector>
#include "Geometry.h"
#include "GLDebug.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Window.h"

using namespace std;

struct State {
	int SierpinskiIterations = 0;
	int squareIterations = 0;
	int snowIterations = 0;
	int dragonIterations = 0 ;
	int scene = 1;


	bool operator == (State const& other) const {
		return SierpinskiIterations == other.SierpinskiIterations && squareIterations == other.squareIterations
			&& snowIterations == other.snowIterations && dragonIterations == other.dragonIterations;
	}
};


// EXAMPLE CALLBACKS
class MyCallbacks : public CallbackInterface {

public:
	MyCallbacks(ShaderProgram& shader) : shader(shader) {}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS || action == GLFW_REPEAT) {
			if (key == GLFW_KEY_R && action == GLFW_PRESS) {
				shader.recompile();
			}

			//start sierpinski
			if (state.scene == 1) {
				if (key == GLFW_KEY_RIGHT) {
					if (state.SierpinskiIterations < 10) {
						state.SierpinskiIterations++;

					}
					else {
						std::cerr << "Error: Attempt to increment iterations beyond maximum: triangle" << std::endl;
					}

				}
				if (key == GLFW_KEY_LEFT) {
					if (state.SierpinskiIterations > 0) {
						state.SierpinskiIterations--;

					}
					else {
						std::cerr << "Error: Attempt to decrement iterations below 0" << std::endl;
					}
				}
			}
			else if (state.scene == 2) {
				if (key == GLFW_KEY_RIGHT) {
					if (state.squareIterations < 10) {
						state.squareIterations++;
					}
					else {
						std::cerr << "Error: Attempt to increment iterations beyond maximum" << std::endl;
					}
				}
				if (key == GLFW_KEY_LEFT) {
					if (state.squareIterations > 0) {
						state.squareIterations--;
					}
					else {
						std::cerr << "Error: Attempt to decrement iterations below 0" << std::endl;
					}
				}
			}
			else if (state.scene == 3) {
				if (key == GLFW_KEY_RIGHT) {
					if (state.snowIterations < 10) {
						state.snowIterations++;
					}
					else {
						std::cerr << "Error: Attempt to increment iterations beyond maximum" << std::endl;
					}
				}
				if (key == GLFW_KEY_LEFT) {
					if (state.snowIterations > 0) {
						state.snowIterations--;
					}
					else {
						std::cerr << "Error: Attempt to decrement iterations below 0" << std::endl;
					}
				}
			}
			else if (state.scene == 4) {
				if (key == GLFW_KEY_RIGHT) {
					if (state.dragonIterations < 10) {
						state.dragonIterations++;
					}
					else {
						std::cerr << "Error: Attempt to increment iterations beyond maximum" << std::endl;
					}
				}
				if (key == GLFW_KEY_LEFT) {
					if (state.dragonIterations > 0) {
						state.dragonIterations--;
					}
					else {
						std::cerr << "Error: Attempt to decrement iterations below 0" << std::endl;
					}
				}
			}

			if (key == GLFW_KEY_1) {
				state.scene = 1; // Switch to triangle
			}
			if (key == GLFW_KEY_2) {
				state.scene = 2; // Switch to tree
			}
			if (key == GLFW_KEY_3) {
				state.scene = 3; // switches to snow
			}
			if (key == GLFW_KEY_4) {
				state.scene = 4; // switches to lines
			}

		}

	}

	State getState() const {
		return state;
	}

private:
	State state;
	ShaderProgram& shader;

};


//colors - could be more creative sha
glm::vec3 const RED = glm::vec3(1.f, 0.f, 0.f);
glm::vec3 const GREEN = glm::vec3(0.f, 1.f, 0.f);
glm::vec3 const BLUE = glm::vec3(0.f, 0.f, 1.f);
glm::vec3 const BLACK = glm::vec3(0.f, 0.f, 0.f);
glm::vec3 const YELLOW = glm::vec3(1.f, 1.f, 0.f);

//any geom will works idk why i called it triangles sha
void generateFixedColors(CPU_Geometry& triangle) {
	for (int i = 0; i < triangle.verts.size(); i += 3) {
		triangle.cols.push_back(RED);
		triangle.cols.push_back(GREEN);
		triangle.cols.push_back(BLUE);
		triangle.cols.push_back(BLACK);
		triangle.cols.push_back(YELLOW);
	}
}

//Start Triangle ///

// getting midpoint from lecture and geek for geek 

std::vector<float> midPoint(std::vector<float> vertex1, std::vector<float> vertex2) {
	std::vector<float>point(2);
	point[0] = (vertex1[0] + vertex2[0]) * 0.5f;
	point[1] = (vertex1[1] + vertex2[1]) * 0.5f;

	return point;

}

// code below is definitely redundant but my dragonLine keeps crashing without it
// 3 vec
vector<float> midPoint3(vector<float> p1, vector<float> p2) {
	vector<float> p12(3);
	p12[0] = (p1[0] + p2[0]) / 2;
	p12[1] = (p1[1] + p2[1]) / 2;
	p12[2] = 0.f;
	return p12;
}

void sierpinskiTriangle(std::vector<float> a, std::vector<float> b, std::vector<float> c, CPU_Geometry& triangle, int iterations)
{

	if (a.size() < 2 || b.size() < 2 || c.size() < 2) {
		std::cerr << "Error: no suff size" << std::endl;
	}

	if (iterations == 0) {
		triangle.verts.push_back(glm::vec3(a[0], a[1], 0.f));
		triangle.verts.push_back(glm::vec3(b[0], b[1], 0.f));
		triangle.verts.push_back(glm::vec3(c[0], c[1], 0.f));

		return;
	}
	// midpoints
	std::vector<float> d = midPoint(a, b);
	std::vector<float> e = midPoint(a, c);
	std::vector<float> f = midPoint(b, c);

	sierpinskiTriangle(a, d, e, triangle, iterations - 1);
	sierpinskiTriangle(d, b, f, triangle, iterations - 1);
	sierpinskiTriangle(e, f, c, triangle, iterations - 1);
}


///End Triangle///


// start pythagoras tree //

// approach is make use of triangles


void drawTriangles(CPU_Geometry& square, vector<float> point1, vector<float> point2, vector<float> point3) {

	square.verts.push_back(glm::vec3(point1[0], point1[1], point1[2]));
	square.verts.push_back(glm::vec3(point2[0], point2[1], point2[2]));
	square.verts.push_back(glm::vec3(point3[0], point3[1], point3[2]));

}

void calculateNewBranchPoints(vector<float> leftTop, vector<float> rightTop, vector<float>& p1, vector<float>& p2, vector<float>& p3, vector<float>& p4, vector<float>& tip, vector<float> v) {
	float tipx = tip[0] + 0.5f * v[0];
	float tipy = tip[1] + 0.5f * v[1];

	vector<float> v1 = { tipx - rightTop[0], tipy - rightTop[1], 0.f };
	vector<float> v2 = { tipx - leftTop[0], tipy - leftTop[1], 0.f };

	p1 = { tipx + v1[0], tipy + v1[1], 0.f };
	p2 = { leftTop[0] + v1[0], leftTop[1] + v1[1], 0.f };
	p3 = { tipx + v2[0], tipy + v2[1], 0.f };
	p4 = { rightTop[0] + v2[0], rightTop[1] + v2[1], 0.f };
}

void drawTree(CPU_Geometry& square, vector<float> lbase, vector<float> rbase, vector<float> rightTop, vector<float> leftTop, int iterations) {
	drawTriangles(square, lbase, rbase, rightTop);
	drawTriangles(square, lbase, rightTop, leftTop);

	if (iterations == 0) {
		
		return;
	}
	else {
		vector<float> tip = midPoint(leftTop, rightTop);
		vector<float> v = { (leftTop[0] - lbase[0]), (leftTop[1] - lbase[1]), 0.f };

		vector<float> p1, p2, p3, p4;
		calculateNewBranchPoints(leftTop, rightTop, p1, p2, p3, p4, tip, v);

		// basically drawing two triangles is thre idea yuhhh
		drawTree(square, leftTop, { static_cast<float>(tip[0] + 0.5f * v[0]), static_cast<float>(tip[1] + 0.5f * v[1]), 0.f }, p1, p2, iterations - 1);
		drawTree(square, { static_cast<float>(tip[0] + 0.5f * v[0]), static_cast<float>(tip[1] + 0.5f * v[1]), 0.f }, rightTop, p4, p3, iterations - 1);
	}
	
}


// dragon line starts
void drawDragon(CPU_Geometry& dragon, vector<float> pl, vector<float> pb, vector<float> pr, int iterations) {


	
	if (iterations == 0) {
		dragon.verts.push_back(glm::vec3(pl[0], pl[1], pl[2]));
		dragon.verts.push_back(glm::vec3(pb[0], pb[1], pb[2]));
		dragon.verts.push_back(glm::vec3(pb[0], pb[1], pb[2]));
		dragon.verts.push_back(glm::vec3(pr[0], pr[1], pr[2]));
		return;
	}
	else {
		vector<float> mid = midPoint3(pl, pr);
		vector<float> pbmid = { pb[0] - mid[0], pb[1] - mid[1], pb[2] - mid[2] };
		vector<float> p1 = { pbmid[0] + pl[0], pbmid[1] + pl[1], pbmid[2] + pl[2] };


		drawDragon(dragon, pl, p1, pb, iterations - 1);
		drawDragon(dragon, pr, mid, pb, iterations - 1);
	}

	
}

// dragon line ends

// snow flakes starts
// youtube,geek for geek and stackoverflow
// https://www.geeksforgeeks.org/koch-curve-koch-snowflake/
// https://community.khronos.org/t/koch-curve/49542/7
void generateKochSnowflakes(CPU_Geometry& snow, glm::vec3 p1, glm::vec3 p2, int iterations) {

	if (iterations == 0) {
		snow.verts.push_back(p1);
		snow.verts.push_back(p2);

		snow.cols.push_back(glm::vec3(1.f, 1.f, 1.f));
		snow.cols.push_back(glm::vec3(1.f, 1.f, 1.f));
		return;


	}else {

		glm::vec3 length = (p2 - p1) / 3.0f; //1/3

		// create the new point
		glm::vec3 p3 = p1 + length;
		glm::vec3 p4 = p2 - length;

		// midpoint 
		glm::vec3 midPoint = (p4 + p3) * 0.5f; 

		glm::vec3 direction = p3 - p2;
		float height = (sqrt(3) * glm::length(direction)) / 4.0f; // 3 or 4 works best for this, i am getting issues with 2

		// use the below to rotate 90 degrees and normalize it - didnt want to build a new function 
		glm::vec3 perpendicular(-direction.y, direction.x, 0.0f);
		perpendicular = glm::normalize(perpendicular) * height;

		glm::vec3 p5 = midPoint + perpendicular;

		generateKochSnowflakes(snow, p1, p3, iterations - 1);
		generateKochSnowflakes(snow, p3, p5, iterations - 1);
		generateKochSnowflakes(snow, p5, p4, iterations - 1);
		generateKochSnowflakes(snow, p4, p2, iterations - 1);
	}
	
}

// this could be put in the above method lowkey
void draw_koch_snowflake(CPU_Geometry& snow, int iterations) {
	snow.verts.clear();
	snow.cols.clear();

	
	glm::vec3 a(-0.5f, -0.5f, 0.f);  
	glm::vec3 b(0.5f, -0.5f, 0.f); 
	glm::vec3 c(0.f, -0.5f + sqrt(3.0f) / 2.0f, 0.f); 


	// Generate Koch snowflake by subdividing each triangle side
	generateKochSnowflakes(snow, a,b, iterations); 
	generateKochSnowflakes(snow, b, c, iterations);
	generateKochSnowflakes(snow, c,a, iterations); 
}

// snow flake ends

void clearScene(CPU_Geometry& triangles, CPU_Geometry& squareDiamond, CPU_Geometry& snowFlakes, CPU_Geometry& dragonLines) {

	triangles.verts.clear();
	triangles.cols.clear();

	squareDiamond.verts.clear();
	squareDiamond.cols.clear();

	snowFlakes.verts.clear();
	snowFlakes.cols.clear();

	dragonLines.verts.clear();
	dragonLines.cols.clear();

}


int main()
{
	Log::debug("Starting main");

	// WINDOW
	glfwInit();
	Window window(800, 800, "CPSC 453"); // can set callbacks at construction if desired

	GLDebug::enable();

	// SHADERS
	ShaderProgram shader("shaders/test.vert", "shaders/test.frag");

	// CALLBACKS
	auto callbacks = std::make_shared<MyCallbacks>(shader);
	window.setCallbacks(callbacks); // can also update callbacks to new ones

	CPU_Geometry triangles;
	GPU_Geometry trianglesGPU;

	CPU_Geometry squareDiamond;
	GPU_Geometry squareDiamondGPU;

	CPU_Geometry snowFlakes;
	GPU_Geometry snowFlakesGPU;

	CPU_Geometry dragonLines;
	GPU_Geometry dragonLinesGPU;

	// triangle points

	std::vector<float> first{ 0.0f, 0.288f };
	std::vector<float> second{ -0.5f, -0.288f };
	std::vector<float> third{ 0.5f, -0.288f };
	  
	vector<float> lbase{ -0.1f, -0.5f, 0.f };
	vector<float> rbase{ 0.1f, -0.5f, 0.f };
	vector<float> rightTop{ 0.1f, -0.3f, 0.f };
	vector<float> leftTop{ -0.1f, -0.3f, 0.f };

	vector<float> pl = { -0.5f, 0.0f, 0.0f };
	vector<float> pb = { 0.0f, -0.5f, 0.0f };
	vector<float> pr = { 0.5f, 0.0f, 0.0f };

	/// snow

	vector<float> p1 = { -0.5f, -0.5f, 0.0f };  
	vector<float> p2 = { 0.5f, -0.5f, 0.0f };   
	vector<float> p3 = { 0.0f, 0.5f, 0.0f };

	//drawSnow(snowFlakes, p1, p2, 6);

	State state;
	// RENDER LOOP
	while (!window.shouldClose())  
	{
		glfwPollEvents();

		glEnable(GL_FRAMEBUFFER_SRGB);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();

		
		if (callbacks->getState().scene == 1)
		{
			clearScene(triangles, squareDiamond, snowFlakes, dragonLines);
			sierpinskiTriangle(first, second, third, triangles, callbacks->getState().SierpinskiIterations);
			generateFixedColors(triangles);
			trianglesGPU.setVerts(triangles.verts);
			trianglesGPU.setCols(triangles.cols);
			trianglesGPU.bind();
			glDrawArrays(GL_TRIANGLES, 0, GLsizei(triangles.verts.size()));
		}
		else if (callbacks->getState().scene == 2) {
			clearScene(triangles, squareDiamond, snowFlakes, dragonLines);
			drawTree(squareDiamond, lbase, rbase, rightTop, leftTop, callbacks->getState().squareIterations);
			generateFixedColors(squareDiamond);
			squareDiamondGPU.setVerts(squareDiamond.verts);
			squareDiamondGPU.setCols(squareDiamond.cols);
			squareDiamondGPU.bind();
			glDrawArrays(GL_TRIANGLES, 0, GLsizei(squareDiamond.verts.size()));
		}
		else if (callbacks->getState().scene == 3) {
			clearScene(triangles, squareDiamond, snowFlakes, dragonLines);
			draw_koch_snowflake(snowFlakes, callbacks->getState().snowIterations);
			snowFlakesGPU.setVerts(snowFlakes.verts);
			snowFlakesGPU.setCols(snowFlakes.cols);
			snowFlakesGPU.bind();
			glDrawArrays(GL_LINES, 0, GLsizei(snowFlakes.verts.size()));
		}

		else if (callbacks->getState().scene == 4) {
			clearScene(triangles, squareDiamond, snowFlakes, dragonLines);
			drawDragon(dragonLines, pl, pb, pr, callbacks->getState().dragonIterations);
			generateFixedColors(dragonLines);
			dragonLinesGPU.setCols(dragonLines.cols);
			dragonLinesGPU.setVerts(dragonLines.verts);
			dragonLinesGPU.bind();
			glDrawArrays(GL_LINES, 0, GLsizei(dragonLines.verts.size()));

		}
		
		/*glDrawArrays(GL_TRIANGLES, 0, GLsizei(triangles.verts.size()));
		glDrawArrays(GL_TRIANGLES, 0, GLsizei(squareDiamond.verts.size()));
		glDrawArrays(GL_LINES, 0, GLsizei(snowFlakes.verts.size()));
		glDrawArrays(GL_LINES, 0, GLsizei(dragonLines.verts.size()));*/


		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui

		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}
