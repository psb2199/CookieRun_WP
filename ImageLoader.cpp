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
	I_Bridge1.Load(L".\\CookieRun_Resource\\Land1\\Land1 장애물\\epN01_tm02_tb1.png");
	I_Sd1.Load(L".\\CookieRun_Resource\\Land1\\Land1 장애물\\epN01_tm02_sdA.png");
	I_jp1A.Load(L".\\CookieRun_Resource\\Land1\\Land1 장애물\\epN01_tm02_jp1A.png");
	I_jp2A.Load(L".\\CookieRun_Resource\\Land1\\Land1 장애물\\epN01_tm02_jp2A.png");
	I_jp2B.Load(L".\\CookieRun_Resource\\Land1\\Land1 장애물\\epN01_tm02_jp2B.png");
	I_CommonJelly.Load(L".\\CookieRun_Resource\\GeneralJelly.png");
	I_YellowBear.Load(L".\\CookieRun_Resource\\YellowBear.png");
	I_PinkBear.Load(L".\\CookieRun_Resource\\PinkBear.png");
	I_BigBear.Load(L".\\CookieRun_Resource\\BigBear.png");
	I_SilverCoin.Load(L".\\CookieRun_Resource\\SilverCoin.png");
	I_GoldCoin.Load(L".\\CookieRun_Resource\\GoldCoin.png");
	I_BigCoin.Load(L".\\CookieRun_Resource\\BigCoin.png");
	I_Bigitem.Load(L".\\CookieRun_Resource\\big_item.png");
	I_Fastitem.Load(L".\\CookieRun_Resource\\fast_item.png");
	I_Energyitem.Load(L".\\CookieRun_Resource\\energy_item.png");
	I_Magnetitem.Load(L".\\CookieRun_Resource\\magnet_item.png");
	I_FastEffect.Load(L".\\CookieRun_Resource\\FastEffect.png");
	I_LifeBar1.Load(L".\\CookieRun_Resource\\gaugebg_heart02.png");
	I_LifeBar2.Load(L".\\CookieRun_Resource\\gauge_heart_orange.png");
	I_Heart.Load(L".\\CookieRun_Resource\\icon_heartLife.png");
	I_Pause.Load(L".\\CookieRun_Resource\\puase.png");
	I_Continue_no.Load(L".\\CookieRun_Resource\\gamestart_no.png");
	I_Continue_dim.Load(L".\\CookieRun_Resource\\gamestart_dim.png");
	I_Exit_no.Load(L".\\CookieRun_Resource\\exit_no.png");
	I_Exit_dim.Load(L".\\CookieRun_Resource\\exit_dim.png");
	I_BackGround_start.Load(L".\\CookieRun_Resource\\시작화면.png");
	I_ScoreBoard.Load(L".\\CookieRun_Resource\\score.png");
	I_Playagain_dim.Load(L".\\CookieRun_Resource\\Playagain_dim.png");
	I_Playagain_no.Load(L".\\CookieRun_Resource\\Playagain_no.png");
	I_EndGame_dim.Load(L".\\CookieRun_Resource\\Exit_dim_r.png");
	I_EndGame_no.Load(L".\\CookieRun_Resource\\Exit_no_r.png");
}
