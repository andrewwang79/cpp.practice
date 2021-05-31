#include <itkBMPImageIOFactory.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkJPEGImageIOFactory.h>
#include <itkLineCell.h>
#include <itkMesh.h>
#include <itkPNGImageIOFactory.h>
#include <itkVertexCell.h>

#include <iostream>

using namespace std;

void rgb(std::string from, std::string to) {
  const unsigned int Dimension = 2;  // 定义图像维数
  // 定义像素类型
  typedef itk::RGBPixel<unsigned char> PixelType;
  typedef itk::Image<PixelType, Dimension> ImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  typedef itk::ImageFileWriter<ImageType> WriterType;
  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName(from);
  writer->SetFileName(to);

  ImageType::Pointer image = reader->GetOutput();
  writer->SetInput(image);

  writer->Update();  //  Aliased to the Write() method to be consistent with the
                     //  rest of the pipeline.
}

void helloWorld() {
  typedef itk::Image<unsigned short, 3> ImageType;

  ImageType::Pointer image = ImageType::New();

  ImageType::IndexType start;
  start[0] = 0;  // first index on X
  start[1] = 0;  // first index on Y
  start[2] = 0;  // first index on Z
  ImageType::SizeType size;
  size[0] = 200;  // size along X
  size[1] = 200;  // size along Y
  size[2] = 200;  // size along Z
  ImageType::RegionType region;
  region.SetSize(size);
  region.SetIndex(start);
  image->SetRegions(region);
  ImageType::SpacingType spacing;
  spacing[0] = 0.33;  // spacing along X
  spacing[1] = 0.33;  // spacing along Y
  spacing[2] = 1.20;  // spacing along Z
  image->SetSpacing(spacing);
  image->Allocate();

  ImageType::IndexType pi1 = image->GetLargestPossibleRegion().GetIndex();
  int size1 = pi1.GetIndexDimension();

  // 读取像素，+1，设置像素
  ImageType::IndexType pixelIndex;
  pixelIndex[0] = 0;  // x position
  pixelIndex[1] = 0;  // y position
  pixelIndex[2] = 0;  // z position
  ImageType::PixelType pixelValue = image->GetPixel(pixelIndex);
  image->SetPixel(pixelIndex, pixelValue + 1);

  std::cout << "Hello ITK World !" << std::endl;
}

void mesh() {
  typedef float PixelType;
  typedef itk::Mesh<PixelType, 2> MeshType;
  typedef MeshType::CellType CellType;
  typedef itk::VertexCell<CellType> VertexType;
  typedef itk::LineCell<CellType> LineType;

  MeshType::Pointer mesh = MeshType::New();
  MeshType::PointType point0;
  MeshType::PointType point1;
  MeshType::PointType point2;
  MeshType::PointType point3;

  point0[0] = -1;
  point0[1] = -1;
  point1[0] = 1;
  point1[1] = -1;
  point2[0] = 1;
  point2[1] = 1;
  point3[0] = -1;
  point3[1] = 1;

  mesh->SetPoint(0, point0);
  mesh->SetPoint(1, point1);
  mesh->SetPoint(2, point2);
  mesh->SetPoint(3, point3);

  CellType::CellAutoPointer cellpointer;

  cellpointer.TakeOwnership(new LineType);
  cellpointer->SetPointId(0, 0);
  cellpointer->SetPointId(1, 1);
  mesh->SetCell(0, cellpointer);

  cellpointer.TakeOwnership(new LineType);
  cellpointer->SetPointId(0, 1);
  cellpointer->SetPointId(1, 2);
  mesh->SetCell(1, cellpointer);

  cellpointer.TakeOwnership(new LineType);
  cellpointer->SetPointId(0, 2);
  cellpointer->SetPointId(1, 0);
  mesh->SetCell(2, cellpointer);
}

int main() {
  itk::JPEGImageIOFactory::RegisterOneFactory();
  itk::PNGImageIOFactory::RegisterOneFactory();
  itk::BMPImageIOFactory::RegisterOneFactory();

  helloWorld();
  rgb("./s/demo.jpg", "./s/demo.out.jpg");

  return 0;
}
