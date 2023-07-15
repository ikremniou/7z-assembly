
#ifndef LIB_EXPORT_H
#define LIB_EXPORT_H

#ifdef _WIN32
#   define EXPORTED  __declspec( dllexport )
#else
# define EXPORTED
#endif

#endif /* LIB_EXPORT_H */
