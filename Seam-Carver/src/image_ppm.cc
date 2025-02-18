#include "image_ppm.hpp"

// implement the rest of ImagePPM's functions here

// given functions below, DO NOT MODIFY

ImagePPM::ImagePPM(const std::string& path) {
  std::ifstream input_file(path);
  input_file >> *this;
  input_file.close();
}

ImagePPM::ImagePPM(const ImagePPM& source) { *this = source; }

ImagePPM& ImagePPM::operator=(const ImagePPM& source) {
  if (this == &source) {
    return *this;
  }

  Clear();

  height_ = source.height_;
  width_ = source.width_;
  max_color_value_ = source.max_color_value_;

  pixels_ = new Pixel*[height_];
  for (int row = 0; row < height_; ++row) {
    pixels_[row] = new Pixel[width_];
    for (int col = 0; col < width_; ++col) {
      pixels_[row][col] = source.pixels_[row][col];
    }
  }

  return *this;
}

ImagePPM::~ImagePPM() { Clear(); }

void ImagePPM::Clear() {
  if (pixels_) {
    for (int i = 0; i < height_; ++i) {
      delete[] pixels_[i];
    }
    delete[] pixels_;
  }

  height_ = 0;
  width_ = 0;
  pixels_ = nullptr;
}

std::istream& operator>>(std::istream& is, ImagePPM& image) {
  image.Clear();
  std::string line;
  getline(is, line);
  getline(is, line);
  if (line[0] == '#') {
    getline(is, line);
  }
  int space = line.find_first_of(' ');
  image.width_ = std::stoi(line.substr(0, space));
  image.height_ = std::stoi(line.substr(space + 1));
  getline(is, line);
  image.max_color_value_ = std::stoi(line);
  image.pixels_ = new Pixel*[image.height_];
  for (int i = 0; i < image.height_; i++) {
    image.pixels_[i] = new Pixel[image.width_];
  }
  for (int row = 0; row < image.height_; row++) {
    for (int col = 0; col < image.width_; col++) {
      getline(is, line);
      int red = std::stoi(line);
      getline(is, line);
      int green = std::stoi(line);
      getline(is, line);
      int blue = std::stoi(line);

      Pixel p(red, green, blue);
      image.pixels_[row][col] = p;
    }
  }
  return is;
}

Pixel ImagePPM::GetPixel(int row, int col) const { return pixels_[row][col]; }

int ImagePPM::GetMaxColorValue() const { return max_color_value_; }

std::ostream& operator<<(std::ostream& os, const ImagePPM& image) {
  os << "P3\n" << image.width_ << " " << image.height_ << "\n";
  os << image.max_color_value_ << "\n";

  for (int row = 0; row < image.height_; ++row) {
    for (int col = 0; col < image.width_; ++col) {
      const Pixel& pixel = image.GetPixel(row, col);
      os << pixel.GetRed() << "\n";
      os << pixel.GetGreen() << "\n";
      os << pixel.GetBlue() << "\n";
    }
  }

  return os;
}

void ImagePPM::RemoveRow(const int* seam) {
  for (int col = 0; col < width_; ++col) {
    for (int row = seam[col]; row < height_ - 1; ++row) {
      pixels_[row][col] = pixels_[row + 1][col];
    }
  }

  delete[] pixels_[height_ - 1];
  --height_;
}

void ImagePPM::RemoveColumn(const int* seam) {
  for (int row = 0; row < height_; ++row) {
    for (int col = seam[row]; col < width_ - 1; ++col) {
      pixels_[row][col] = pixels_[row][col + 1];
    }
  }

  --width_;
}
