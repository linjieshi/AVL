//AVL(自动平衡二叉树)
#include <stdio.h>
#include <stdlib.h>
typedef int ElemType;
//每个结点的平均值
typedef enum
{
	EH = 0,
	LH = 1,
	RH = -1
}bh_t;

typedef enum
{
	FALSE = 0,
	TRUE = 1
}bool_t;

//定义平衡二叉树
typedef struct BSTNode
{
	ElemType key;//平衡值
	int bf;
	struct BSTNode *lchild, *rchild;
}BSTNode, *BSTree;


//中序遍历
void InOrderTraverse(BSTree root)
{
	if (NULL != root)
	{
		InOrderTraverse(root->lchild);
		printf("%d\t", root->key);
		InOrderTraverse(root->rchild);
	}
}


//前序遍历
void PreOrderTraverse(BSTree root)
{
	if (NULL != root)
	{
		printf("%d\t", root->key);
		PreOrderTraverse(root->lchild);
		PreOrderTraverse(root->rchild);
	}
}


//单向右旋转
void R_Rotate(BSTree *p)
{
	BSTree lc = (*p)->lchild;
	(*p)->lchild = lc->rchild;
	lc->rchild = *p;
	*p = lc;
}

//单向左旋转
void L_Rotate(BSTree *p)
{
	BSTree rc = (*p)->rchild;
	(*p)->rchild = rc->lchild;
	rc->lchild = *p;
	*p = rc;
}


//先左旋后右旋平衡旋转
void LeftBalance(BSTree *T)
{
	BSTree lc = (*T)->lchild;
	BSTree rd = lc->rchild;
	//判断进行向哪边旋转
	switch (lc->bf)
	{
	case LH:
		(*T)->bf = lc->bf = EH;
		R_Rotate(T);
		break;
	case RH:
		switch (rd->bf)
		{
		case LH:
			(*T)->bf = RH;
			lc->bf = EH;
			break;
		case EH:
			(*T)->bf = lc->bf = EH;
			break;
		case RH:
			(*T)->bf = EH;
			lc->bf = LH;
			break;
		}
		rd->bf = EH;
		L_Rotate(&((*T)->lchild));
		R_Rotate(T);
		break;
	}
}

//先右旋后左旋平衡旋转
void RightBalance(BSTree *T)
{
	BSTree rc = (*T)->rchild;
	BSTree ld = rc->lchild;
	switch (rc->bf)
	{
	case RH:
		(*T)->bf = rc->bf = EH;
		L_Rotate(T);
		break;
	case LH:
		switch (ld->bf)
		{
		case RH:
			(*T)->bf = LH;
			rc->bf = EH;
			break;
		case EH:
			(*T)->bf = rc->bf = EH;
			break;
		case LH:
			(*T)->bf = EH;
			rc->bf = RH;
			break;
		}
		ld->bf = EH;
		R_Rotate(&((*T)->rchild));
		L_Rotate(T);
		break;
	}
}


//插入元素
bool_t InsertAVL(BSTree *t, ElemType e, bool_t *taller)
{
	if (NULL == t)
		return FALSE;
	if (NULL == *t)
	{
		*t = (BSTree)malloc(sizeof(BSTNode));
		if (NULL == *t)
			return FALSE;
		(*t)->key = e;
		(*t)->lchild = (*t)->rchild = NULL;
		(*t)->bf = EH;
		*taller = TRUE;
	}
	else
	{
		if (e == (*t)->key)
		{
			*taller = FALSE;
			return FALSE;
		}
		if (e < (*t)->key)
		{
			if (FALSE == InsertAVL(&((*t)->lchild), e, taller))
				return FALSE;
			if (*taller)
			{
				switch ((*t)->bf)
				{
				case LH:
					LeftBalance(t);
					*taller = FALSE;
					break;
				case EH:
					(*t)->bf = LH;
					*taller = TRUE;
					break;
				case RH:
					(*t)->bf = EH;
					*taller = FALSE;
					break;
				}
			}
		}
		else
		{
			if (FALSE == InsertAVL(&((*t)->rchild), e, taller))
				return FALSE;
			if (*taller)
			{
				switch ((*t)->bf)
				{
				case RH:
					RightBalance(t);
					*taller = FALSE;
					break;
				case EH:
					(*t)->bf = RH;
					*taller = TRUE;
					break;
				case LH:
					(*t)->bf = EH;
					*taller = FALSE;
					break;
				}
			}
		}
	}
	return TRUE;
}


BSTree searchAVL(BSTree t, ElemType key)
{
	BSTree p = t;
	while (p)
	{
		if (p->key == key)
			return p;
		else if (p->key < key)
			p = p->rchild;
		else
			p = p->lchild;
	}
	return p;
}

static void destroy(BSTree *t)
{
	if (NULL != *t)
	{
		destroy(&((*t)->lchild));
		destroy(&((*t)->rchild));
		free(*t);
		*t = NULL;
	}
	return;
}
void destroyAVL(BSTree root)
{
	if (NULL != root)
	{
		destroy(&root);
	}
	return;
}

int main()
{
	BSTree root = NULL, r;
	bool_t taller = FALSE;
	int array[10000];
	int n,se;
	printf("请输入数组大小：\n");
	scanf_s("%d", &n);
	printf("请输入数组元素：\n");
	for (int i = 0; i < n; i++)
	{
		scanf_s("%d", &array[i]);
	}
	for (int i = 0; i < n; i++)
		InsertAVL(&root, array[i], &taller);
	printf("中序遍历:\n");
	InOrderTraverse(root);
	printf("\n先序遍历\n");
	PreOrderTraverse(root);
	printf("\n请输入搜索元素：\n");
	scanf_s("%d", &se);
	r = searchAVL(root, se);
	if (r)
	{
		printf("%d\n", r->key);
	}
	else
	{
		printf("not find!\n");
	}
	destroyAVL(root);
	root = NULL;
	system("pause");
	return 0;
}