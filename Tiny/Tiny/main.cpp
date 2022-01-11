#include "tgaimage.h"
#include "vector.hpp"
#include "model.h"

#include <queue>

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);

void line(Vec2i v0, Vec2i v1, TGAImage& image, const TGAColor& color);
void triangle(Vec2i* v, TGAImage& image, const TGAColor& color);
Vec3f barycentric(Vec2i* v, Vec2i p, TGAImage& image, TGAColor color);

enum class DrawMode
{
	WIREFRAME = 0,
	FILL,
};

DrawMode drawMode = DrawMode::WIREFRAME;

int main(int argc, char** argv)
{
	int width = 600;
	int height = 800;
	TGAImage image(width, height, TGAImage::RGB);
	Model* model = new Model("./Objects/african_head.obj");
	for (int i = 0; i < model->fsize(); i++)
	{
		for (int j = 0; j < 3; i++)
		{
			Vec3f t[3] = {model->vertex(model->face(i)[0]), model->vertex(model->face(i)[1]), model->vertex(model->face(i)[2])};
		}
	}


	image.flip_vertically(); // I want to have the origin at the left bottom corner of the image
	image.write_tga_file("../../output_head.tga");


	return 0;
}

// Use Bresenham's line algorithm.
void line(Vec2i v0, Vec2i v1, TGAImage& image, const TGAColor& color)
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

void triangle(Vec2i* v, TGAImage& image, const TGAColor& color)
{
	if (drawMode == DrawMode::WIREFRAME)
	{
		line(v[0], v[1], image, color);
		line(v[0], v[2], image, color);
		line(v[1], v[2], image, color);
	}
	else if (drawMode == DrawMode::FILL)
	{
		Vec2i minBoxPos;
		Vec2i maxBoxPos;

		minBoxPos.x = v[0].x < v[1].x ? v[0].x < v[2].x ? v[0].x : v[2].x : v[1].x < v[2].x ? v[1].x : v[2].x;
		minBoxPos.y = v[0].y < v[1].y ? v[0].y < v[2].y ? v[0].y : v[2].y : v[1].y < v[2].y ? v[1].y : v[2].y;

		maxBoxPos.x = v[0].x > v[1].x ? v[0].x > v[2].x ? v[0].x : v[2].x : v[1].x > v[2].x ? v[1].x : v[2].x;
		maxBoxPos.y = v[0].y > v[1].y ? v[0].y > v[2].y ? v[0].y : v[2].y : v[1].y > v[2].y ? v[1].y : v[2].y;

		Vec2i p;
		for (p.y = minBoxPos.y; p.y <= maxBoxPos.y; p.y++)
		{
			for (p.x = minBoxPos.x; p.x <= maxBoxPos.x; p.x++)
			{
				Vec3f bc = barycentric(v, p, image, color);
				if (bc.x < 0.0f || bc.x >1.0f ||
					bc.y < 0.0f || bc.y> 1.0f ||
					bc.z < 0.0f || bc.z> 1.0f)
					continue;

				image.set(p.x, p.y, color);
			}
		}
	}
}

Vec3f barycentric(Vec2i* v, Vec2i p, TGAImage& image, TGAColor color)
{
	Vec3f tx = { (float)v[0].x - p.x, (float)v[1].x - v[0].x, (float)v[2].x - v[0].x };
	Vec3f ty = { (float)v[0].y - p.y, (float)v[1].y - v[0].y, (float)v[2].y - v[0].y };

	Vec3f eq = tx ^ ty;
	// [a,b,c] to [u, v, 1]
	if (std::abs(eq.x) < 1.0f)
		return Vec3f(-1.0f, -1.0f, -1.0f);

	return Vec3f(1.0f - (eq.y + eq.z) / eq.x, eq.y / eq.x, eq.z / eq.x);
}