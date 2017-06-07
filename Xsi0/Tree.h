#pragma once
#include<vector>

using namespace std;


template <class T> class TreeNode {
private:
	T _data;
	vector<TreeNode> _children;
public:
	T getData();

	
};
