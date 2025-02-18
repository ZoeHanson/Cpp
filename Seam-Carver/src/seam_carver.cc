#include "seam_carver.hpp"

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}

int SeamCarver::GetEnergy(int row, int col) const {
  Pixel left_pixel = image_.GetPixel(row, (col - 1 + width_) % width_);
  Pixel right_pixel = image_.GetPixel(row, (col + 1) % width_);
  Pixel up_pixel = image_.GetPixel((row - 1 + height_) % height_, col);
  Pixel down_pixel = image_.GetPixel((row + 1) % height_, col);

  int r_col = left_pixel.GetRed() - right_pixel.GetRed();
  int g_col = left_pixel.GetGreen() - right_pixel.GetGreen();
  int b_col = left_pixel.GetBlue() - right_pixel.GetBlue();

  int r_row = up_pixel.GetRed() - down_pixel.GetRed();
  int g_row = up_pixel.GetGreen() - down_pixel.GetGreen();
  int b_row = up_pixel.GetBlue() - down_pixel.GetBlue();

  return (r_col * r_col + g_col * g_col + b_col * b_col) +
         (r_row * r_row + g_row * g_row + b_row * b_row);
}

void SeamCarver::UpdateVertEnergyArray(int** energy_array) const {
  for (int row = height_ - 2; row >= 0; --row) {
    for (int col = 0; col < width_; ++col) {
      int min_energy = energy_array[row + 1][col];

      if (col > 0) {
        min_energy = std::min(min_energy, energy_array[row + 1][col - 1]);
      }
      if (col < width_ - 1) {
        min_energy = std::min(min_energy, energy_array[row + 1][col + 1]);
      }

      energy_array[row][col] = min_energy + GetEnergy(row, col);
    }
  }
}

void SeamCarver::UpdateHorizEnergyArray(int** energy_array) const {
  for (int col = width_ - 2; col >= 0; --col) {
    for (int row = 0; row < height_; ++row) {
      int min_energy = energy_array[row][col + 1];

      if (row > 0) {
        min_energy = std::min(min_energy, energy_array[row - 1][col + 1]);
      }
      if (row < height_ - 1) {
        min_energy = std::min(min_energy, energy_array[row + 1][col + 1]);
      }

      energy_array[row][col] = min_energy + GetEnergy(row, col);
    }
  }
}

int* SeamCarver::FindVertMinSeam(int** energy_array) const {
  // int min_energy = energy_array[0][0];
  int min_index = 0;
  for (int col = 1; col < width_; col++) {
    if (energy_array[0][col] < energy_array[0][min_index]) {
      // min_energy = energy_array[0][col];
      min_index = col;
    }
  }

  int* seam = new int[height_];
  seam[0] = min_index;
  for (int row = 1; row < height_; row++) {
    int curr = seam[row - 1];
    int min_col = curr;
    if (curr == 0 && curr != width_ - 1) {
      if (energy_array[row][curr + 1] < energy_array[row][min_col]) {
        min_col = curr + 1;
      }
    } else if (curr > 0 && curr != width_ - 1) {
      if (energy_array[row][min_col] > energy_array[row][curr - 1]) {
        min_col = curr - 1;
      }
      if (energy_array[row][curr + 1] < energy_array[row][min_col]) {
        min_col = curr + 1;
      }
    } else if (curr > 0) {
      if (energy_array[row][min_col] > energy_array[row][curr - 1]) {
        min_col = curr - 1;
      }
    }

    seam[row] = min_col;
  }  // fix FIXED
  return seam;
}

int* SeamCarver::FindHorizMinSeam(int** energy_array) const {
  int* seam = new int[width_];
  int min_index = 0;

  for (int row = 1; row < height_; ++row) {
    if (energy_array[row][0] < energy_array[min_index][0]) {
      min_index = row;
    }
  }

  seam[0] = min_index;
  for (int col = 1; col < width_; ++col) {
    int curr = seam[col - 1];
    int min_row = curr;

    if (curr > 0 && energy_array[curr - 1][col] < energy_array[min_row][col]) {
      min_row = curr - 1;
    }
    if (curr < height_ - 1 &&
        energy_array[curr + 1][col] < energy_array[min_row][col]) {
      min_row = curr + 1;
    }

    seam[col] = min_row;
  }

  return seam;
}

int* SeamCarver::GetHorizontalSeam() const {
  int** energy_array = new int*[height_];
  for (int i = 0; i < height_; ++i) {
    energy_array[i] = new int[width_];
  }

  for (int row = 0; row < height_; ++row) {
    energy_array[row][width_ - 1] = GetEnergy(row, width_ - 1);
  }

  UpdateHorizEnergyArray(energy_array);
  int* seam = FindHorizMinSeam(energy_array);

  for (int row = 0; row < height_; ++row) {
    delete[] energy_array[row];
  }
  delete[] energy_array;

  return seam;
}

int* SeamCarver::GetVerticalSeam() const {
  int** energy_array = new int*[height_];
  for (int i = 0; i < height_; ++i) {
    energy_array[i] = new int[width_];
  }

  for (int col = 0; col < width_; ++col) {
    energy_array[height_ - 1][col] = GetEnergy(height_ - 1, col);
  }

  UpdateVertEnergyArray(energy_array);
  int* seam = FindVertMinSeam(energy_array);

  for (int row = 0; row < height_; ++row) {
    delete[] energy_array[row];
  }
  delete[] energy_array;

  return seam;
}

void SeamCarver::RemoveHorizontalSeam() {
  int* seam = GetHorizontalSeam();
  image_.RemoveRow(seam);

  delete[] seam;
  height_ = image_.GetHeight();
}

void SeamCarver::RemoveVerticalSeam() {
  int* seam = GetVerticalSeam();
  image_.RemoveColumn(seam);

  delete[] seam;
  width_ = image_.GetWidth();
}

const ImagePPM& SeamCarver::GetImage() const { return image_; }

int SeamCarver::GetWidth() const { return width_; }

int SeamCarver::GetHeight() const { return height_; }
