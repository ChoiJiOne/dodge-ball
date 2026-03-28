#pragma once

#include <utility>
#include <variant>

#include "Error.h"

template <typename T>
class Result
{
public:
	static Result Success(T value) { return Result(std::in_place_index<0>, std::move(value)); }
	static Result Fail(Error error) { return Result(std::in_place_index<1>, std::move(error)); }

	bool IsSuccess() const { return std::holds_alternative<T>(_data); }

	T& GetValue() { return std::get<T>(_data); }
	const T& GetValue() const { return std::get<T>(_data); }
	const Error& GetError() const { return std::get<Error>(_data); }

private:
	explicit Result(std::in_place_index_t<0>, T value) : _data(std::in_place_index<0>, std::move(value)) {}
	explicit Result(std::in_place_index_t<1>, Error error) : _data(std::in_place_index<1>, std::move(error)) {}

private:
	std::variant<T, Error> _data;
};

template <>
class Result<void>
{
public:
	static Result Success() { return Result(std::in_place_index<0>); }
	static Result Fail(Error error) { return Result(std::in_place_index<1>, std::move(error)); }

	bool IsSuccess() const { return std::holds_alternative<std::monostate>(_data); }

	const Error& GetError() const { return std::get<Error>(_data); }

private:
	explicit Result(std::in_place_index_t<0>) : _data(std::in_place_index<0>) {}
	explicit Result(std::in_place_index_t<1>, Error error) : _data(std::in_place_index<1>, std::move(error)) {}

private:
	std::variant<std::monostate, Error> _data;
};
