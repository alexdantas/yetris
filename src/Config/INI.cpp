#include <Config/INI.hpp>

#include <cstring>				// strncpy()

INI::INI():
	ini(nullptr)
{ }
INI::~INI()
{
	this->free();
}

bool INI::load(std::string file)
{
	this->ini = iniparser_load(file.c_str());

	return (this->ini ?
	        true :
	        false);
}
void INI::free()
{
	if (this->ini)
	{
		iniparser_freedict(this->ini);
		this->ini = nullptr;
	}
}
bool INI::get(std::string where, bool default_value)
{
	int test = -666;

	test = iniparser_getboolean(this->ini, where.c_str(), -1);

	if (test == -666)
		return default_value;

	return ((test == 1) ?
	        true:
	        false);
}
int INI::get(std::string where, int default_value)
{
	return iniparser_getint(this->ini, where.c_str(), default_value);
}
double INI::get(std::string where, double default_value)
{
	return iniparser_getdouble(this->ini, where.c_str(), default_value);
}
std::string INI::get(std::string where, char* default_value)
{
	return iniparser_getstring(this->ini, where.c_str(), default_value);
}
std::string INI::get(std::string where, std::string default_value)
{
	// All this because I can't convert `const char*` to `char*`
	size_t len = default_value.size();

	char s[len+1];

	strncpy(s, default_value.c_str(), len);

	// Finally!
	return iniparser_getstring(this->ini, where.c_str(), s);
}
void INI::save(std::string file)
{
	FILE* fp = fopen(file.c_str(), "r");
	if (fp)
	{
		this->save(fp);
		fclose(fp);
	}
}
void INI::save(FILE* file)
{
	if (! file)
		return;

	iniparser_dump_ini(this->ini, file);
}

