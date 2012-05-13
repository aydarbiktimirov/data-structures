#pragma once

#include "persistent-data-structure.h"
#include "segment-tree.h"

#include <vector>

#include <stdexcept>

template<typename T, typename Operation>
class CPersistentSegmentTree:
	public CAbstractPersistentDataStructure, public ISegmentTree<T, Operation>
{

	public:
		CPersistentSegmentTree(const T & = T());
		template<typename InputIterator>
		CPersistentSegmentTree(InputIterator, InputIterator, const T &);

		virtual CPersistentSegmentTree<T, Operation> &Change(size_t, const T &);
		virtual T operator()(size_t, size_t) const;
		T operator()(size_t, size_t, size_t) const;

	private:
		struct CNode
		{
			CNode(const T &v = T(), size_t l = (size_t)-1, size_t r = (size_t)-1): value(v), left(l), right(r) {}
			T value;
			size_t left;
			size_t right;
		};

		size_t element_count;
		size_t values_first_index;
		std::vector<CNode> values;
		std::vector<size_t> roots;

		T default_value;

		template<typename InputIterator>
		void BuildTree(size_t, size_t, InputIterator &, InputIterator);

		T Calculate(size_t, size_t, size_t, size_t, size_t) const;

		void AddBranch(size_t, const T &, size_t, size_t, size_t, size_t);

};

template<typename T, typename Operation>
CPersistentSegmentTree<T, Operation>::CPersistentSegmentTree(const T &_default_value):
	element_count(0),
	values_first_index((size_t)-1),
	values(),
	roots(),
	default_value(_default_value)
{
}

template<typename T, typename Operation>
template<typename InputIterator>
CPersistentSegmentTree<T, Operation>::CPersistentSegmentTree(InputIterator from, InputIterator to, const T &_default_value):
	element_count((size_t)(to - from)),
	values_first_index((size_t)-1),
	values(),
	roots(1, 0),
	default_value(_default_value)
{
	BuildTree(0, 1, from, to);
}

template<typename T, typename Operation>
CPersistentSegmentTree<T, Operation> &CPersistentSegmentTree<T, Operation>::Change(size_t i, const T &new_value)
{
	roots.push_back(values.size());
	AddBranch(i, new_value, roots[this->GetCurrentVersion()], 1, 0, values_first_index);
	return static_cast<CPersistentSegmentTree<T, Operation> &>(this->SetVersion(this->AddVersion() - 1));
}

template<typename T, typename Operation>
T CPersistentSegmentTree<T, Operation>::operator()(size_t left, size_t right) const
{
	return operator()(left, right, this->GetCurrentVersion());
}

template<typename T, typename Operation>
T CPersistentSegmentTree<T, Operation>::operator()(size_t left, size_t right, size_t version) const
{
	if (version >= this->GetVersionCount())
	{
		throw std::out_of_range("version doesn't exist");
	}
	if (left > right)
	{
		return operator()(right, left);
	}
	if (right >= element_count)
	{
		throw std::out_of_range("index out of bounds");
	}

	return Calculate(left, right, roots[version], 0, values_first_index);
}

template<typename T, typename Operation>
template<typename InputIterator>
void CPersistentSegmentTree<T, Operation>::BuildTree(size_t current, size_t current_depth_count, InputIterator &from, InputIterator to)
{ // построение поддерева, аналогично построению в эфемерном дереве отрезков
	values.resize(std::max(values.size(), current + 1));

	if (current_depth_count >= element_count)
	{
		if (values_first_index == (size_t)-1)
		{
			values_first_index = current;
		}

		if (from != to)
		{
			values[current] = CNode(*(from++));
		} else {
			values[current] = CNode(default_value);
		}
	} else {
		size_t left = 2 * current + 1;
		size_t right = 2 * current + 2;

		BuildTree(left, current_depth_count * 2, from, to);
		BuildTree(right, current_depth_count * 2, from, to);

		values[current] = CNode(Operation()(values[left].value, values[right].value), left, right);
	}
}

template<typename T, typename Operation>
T CPersistentSegmentTree<T, Operation>::Calculate(size_t from, size_t to, size_t current, size_t left_bound, size_t right_bound) const
{ // сумма элементов между left_bound и right_bound хранится в элемнете current
	if (from > to)
	{
		return default_value;
	}
	if (from == left_bound && to == right_bound)
	{
		return values[current].value;
	}
	size_t middle_bound = (left_bound + right_bound) / 2;
	return Operation()(Calculate(from, std::min(to, middle_bound), values[current].left, left_bound, middle_bound),
		Calculate(std::max(middle_bound + 1, from), to, values[current].right, middle_bound + 1, right_bound));
}

template<typename T, typename Operation>
void CPersistentSegmentTree<T, Operation>::AddBranch(size_t changed_position, const T &changed_value, size_t previous, size_t current_depth_count, size_t left_bound, size_t right_bound)
{ // добавление новой ветки рекурсивно сверху вниз
	size_t new_node = values.size();
	values.push_back(CNode());

	if (current_depth_count >= element_count)
	{ // добрались до листа
		values[new_node] = CNode(changed_value);
	} else {
		size_t middle_bound = (left_bound + right_bound) / 2; // добавляем поддеревья в зависимости от того, которое из поддеревьев изменилось
		if (changed_position <= middle_bound)
		{
			values[new_node].left = values.size();
			values[new_node].right = values[previous].right;
			AddBranch(changed_position, changed_value, values[previous].left, 2 * current_depth_count, left_bound, middle_bound);
		} else {
			values[new_node].left = values[previous].left;
			values[new_node].right = values.size();
			AddBranch(changed_position, changed_value, values[previous].right, 2 * current_depth_count, middle_bound + 1, right_bound);
		}

		values[new_node].value = Operation()(values[values[new_node].left].value, values[values[new_node].right].value); // вычисление значения для новой вершины
	}
}
