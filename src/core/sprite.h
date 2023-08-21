#ifndef DOG_ENGINE_SPRITES
#define DOG_ENGINE_SPRITES

#include "../../ext/include/wfsgl32.h"
#include "../lib/math.h"
#include "component.h"
#include "camera.h"
#include "../lib/io.h"

class sprite : public base_component {
private:
	WFSGL ctx;
	std::vector<Pixel> pixels;
	int width;
	int height;
	vec2d pos;
	camera cam2d;

public:

	sprite() { }

	sprite(WFSGL ctx) {
		width = 0;
		height = 0;
		pos = vec2d(0, 0);
		pixels = std::vector<Pixel>(0);
		this->ctx = ctx;
		this->cam2d = mainCamera;
	}

	sprite(WFSGL ctx, int width, int height, int px = 0, int py = 0, camera cam2d = mainCamera) {
		this->width = width;
		this->height = height;
		this->pixels = std::vector<Pixel>(width * height);
		for (int i = 0; i < width * height; i++)
			pixels[i] = Pixel(0, 0, 0);
		this->ctx = ctx;
		this->cam2d = cam2d;
		this->pos = vec2d(px, py) - vec2d(width / 2, height / 2);
	}

	sprite(WFSGL ctx, int width, int height, std::vector<Pixel> pixels, int px = 0, int py = 0, camera cam2d = mainCamera) {
		this->width = width;
		this->height = height;
		this->pixels = pixels;
		this->ctx = ctx;
		this->cam2d = cam2d;
		this->pos = vec2d(px, py) - vec2d(width / 2, height / 2);
	}

	void fromFile(const char* filename, WFSGL ctx, int width, int height, int px = 0, int py = 0, camera cam2d = mainCamera) {
		this->pixels = IO::loadPNG(filename);
		this->width = width;
		this->height = height;
		this->ctx = ctx;
		this->cam2d = cam2d;
		this->pos = vec2d(px, py) - vec2d(width / 2, height / 2);
	}

	void update() override {
		this->cam2d = cam2d;
		this->Draw();
	}

	void Draw() {
#pragma omp parallel for
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				vec2d point = mainCamera.project(vec2d(x + pos.x, y + pos.y));
				ctx.WFSGLSetPixel(point.x, point.y, pixels[x + y * width]);
			}
		}
		
	}

	bool FillArea(vec2d f, vec2d s, Pixel color) {
		if (
			(f.x < 0 || f.x > width) ||
			(s.x < 0 || s.x > width) ||
			(f.y < 0 || f.y > height) ||
			(f.y < 0 || f.y > height)
			)
			return false;

		for (int x = f.x; x < s.x; x++) {
			for (int y = f.y; y < s.y; y++) {
				pixels[x + y * width] = color;
			}
		}

		return true;
	}
};

#endif