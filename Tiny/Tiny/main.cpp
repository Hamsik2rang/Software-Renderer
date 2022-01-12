#include "tgaimage.h"
#include "vector.hpp"
#include "model.h"
#include "Random.hpp"

#include <queue>


const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);

void line(hs::Vec2i v0, hs::Vec2i v1, TGAImage& image, const TGAColor& color);
void triangle(hs::Vec2i* v, TGAImage& image, const TGAColor& color);
hs::Vec3f barycentric(hs::Vec2i* v, hs::Vec2i p, TGAImage& image, TGAColor color);

enum class DrawMode
{
	WIREFRAME = 0,
	FILL,
};

DrawMode drawMode = DrawMode::FILL;
Random random;

int main(int argc, char** argv)
{
	int width = 600;
	int height = 600;
	TGAImage image(width, height, TGAImage::RGB);
	Model* model = new Model("./Objects/african_head.obj");
	hs::Vec3f lightDirection = { 0.0f, 0.0f, -1.0f };
	for (int i = 0; i < model->fsize(); i++)
	{
		std::vector<int> face = model->face(i);
		hs::Vec2i t[3];
		for (int j = 0; j < 3; j++)
		{
			int x = (model->vertex(face[j]).x + 1.0f) * width / 2.0f;
			int y = (model->vertex(face[j]).y + 1.0f) * width / 2.0f;
			t[j] = { x,y };
		}
		
		hs::Vec3f n = (model->vertex(face[2]) - model->vertex(face[0])) ^ (model->vertex(face[1]) - model->vertex(face[0]));
		n.normalize();
		float intensity = n * lightDirection;
		if(intensity>0)
			triangle(t, image, TGAColor(255 * intensity, 255 * intensity, 255 * intensity, 255));
	}

	delete model;

	image.flip_vertically(); // I want to have the origin at the left bottom corner of the image
	image.write_tga_file("../../output_norm.tga");

	return 0;
}

// Use Bresenham's line algorithm.
void line(hs::Vec2i v0, hs::Vec2i v1, TGAImage& image, const TGAColor& color)
{
	bool steep = false;

	if (std::abs(v1.x - v0.x) < std::abs(v1.y - v0.y))
	{
		std::swap(v0.x, v0.y);
		std::swap(v1.x, v1.y);
		steep = true;
	}

	if (v0.x > v1.x)
	{
		std::swap(v0.x, v1.x);
		std::swap(v0.y, v1.y);
	}

	int dx = v1.x - v0.x;
	int dy = v1.y - v0.y;

	const float slope = std::abs((float)dy / dx);	//slope is less than 1.0f

	float ds = 0.0f;	// delta slope
	int y = v0.y;
	for (int x = v0.x; x <= v1.x; x++)
	{
		if (steep)
			image.set(y, x, color); // x and y are switched
		else
			image.set(x, y, color);

		ds += slope;
		if (ds > 0.5f)
		{
			y += dy > 0.0f ? 1 : -1;
			ds -= 1.0f;
		}
	}
}

void triangle(hs::Vec2i* v, TGAImage& image, const TGAColor& color)
{
	if (drawMode == DrawMode::WIREFRAME)
	{
		line(v[0], v[1], image, color);
		line(v[0], v[2], image, color);
		line(v[1], v[2], image, color);
	}
	else if (drawMode == DrawMode::FILL)
	{
		hs::Vec2i minBoxPos;
		hs::Vec2i maxBoxPos;

		minBoxPos.x = v[0].x < v[1].x ? v[0].x < v[2].x ? v[0].x : v[2].x : v[1].x < v[2].x ? v[1].x : v[2].x;
		minBoxPos.y = v[0].y < v[1].y ? v[0].y < v[2].y ? v[0].y : v[2].y : v[1].y < v[2].y ? v[1].y : v[2].y;

		maxBoxPos.x = v[0].x > v[1].x ? v[0].x > v[2].x ? v[0].x : v[2].x : v[1].x > v[2].x ? v[1].x : v[2].x;
		maxBoxPos.y = v[0].y > v[1].y ? v[0].y > v[2].y ? v[0].y : v[2].y : v[1].y > v[2].y ? v[1].y : v[2].y;

		hs::Vec2i p;
		for (p.y = minBoxPos.y; p.y <= maxBoxPos.y; p.y++)
		{
			for (p.x = minBoxPos.x; p.x <= maxBoxPos.x; p.x++)
			{
				hs::Vec3f bc = barycentric(v, p, image, color);
				if (bc.x < 0.0f ||
					bc.y < 0.0f ||
					bc.z < 0.0f)
					continue;

					image.set(p.x, p.y, color);
			}
		}
	}
}

hs::Vec3f barycentric(hs::Vec2i* v, hs::Vec2i p, TGAImage& image, TGAColor color)
{
	hs::Vec3f tx = { (float)v[0].x - p.x, (float)v[1].x - v[0].x, (float)v[2].x - v[0].x };
	hs::Vec3f ty = { (float)v[0].y - p.y, (float)v[1].y - v[0].y, (float)v[2].y - v[0].y };

	hs::Vec3f eq = tx ^ ty;
	// [a,b,c] to [u, v, 1]
	if (std::abs(eq.x) < 1.0f)
		return hs::Vec3f(-1.0f, -1.0f, -1.0f);

	return hs::Vec3f(1.0f - (eq.y + eq.z) / eq.x, eq.y / eq.x, eq.z / eq.x);
}