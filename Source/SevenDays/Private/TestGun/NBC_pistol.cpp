// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGun/NBC_pistol.h"


UNBC_pistol::UNBC_pistol()
{
    //총기 관련 셋팅
    FPistol.SetReloadDelay(1.0f)
        .SetShotDelay(2.0f)
        .SetDamage(20)
        .SetMaxRecoil(2.0f, 4.0f)
        .SetBulletInfo(6, 30);
}
