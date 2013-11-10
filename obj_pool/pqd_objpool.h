/**
  \file pqd_objpool.h

  \brief PQDIF����������
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
      \brief ��ȡ����ص���������
      \param[in] elemType ������������,ȡֵ��Ϊ:
                          ID_ELEMENT_TYPE_COLLECTION��
                          ID_ELEMENT_TYPE_SCALAR��
                          ID_ELEMENT_TYPE_VECTOR
     */
    PQDIF_Element *GetElementObject(int elemType);

    /**
      \brief �������Ԫ�ػ��ض����
     */
    void ReleaseElementObject(PQDIF_Element *element);
	
	/**
	  \brief �Ӷ���ػ�ȡ�߼����¼����
	 */
	PQDIFRecord * GetRecordObject( RECORD_TYPE type);

	/**
	  \brief ���߼����¼���󻹻ص������
	 */
	void ReleaseRecordObject( PQDIFRecord * record);

private:
	/**
	  \brief ��ͨcollection�����
	 */
	ObjectPool<PQDIF_E_Collection>*  m_normalCollectionPool;

	/**
	  \brief ����collection�����
	         ���ڱ���ChannelDefn����ChannelInst
	 */
	ObjectPool<PQDIF_E_Collection2>*  m_specialCollectionPool;

	/**
	  \brief scalar�����
	 */
	ObjectPool<PQDIF_E_Scalar>*      m_scalarPool;

	/**
	  \brief vector�����
	 */
	ObjectPool<PQDIF_E_Vector>*      m_vectorPool;

	/**
	  \brief record�����
	 */
	ObjectPool<PQDIF_R_General>*     m_recordPool;
};
#endif