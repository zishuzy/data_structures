#ifndef STRING_UTILS_HPP_H
#define STRING_UTILS_HPP_H

#include <iostream>
#include <sstream>

namespace string_utils
{
inline std::string Bool2str(bool b_flag)
{
    return b_flag ? "success" : "failed";
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

template <typename T>
void stringHelper(std::string &str_fmt, int &n_start, T t_arg)
{
    std::string::size_type position;
    position = str_fmt.find("%", n_start);
    if (position != str_fmt.npos) {
        if (str_fmt.at(position + 1) == '%') {
            str_fmt.erase(position, 1);
            n_start = position + 1;
            stringHelper(str_fmt, n_start, t_arg);
        } else if (str_fmt.at(position + 1) == 'v') {
            std::string strArg = ToString(t_arg);
            str_fmt.replace(position, 2, ToString(t_arg));
            n_start = position + strArg.size();
        }
    } else {
        n_start = str_fmt.size();
    }
}

template <typename T0, typename... T1>
void stringHelper(std::string &str_fmt, int &n_start, T0 t0, T1... args)
{
    stringHelper(str_fmt, n_start, t0);
    if (n_start < (int)str_fmt.size()) {
        stringHelper(str_fmt, n_start, args...);
    }
}

template <typename... T>
std::string fmtString(const std::string &str_fmt, T... args)
{
    std::string strRet = str_fmt;
    int nStart = 0;
    stringHelper(strRet, nStart, args...);
    return strRet;
}
} // namespace string_utils

#endif // STRING_UTILS_HPP_H