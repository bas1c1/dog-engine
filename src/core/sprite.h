#ifndef DOG_ENGINE_SPRITES
#define DOG_ENGINE_SPRITES

#include "../../ext/include/wfsgl32.h"
#include "component.h"
#include "camera.h"

#define SCREEN_TO_NDC_X(x) (x / ctx.width * 2 - 1)
#define SCREEN_TO_NDC_Y(y) (y / ctx.height * 2 - 1)
#define writeSprite(pos, offset) glBegin(GL_QUADS);\
								 glTexCoord2f(0, 1);\
								 glVertex2f(SCREEN_TO_NDC_X(pos.x), SCREEN_TO_NDC_Y(pos.y));\
								 glTexCoord2f(0, 0);\
								 glVertex2f(SCREEN_TO_NDC_X(pos.x), SCREEN_TO_NDC_Y(offset.y));\
								 glTexCoord2f(1, 0);\
								 glVertex2f(SCREEN_TO_NDC_X(offset.x), SCREEN_TO_NDC_Y(offset.y));\
								 glTexCoord2f(1, 1);\
								 glVertex2f(SCREEN_TO_NDC_X(offset.x), SCREEN_TO_NDC_Y(pos.y));\
								 glEnd();\

class sprite : public base_component {
private:
	WFSGL ctx;
	std::vector<Pixel> pixels;
	int width;
	int height;
	vec2d pos;
	camera cam2d;
	GLuint gpuTexture;

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

	sprite(const char* filename, WFSGL ctx, int width, int height, int px = 0, int py = 0, camera cam2d = mainCamera) {
		this->pixels = IO::loadPNG(filename);
		this->width = width;
		this->height = height;
		this->ctx = ctx;
		this->cam2d = cam2d;
		this->pos = vec2d(px, py) - vec2d(width / 2, height / 2);

		GenerateGPUTexture();
	}

	void update() override {
		this->cam2d = cam2d;
		this->Draw();
	}

	void Draw() {
		vec2d oldPos = pos;
		pos = mainCamera.project(pos);
		const vec2d offset(pos.x + width, pos.y + height);

		writeSprite(pos, offset);

		this->pos = oldPos;
	}

	void GenerateGPUTexture() {
		glGenTextures(1, &gpuTexture);
		glBindTexture(GL_TEXTURE_2D, gpuTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());
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