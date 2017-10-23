#pragma once
#include <limits>
#include "Image.h"
#include "../lib/EasyBMP_1.06/EasyBMP.h"

struct BoundingBox {
  Point min, max;

  BoundingBox()
    : min(std::numeric_limits<int>::max(), std::numeric_limits<int>::max()),
    max(std::numeric_limits<int>::min(), std::numeric_limits<int>::min()) {
  }
};

class Shape {
protected:
  int max_height_, max_width_;
private:
  RGBApixel color_;
public:

  void SetSize(const int width, const int height) {
    max_height_ = height;
    max_width_ = width;
  }

  RGBApixel GetColor() const { return color_; }
  void SetColor(const RGBApixel& color) { color_ = color; }

  virtual bool Contains(const Point& point) const = 0;
  virtual bool Valid() const = 0;
  virtual BoundingBox GetBBox() const = 0;
  virtual void Mutate() = 0;
  virtual void Rollback() = 0;

  void AddToImage(Image& image) const {
    const BoundingBox bbox = this->GetBBox();
    for (int x = bbox.min.x; x < bbox.max.x; ++x) {
      for (int y = bbox.min.y; y < bbox.max.y; ++y) {
        if (this->Contains(Point(x, y))) {
          image.SetPixel(x, y, this->GetColor());
        }
      }
    }
  }

  void SetColor(const Image& image) {
    const BoundingBox bbox = this->GetBBox();
    //const alpha = 
    // TODO Alpha Shit
    // Try default alpha value 128
    // Try combination of prev value + new color
    long long r = 0,
      g = 0,
      b = 0;
    long count = 0;
    for (int x = bbox.min.x; x <= bbox.max.x; ++x) {
      for (int y = bbox.min.y; y <= bbox.max.y; ++y) {
        if (this->Contains(Point(x, y))) {
          const RGBApixel color = image.GetPixel(x, y);
          r += color.Red;
          g += color.Green;
          b += color.Blue;
          ++count;
        }
      }
    }

    color_.Red = r / count;
    color_.Green = g / count;
    color_.Blue = b / count;
    color_.Alpha = 128;
  }
};
