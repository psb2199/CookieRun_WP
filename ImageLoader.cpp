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
	// I_BraveCookie.Load(L".\\CookieRun_Resource\\cookie\\Brave_Cookie.bmp");
	I_AngelCookie.Load(L".\\CookieRun_Resource\\cookie\\Angel Cookie.png");
	I_BackGround.Load(L".\\CookieRun_Resource\\Land1\\Land1배경2.png");
	I_Ground.Load(L".\\CookieRun_Resource\\Land1\\Land1 장애물\\epN01_tm02_fh.png");
}
