#pragma once

#include <vector>

#include <stdexcept>

template<typename T, typename Operation>
class ISegmentTree
{

	public:
		virtual ~ISegmentTree() {}

		virtual ISegmentTree<T, Operation> &Change(size_t, const T &) = 0;
		virtual T operator()(size_t, size_t) const = 0; // вычисление значения операции Operation для диапазона элементов

};

// -----

template<typename T, typename Operation, T DefaultValue>
class CSegmentTree:
	public ISegmentTree<T, Operation>
{

	public:
		CSegmentTree();
		template<typename InputIterator>
		CSegmentTree(InputIterator, InputIterator);

		virtual CSegmentTree<T, Operation, DefaultValue> &Change(size_t, const T &);
		virtual T operator()(size_t, size_t) const;

	private:
		std::vector<T> values;
		size_t element_count;
		size_t values_first_index;
		Operation operation;

		template<typename InputIterator>
		void BuildTree(size_t, size_t, InputIterator &, InputIterator);

		T Calculate(size_t, size_t, size_t, size_t, size_t) const;

};

template<typename T, typename Operation, T DefaultValue>
CSegmentTree<T, Operation, DefaultValue>::CSegmentTree():
	values(),
	element_count(0),
	values((size_t)-1),
	operation()
{
}

template<typename T, typename Operation, T DefaultValue>
template<typename InputIterator>
CSegmentTree<T, Operation, DefaultValue>::CSegmentTree(InputIterator from, InputIterator to):
	values(),
	element_count((size_t)(to - from)),
	values_first_index((size_t)-1),
	operation()
{
	BuildTree(0, 1, from, to);
}

template<typename T, typename Operation, T DefaultValue>
CSegmentTree<T, Operation, DefaultValue> &CSegmentTree<T, Operation, DefaultValue>::Change(size_t i, const T &new_value)
{
	if (i >= element_count)
	{
		throw std::out_of_range("index out of bounds");
	}

	i += values_first_index;
	values[i] = new_value;
	for (; i > 0; )
	{
		size_t parent = (i - 1) / 2;
		values[parent] = operation(values[2 * parent + 1], values[2 * parent + 2]); // обновление значений предков
		i = parent;
	}

	return *this;
}

template<typename T, typename Operation, T DefaultValue>
T CSegmentTree<T, Operation, DefaultValue>::operator()(size_t left, size_t right) const
{
	if (left > right)
	{
		return operator()(right, left);
	}
	if (right >= element_count)
	{
		throw std::out_of_range("index out of bounds");
	}

	return Calculate(left, right, 0, 0, values_first_index);
}

template<typename T, typename Operation, T DefaultValue>
template<typename InputIterator>
void CSegmentTree<T, Operation, DefaultValue>::BuildTree(size_t current, size_t current_depth_count, InputIterator &from, InputIterator to)
{ // построение поддерева с корнем current
	values.resize(std::max(values.size(), current + 1));

	if (current_depth_count >= element_count)
	{
		if (values_first_index == (size_t)-1)
		{ // запоминаем индекс, с которого начинаются реальные данные
			values_first_index = current;
		}

		if (from != to)
		{
			values[current] = *(from++);
		} else {
			values[current] = DefaultValue;
		}
	} else {
		size_t left = 2 * current + 1;
		size_t right = 2 * current + 2;

		BuildTree(left, current_depth_count * 2, from, to); // построение левого
		BuildTree(right, current_depth_count * 2, from, to); // и правого поддерева

		values[current] = operation(values[left], values[right]); // вычисление значения операции Operation для корня
	}
}

template<typename T, typename Operation, T DefaultValue>
T CSegmentTree<T, Operation, DefaultValue>::Calculate(size_t from, size_t to, size_t current, size_t left_bound, size_t right_bound) const
{ // сумма элементов между left_bound и right_bound хранится в элемнете current
	if (from > to)
	{
		return DefaultValue;
	}
	if (from == left_bound && to == right_bound)
	{
		return values[current];
	}

	size_t middle_bound = (left_bound + right_bound) / 2;
	return operation(Calculate(from, std::min(to, middle_bound), 2 * current + 1, left_bound, middle_bound),
		Calculate(std::max(middle_bound + 1, from), to, 2 * current + 2, middle_bound + 1, right_bound));
}
