#include "ImageLoader.h"
#include <iostream>

ImageLoader::ImageLoader()
{
}

ImageLoader::~ImageLoader()
{
}

void ImageLoader::LoadAllImage()
{
	I_BraveCookie.Load(L"Brave_Cookie.bmp");
	if (!I_BraveCookie) std::cout << "LoadFail" << std::endl;
}
