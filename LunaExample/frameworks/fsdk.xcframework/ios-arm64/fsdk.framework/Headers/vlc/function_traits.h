#pragma once

#include <type_traits>
#include <tuple>
#include <functional>

namespace vlc
{
    template <typename T>
    struct function_traits
        : public function_traits<decltype(&T::operator())>
    {};

    template<typename R, typename ...Args>
    struct function_traits<R(Args...)>
    {
        static const size_t nargs = sizeof...(Args);

        using result_type = R;
        using function_type = R(Args...);

        template <size_t i>
        struct arg
        {
            using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
        };
    };

    template <typename ReturnType, typename... Args>
    struct function_traits<ReturnType(*)(Args...)>
        : public function_traits<ReturnType(Args...)>
    {};

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType(ClassType::*)(Args...)>
        : public function_traits<ReturnType(Args...)>
    {
        typedef ClassType& owner_type;
    };

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType(ClassType::*)(Args...) const>
        : public function_traits<ReturnType(Args...)>
    {
        typedef const ClassType& owner_type;
    };

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType(ClassType::*)(Args...) volatile>
        : public function_traits<ReturnType(Args...)>
    {
        typedef volatile ClassType& owner_type;
    };

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits<ReturnType(ClassType::*)(Args...) const volatile>
        : public function_traits<ReturnType(Args...)>
    {
        typedef const volatile ClassType& owner_type;
    };

    template <typename FunctionType>
    struct function_traits<std::function<FunctionType>>
        : public function_traits<FunctionType>
    {};
}
