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
//void fill(Vec2i cur, TGAImage& image, const TGAColor& color);


int main(int argc, char** argv)
{
	int width = 256;
	int height = 256;
	TGAImage image(width, height, TGAImage::RGB);

	Vec2i t0[3] = { Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80) };
	Vec2i t1[3] = { Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180) };
	Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };
	triangle(t0, image, red);
	triangle(t1, image, white);
	triangle(t2, image, green);

	image.flip_vertically(); // I want to have the origin at the left bottom corner of the image
	image.write_tga_file("output_fill.tga");


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
	Vec2i minBoxPos(image.get_width() - 1, image.get_height() - 1);
	Vec2i maxBoxPos(0, 0);
	Vec2i clamp(image.get_width() - 1, image.get_height() - 1);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			minBoxPos[j] = std::max(0, std::min(minBoxPos[j], v[i][j]));
			maxBoxPos[j] = std::min(clamp[j], std::max(maxBoxPos[j], v[i][j]));
		}
	}

	Vec2i p;
	for (p.y = minBoxPos.y; p.y <= maxBoxPos.y; p.y++)
	{
		for (p.x = minBoxPos.x; p.x <= maxBoxPos.x; p.x++)
		{
			Vec3f tx = { (float)v[0].x - p.x, (float)v[1].x - v[0].x, (float)v[2].x - v[0].x };
			Vec3f ty = { (float)v[0].y - p.y, (float)v[1].y - v[0].y, (float)v[2].y - v[0].y };

			Vec3f eq = tx ^ ty;

			if (std::abs(eq.x) < 1)
				continue;

			Vec3f barycentric = { 1.0f - (eq.y + eq.z) / eq.x, eq.y / eq.x , eq.z / eq.x };
			if (barycentric.x < 0.0f || barycentric.x >1.0f ||
				barycentric.y < 0.0f || barycentric.y> 1.0f ||
				barycentric.z < 0.0f || barycentric.z> 1.0f)
				continue;

			image.set(p.x, p.y, color);
		}
	}
}

//Vec3f barycentric(Vec2i* v, Vec2i p)
//{
//	Vec3f u;
//	Vec3f eqx = { (float)v[2].x - v[0].x, (float)v[1].x - v[0].x, (float)v[0].x - p.x };
//	Vec3f eqy = { (float)v[2].y - v[0].y, (float)v[1].y - v[0].y, (float)v[0].y - p.y };
//	u = eqx ^ eqy;
//
//	if (std::abs(u.z) < 1) 
//		return Vec3f(-1.0f, -1.0f, -1.0f);
//	return Vec3f(1.0f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z);
//}

//
//void fill(Vec2i start, TGAImage& image, const TGAColor& color)
//{
//	const static int dx[] = { 0, -1, 0, 1 };
//	const static int dy[] = { -1, 0, 1, 0 };
//	std::queue<Vec2i> q;
//	image.set(start.x, start.y, color);
//	q.push(start);
//	while (!q.empty())
//	{
//		Vec2i cur = q.front();
//		q.pop();
//
//		for (int i = 0; i < 4; i++)
//		{
//			Vec2i next = { cur.x + dx[i], cur.y + dy[i] };
//			bool isSamePixel = true;
//			TGAColor nextColor = image.get(next.x, next.y);
//			for (int i = 0; i < image.get_bytespp(); i++)
//			{
//				if (nextColor.raw[i] != color.raw[i])
//					isSamePixel = false;
//			}
//
//			if (!isSamePixel)
//			{
//				image.set(next.x, next.y, color);
//				q.push(next);
//			}
//		}
//	}
//}
