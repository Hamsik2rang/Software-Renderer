#include "tgaimage.h"
#include "vector.hpp"
#include "model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);

void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color);
void triangle(Vec2i v0, Vec2i v1, Vec2i v2, TGAImage& image, const TGAColor& color);


int main(int argc, char** argv)
{
	int width = 800;
	int height = 800;
	TGAImage image(width, height, TGAImage::RGB);
	auto model = new Model("./Objects/african_head.obj");

	for (int i = 0; i < model->fsize(); i++)
	{
		Face face = model->face(i);
		for (int j = 0; j < 3; j++)
		{
			Vec3f v0 = model->vertex(face.vIndex[j]);
			Vec3f v1 = model->vertex(face.vIndex[(j + 1) % 3]);
			int x0 = (int)((v0.x + 1.0f) * width / 2);
			int y0 = (int)((v0.y + 1.0f) * height / 2);
			int x1 = (int)((v1.x + 1.0f) * width / 2);
			int y1 = (int)((v1.y + 1.0f) * height / 2);

			line(x0, y0, x1, y1, image, white);
		}
	}
	/*Vec2i t0[3] = { Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80) };
	Vec2i t1[3] = { Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180) };
	Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };
	triangle(t0[0], t0[1], t0[2], image, red);
	triangle(t1[0], t1[1], t1[2], image, white);
	triangle(t2[0], t2[1], t2[2], image, green);*/

	image.flip_vertically(); // I want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");

	delete model;

	return 0;
}

// Use Bresenham's line algorithm.
void line(int x0, int y0, int x1, int y1, TGAImage& image, const TGAColor& color)
{
	bool steep = false;

	if (std::abs(x1 - x0) < std::abs(y1 - y0))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;

	const float slope = std::abs((float)dy / dx);	//slope is less than 1.0f
	float ds = 0.0f;	// delta slope
	int y = y0;
	for (int x = x0; x <= x1; x++)
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

void triangle(Vec2i v0, Vec2i v1, Vec2i v2, TGAImage& image, const TGAColor& color)
{
	line(v0.x, v0.y, v1.x, v1.y, image, color);
	line(v1.x, v1.y, v2.x, v2.y, image, color);
	line(v0.x, v0.y, v2.x, v2.y, image, color);
}