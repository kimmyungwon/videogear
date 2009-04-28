#pragma once

void HexToBin(LPCTSTR str, std::string& bytes);
bool IsPinConnected(IPin* pin);
PIN_DIRECTION GetPinDir(IPin* pin);