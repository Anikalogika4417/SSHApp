#ifndef GENERATORUUUID_H
#define GENERATORUUUID_H

#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace std;

class GeneratorUuid
{
public:
	static std::string generate();
};

#endif //GENERATORUUUID_H
