#include <itkBMPImageIOFactory.h>
#include <itkImage.h>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include <itkJPEGImageIOFactory.h>
#include <itkPNGImageIOFactory.h>

#include <iostream>

using namespace std;

void jpg(std::string from, std::string to) {
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

  std::cout << "Hello ITK World !" << std::endl;
}

int main() {
  itk::JPEGImageIOFactory::RegisterOneFactory();
  itk::PNGImageIOFactory::RegisterOneFactory();
  itk::BMPImageIOFactory::RegisterOneFactory();

  // helloWorld();
  jpg("./s/demo.jpg", "./s/demo.out.jpg");

  return 0;
}
