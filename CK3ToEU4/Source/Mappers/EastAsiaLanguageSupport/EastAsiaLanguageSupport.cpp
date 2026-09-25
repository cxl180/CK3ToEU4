#include <windows.h>
#include <string>
#include <algorithm>
#include "EastAsiaLanguageSupport.h"
std::string mappers::EastAsiaLanguageSupport::utf8StringToSpecial(std::string& s, bool toUTF8, bool isEU4)
{
	std::wstring ws = utf8ToWString(s);
	std::string out;
	unsigned int size = ws.size();

	for (unsigned int fromIndex = 0; fromIndex < size; fromIndex++)
	{
		wchar_t c = ws[fromIndex];
		// 主要来自对https://gist.github.com/bruceCzK/96ad6e054111f929ed67291552d36334的改写。
		byte high = (c >> 8) & 0x000000FF;
		byte low = c & 0x000000FF;
		byte escapeChr = 0x10;
		// Magic numbers
		int lowByteOffset = 15;
		const int highByteOffset = -9;

		if (isEU4)
		{
			lowByteOffset = 14;
		}

		// 以下是原注释：
		// because characters in internalChars will be used in game as special characters, such as csv delimiter
		// so we have to escape these characters
		// 0x10 0x11 0x12 0x13 are all leading character to determine a multibyte letter start, depends on how the escape works
		if (internalChars(high, toUTF8, isEU4))
		{
			escapeChr += 2;
		}
		if (internalChars(low, toUTF8, isEU4))
		{
			escapeChr++;
		}
		switch (escapeChr)
		{
			case 0x11:
				low += lowByteOffset;
				break;
			case 0x12:
				high += highByteOffset;
				break;
			case 0x13:
				low += lowByteOffset;
				high += highByteOffset;
				break;
			case 0x10:
			default:
				break;
		}
		if (toUTF8)
		{
			// 以下是原注释：
			//   For EU4
			//  Transform Latin1 extended control characters to utf8
			//  Chars in this section will not display correctly in utf8 encoding
			low = cp1252ToUCS2(low);
			high = cp1252ToUCS2(high);
		}
		out.push_back(escapeChr);
		out.push_back(cp1252ToUCS2(low));
		out.push_back(cp1252ToUCS2(high));
	}
	return out;
}
std::wstring mappers::EastAsiaLanguageSupport::utf8ToWString(const std::string& utf8)
{
	if (utf8.empty())
		return L"";
	int size = MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), nullptr, 0);
	std::wstring result(size, L'\0');
	MultiByteToWideChar(CP_UTF8, 0, utf8.data(), static_cast<int>(utf8.size()), result.data(), size);
	return result;
}

// 看起来，ck和EU4都是非Linux的样子，在此不考虑linux。

wchar_t mappers::EastAsiaLanguageSupport::UCS2ToCP1252(int c)
{
	wchar_t result = c;
	switch (c)
	{
		case 0x20AC:
			result = 0x80;
			break;
		case 0x201A:
			result = 0x82;
			break;
		case 0x0192:
			result = 0x83;
			break;
		case 0x201E:
			result = 0x84;
			break;
		case 0x2026:
			result = 0x85;
			break;
		case 0x2020:
			result = 0x86;
			break;
		case 0x2021:
			result = 0x87;
			break;
		case 0x02C6:
			result = 0x88;
			break;
		case 0x2030:
			result = 0x89;
			break;
		case 0x0160:
			result = 0x8A;
			break;
		case 0x2039:
			result = 0x8B;
			break;
		case 0x0152:
			result = 0x8C;
			break;
		case 0x017D:
			result = 0x8E;
			break;
		case 0x2018:
			result = 0x91;
			break;
		case 0x2019:
			result = 0x92;
			break;
		case 0x201C:
			result = 0x93;
			break;
		case 0x201D:
			result = 0x94;
			break;
		case 0x2022:
			result = 0x95;
			break;
		case 0x2013:
			result = 0x96;
			break;
		case 0x2014:
			result = 0x97;
			break;
		case 0x02DC:
			result = 0x98;
			break;
		case 0x2122:
			result = 0x99;
			break;
		case 0x0161:
			result = 0x9A;
			break;
		case 0x203A:
			result = 0x9B;
			break;
		case 0x0153:
			result = 0x9C;
			break;
		case 0x017E:
			result = 0x9E;
			break;
		case 0x0178:
			result = 0x9F;
			break;
	}

	return result;
}

wchar_t mappers::EastAsiaLanguageSupport::cp1252ToUCS2(byte c)
{
	wchar_t result = c;
	switch (c)
	{
		case 0x80:
			result = 0x20AC;
			break;
		case 0x82:
			result = 0x201A;
			break;
		case 0x83:
			result = 0x0192;
			break;
		case 0x84:
			result = 0x201E;
			break;
		case 0x85:
			result = 0x2026;
			break;
		case 0x86:
			result = 0x2020;
			break;
		case 0x87:
			result = 0x2021;
			break;
		case 0x88:
			result = 0x02C6;
			break;
		case 0x89:
			result = 0x2030;
			break;
		case 0x8A:
			result = 0x0160;
			break;
		case 0x8B:
			result = 0x2039;
			break;
		case 0x8C:
			result = 0x0152;
			break;
		case 0x8E:
			result = 0x017D;
			break;
		case 0x91:
			result = 0x2018;
			break;
		case 0x92:
			result = 0x2019;
			break;
		case 0x93:
			result = 0x201C;
			break;
		case 0x94:
			result = 0x201D;
			break;
		case 0x95:
			result = 0x2022;
			break;
		case 0x96:
			result = 0x2013;
			break;
		case 0x97:
			result = 0x2014;
			break;
		case 0x98:
			result = 0x02DC;
			break;
		case 0x99:
			result = 0x2122;
			break;
		case 0x9A:
			result = 0x0161;
			break;
		case 0x9B:
			result = 0x203A;
			break;
		case 0x9C:
			result = 0x0153;
			break;
		case 0x9E:
			result = 0x017E;
			break;
		case 0x9F:
			result = 0x0178;
			break;
	}

	return result;
}

bool mappers::EastAsiaLanguageSupport::internalChars(byte highOrLow, bool toUTF8, bool isEU4)
{
	switch (highOrLow)
	{
		case 0xA4:
		case 0xA3:
		case 0xA7:
		case 0x24:
		case 0x5B:
		case 0x00:
		case 0x5C:
		case 0x0D:
		case 0x0A:
		case 0x22:
		case 0x7B:
		case 0x7D:
		case 0x40:
		case 0x80:
		case 0x7E:
		case 0xBD:
		case 0x3B:
		case 0x5D:
		case 0x5F:
		case 0x3D:
		case 0x23:
		case 0x3F:
		case 0x3A:
			return true;
	}
	// 以下是原注释：
	// 0x20 in oldVersion escape will be transform to backslash which will not be parsed right by the engine
	// So it has to be removed
	// Will be transformed to 0x2f (backslash)（此处应为slash） in high byte, remove it
	if (toUTF8)
	{
		if (highOrLow == 0x2F)
		{
			return true;
		}

	} // 若是转到UTF8，则加入0x2F，在此之后，若还是CK2，则去掉0x20；即除(转到UTF8并且是CK2)之外，都要添加0x20.
	if ((!toUTF8 || isEU4) && highOrLow == 0x20)
	{
		return true;
	}
	return false;
}




