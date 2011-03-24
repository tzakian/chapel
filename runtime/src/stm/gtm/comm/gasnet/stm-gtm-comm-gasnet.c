#include <signal.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "chplrt.h"
#include "chpl-comm.h"
#include "chpl_mem.h"
#include "chplsys.h"
#include "chpltasks.h"
#include "chplstm.h"
#include "error.h"
#include "gasnet.h"
#include "comm-gasnet.h"

void tx_fork_wrapper(tx_fork_t* buf);

void AM_tx_signal(gasnet_token_t token, void* buf, size_t nbytes, int status) {
  int **commstatus = (int**)buf;
  **commstatus = status;
}

void AM_tx_abort(gasnet_token_t token, void* msg, size_t nbytes) {
  chpl_stm_tx_p tx = NULL;
  int status = TX_OK;
  tx_generic_t *buf = (tx_generic_t*) msg;

  tx = gtm_tx_comm_create(buf->txid, buf->txlocale, buf->txstatus);
  CHPL_STM_COMM_STATS_START(tx->counters, STATS_TX_COMM_ABORT);
  gtm_tx_abort(tx);
  GASNET_Safe(gasnet_AMReplyMedium1(token, 
				    TX_SIGNAL,
				    &(buf->commstatus), 
				    sizeof(int*), 
				    status));
  CHPL_STM_COMM_STATS_STOP(tx->counters, STATS_TX_COMM_ABORT, status);
}

void gtm_tx_comm_abort(chpl_stm_tx_p tx) { 
  tx_generic_t *buf;
  size_t bufsize = sizeof(tx_generic_t); 
  int32_t i; 
  int commstatus = TX_BUSY;

  buf = (tx_generic_t*) chpl_malloc(1, bufsize, CHPL_RT_MD_STM_AM_GENERIC_T, __LINE__, __FILE__);
  buf->txid = tx->id;
  buf->txlocale = tx->locale;
  buf->caller = chpl_localeID;
  buf->txstatus = tx->status;
  buf->commstatus = &commstatus; 
  for (i = 0; i <= tx->numremlocales; i++) {
    commstatus = TX_BUSY;
    GASNET_Safe(gasnet_AMRequestMedium0(tx->remlocales[i], 
					TX_ABORT, 
					buf, 
					bufsize));
    GASNET_BLOCKUNTIL(commstatus != TX_BUSY);
  } 
  chpl_free(buf, __LINE__, __FILE__);
}

void AM_tx_commitPh1(gasnet_token_t token, void* msg, size_t nbytes) {
  chpl_stm_tx_p tx = NULL;
  int status = TX_OK;  
  tx_generic_t *buf = (tx_generic_t*) msg;

  tx = gtm_tx_comm_create(buf->txid, buf->txlocale, buf->txstatus);
  CHPL_STM_COMM_STATS_START(tx->counters, STATS_TX_COMM_COMMITPH1);
  GTM_Safe(tx, gtm_tx_commitPh1(tx));
  if (tx->status == TX_AMABORT) status = TX_FAIL;
  GASNET_Safe(gasnet_AMReplyMedium1(token, 
				    TX_SIGNAL,
				    &(buf->commstatus), 
				    sizeof(int*), 
				    status));
  CHPL_STM_COMM_STATS_STOP(tx->counters, STATS_TX_COMM_COMMITPH1, status);
}

int gtm_tx_comm_commitPh1(chpl_stm_tx_p tx) { 
  tx_generic_t *buf;
  size_t bufsize = sizeof(tx_generic_t);
  int32_t i;
  int commstatus = TX_BUSY;

  buf = (tx_generic_t*) chpl_malloc(1, bufsize, CHPL_RT_MD_STM_AM_GENERIC_T, __LINE__, __FILE__);
  buf->txid = tx->id;
  buf->txlocale = tx->locale;
  buf->caller = chpl_localeID;
  buf->txstatus = tx->status;
  buf->commstatus = &commstatus;

  for (i = 0; i <= tx->numremlocales; i++) {
    commstatus = TX_BUSY;
    GASNET_Safe(gasnet_AMRequestMedium0(tx->remlocales[i], 
					TX_COMMITPH1, 
					buf, 
					bufsize));
    GASNET_BLOCKUNTIL(commstatus != TX_BUSY);
    if (commstatus == TX_FAIL) break;
  }

  chpl_free(buf, __LINE__, __FILE__);
  return commstatus;
}

void AM_tx_commitPh2(gasnet_token_t token, void* msg, size_t nbytes) {
  chpl_stm_tx_p tx = NULL;
  int status = TX_OK;  
  tx_generic_t *buf = (tx_generic_t*) msg;

  tx = gtm_tx_comm_create(buf->txid, buf->txlocale, buf->txstatus);
#ifdef GTM_COMM_COMBINED_COMMIT
  CHPL_STM_COMM_STATS_START(tx->counters, STATS_TX_COMM_COMMITPH1);
  GTM_Safe(tx, gtm_tx_commitPh1(tx));
  if (tx->status == TX_AMABORT) {
    status = TX_FAIL;
    GASNET_Safe(gasnet_AMReplyMedium1(token,
				      TX_SIGNAL,
				      &(buf->commstatus),
				      sizeof(int*),
				      status));
    return;
  }
  CHPL_STM_COMM_STATS_STOP(tx->counters, STATS_TX_COMM_COMMITPH1, status);
#endif
  status = TX_OK;
  CHPL_STM_COMM_STATS_START(tx->counters, STATS_TX_COMM_COMMITPH2); 
  GTM_Safe(tx, gtm_tx_commitPh2(tx));
  if (tx->status == TX_AMABORT) status = TX_FAIL;   
  GASNET_Safe(gasnet_AMReplyMedium1(token, 
				    TX_SIGNAL,
				    &(buf->commstatus), 
				    sizeof(int*), 
				    status));
  CHPL_STM_COMM_STATS_STOP(tx->counters, STATS_TX_COMM_COMMITPH2, status);
  gtm_tx_comm_destroy(tx);
}

int gtm_tx_comm_commitPh2(chpl_stm_tx_p tx) { 
  tx_generic_t *buf;
  size_t bufsize = sizeof(tx_generic_t);
  int32_t i;
  int commstatus = TX_BUSY;

  buf = (tx_generic_t*) chpl_malloc(1, bufsize, CHPL_RT_MD_STM_AM_GENERIC_T, __LINE__, __FILE__);
  buf->txid = tx->id;
  buf->txlocale = tx->locale;
  buf->caller = chpl_localeID;
  buf->txstatus = tx->status;
  buf->commstatus = &commstatus;

  for (i = 0; i <= tx->numremlocales; i++) {
    commstatus = TX_BUSY;
    GASNET_Safe(gasnet_AMRequestMedium0(tx->remlocales[i], 
					TX_COMMITPH2, 
					buf, 
					bufsize));
    GASNET_BLOCKUNTIL(commstatus != TX_BUSY);
    if (commstatus == TX_FAIL) break;
  }

  chpl_free(buf, __LINE__, __FILE__);
  return commstatus;
}

void AM_tx_getdata (gasnet_token_t token, void* msg, size_t nbytes, int status) {
  tx_getdata_t* buf = (tx_getdata_t*) msg;  
  int *commstatus = buf->commstatus;
  memcpy(buf->retaddr, &buf->data, buf->datasize);
  *commstatus = status; 
}

void AM_tx_get(gasnet_token_t token, void* msg, size_t nbytes) {
  chpl_stm_tx_p tx = NULL;
  int status = TX_OK;  
  tx_get_t *buf = (tx_get_t*) msg;
  tx_getdata_t* getdata;
  size_t getdatasize = sizeof(tx_getdata_t) + buf->datasize;

  tx = gtm_tx_comm_create(buf->txid, buf->txlocale, buf->txstatus);
  CHPL_STM_COMM_STATS_START(tx->counters, STATS_TX_COMM_GET);
  // build the return payload
  getdata = (tx_getdata_t*) chpl_malloc(1, getdatasize, CHPL_RT_MD_STM_AM_GETDATA_T, __LINE__, __FILE__);
  getdata->retaddr = buf->retaddr;
  getdata->commstatus = buf->commstatus;
  getdata->datasize = buf->datasize;
  // perform transactional load
  GTM_Safe(tx, gtm_tx_load_wrap(tx, &(getdata->data), buf->remaddr, buf->datasize));
  if (tx->status == TX_AMABORT) {
    status = TX_FAIL;
    GASNET_Safe(gasnet_AMReplyMedium1(token, 
				      TX_SIGNAL,
				      &(buf->commstatus), 
				      sizeof(int*), 
				      status));
  } else {
    // return the data
    GASNET_Safe(gasnet_AMReplyMedium1(token, 
				      TX_GETDATA,
				      getdata, 
				      getdatasize, 
				      status));
  }

  chpl_free(getdata, __LINE__, __FILE__);
  CHPL_STM_COMM_STATS_STOP(tx->counters, STATS_TX_COMM_GET, status);
}

int gtm_tx_comm_get(chpl_stm_tx_p tx, void* addr, int32_t remlocale, void* remaddr, size_t size) { 
  tx_get_t *buf;
  size_t bufsize = sizeof(tx_get_t) + size; 
  int commstatus = TX_BUSY;

  assert(bufsize < gasnet_AMMaxMedium()); 

  gtm_tx_comm_register(tx, remlocale);
  buf = (tx_get_t*) chpl_malloc(1, bufsize, CHPL_RT_MD_STM_AM_GET_T, __LINE__, __FILE__);
  buf->txid = tx->id;
  buf->txlocale = tx->locale;
  buf->caller = chpl_localeID;
  buf->txstatus = tx->status;
  buf->commstatus = &commstatus;
  buf->remaddr = remaddr;
  buf->retaddr = addr;   
  buf->datasize = size;
  GASNET_Safe(gasnet_AMRequestMedium0(remlocale, TX_GET, buf, bufsize));
  GASNET_BLOCKUNTIL(commstatus != TX_BUSY);

  chpl_free(buf, __LINE__, __FILE__);
  return commstatus;
}

void AM_tx_put(gasnet_token_t token, void* msg, size_t nbytes) {
  chpl_stm_tx_p tx = NULL;
  int status = TX_OK;  
  tx_put_t *buf = (tx_put_t*) msg;

  tx = gtm_tx_comm_create(buf->txid, buf->txlocale, buf->txstatus);
  CHPL_STM_COMM_STATS_START(tx->counters, STATS_TX_COMM_PUT);
  GTM_Safe(tx, gtm_tx_store_wrap(tx, &(buf->data), buf->remaddr, buf->datasize));
  if (tx->status == TX_AMABORT) status = TX_FAIL; 
  GASNET_Safe(gasnet_AMReplyMedium1(token, 
				    TX_SIGNAL,
				    &(buf->commstatus), 
				    sizeof(int*), 
				    status));

  CHPL_STM_COMM_STATS_STOP(tx->counters, STATS_TX_COMM_PUT, status);
}

int gtm_tx_comm_put(chpl_stm_tx_p tx, void* addr, int32_t remlocale, void* remaddr, size_t size) { 
  tx_put_t *buf;
  size_t bufsize = sizeof(tx_put_t) + size; 
  int commstatus = TX_BUSY;

  assert(bufsize < gasnet_AMMaxMedium()); 

  gtm_tx_comm_register(tx, remlocale);
  buf = (tx_put_t*) chpl_malloc(1, bufsize, CHPL_RT_MD_STM_AM_PUT_T, __LINE__, __FILE__);
  buf->txid = tx->id;
  buf->txlocale = tx->locale;
  buf->caller = chpl_localeID;
  buf->txstatus = tx->status;
  buf->commstatus = &commstatus;
  buf->remaddr = remaddr;
  buf->datasize = size;
  memcpy(&(buf->data), addr, size);

  GASNET_Safe(gasnet_AMRequestMedium0(remlocale, TX_PUT, buf, bufsize));
  GASNET_BLOCKUNTIL(commstatus != TX_BUSY);

  chpl_free(buf, __LINE__, __FILE__);
  return commstatus;
}

void tx_fork_wrapper(tx_fork_t* buf) {
  chpl_stm_tx_p tx = NULL;
  int status = TX_OK;  

  tx = gtm_tx_comm_create(buf->txid, buf->txlocale, buf->txstatus);
  CHPL_STM_COMM_STATS_START(tx->counters, STATS_TX_COMM_FORK);
  tx->rollback = true;
  setjmp(tx->env);
  if (tx->status == TX_AMACTIVE) {
    if (buf->argsize) 
      (*chpl_txftable[buf->fid])(tx, &buf->arg);
    else 
      (*chpl_txftable[buf->fid])(tx, 0);
  } else {
    assert(tx->status == TX_AMABORT); 
    status = TX_FAIL;
  }
  tx->rollback = false;

  GASNET_Safe(gasnet_AMRequestMedium1(buf->caller, TX_SIGNAL,
				      &(buf->commstatus), sizeof(int*), 
				      status));

  chpl_free(buf, __LINE__, __FILE__);
  CHPL_STM_COMM_STATS_STOP(tx->counters, STATS_TX_COMM_FORK, status);
}

void AM_tx_fork(gasnet_token_t token, void* msg, size_t nbytes) {
  tx_fork_t *buf = (tx_fork_t*) chpl_malloc(nbytes, 
					    sizeof(char),
					    CHPL_RT_MD_STM_AM_FORK_T,
					    __LINE__, __FILE__);
  memcpy(buf, msg, nbytes);
  chpl_task_begin((chpl_fn_p)tx_fork_wrapper, (void*)buf, true, true, NULL);
}

int gtm_tx_comm_fork(chpl_stm_tx_p tx, int32_t remlocale, chpl_fn_int_t fid, void *arg, size_t argsize) {
  tx_fork_t *buf;
  size_t bufsize = sizeof(tx_fork_t) + argsize; 
  int commstatus = TX_BUSY;
  
  assert(bufsize < gasnet_AMMaxMedium()); 

  gtm_tx_comm_register(tx, remlocale);
  buf = (tx_fork_t*) chpl_malloc(1, bufsize, CHPL_RT_MD_STM_AM_FORK_T, __LINE__, __FILE__);
  buf->txid = tx->id;
  buf->txlocale = tx->locale;
  buf->caller = chpl_localeID;
  buf->txstatus = tx->status;
  buf->commstatus = &commstatus;
  buf->fid = fid;
  buf->argsize = argsize;

  if (argsize)
    memcpy(&(buf->arg), arg, argsize);

  GASNET_Safe(gasnet_AMRequestMedium0(remlocale, TX_FORK, buf, bufsize));
  GASNET_BLOCKUNTIL(commstatus != TX_BUSY);

  chpl_free(buf, __LINE__, __FILE__);
  return commstatus;
}