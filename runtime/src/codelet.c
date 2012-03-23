#include <chplrt.h>
#include "codelet.h"
/* 
 * These are the runtime related files that will be used to call into the 
 * codelet base runtime 
 *
 */

/* initialize codelet runtime : passing a NULL argument means that we use
 * default configuration for the scheduling policies and the number of
 * processors/accelerators */
//int chpl_codelet_init(_codelet_config *conf) {
int chpl_codelet_init(void) {
  return _codelet_init(NULL);
}

/* Create codelet */
int chpl_codelet_create_task(void) {
  return -1;
}