//------------------------------------------------------------------------------
// mutation strategy: replace/remove unary operation
// inputs: oldOp, newOp
// select posisions that can mutate
// usage: repRemUnaryOp-sel <path to main.c> -- <oldOp> <newOp> <output dir>
//------------------------------------------------------------------------------
#include <sstream>
#include <string>
#include <iostream>
#include <map> 
#include <fstream>
#include <memory>
#include <map>
#include <vector>
#include <utility>
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/Lex/Lexer.h"
using namespace std;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;

static llvm::cl::OptionCategory VisitAST_Category("repRemUnaryOp-sel <path to main.c> -- <oldOp> <newOp> <output dir>");
static string oldOp = "";
static string newOp = "";
static string output_dir = "";
static ofstream out;
static vector<int64_t> res_pos;

class MyASTVisitor : public RecursiveASTVisitor<MyASTVisitor> {
public:
  ASTContext *Context=NULL;
  MyASTVisitor(Rewriter &R) : TheRewriter(R) {}

  bool VisitUnaryOperator(UnaryOperator *u) {
    if(strcmp(oldOp.c_str(),"pre++")==0) { //
        if(u->getOpcode() == UO_PreInc) { //
            // can mutate
            res_pos.push_back(u->getID(*Context));
        }  
    } else if(strcmp(oldOp.c_str(),"pre--")==0) { //
        if(u->getOpcode() == UO_PreDec) { //
            // can mutate
            res_pos.push_back(u->getID(*Context));
        }  
    } else if(strcmp(oldOp.c_str(),"post++")==0) { //
        if(u->getOpcode() == UO_PostInc) { //
            // can mutate
            res_pos.push_back(u->getID(*Context));
        }  
    } else if(strcmp(oldOp.c_str(),"post--")==0) { //
        if(u->getOpcode() == UO_PostDec) { //
            // can mutate
            res_pos.push_back(u->getID(*Context));
        }  
    } else if(strcmp(oldOp.c_str(),"~")==0) { //
        if(u->getOpcode() == UO_Not) { //
            // can mutate
            res_pos.push_back(u->getID(*Context));
        }  
    }
    return true;
  }

private:
  Rewriter &TheRewriter;
};

// Implementation of the ASTConsumer interface for reading an AST produced
// by the Clang parser.
class MyASTConsumer : public ASTConsumer {
public:
  MyASTConsumer(Rewriter &R) : Visitor(R) {}

  void HandleTranslationUnit(ASTContext &Context) override  {
    /* we can use ASTContext to get the TranslationUnitDecl, which is
       a single Decl that collectively represents the entire source file */
    Visitor.Context=&Context;
    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  MyASTVisitor Visitor;
};

// For each source file provided to the tool, a new FrontendAction is created.
class MyFrontendAction : public ASTFrontendAction {
public:
  MyFrontendAction() {}

  void EndSourceFileAction() override {
    SourceManager &SM = TheRewriter.getSourceMgr();
    llvm::errs() << "** EndSourceFileAction for: "
                 << SM.getFileEntryForID(SM.getMainFileID())->getName() << "\n";
  }

  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef file) override {
    llvm::errs() << "** Creating AST consumer for: " << file << "\n";
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return make_unique<MyASTConsumer>(TheRewriter);
  }

private:
  Rewriter TheRewriter;
};

int main(int argc, const char **argv) {
  srand( (unsigned)time( NULL ) );
  // mutateType = argv[3];
  oldOp = argv[3];
  newOp = argv[4];
  output_dir = argv[5];

  cout<<"-- running: repRemUnaryOp-sel"<<endl;
  cout<<"---- oldOp: "<<oldOp<<endl;
  cout<<"---- newOp: "<<newOp<<endl;
  cout<<"---- output_dir: "<<output_dir<<endl;

  llvm::Expected<clang::tooling::CommonOptionsParser> OptionsParser=CommonOptionsParser::create(argc, argv, VisitAST_Category);
  ClangTool Tool(OptionsParser->getCompilations(), OptionsParser->getSourcePathList());

  Tool.run(newFrontendActionFactory<MyFrontendAction>().get());

  out.open(output_dir,ios::trunc);
  if(out.fail()){     
    llvm::errs() <<"Error: Fail to open the out file." << "\n";
    out.close();
    return 0; 
  }
  for(long unsigned int i=0;i<res_pos.size();i++){
    out<<res_pos[i]<<endl;
  }

  return 0;
}
