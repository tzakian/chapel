#include "stmt.h"
#include "bb.h"
#include "expr.h"
#include "interval.h"
#include "passes.h"
#include "symbol.h"
#include "astutil.h"
#include "optimizations.h"

#define END_NODE 0

/* Routine to remove empty if or while blocks.
 * These typically occur due to previous transformations */
static void removeEmptyBlocks(FnSymbol *fn) {
  Vec<BaseAST*> asts;
  collect_asts_postorder(fn, asts);
  forv_Vec(BaseAST, ast, asts) {
    /* if stmt is an if of some kind, and if its body is empty, then remove it */
    if (CondStmt *cond = toCondStmt(ast)) {
      bool thenEmpty = false;
      bool elseEmpty = false;
      if ((cond->thenStmt && cond->thenStmt->length() == 0) || cond->thenStmt == NULL)
        thenEmpty = true;
      if ((cond->elseStmt && cond->elseStmt->length() == 0) || cond->elseStmt == NULL)
        elseEmpty = true;
      if (thenEmpty && elseEmpty) {
        printf("removing empty if\n");
        cond->remove();
      }
    }
    else if (BlockStmt *block = toBlockStmt(ast)) {
      /* if block is a loop of some kind, and if its body is empty, then remove it */
      if (block->isLoop() && block->length() == 0) {
        printf("removing empty while\n");
        block->remove();
      }
    }
  }
}

static void
innerCodelets(FnSymbol* originalFn, BlockStmt* codeletBlock, BasicBlock* curBB, 
              Vec<BasicBlock*>& innerBB, Interval* interval, Map<BasicBlock*, 
              Interval*>& intervalMaps) {

  bool FLAG_MOVE_DEF = (interval->intervalTag != FORALL_LOOP);
  forv_Vec(Expr, expr, curBB->exprs) {
    /* more than likely if we are in variable declarations */
    if (CallExpr *call = toCallExpr(expr)) {
      if (call->isPrimitive(PRIM_RETURN)) {
        call->get(1)->remove();
        call->remove();
      } else if (call->isPrimitive(PRIM_ON_IL)) {
        call->remove();
      } else {
        codeletBlock->insertAtTail(call->remove());
      }
    }
    else if (DefExpr *def = toDefExpr(expr)) {
      if (FLAG_MOVE_DEF)
        originalFn->insertAtHead(def->remove());
      else
        codeletBlock->insertAtTail(def->remove());
      /* Instead, we should search the other intervals to see if this definition is
       * used elsewhere. If not, no need to move it out of the interval */
    }
    else if (SymExpr *sym = toSymExpr(expr)) {
      /* figure out where it is */
      if (sym->parentExpr) {
        if (BlockStmt *stmt = toBlockStmt(sym->parentExpr)) {
          if (stmt != originalFn->body) {
            ;
          }
        }
        else if (toCondStmt(sym->parentExpr)) {
          CallExpr* ci = new CallExpr(PRIM_IDEN, sym->copy());
          BasicBlock* thenbb = curBB->thenBB;
          BasicBlock* elsebb = curBB->elseBB;
          BlockStmt* thenStmt = new BlockStmt();
          BlockStmt* elseStmt = new BlockStmt();
          if (intervalMaps.get(thenbb) == interval){
            innerBB.remove(innerBB.index(thenbb));
            innerCodelets(originalFn, thenStmt, thenbb, innerBB, interval, intervalMaps);
          }
          if (intervalMaps.get(elsebb) == interval){
            innerBB.remove(innerBB.index(elsebb));
            innerCodelets(originalFn, elseStmt, elsebb, innerBB, interval, intervalMaps);
          }
          codeletBlock->insertAtTail(new CondStmt(ci,thenStmt,elseStmt));
        }
      }
    }
    else if (BlockStmt *block = toBlockStmt(expr)) {
      codeletBlock->insertAtTail(block->remove());
    }
    else if (CondStmt* cond = toCondStmt(expr)){
      codeletBlock->insertAtTail(cond->remove());
    } else {
      INT_FATAL(expr, "NOT SUPPOSED TO BE HERE...DEBUG ASAP!\n");
    }
  }
}

static void
createCodelet(FnSymbol *originalFn, FnSymbol *codeletFn, Interval *interval, Map<BasicBlock*, Interval*>& intervalMaps) {
  Vec<BasicBlock*> innerBB;
  innerBB.copy(interval->bb);

  while (innerBB.n > 0){
    BasicBlock* bb = innerBB.v[0];
    innerBB.remove(innerBB.index(bb));
    innerCodelets(originalFn, codeletFn->body, bb, innerBB, interval, intervalMaps);
  }

  ArgSymbol *condFormal = new ArgSymbol( INTENT_REF, "numID", dtInt[INT_SIZE_32]);
  codeletFn->insertFormalAtTail( new DefExpr(condFormal));

  Vec<Expr*> tmpExpr;
  Vec<bool> tmpBools;
  
  /* No need to generate conditions if there are > 1 outgoing interval edges */
  if (interval->outs.n > 1) {
    for (int i = 0; i < interval->outs.n; i++){
      Interval* targetInt = interval->outs.v[i];
      BasicBlock* sourceBB = interval->sourceEdges.v[i];
      tmpExpr.copy(sourceBB->condExprs);
      tmpBools.copy(sourceBB->condBools);
      if (sourceBB->outs.n > 1){
        tmpExpr.add(sourceBB->branch);
        if (targetInt->bb.v[0] == sourceBB->thenBB)
          tmpBools.add(true);
        else if (targetInt->bb.v[0] == sourceBB->elseBB)
          tmpBools.add(false);
      }
      if (tmpExpr.n >= 1){
        CallExpr* condNumID;
        BlockStmt* thenStmt = new BlockStmt();
        if (tmpBools.v[0] == true)
          condNumID = new CallExpr(PRIM_IDEN, tmpExpr.v[0]->copy());
        else if (tmpBools.v[0] == false)
          condNumID = new CallExpr(PRIM_UNARY_LNOT, tmpExpr.v[0]->copy());
        for (int j = 1; j < tmpExpr.n; j++){
          if (tmpBools.v[j] == true)
            condNumID = new CallExpr(PRIM_AND, condNumID, tmpExpr.v[j]->copy());
          else if (tmpBools.v[j] == false)
            condNumID = new CallExpr(PRIM_AND, condNumID, new CallExpr(PRIM_UNARY_LNOT, tmpExpr.v[j]->copy()));
        }
        thenStmt->insertAtTail(new CallExpr(PRIM_MOVE,condFormal,new_IntSymbol(targetInt->id)));
        codeletFn->insertAtTail(new CondStmt(condNumID,thenStmt));
      }
      tmpExpr.clear();
      tmpBools.clear();
    }
  }
  else if (interval->outs.n == 1) {
    Interval* targetInt = interval->outs.v[0];
    //BasicBlock *sourceBB = interval->sourceEdges.[0];
    //if (sourceBB->outs.n > 1)
    codeletFn->insertAtTail(new CallExpr(PRIM_MOVE,condFormal,new_IntSymbol(targetInt->id)));

  }
  else if (interval->outs.n == 0) {
    codeletFn->insertAtTail(new CallExpr(PRIM_MOVE,condFormal,new_IntSymbol(END_NODE)));
  }
  
  codeletFn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  codeletFn->retType = dtVoid;
}


static void
createFn(FnSymbol *fn, Map<BasicBlock*, Interval*>& intervalMaps) {
  if (fn->intervals) {
    Vec<FnSymbol*> nestedFunctions;
    FnSymbol *intervalFn;
    Map<Symbol*,Vec<SymExpr*>*> defMap;
    Map<Symbol*,Vec<SymExpr*>*> useMap;


    buildDefUseMaps(fn, defMap, useMap);

    /* here we declare the branching conditional needed for MAP */
    Symbol *numID = newTemp("numID", dtInt[INT_SIZE_32]);
    fn->insertAtHead(new DefExpr(numID));

    forv_Vec(Interval, interval, *fn->intervals) {

      intervalFn = new FnSymbol(astr("intervalFn", istr(interval->id)));
      intervalFn->intervals = new Vec<Interval*>();
      intervalFn->intervals->add(interval);
      nestedFunctions.add(intervalFn);
      createCodelet(fn, intervalFn, interval, intervalMaps);
      insert_help(intervalFn, NULL, intervalFn);

      /* build the list of outgoing edges */
      char *labelStr = NULL;
      if (interval->outs.n > 0) {
        forv_Vec(Interval, intOut, interval->outs) {
          if (labelStr == NULL)
            labelStr = (char *)astr(istr(intOut->id));
          else
            labelStr = (char *)astr(labelStr, ",",astr(istr(intOut->id)));
        }
        labelStr = (char *)astr("[", labelStr, "]");
      }
      else 
        /* if this is the last actual map, we return back to the MAIN.c*/
        labelStr = (char *)astr("[",istr(END_NODE),"]");


      CallExpr *intervalCall = new CallExpr(intervalFn);
      intervalCall->insertAtTail(numID);


      fn->insertAtTail(new DefExpr(intervalFn));
      CallExpr *call = NULL;
      if (interval->intervalTag == BLOCK || interval->intervalTag == FOR_LOOP) {
        call = new CallExpr(PRIM_MAP_SEQ, new_IntSymbol(interval->id), 
                            new_StringSymbol(labelStr), numID, intervalCall);
      }
      else if (interval->intervalTag == FORALL_LOOP) {
        call = new CallExpr(PRIM_MAP_PAR, new_IntSymbol(interval->id), 
                            new_StringSymbol(labelStr), numID, interval->low, 
                            interval->stride, interval->high, interval->ILIndex,
                            intervalCall);
      }
      fn->insertAtTail(call);
    }

    fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
    fn->retType = dtVoid;

    flattenNestedFunctions(nestedFunctions, true);
  }
  collapseBlocks(fn->body);
}

static void inlineMainFunction(void) {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (!strcmp(fn->name, "main")) {
      fn->addFlag(FLAG_INLINE);
      break;
      //inlineFunction(fn);
      //return;
    }
  }
  inlineFunctions();
}

void 
targetIL() {

  if (fTargetIL) {

    /* First thing we need to do is inline the main function */
    inlineMainFunction();

    /* For every other function besides chpl_main(), we build the intervals */
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (!strcmp(fn->name, "chpl_main")) {

        Map<BasicBlock*, Interval*> intervalMaps;

        singleAssignmentRefPropagation(fn);
        localCopyPropagation(fn);
        deadCodeElimination(fn);
        globalCopyPropagation(fn);
        deadCodeElimination(fn);
        removeUnnecessaryGotos(fn);

        /* 1. Coursen the basic blocks for the given function */
        buildCoarseBasicBlocks(fn);
        
        /* 2. Build intervals */
        buildIntervals(fn, intervalMaps);
      
        /* 3. Convert Basic Blocks to separate function */
        createFn(fn, intervalMaps);
        removeEmptyBlocks(fn);
        inlineFunction(fn);
      }
    }
  }
}