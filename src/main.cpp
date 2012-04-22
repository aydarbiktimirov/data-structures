#include "include/persistent-segment-tree.h"

#include <iostream>
#include <functional>

int main()
{
	std::cout << "array size: ";
	size_t n;
	std::cin >> n;
	std::vector<int> a(n);
	std::cout << "array: ";
	for (size_t i = 0; i < a.size(); ++i)
	{
		std::cin >> a[i];
	}

	CPersistentSegmentTree<int, std::plus<int>, 0> tree(a.begin(), a.end());
	for (std::string cmd; !std::cin.eof(); )
	{
		std::cout << "> ";
		std::cin >> cmd;
		if (cmd == "help")
		{
			std::cout << "\tprint\t\tprint current version of a" << std::endl;
			std::cout << "\tversion count\tprint number of versions" << std::endl;
			std::cout << "\tversion show\tprint current version" << std::endl;
			std::cout << "\tversion set i\tselect i'th version of segment tree" << std::endl;
			std::cout << "\tchange i x\tchange i'th element to x" << std::endl;
			std::cout << "\tsum i j\t\tprint sum from i'th element to j'th" << std::endl;
			std::cout << "\texit\t\texit" << std::endl;
		} else if (cmd == "print") {
			std::cout << "\t";
			for (size_t i = 0; i < a.size(); ++i)
			{
				std::cout << tree(i, i) << " ";
			}
			std::cout << std::endl;
		} else if (cmd == "version") {
			std::cin >> cmd;
			if (cmd == "count")
			{
				std::cout << "\t" << tree.GetVersionCount() << std::endl;
			} else if (cmd == "set") {
				size_t v;
				std::cin >> v;
				tree.SetVersion(v);
			} else if (cmd == "show") {
				std::cout << tree.GetCurrentVersion() << std::endl;
			} else {
				std::cout << "\tincorrect command" << std::endl;
			}
		} else if (cmd == "change") {
			size_t i;
			int v;
			std::cin >> i >> v;
			tree.Change(i, v);
		} else if (cmd == "sum") {
			size_t i, j;
			std::cin >> i >> j;
			std::cout << "\t" << tree(i, j) << std::endl;
		} else if (cmd == "exit") {
			break;
		} else {
			std::cout << "\tincorrect command" << std::endl;
		}
	}

	return 0;
}
