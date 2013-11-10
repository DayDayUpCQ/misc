/**
  \file object_pool.h

  \brief 基于链表的对象缓存池

  \note 使用该用对象池的对象都必须继承自ObjectPoolElem

  \version 1.2
 */
#ifndef _OBJECTPOOL_H_
#define _OBJECTPOOL_H_
extern "C"
{
    #include <assert.h>
    #include <stdio.h>
};

class ObjectPoolElem
{
public:

	ObjectPoolElem()
	{
		m_next = NULL;
	}
	
	explicit ObjectPoolElem( ObjectPoolElem *next )
	{
		m_next = next;
	}
	
	~ObjectPoolElem()
	{
		m_next = NULL;
	}
	
	ObjectPoolElem *GetNext(void)
	{
		return m_next;
	}
	
	void SetNext(ObjectPoolElem *next)
	{
		m_next = next;
	}
private:

        /**
          \brief 指向下一个对象池的元素的指针
         */
        ObjectPoolElem *m_next;
};

/**
  \brief 基于链表实现的对象池
  \note  该对象必须继承自ObjectPoolElem
 */
template <class Type>
class ObjectPool
{
public:

    ObjectPool(int size) 
        : m_listHead(NULL), m_listTail(NULL), m_size(size)
    {
        InitPool(size);
    }

    /**
      \note 使用该模板时,必须保证在该对象析构时所有
            的元素都已使用ReleaseElem()还回到该对象池
            否则会造成内存泄漏
     */
    ~ObjectPool()
    {
        ExitPool();
    }

    /**
      \brief 从对象池获取一个已创建好的元素
     */
    Type *GetObj()
    {
        ObjectPoolElem *eleTmp = NULL;
        if ( m_listHead == NULL ) 
        {
            return NULL;
        }
        eleTmp = m_listHead;
        m_listHead = m_listHead->GetNext();
		if ( m_listHead == NULL )
		{
			m_listTail = NULL;
		}
        m_size --;
        return (Type *)eleTmp;
    }

    /**
      \brief 将不使用的元素返回到对象池
     */
    void ReleaseObj(ObjectPoolElem  *elemPtr )
    {
		elemPtr->SetNext(NULL);

		if ( m_listTail == NULL )
		{
			m_listHead = elemPtr;
			m_listTail = elemPtr;
		}
		else
		{
			m_listTail->SetNext(elemPtr);
			m_listTail = elemPtr;
		}
		m_size ++;
    }

	/**
	  \brief 获取对象池中元素个数
	 */
	int GetSize()
	{
		return m_size;
	}

protected:
    /**
      \brief 初始化对象池
     */
    void InitPool( int size )
    {
        Type *elemTmp = NULL;

        for ( int i = 0; i < size; i++ ) 
        {
            elemTmp = new Type();
            assert(elemTmp);
            if ( m_listHead == NULL ) 
            {
                m_listHead = (ObjectPoolElem *)elemTmp;
                m_listTail = (ObjectPoolElem *)elemTmp;
            }
            else
            {
                elemTmp->SetNext(m_listHead);
                m_listHead = (ObjectPoolElem *)elemTmp;
            }
        }
    }

    void ExitPool(void)
    {
        Type *eleTmp;
        while ( m_listHead != NULL )
        {
            eleTmp = (Type *)m_listHead;
            m_listHead = m_listHead->GetNext();
            delete eleTmp;
        }
        m_listTail = NULL;
    }
private:
    /**
      \brief 对象池队列的头指针
     */
    ObjectPoolElem *m_listHead;

    /**
      \brief 对象池队列的尾指针
     */
    ObjectPoolElem *m_listTail;

    /**
      \brief 缓存池保存对象的缓存单元个数
     */
    int m_size;
};
#endif