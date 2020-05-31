#include "Global.h"
#include "SDL.h"

#include "Rasterizer.h"

#define WIDTH 640
#define HEIGHT 480

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        *p = pixel;
        break;

    case 2:
        *(Uint16 *)p = pixel;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        } else {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16) & 0xff;
        }
        break;

    case 4:
        *(Uint32 *)p = pixel;
        break;
    }
}

void drawTriangle(SDL_Surface* screen, const gpg::Vertex& v0, const gpg::Vertex& v1, const gpg::Vertex& v2) {
	int minX = std::min(std::min(v0.x, v1.x), v2.x);
	int maxX = std::max(std::max(v0.x, v1.x), v2.x);
	int minY = std::min(std::min(v0.y, v1.y), v2.y);
	int maxY = std::max(std::max(v0.y, v1.y), v2.y);

	// Clip
	minX = std::max(minX, 0);
	maxX = std::min(maxX, WIDTH);
	minY = std::max(minY, 0);
	maxY = std::min(maxY, HEIGHT);

	// Compute edge equations
	gpg::EdgeEquation e0(v0, v1);
	gpg::EdgeEquation e1(v1, v2);
	gpg::EdgeEquation e2(v2, v0);

	double area = 0.5f * (e0.c + e1.c + e2.c);


	// Check if triangle is backfacing.
	if (area < 0)
		return ;

	// Interpolate parameters
	gpg::ParameterEquation r(v0.r, v1.r, v2.r, e0, e1, e2, area);
	gpg::ParameterEquation g(v0.g, v1.g, v2.g, e0, e1, e2, area);
	gpg::ParameterEquation b(v0.g, v1.b, v2.b, e0, e1, e2, area);

	// Check
	for (double x = minX; x <= maxX; x += 1.0f) {
		for (double y = minY; y <= maxY; y += 1.0f) {
			if (e0.test(x, y) && e1.test(x, y) && e2.test(x, y)) {
				int rint = r.evaluate(x, y) * 255;
				int gint = g.evaluate(x, y) * 255;
				int bint = b.evaluate(x, y) * 255;
				Uint32 color = SDL_MapRGB(screen->format, rint, gint, bint);
				putpixel(screen, x, y, color);
			}
		}
	}
}

int main(int argc, char** argv) {
	
	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window* window = SDL_CreateWindow(
		"Playground",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		WIDTH,
		HEIGHT,
		0);

	SDL_Surface* screen = SDL_GetWindowSurface(window);
	SDL_FillRect(screen, 0, 0);

	gpg::Vertex v0(100, 300, 0, 1, 0, 0);
	gpg::Vertex v1(200, 100, 0, 0, 1, 0);
	gpg::Vertex v2(300, 300, 0, 0, 0, 1);

	drawTriangle(screen, v0, v1, v2);

	SDL_UpdateWindowSurface(window);

	SDL_Event e;
	while (SDL_WaitEvent(&e) && e.type != SDL_QUIT);

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}