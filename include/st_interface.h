#ifndef __ST_INTERFACE_H
#define __ST_INTERFACE_H

#include "st_others.h"
/**
 * For st_epoll
 */
#include <sys/epoll.h>
#include "WINDEF.H"
#include "st_threadpool.h"

typedef struct __epoll_event 
{
    struct epoll_event event;
    int listen_socket;
    int max_events;
    int event_fd;
    struct epoll_event* p_events;
} EPOLL_STRUCT, *P_EPOLL_STRUCT;

void st_epoll_test(void);
void st_event_loop(P_EPOLL_STRUCT p_epoll, P_ST_THREAD_MANAGE p_manage, void* handler(void* data));
P_EPOLL_STRUCT st_make_events(int lsocket, size_t maxepoll_size);
static int st_make_nonblock(int lsocket);
static int st_add_new_event(int accepted_socket, P_EPOLL_STRUCT p_epoll);
int st_buildsocket(int listen_cnt, int port);


/**
 * For st_memmap
 */

#include <limits.h>

typedef struct _st_memmap_t
{    	
    void*   location;   //ӳ��õ����ڴ��ַ
    int	    fd;	        //�ļ��������
    size_t	size;	    //���ӳ���С
    char    filename[PATH_MAX]; //ӳ�䵽��������ļ�
    char    mapname[PATH_MAX];  //map�����֣����� RPCShare��
} ST_MEMMAP_T, * P_ST_MEMMAP_T;

void* st_memmap_create(const char* filename, const char* share_name, size_t max_size);
void* st_memmap_open(const char* share_name, int fixaddr, int writable);
void st_memmap_close(P_ST_MEMMAP_T p_token);
void st_memmap_destroy(P_ST_MEMMAP_T p_token);
void st_memmap_test(void);


/**
 *  For Win Sync
 */

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>

// CriticalSection
typedef pthread_mutex_t CRITICAL_SECTION;
typedef CRITICAL_SECTION *LPCRITICAL_SECTION;
void InitializeCriticalSection(LPCRITICAL_SECTION section);
void EnterCriticalSection(LPCRITICAL_SECTION section);
void LeaveCriticalSection(LPCRITICAL_SECTION section);
void DeleteCriticalSection(LPCRITICAL_SECTION section);


// Mutex

//Ϊ��ʵ��Wait Close �ӿڵ�ͳһ���Լ�����ͬ����ʽ����չ
enum SYNC_TYPE 
{
    SYNC_MUTEX,
    SYNC_EVENT,
};

typedef struct _st_winsync_t
{
    char    sync_name[PATH_MAX];   //strlen==0 if Intra-process
    enum    SYNC_TYPE type;
    union
    {
        sem_t   sem;      //unamed mutex
        sem_t*  p_sem;    //named mutex
    };
    void*   extra;
} ST_WINSYNC_T, * P_ST_WINSYNC_T;

BOOL  st_winsync_destroy(P_ST_WINSYNC_T p_sync);

HANDLE CreateMutex( void* lpMutexAttributes,
                BOOL bInitialOwner, const char* lpName);
HANDLE OpenMutex( DWORD dwDesiredAccess,
                BOOL bInheritHandle, const char* lpName);
//�����ʱ������ETIMEDOUT
DWORD  WaitForSingleObject(HANDLE hHandle,
                DWORD dwMilliseconds);
BOOL  ReleaseMutex(HANDLE hMutex);
BOOL  CloseHandle( HANDLE hObject);


// Event
HANDLE CreateEvent(void* lpEventAttributes,
                BOOL bManualReset,BOOL bInitialState,
                const char* lpName);
HANDLE WINAPI OpenEvent( DWORD dwDesiredAccess,
                         BOOL bInheritHandle, const char* lpName );
BOOL ResetEvent( HANDLE hEvent);
// ����Ѿ����¼��ˣ��ͷ���EBUSY����ʵ���ٷ���
BOOL SetEvent( HANDLE hEvent);


void Sleep(DWORD dwMilliseconds);
BOOL get_workdir( char* store);

#endif
