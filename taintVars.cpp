#include <cstdio>
#include <iostream>
#include <set>
#include <map>
#include <stack>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IRReader/IRReader.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

void printAnalysisMap(std::map<std::string,std::set<Instruction*>> analysisMap);
std::string getSimpleNodeLabel(const BasicBlock *Node);
std::set<Instruction*> getInstructionsFromBB(const BasicBlock *BB);
std::set<Instruction*> findTaintedVars(BasicBlock*,std::set<Instruction*>);

int main(int argc, char **argv)
{
	// Read the IR file.
	LLVMContext &Context = getGlobalContext();
	SMDiagnostic Err;
	Module *M = ParseIRFile(argv[1], Err, Context);
	if (M == nullptr)
	{
		fprintf(stderr, "error: failed to load LLVM IR file \"%s\"", argv[1]);
		return EXIT_FAILURE;
	}
    Function *F = M->getFunction("main");
	std::map<std::string,std::set<Instruction*>> analysisMap;
        for (auto &BB: *F){
	std::set<Instruction*> emptySet;
	analysisMap[getSimpleNodeLabel(&BB)] = emptySet;
    }  
    printAnalysisMap(analysisMap);   

    std::stack<std::pair<BasicBlock*,std::set<Instruction*> > > traversalStack;
    BasicBlock* entryBB = &F->getEntryBlock();
    std::set<Instruction*> emptySet;
    std::pair<BasicBlock*,std::set<Instruction*> > analysisNode = std::make_pair(entryBB,emptySet);
    traversalStack.push(analysisNode);

    while(!traversalStack.empty()){
        std::pair<BasicBlock*,std::set<Instruction*> > analysisNode = traversalStack.top();
       	traversalStack.pop();
       	
	    // Extract the basic block and the set of initialized variables from  analysisNode
	    BasicBlock* BB = analysisNode.first;
      	std::set<Instruction*> initializedVars = analysisNode.second;     

        // Extract updatedInitializedVars (The list of initialized variables 
	    // after BB) from BB and initializedVars
        std::set<Instruction*> updatedInitializedVars = findInitializedVars(BB,initializedVars);
        
        // Update the analysis of node BB in the MAP to the union of currently sored InitializedVars 
        // and the generated updatedInitializedVars
        std::set<Instruction*> unionInitializedVars = union_sets(analysisMap[BB->getName()],updatedInitializedVars); 
       	analysisMap[BB->getName()] = unionInitializedVars;
        
        // Extract the last instruction in the stack (Terminator Instruction)
        const TerminatorInst *TInst = BB->getTerminator();
        
	    // Extract the number of successors the terminator instructor has
	    int NSucc = TInst->getNumSuccessors();
	
	    for (int i = 0;  i < NSucc; ++i) {
            // for all successor basic blocks, add them plus the updatedInitializedVars to the stack 
            // if fixpoint condition is not met.
            //
            // Fixpoint Condition:
            // We only add successor nodes to the stack if the union of the new list of initialzied variables for 
            // the successor node is different from the currently stored list of initialzied variables
            // for the successor node.
            
            // Load the current stored analysis for a successor node
            BasicBlock *Succ = TInst->getSuccessor(i);    
            std::set<Instruction*> succInitializedVars = analysisMap[Succ->getName()];
	        if (succInitializedVars != unionInitializedVars){
	            std::pair<BasicBlock*,std::set<Instruction*> > succAnalysisNode = std::make_pair(Succ,updatedInitializedVars);
	           traversalStack.push(succAnalysisNode);
            }

    	}	
    }



    return 0;
}


void printAnalysisMap(std::map<std::string,std::set<Instruction*>> analysisMap) {
	for (auto& row : analysisMap){
		std::set<Instruction*> initializedVars = row.second;
		std::string BBLabel = row.first;
		errs() << BBLabel << ":\n";
		for (Instruction* var : initializedVars){
			errs() << "\t";
			var->dump();
		}
		errs() << "\n";
	}
}

std::string getSimpleNodeLabel(const BasicBlock *Node) {
    if (!Node->getName().empty())
        return Node->getName().str();
    std::string Str;
    raw_string_ostream OS(Str);
    Node->printAsOperand(OS, false);
    return OS.str();
}

std::set<Instruction*> getInstructionsFromBB(const BasicBlock *BB){
    std::set<Instruction*> returnSet;
    for (auto &I: *BB){ 
        if(isa<StoreInst>(I)) {
            Value* v = I.getOperand(1); // reterieving second argument
            Instruction* var = dyn_cast<Instruction>(v);
            returnSet.insert(var);
        }
    }
    return returnSet;
}

std::set<Instruction*> findTaintedVars(BasicBlock* BB,
				    std::set<Instruction*> initializedVars)
{
  std::set<Instruction*> updatedTaintedVars(initializedVars);
  
  // Loop through instructions in BB
  for (auto &I: *BB)
  {
    // Check if an Instruction is of the type Store Instruction
    // We assume variables are only initialized if they are 
    // assigned a value by the StoreInst.
    if (isa<StoreInst>(I)){

      // A Store Instruction is of the type:
      // Store Arg0, Arg1 
      // Where Arg0 is a %reg and Arg1 is a %variable.
      // We need to store the %variable since it is initialized 
      
      // We load Arg1 to a an instance of the class Value 
      // Note: All instructions and arguments are instaces of the Value class
      Value*  = I.getOperand(1);
      Value* v = I.getOperand(1);
      
      // Next we convert the instance of Value class to a variable
      // Note: All variables are of type "alloca" instructions. Ex.
      // Variable a: %a = alloca i32, align 4
      Instruction* var = dyn_cast<Instruction>(v); 
      
      // Finally, var is added to updatedInitializedVars
      updatedTaintedVars.insert(var);	
    }
  }
  return updatedInitializedVars;
}
