#include "GeneratorUuid.h"

string GeneratorUuid::generate()
{
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    return boost::uuids::to_string(uuid);
}
