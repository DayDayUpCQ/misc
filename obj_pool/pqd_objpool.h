/**
  \file pqd_objpool.h

  \brief PQDIF基础库对象池
 */
#ifndef _PQD_OBJPOOL_H_
#define _PQD_OBJPOOL_H_

#include "object_pool.h"
#include "el_base.h"
#include "el_vect.h"
#include "el_coll.h"
#include "el_coll2.h"
#include "el_scal.h"
#include "rec_base.h"
#include "rec_general.h"
#include "pqd_config.h"


typedef enum 
{
	RECORD_TYPE_GENERAL,
	RECORD_TYPE_CONTAINER,
	RECORD_TYPE_DATASOURCE,
	RECORD_TYPE_MONITORSETTINGS,
	RECORD_TYPE_OBSERVATION
}RECORD_TYPE;

class PQDObjectPool
{
public:
    static const int NORMAL_COLLECTION_NUM = PQD_CONFIG_NORMAL_COLLECTION_OBJECT_NUM;
	static const int SPECIAL_COLLECTION_NUM = PQD_CONFIG_SPECIAL_COLLECTION_OBJECT_NUM;
    static const int SCALAR_NUM = PQD_CONFIG_SCALAR_OBJECT_NUM;
    static const int VECTOR_NUM = PQD_CONFIG_VECTOR_OBJECT_NUM;
	static const int RECORD_NUM = PQD_CONFIG_RECORD_OBJECT_NUM;

public:

    static PQDObjectPool *GetInstance();

	PQDObjectPool();

	~PQDObjectPool();

    /**
      \brief 获取对象池的物理层对象
      \param[in] elemType 物理层对象类型,取值可为:
                          ID_ELEMENT_TYPE_COLLECTION、
                          ID_ELEMENT_TYPE_SCALAR、
                          ID_ELEMENT_TYPE_VECTOR
     */
    PQDIF_Element *GetElementObject(int elemType);

    /**
      \brief 将物理层元素还回对象池
     */
    void ReleaseElementObject(PQDIF_Element *element);
	
	/**
	  \brief 从对象池获取逻辑层记录对象
	 */
	PQDIFRecord * GetRecordObject( RECORD_TYPE type);

	/**
	  \brief 将逻辑层记录对象还回到对象池
	 */
	void ReleaseRecordObject( PQDIFRecord * record);

private:
	/**
	  \brief 普通collection对象池
	 */
	ObjectPool<PQDIF_E_Collection>*  m_normalCollectionPool;

	/**
	  \brief 特殊collection对象池
	         用于保存ChannelDefn或者ChannelInst
	 */
	ObjectPool<PQDIF_E_Collection2>*  m_specialCollectionPool;

	/**
	  \brief scalar对象池
	 */
	ObjectPool<PQDIF_E_Scalar>*      m_scalarPool;

	/**
	  \brief vector对象池
	 */
	ObjectPool<PQDIF_E_Vector>*      m_vectorPool;

	/**
	  \brief record对象池
	 */
	ObjectPool<PQDIF_R_General>*     m_recordPool;
};
#endif