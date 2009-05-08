#pragma once

#define _FACVG  0x260
#define MAKE_VGHRESULT( code )  MAKE_HRESULT( 1, _FACVG, code )

#define VGERR_NOT_SUPPORTED		MAKE_VGHRESULT(1000)
#define VGERR_INVALID_DIRECTION	MAKE_VGHRESULT(1001)
#define VGERR_ALREADY_CONNECTED	MAKE_VGHRESULT(1002)
#define VGERR_NOT_CONNECTED		MAKE_VGHRESULT(1003)