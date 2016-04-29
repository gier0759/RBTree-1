#include"RBTree.h"


void TestRBTree()
{
	int array[] = { 4, 5, 13, 28, 5, 19, 18, 37, 10, 20 };
	RBTree<int, int> t;
	for (size_t i = 0; i < 10; ++i)
	{
		t.Insert(array[i], array[i]);
	}

	t.InOrder();

	cout << endl;
	cout << "IsBlance?" << " " << t.IsBlance() << endl;
}

int main()
{
	TestRBTree();
	system("pause");
	return 0;
}