#include "AST.h"
#include "ContextInfo.h"
#include "hc_list_helpers.h"
#include "streams.h"
#include "Dictionary.h"

std::string generateFullHERA(ExprNode *presumedRoot)
{
	return "CBON()\n" + presumedRoot->generateHERA(ContextInfo());
}


using HaverfordCS::list;
using std::string;
using std::endl;

string IntLiteralNode::generateHERA(const ContextInfo &context) const
{
	trace << "Entered IntLiteralNode::generateHERA for integer " + std::to_string(v) << endl;
	return "SET(" + context.getReg() + ", " + std::to_string(v) + ")\n";            //SET(R10, 2)
}
//**************************************
string BoolLiteralNode::generateHERA(const ContextInfo &context) const
{
    trace << "Entered BoolLiteralNode::generateHERA for bool " + std::to_string(v) << endl;
    return "SET(" + context.getReg() + ", " + std::to_string(v? 1:0) + ")\n";            //SET(R10, 2)  (v? std::string("true"):std::string("false"));
}
//static Dictionary constructDict();
static Dictionary *pVar = new Dictionary();
string VarUseNode::generateHERA(const ContextInfo &context) const
{
    trace << "Entered VarUseNode::generateHERA for variable " + n << endl;
    list<ExprNode*> ops = subexps;
    if(! empty(ops)) {
        ExprNode* firstchild = first(ops);
        return firstchild->generateHERA(context);
    } return "";
}
string CallNode::generateHERA(const ContextInfo &context) const
{
    trace << "Entered CallNode::generateHERA for call to " + n << endl;

    if (length(argList) != 0 || (n != "exit" && n != "getint")) {
        if(pVar->contains(n)) {
            return std::to_string(pVar->lookup(n)) + "\n";
            list<ExprNode*> ops = argList;
            ExprNode* firstchild = first(ops);
            if(firstchild) return firstchild->generateHERA(context);

        }//return std::to_string(pVar->lookup(n));
    }
    // NOTE that calls to exit and getint don't need parameters and don't perturb registers
    return ("MOVE(FP_alt, SP)\nCALL(FP_alt," + n + ")\n"+
            (context.getReg()=="R1"?"":"MOVE("+context.getReg()+", R1)\n"));
}

string LetStarNode::generateHERA(const ContextInfo &context) const
{
    trace << "Entered LetStarNode::generateHERA for evaluation " + o << endl;
    list<ExprNode*> ops = subexps;
    ExprNode* firstchild = first(subexps);
    ExprNode* secondchild = first(rest(subexps));
    ops = ((CallNode*) firstchild)->sub();
    firstchild = first(ops);

    ContextInfo currentContext = context;
    int len = length(ops);
    string result = "";
    for (int i=0; i<len; i++){
        string variableName =  ((CallNode*)firstchild)->value() ; // + currentContext.getReg()
        *pVar = Dictionary(*pVar, variableName, i);
        result = result + ((CallNode*)firstchild)->generateHERA(currentContext);
        result = result + "STORE(" + currentContext.getReg() + ", " + std::to_string(len-i-1) + ", FP)\n" ;
        ops = rest(ops);
        if(not empty(ops)) {
            firstchild = first(ops);
            currentContext = currentContext.evalThisAfter();
        }
    }
    currentContext = context;
    for (int i=0; i<len; i++){
        result = result + "LOAD(" + currentContext.getReg() + ", " + std::to_string(len-i-1) + ", FP)\n" ;
        currentContext = currentContext.evalThisAfter();
    }

    result = "INC(SP, " + std::to_string(len) + ")\n" +
            result +
            secondchild->generateHERA(context)+
            "DEC(SP, " + std::to_string(len) + ")\n";

    return result;
    //throw "LetStar node code generation not implemented yet";
}
std::string HERA_op(const std::string &AST_op_name);

string ComparisonNode::generateHERA(const ContextInfo &context) const
{
    trace << "Entered ComparisonNode::generateHERA for comparison " + o << endl;

    // see arithmetic node for more about the "context" stuff:
    //	trace << "need to compare the result of left-hand-side:\n" << left->generateHERA(context) << endl;
    //	trace << "                        with right-hand-side:\n" << left->generateHERA(context.evalThisAfter()) << endl;

    ContextInfo rhsContext = context.evalThisAfter();
    ContextInfo lhsContext = context;  // just named for symmetry

    return (left->generateHERA(lhsContext) +
            right->generateHERA(rhsContext) +
            "CMP("+lhsContext.getReg()+","+rhsContext.getReg()+")\n" +
            HERA_op(o)+"("+"ELSE)\n");
}
string IfNode::generateHERA(const ContextInfo &context) const
{
    trace << "Entered IfNode::generateHERA for evaluation " + o << endl;

    list<ExprNode*> ops = subexps;
    ExprNode* firstchild = first(ops);
    ExprNode* secondchild = first(rest(ops));
    ExprNode* thirdchild = first(rest(rest(ops)));

    if(firstchild->type() == ET_BOOL){
        if(((BoolLiteralNode*)firstchild)->value() == true){
            return (firstchild->generateHERA(context)+
                    secondchild->generateHERA(context)+
                    "BR(END)" + "\n" +
                    "LABEL(ELSE)" "\n" +
                    thirdchild->generateHERA(context) +
                    "LABEL(END)" "\n"
            );
        } else {
            return (firstchild->generateHERA(context)+
                    "BR(ELSE)" + "\n" +
                    secondchild->generateHERA(context)+
                    "BR(END)" + "\n" +
                    "LABEL(ELSE)" "\n" +
                    thirdchild->generateHERA(context) +
                    "LABEL(END)" "\n");

        }
    }else{ // evaluated to be bool
        return (firstchild->generateHERA(context)+
                secondchild->generateHERA(context)+
                "BR(END)" + "\n" +
                "LABEL(ELSE)" "\n" +
                thirdchild->generateHERA(context) +
                "LABEL(END)" "\n"
        );

    }

}


//**************************************


string ArithmeticNode::generateHERA(const ContextInfo &context) const
{
    trace << "Entered ArithmeticNode::generateHERA for operator " + o << endl;

    string result="";
    list<ExprNode*> ops = subexps;
    ContextInfo rhsContext = context.evalThisAfter();
    ContextInfo lhsContext = context;  // just named for symmetry

    while (!empty(rest(ops))) {
        result = (first(ops)->generateHERA(lhsContext) +                                            //Reuse R10 as left's register
                  first(rest(ops))->generateHERA(rhsContext) +                                        //use new register for right tree
                  HERA_op(o)+"("+context.getReg()+", "+lhsContext.getReg()+","+rhsContext.getReg()+")\n") + result;  //MUL(R10, R10,R9)
        ops = rest(ops);
    }
    return result;
}

std::string HERA_op(const std::string &AST_op_name)
{
	if (AST_op_name == "+") {
		return "ADD";
	} else if (AST_op_name == "-") {
		return "SUB";
	} else if (AST_op_name == "*") {
		return "MUL";
	} else if (AST_op_name == "=" ) {    //**************************************
        return "BNZ";
    } else if (AST_op_name == "<=" ) {
            return "BG";
    } else if (AST_op_name == ">=" ) {
        return "BL";
    } else{
		throw "Internal compiler inconsistency/incompleteness: HERA_op not implemented for " + AST_op_name;
	}
}




