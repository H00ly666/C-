
#include "json.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <streambuf>
#include <string>
#include <sstream>
#include <map>
#include <queue>
using namespace std;
using json::JSON;


JSON getJsonFromFile(string path) {
	string json_content;
	ifstream input(path);
	input.seekg(0, ios::end);
	json_content.reserve(input.tellg());
	input.seekg(0, ios::beg);
	json_content.assign((istreambuf_iterator<char>(input)), istreambuf_iterator<char>());
	JSON json_data = JSON::Load(json_content);
	return json_data;
}

//������ת��Ϊ�ַ���
string intToString(int i) {
	stringstream ss;
	ss << i;
	return ss.str();
}

//�������ڵ�
class BiTreeNode {
public:
	int val; //�ֵ�
	string index; //����
	BiTreeNode* left;
	BiTreeNode* right;
	BiTreeNode(int v, string i, BiTreeNode* l = NULL, BiTreeNode* r = NULL) {
		val = v;
		index = i;
		left = l;
		right = r;
	}
};

//����JSON���ݵݹ鴴��������
BiTreeNode* createTreeByJson(BiTreeNode* this_node, JSON tree_data, string index) {
	int value = tree_data[index]["value"].ToInt();
	string l_index = tree_data[index]["left"].ToString();
	string r_index = tree_data[index]["right"].ToString();

	this_node = new BiTreeNode(value, index, NULL, NULL);
	if (l_index != "") {
		this_node->left = createTreeByJson(this_node->left, tree_data, l_index);
	}
	if (r_index != "") {
		this_node->right = createTreeByJson(this_node->right, tree_data, r_index);
	}

	return this_node;
}


//����ڵ��ֵ
void printValue(BiTreeNode* root) {
	cout << root->val;
}


//ǰ�����������
void preOrderTravel(BiTreeNode* root) {
	if (root == NULL)
		return;
	printValue(root);
	preOrderTravel(root->left);
	preOrderTravel(root->right);
}


//�������������
void inOrderTravel(BiTreeNode* root) {
	if (root == NULL)
		return;
	inOrderTravel(root->left);
	printValue(root);
	inOrderTravel(root->right);
}


//�������������
void postOrderTravel(BiTreeNode* root) {
	if (root == NULL)
		return;
	postOrderTravel(root->left);
	postOrderTravel(root->right);
	printValue(root);

}


//�������������
void LevelOrderTravel(BiTreeNode* root) {
	if (root == NULL)
		return;
	queue<BiTreeNode*> q;
	q.push(root);
	while (q.empty() == false) {
		root = q.front();
		printValue(root);
		q.pop();
		if (root->left)
			q.push(root->left);
		if (root->right)
			q.push(root->right);
	}
}


//�ж����ö������Ƿ���ͬ
bool TreeStructCmp(BiTreeNode* root1, BiTreeNode* root2) {
	if (root1 == NULL && root2 == NULL)
		return true;
	if (root1->val != root2->val)
		return false;
	bool leftResult = TreeStructCmp(root1->left, root2->left);
	bool rightResult = TreeStructCmp(root1->right, root2->right);
	return leftResult && rightResult;
}


//�������������-��������
void testPrintBiTree(BiTreeNode* T) {
	cout << "�ڵ�" << T->val << "(" << T->index << ")";
	if (T->left == NULL) {
		cout << "û��������" << endl;
	}
	else {
		cout << "������" << T->left->val << "(" << T->left->index << ")" << endl;
		testPrintBiTree(T->left);
	}
	cout << "�ڵ�" << T->val << "(" << T->index << ")";
	if (T->right == NULL) {
		cout << "û��������" << endl;
	}
	else {
		cout << "���Һ���" << T->right->val << "(" << T->right->index << ")" << endl;
		testPrintBiTree(T->right);
	}
}


//�ݹ�����json����
void _treeToJson(BiTreeNode* T, JSON& j, int& index) {
	int i = index;
	j[i]["value"] = T->val;
	//j[i]["index"] = intToString(index);
	if (T->left != NULL) {
		j[i]["left"] = intToString(index + 1);
		_treeToJson(T->left, j, ++index);
	}
	if (T->right != NULL) {
		j[i]["right"] = intToString(index + 1);
		_treeToJson(T->right, j, ++index);
	}
}

//�������ת��ΪJson����
JSON treeToJson(BiTreeNode* T) {
	JSON j;
	int index = 0;
	//��TΪ���ڵ㣬��0��ʼ���¹���ڵ�����������JSON����
	_treeToJson(T, j, index);
	return j;
}


//��json����ת��ΪΨһ���ַ���
string jsonToString(JSON arr) {
	string jstr = "";
	int index = 0;
	//��������json���飬������ֵȡ��ƴ�ӳ��ַ���
	for (auto &j : arr.ArrayRange()) {
		jstr += intToString(index) + "|";
		jstr += j["value"].ToString() + "|";
		jstr += j["left"].ToString() + "|";
		jstr += j["right"].ToString() + "|";
		index++;
	}
	//���ص��ַ�������Ψһ�ö�����
	return jstr;
}


//�ݹ������������
void searchChildTrees(BiTreeNode* T, map<string, int> &treeCount, map<string, JSON> &treeJSON) {
	//���Ա����Ϊ������ת��Ϊһ��json���飬�ٽ����json����ת����Ψһ���ַ�������
	JSON jArr = treeToJson(T);
	string treeKey = jsonToString(jArr);

	//�����������-����ӳ���У������ҵ��������ɵ�����Ϊkey��ӳ��
	//�������µ�����-����ӳ�䣬����-jsonӳ�䡣
	if (treeCount.count(treeKey) == 0) {
		treeCount.insert(pair<string, int>(treeKey, 1));
		treeJSON.insert(pair<string, JSON>(treeKey, jArr));
	}
	//����ʹ���������е����У��������ֳ�����һ��
	else {
		treeCount[treeKey] += 1;
	}

	if (T->left != NULL) {
		searchChildTrees(T->left, treeCount, treeJSON);
	}
	if (T->right != NULL) {
		searchChildTrees(T->right, treeCount, treeJSON);
	}
}


//��ȡ����������������Json
JSON getAllChildTreesJson(BiTreeNode* T) {
	//��������-����ӳ�䣬����-jsonӳ��
	map<string, int> treeCount;
	map<string, JSON> treeJSON;
	searchChildTrees(T, treeCount, treeJSON);

	JSON res;
	int index = 0;
	map<string, int>::iterator iter;
	//������-����-json����Ҫ���ʽ��json
	for (iter = treeCount.begin(); iter != treeCount.end(); iter++, index++) {
		res[index]["count"] = iter->second;
		res[index]["subtree"] = treeJSON[iter->first];
	}
	return res;
}


//��ȡ�������ڵ����
int getNums(BiTreeNode* root) {
	if (root == NULL)
		return 0;
	return getNums(root->left) + getNums(root->right) + 1;
}


//��ȡ������Ҷ�ڵ����
int getLeafNodeNums(BiTreeNode* root) {
	if (root == NULL)
		return 0;
	if (root->left == NULL && root->right == NULL)
		return 1;
	int l_nums = getLeafNodeNums(root->left);
	int r_nums = getLeafNodeNums(root->right);
	return l_nums + r_nums;
}


// ������������
int getDepth(BiTreeNode* root) {
	if (root == NULL)
		return 0;
	return max(getDepth(root->left), getDepth(root->right)) + 1;
}


// ���������K��Ľڵ����
int getNLevelNums(BiTreeNode* root, int k) {
	if (root == NULL || k == 0)
		return 0;
	if (k == 1)
		return 1;
	// ��������K��Ľڵ���� = ������K-1��Ľڵ���� + ������K-1��Ľڵ����
	return getNLevelNums(root->left, k - 1) + getNLevelNums(root->right, k - 1);
}


int main()
{
	string json_path = "./bt_ABCD_10_2.json.txt";
	JSON json_data = getJsonFromFile(json_path);

	BiTreeNode* BiTree = createTreeByJson(NULL, json_data, "0");
	cout << getAllChildTreesJson(BiTree) << endl;
	//cout << getNLevelNums(BiTree, 50) << endl;

	system("pause");
	return 0;
}


