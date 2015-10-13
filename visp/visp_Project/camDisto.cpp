#include <visp/vpCameraParameters.h>
#include <visp/vpImage.h>

int main()
{
  vpImage<unsigned char>I(480,640);
  double u0 = I.getWidth() / 2;
  double u1 = I.getHeight() /2;
  double px = 600;
  double py = 600;
  vpCameraParameters cam;
  cam.initPersProjWithoutDistortion(px,py,u0,u1);
  cam.computeFov(I.getWidth(),I.getHeight());
  std::cout << cam << std::endl;
  std::cout << "Fov : H " << vpMath::deg(cam.getHorizontalFovAngle()) << "/ V "<< vpMath::deg(cam.getVerticalFovAngle())<< std::endl;

  return 0;
}
