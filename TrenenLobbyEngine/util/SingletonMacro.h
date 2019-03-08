#pragma once

// Implement Singletons
#define Singleton(TYPE) \
	public: \
	static TYPE* Get() { return &Single; } \
	private: \
	static TYPE Single;

#define Singleton_CPP(TYPE) \
	TYPE TYPE::Single;
