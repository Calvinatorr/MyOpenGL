#pragma once

#include "Utility.h"


// JSON serialization
#include <nlohmann/json.hpp>
using json = nlohmann::json;


// GLM Mathematics library
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp> // after <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include "glm/gtx/string_cast.hpp"



// ===================================== REFLECTION ============================================

#include <type_traits> // C++11 (Boost)
#include <regex>

// https://stackoverflow.com/questions/41453/how-can-i-add-reflection-to-a-c-application

#define REM(...) __VA_ARGS__
#define EAT(...)

// Retrieve the type
#define TYPEOF(x) DETAIL_TYPEOF(DETAIL_TYPEOF_PROBE x,)
#define DETAIL_TYPEOF(...) DETAIL_TYPEOF_HEAD(__VA_ARGS__)
#define DETAIL_TYPEOF_HEAD(x, ...) REM x
// Strip off the type
#define STRIP(x) EAT x
// Show tyhe type without parentheses
#define PAIR(x) REM x


// Helper metafunction for adding const to a type
template<class M, class T>
struct make_const
{
	typedef T type;
};

template<class M, class T>
struct make_const<const M, T>
{
	typedef typename std::add_const<T>::type type;
};


#define REFLECTABLE(...) \
static const int fields_n = sizeof(__VA_ARGS__); \
friend struct reflector; \
template<int N, class Self> \
struct field data{}; \
BOOST_PP_SEQ_FOR_EACH_I(REFLECT_EACH, data, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))


#define REFLECT_EACH(r, data, i, x) \
PAIR(x); \
template<class Self> \
struct field_data<i, Self> \
{ \
	Self & self; \
	field_data(Self & self) : self(self) {} \
	\
	typename make_const<Self, TYPEOF(x)::type & get() \
	{ \
		return self.STRIP(x); \
	} \
	typename std::add_const<TYPEOF(x)>::type & get() const \
	{ \
		return self.STRIP(x); \
	} \
	const char * name() const \
	{ \
		return BOOST_PP_STRINGIZE(STRIP(x)); \
	} \
}; \


// ===================================== SERIALIZATION ============================================

// Serialize type to JSON data
template<typename T>
json SerializeValue(const T& Value);


// Serialize generic type to JSON data
template<typename T>
inline json SerializeValue(const T & Value)
{
	json jsonData;
	jsonData << Value;
	return jsonData;
}