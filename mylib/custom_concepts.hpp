#include <bits/stdc++.h>
using namespace std;

/// EXPAND FROM HERE

template<class T>
concept gnu_integral = requires {
	requires (integral<T> || same_as<__int128_t, T> || same_as<__uint128_t, T>);
};

template<class T>
concept gnu_signed_integral = requires {
	requires (signed_integral<T> || same_as<__int128_t, T>);
};

template<class T>
concept gnu_unsigned_integral = requires {
	requires (unsigned_integral<T> || same_as<__uint128_t, T>);
};