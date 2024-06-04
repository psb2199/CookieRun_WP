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
	I_AngelCookie.Load(L".\\CookieRun_Resource\\cookie\\Angel Cookie.png");
	I_BackGround.Load(L".\\CookieRun_Resource\\Land1\\Land1배경2.png");
	I_BackGround2.Load(L".\\CookieRun_Resource\\Land1\\Land1배경1.png");
	I_Bridge1.Load(L".\\CookieRun_Resource\\Land1\\Land1epN01_tm02_tb1.png");
}
