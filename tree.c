#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

typedef DWORD attr;

typedef struct tree_node_ {
	char name[MAX_PATH];
	attr type;
	struct tree_node_* parent;
	struct tree_node_* sibling;
	struct tree_node_* child;
}tree_node;

DWORD get_current_dir(char* buff, int bufflen);
tree_node* tree_init_a_node(WIN32_FIND_DATA* data);
void tree_insert_node(tree_node** node, WIN32_FIND_DATA* data);
tree_node* tree_search(tree_node** node, const char* current_dir);
void tree_print(const tree_node* node, int blk, char* sharp);
void tree_free(tree_node* node);


int main(int agrc, char* argv[])
{
	tree_node* root = NULL;
	char current_dir[MAX_PATH] = { 0 };

	root = (tree_node*)malloc(sizeof(tree_node));
	root->sibling = root->child = NULL;
	memset(root->name, 0, MAX_PATH);
	get_current_dir(current_dir, MAX_PATH);
	memmove(root->name, current_dir, strlen(current_dir));
	root->type = FILE_ATTRIBUTE_DIRECTORY;

	tree_search(&root, current_dir);
	tree_print(root, 0, "|__");
	system("PAUSE");
	return 0;
}

DWORD get_current_dir(char* buff, int bufflen)
{
	return GetCurrentDirectory(bufflen, buff);
}

tree_node* tree_init_a_node(WIN32_FIND_DATA* data)
{
	tree_node* node = NULL;

	node = (tree_node*)malloc(sizeof(tree_node));

	if (node == NULL)
	{
		printf("内存分配失败\n");
		return NULL;
	}
	//指针指向空
	node->parent = node->sibling = node->child = NULL;
	//name属性
	memset(node->name, 0, MAX_PATH);
	memmove(node->name, data->cFileName, strlen(data->cFileName));
	//type属性
	node->type = data->dwFileAttributes;

	return node;
}

void tree_insert_node(tree_node** node, WIN32_FIND_DATA* data)
{
	if (*node != NULL)
	{
		tree_insert_node(&((*node)->sibling), data);
	}
	else
	{
		*node = tree_init_a_node(data);
	}
}

tree_node* tree_search(tree_node** node, const char* current_dir)
{
	tree_node** temp = NULL;
	char wildcard[MAX_PATH] = { 0 };
	char sub_dir[MAX_PATH] = { 0 };
	HANDLE hfile;
	WIN32_FIND_DATA file_data;

	sprintf(wildcard, "%s\\*", current_dir);
	hfile = FindFirstFile(wildcard, &file_data);

	if (hfile == INVALID_HANDLE_VALUE)
	{
		printf("搜索文件失败%ld\n", GetLastError());
		return NULL;
	}
	else
	{
		do {
			//过滤 "." 和 ".."
			if (!(strcmp(file_data.cFileName, ".")) || !(strcmp(file_data.cFileName, "..")))
			{
				continue;
			}
			//printf("%s\n", file_data.cFileName);
			tree_insert_node(node, &file_data);
			if (file_data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				sprintf(sub_dir, "%s\\%s", current_dir, file_data.cFileName);
				temp = node;
				while ((*temp)->sibling != NULL)
				{
					temp = &(*temp)->sibling;
				}
				tree_search(&((*temp)->child), sub_dir);
			}
			temp = NULL;
		} while (FindNextFile(hfile, &file_data));
	}
	return *node;
}

void tree_print(const tree_node* node, int blk, char* sharp)
{
	int i;
	if (node != NULL)
	{
		for (i = 0; i < blk; i++)
		{
			printf("   ");
		}
		printf("%s%s\n", sharp, node->name);
		tree_print(node->child, blk + 1, sharp);
		tree_print(node->sibling, blk, sharp);
	}
}

void tree_free(tree_node* node)
{
	if (node != NULL)
	{
		tree_free(node->sibling);
		tree_free(node->child);
		free(node);
	}
}
