/**
  \file pqd_objpool.h

  \brief PQDIF»ù´¡¿â¶ÔÏó³Ø
 */
#include "pqd_objpool.h"
#include "pqd_log.h"
#include "pqdif_id.h"
#include "pqdif_ph.h"
#include "pqdif_lg.h"

PQDObjectPool * PQDObjectPool::GetInstance()
{
    static PQDObjectPool sObjPool;
    return &sObjPool;
}

PQDObjectPool::PQDObjectPool()
{
    m_normalCollectionPool = new ObjectPool<PQDIF_E_Collection>(NORMAL_COLLECTION_NUM);
	m_specialCollectionPool = new ObjectPool<PQDIF_E_Collection2>(SPECIAL_COLLECTION_NUM);
    m_scalarPool     = new ObjectPool<PQDIF_E_Scalar>(SCALAR_NUM);
    m_vectorPool     = new ObjectPool<PQDIF_E_Vector>(VECTOR_NUM);
	m_recordPool     = new ObjectPool<PQDIF_R_General>(RECORD_NUM);
}

PQDObjectPool::~PQDObjectPool()
{
    if ( m_normalCollectionPool != NULL )
    {
        delete m_normalCollectionPool;
    }
	
	if ( m_specialCollectionPool != NULL )
	{
		delete m_specialCollectionPool;
	}

    if (m_scalarPool != NULL)
    {
        delete m_scalarPool;
    }
    if (m_vectorPool != NULL)
    {
        delete m_scalarPool;
    }
}

PQDIF_Element * PQDObjectPool::GetElementObject( int elemType )
{
#ifdef _CHECK_LOG_
	static int normalcollTimes = 0;
	static int specialcollTimes = 0;
	static int scalarTimes = 0;
	static int vectorTimes = 0;
#endif

    PQDIF_Element *element = NULL;
    switch ( elemType )
    {
    case ID_ELEMENT_TYPE_COLLECTION:
#ifdef _CHECK_LOG_
		PQD_LOG(_T("Get Normal Collection Times = %d \r\n"), ++normalcollTimes);
#endif
        element = (PQDIF_Element *)(m_normalCollectionPool->GetObj());
		((PQDIF_E_Collection *)element)->Init();
        break;

	case ID_ELEMENT_TYPE_COLLECTION2:
#ifdef _CHECK_LOG_
		PQD_LOG(_T("Get Special Collection Times = %d \r\n"), ++specialcollTimes);
#endif
		element = (PQDIF_Element *)(m_specialCollectionPool->GetObj());
		((PQDIF_E_Collection2 *)element)->Init();
		break;		

    case ID_ELEMENT_TYPE_VECTOR:
#ifdef _CHECK_LOG_
        PQD_LOG(_T("Get Vector Times = %d \r\n"), ++vectorTimes);
#endif
		element = (PQDIF_Element *)(m_vectorPool->GetObj());
		((PQDIF_E_Vector *)element)->Init();
        break;

    case ID_ELEMENT_TYPE_SCALAR:
#ifdef _CHECK_LOG_
		PQD_LOG(_T("Get Scalar Times = %d \r\n"), ++scalarTimes);
#endif
		element = (PQDIF_Element *)(m_scalarPool->GetObj());
		((PQDIF_E_Scalar *)element)->Init();
        break;

    default:
        PQD_LOG(_T("PQDObjectPool::GetElementObject() UnSupport Element Type \r\n"));
        element = NULL;
        break;
    }
    return element;
}

void PQDObjectPool::ReleaseElementObject( PQDIF_Element *element )
{
#ifdef _CHECK_LOG_
	static int normalcollTimes = 0;
	static int specialcollTimes = 0;
	static int scalarTimes = 0;
	static int vectorTimes = 0;
#endif
    int elemType = element->GetElementType();

    switch ( elemType )
    {
    case ID_ELEMENT_TYPE_COLLECTION:
#ifdef _CHECK_LOG_
		PQD_LOG(_T("Release Normal Collection Times = %d \r\n"), ++normalcollTimes);
#endif
		((PQDIF_E_Collection *)element)->Exit();
        m_normalCollectionPool->ReleaseObj(element);
        break;

	case ID_ELEMENT_TYPE_COLLECTION2:
#ifdef _CHECK_LOG_
		PQD_LOG(_T("Release Special Collection Times = %d \r\n"), ++specialcollTimes);
#endif
		((PQDIF_E_Collection2 *)element)->Exit();
		m_specialCollectionPool->ReleaseObj(element);
		break;

    case ID_ELEMENT_TYPE_VECTOR:
#ifdef _CHECK_LOG_
		PQD_LOG(_T("Release Vector Times = %d \r\n"), ++vectorTimes);
#endif
		((PQDIF_E_Vector *)element)->Exit();
        m_vectorPool->ReleaseObj(element);
        break;

    case ID_ELEMENT_TYPE_SCALAR:
#ifdef _CHECK_LOG_
		PQD_LOG(_T("Release Scalar Times = %d \r\n"), ++scalarTimes);
#endif
		((PQDIF_E_Scalar *)element)->Exit();
        m_scalarPool->ReleaseObj(element);
        break;

    default:
        PQD_LOG(_T("PQDObjectPool::ReleaseElementObject() Error Element !!!\r\n"));
        break;
    }
}

PQDIFRecord * PQDObjectPool::GetRecordObject( RECORD_TYPE type)
{
#ifdef _CHECK_LOG_
	static int recordTimes = 0;
	PQD_LOG(_T("Get Record Times = %d \r\n"), ++recordTimes);
#endif

	PQDIFRecord *record = NULL;

	switch(type)
	{
	case RECORD_TYPE_GENERAL:
		record = m_recordPool->GetObj();
		((PQDIF_R_General *)record)->Init();
		record->HeaderSetTag(tagBlank);
		break;

	case RECORD_TYPE_CONTAINER:
		record = m_recordPool->GetObj();
		((PQDIF_R_General *)record)->Init();
		record->HeaderSetTag(tagContainer);
		break;

	case RECORD_TYPE_DATASOURCE:
		record = m_recordPool->GetObj();
		((PQDIF_R_General *)record)->Init();
		record->HeaderSetTag(tagRecDataSource);
		break;

	case RECORD_TYPE_OBSERVATION:
		record = m_recordPool->GetObj();
		((PQDIF_R_General *)record)->Init();
		record->HeaderSetTag(tagRecObservation);
		break;

	case RECORD_TYPE_MONITORSETTINGS:
		record = m_recordPool->GetObj();
		((PQDIF_R_General *)record)->Init();
		record->HeaderSetTag(tagRecMonitorSettings);
		break;
	default:
		break;
	}
	return record;
}

void PQDObjectPool::ReleaseRecordObject( PQDIFRecord * record)
{
#ifdef _CHECK_LOG_
	static int recordTimes = 0;
	PQD_LOG(_T("Release Record Times = %d \r\n"), ++recordTimes);
#endif
	if ( record == NULL )
	{
		return;
	}
	((PQDIF_R_General *)record)->Exit();
	m_recordPool->ReleaseObj(record);
}