#include "include/persistent-segment-tree.h"

#include <vector>
#include <queue>

#include <iostream>

class CRollback
{

	public:
		CRollback(const std::vector<size_t> &, size_t);

		size_t Find(size_t, size_t) const;

	private:
		CPersistentSegmentTree<size_t, std::plus<size_t>, 0> tree;
		size_t n;

};

CRollback::CRollback(const std::vector<size_t> &a, size_t m):
	tree(),
	n(a.size())
{
	std::vector<size_t> b(n, 0);
	std::vector<bool> is_new(m, true);
	std::vector<std::queue<size_t> > positions(m);
	for (size_t i = 0; i < n; ++i)
	{
		positions[a[i]].push(i);
		if (is_new[a[i]])
		{
			b[i] = 1;
			is_new[a[i]] = false;
		}
	}
	tree = CPersistentSegmentTree<size_t, std::plus<size_t>, 0>(b.begin(), b.end());
	for (size_t i = 0; i < n; ++i)
	{
		tree.Change(i, 0);
		if (!positions[a[i]].empty() && positions[a[i]].front() == i)
		{
			positions[a[i]].pop();
		}
		if (!positions[a[i]].empty())
		{
			tree.Change(positions[a[i]].front(), 1);
		} else {
			tree.Change(i, 0);
		}
	}
}

size_t CRollback::Find(size_t l, size_t k) const
{
	if (tree(l, n - 1, 2 * l) < k)
	{
		return (size_t)-1;
	}

	size_t left = l;
	size_t right = n;
	size_t middle;
	for (; left < right; )
	{
		middle = left + (right - left) / 2;
		if (k <= tree(l, middle, 2 * l))
		{
			right = middle;
		} else {
			left = middle + 1;
		}
	}
	if (k == tree(l, right, 2 * l))
	{
		return right;
	} else {
		return (size_t)-1;
	}
}

int main()
{
	size_t n, m, q, p = 0;
	std::cin >> n >> m;
	std::vector<size_t> a(n);
	for (size_t i = 0; i < n; ++i)
	{
		std::cin >> a[i];
		--a[i];
	}
	CRollback rollback(a, m);
	std::cin >> q;
	for (size_t i = 0; i < q; ++i)
	{
		size_t x, y, l, k;
		std::cin >> x >> y;
		l = (x + p) % n;
		k = ((y + p) % m) + 1;
		p = rollback.Find(l, k) + 1;
		std::cout << p << std::endl;
	}
	return 0;
}
