// spilib.cpp : Defines the exported functions for the DLL.
//

#include "pch.h"
#include "framework.h"
#include "spilib.h"


// This is an example of an exported variable
SPILIB_API int nspilib=0;

// This is an example of an exported function.
SPILIB_API int fnspilib(void)
{
    return 0;
}

// This is the constructor of a class that has been exported.
Cspilib::Cspilib()
{
    return;
}
