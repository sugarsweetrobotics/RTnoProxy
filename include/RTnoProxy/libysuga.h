#ifndef LIBYSUGA_HEADER_INCLUDED
#define LIBYSUGA_HEADER_INCLUDED



#ifdef WIN32

#ifdef LIBYSUGA_EXPORTS
#define LIBYSUGA_API //__declspec(dllexport)
#else  // ifdef LIBYSUGA_EXPORTS
#ifdef LIBYSUGA_STATIC_EXPORTS
#define LIBYSUGA_API 
#else  // ifdef LIBYSUGA_STATIC_EXPORTS
#ifdef _LIB
#define LIBYSUGA_API
#else
#define LIBYSUGA_API //__declspec(dllimport)
#endif // ifdef _LIB
#endif // ifdef LIBYSUGA_STATIC_EXPORTS
#endif // ifdef LIBYSUGA_EXPORTS

#else  // ifdef WIN32
#define LIBYSUGA_API 
#endif // ifdef WIN32




#endif // #ifndef LIBYSUGA_HEADER_INCLUDED
