#ifndef BITMAP_GUARDIAN_H
#define BITMAP_GUARDIAN_H

#include "geometry.h" // Для Vec3f.

void SaveBMP(const char* fname, const std::vector<Vec3f>& pixels, int w, int h);

#endif 
