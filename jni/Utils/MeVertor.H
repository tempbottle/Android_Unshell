/*
 * MeVertor.h
 *
 *  Created on: 2015年12月5日
 *      Author: LsMouse
 */

#ifndef MEVERTOR_H_
#define MEVERTOR_H_
#include <stdio.h>
#include <stddef.h>
//建立一个单向列表，供容器建立
typedef struct Item_MeVertor{
	struct Item_MeVertor* next;
	void*  data;
	Item_MeVertor(void* inData){
		next = NULL;
		data = inData;
	}
}Item_MeVertor;
/*
************************************************************
*			MeVertor
* 一个简单容器使用
************************************************************
*/
template <typename Type_>
class MeVertor{
public:
	Item_MeVertor* First = NULL;
	Item_MeVertor* Last = NULL;
	Item_MeVertor* pType = NULL;
	size_t VertorSize = 0;
/*
************************************************************
*				释放内存
*
************************************************************
*/
	void Free(Item_MeVertor* inItem)
	{
		if(inItem == NULL)
			return ;
		Item_MeVertor* mNext =inItem->next;
		free(inItem);
		Free(mNext);
	}
/*
************************************************************
*				清空整个容器
*  注意这里只是修改指针并没有释放内存
************************************************************
*/
	void Clear(){
	//	Free(First);
		First = NULL;
		Last = NULL;
		pType = NULL;
		VertorSize = 0;
	}
/*
************************************************************
*				复位局部指针
************************************************************
*/
	void Reset(){
		pType = First;
	}
/*
************************************************************
*				获取链表宽度
************************************************************
*/
	size_t GetSize(){
		return VertorSize;
	}
/*
************************************************************
*				获取局部指针变量
************************************************************
*/
	Type_* Get(){
		if(pType == NULL)
			return NULL;
		return (Type_*)pType->data;
	}
/*
************************************************************
*				获取局部指针变量
************************************************************
*/
	Type_* Get(int inNo){
		Item_MeVertor* mType = First;
		for(int m_i=0;m_i < inNo;m_i++){
			if(mType == NULL)return NULL;
			mType = mType->next;
		}
		return (Type_*)mType->data;
	}
/*
************************************************************
*				获取局部指针变量
************************************************************
*/
	void Set(void* inData)
	{
		if(pType == NULL)
			return ;
		pType->data = inData;
	}
/*
************************************************************
*将局部指针指向下一个数据
************************************************************
*/
	void Next(){
		if(pType == NULL)return ;
		pType = pType->next;
	}
/*
************************************************************
*向内存列表中添加一个数据
************************************************************
*/
	void Add(Type_* inType){
		if(First == NULL){
			First = new Item_MeVertor(inType);
			Last = First;
			VertorSize = 1;
			return ;
		}
		Item_MeVertor* mLink = new Item_MeVertor(inType);
		Last->next = mLink;
		Last = mLink;
		VertorSize++;
	}
};


#endif /* MEVERTOR_H_ */
