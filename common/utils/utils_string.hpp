#ifndef UTILS_UTILS_STRING
#define UTILS_UTILS_STRING

#include <string>
#include <sstream>

namespace utils
{
namespace uts_string
{
inline std::string Bool2Str(bool b_flag)
{
    return b_flag ? "True" : "False";
}

template <typename T>
inline std::string ToString(T t_arg)
{
    std::string strvalue;
    std::string strRet;
    std::stringstream ss;
    ss << t_arg;
    return ss.str();
}
} // namespace uts_string
} // namespace utils

#endif /* UTILS_UTILS_STRING */
