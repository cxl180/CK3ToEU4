#ifndef LOC_DEGRADER_MAPPER
#define LOC_DEGRADER_MAPPER
#include "Parser.h"

namespace mappers
{
class LocDegraderMapper: commonItems::parser
{
  public:
	LocDegraderMapper();
	explicit LocDegraderMapper(std::istream& theStream);

	[[nodiscard]] std::string degradeString(const std::string& inputString) const;

  private:
	void registerKeys();
	std::string eU4dllSupport(wchar_t ch, bool toUtf8, bool newVersion);
	std::map<std::string, std::string> replacements;
};
} // namespace mappers

#endif // ISLAM_OVERRIDE_MAPPER