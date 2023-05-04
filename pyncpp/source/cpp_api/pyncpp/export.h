// Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifdef WIN32
    #ifdef pyncpp_cpp_api_EXPORTS
        #define PYNCPP_EXPORT __declspec(dllexport)
    #else
        #define PYNCPP_EXPORT __declspec(dllimport)
    #endif
#else
    #define PYNCPP_EXPORT
#endif
