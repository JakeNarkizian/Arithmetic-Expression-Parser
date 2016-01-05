//
//  Operation.swift
//  Solve
//

import Foundation

class OpList{

    class Operation {
        
        // --- PROPERTIES ---
        let o: Character
        var l: Operation?
        var r: Operation?
        var n: Double?
        var p: Int
        
        
        // --- METHODS ---
        
        /*
        * OPERATOR INIT:
        *      This init is for an operator operation
        */
        init(oper:Character){
            self.o = oper
            switch oper{
            case "^":
                self.p = 5
            case "*":
                self.p = 4
            case "/":
                self.p = 3
            case "+":
                self.p = 2
            case "-":
                self.p = 1
            default:
                self.p = 0
                NSException(name:"Invalid Charachter", reason:"Invalid charachter used to initialize Operation", userInfo:nil).raise()
            }
            
        }
        
        /*
        * IDENTITY INIT:
        *      This init is for an Identity operation
        */
        init(num:Double){
            self.o = "i"
            self.n = num
            self.p = 6
            
        }
        
        /*
        * VARIABLE INIT:
        *      This init is for a Variable
        */
        init(){
            self.o = "x"
            self.p = 6
        }
        
        /*
        * FUNCTION hasVar() -> Bool:
        *       Returns true if O or any of O's child Operations contain a variable. 
        *       Otherwise 0 (false) is returned
        */
        func hasVar() -> Bool{
            
            if self.o == "i"{
                return false
            }
            if self.o == "x"{
                return true
            }
            return (self.l!.hasVar() || self.r!.hasVar())
            
        }
        
        
        /*
        * FUNCTION: eval(plugIn:Double) -> Double
        *      Takes an operation and evaluates it to a double. The function is
        *      recursive, and each child operation of the original will call
        *      eval until an 'i' or 'x' operation is reached. 'i' (short for id)
        *      evaluates to a double.
        *
        * RETURNS: The result of Operation as a double.
        *
        * PRECONDITIONS:
        *      (1) Operation must have valid operator
        *      (2) if dividing, right operand cannot be zero
        */
        func eval(plugIn:Double) -> Double{
        
            
            switch self.o{
            case "i":
                return self.n!
            
            case "x":
                return plugIn
                
            case "^":
                return Operation(num: pow(self.l!.eval(plugIn),self.r!.eval(plugIn))).eval(plugIn)
            
            case "*":
                return Operation(num:self.l!.eval(plugIn) * self.r!.eval(plugIn)).eval(plugIn)
            
            case "/":
                let denom = Operation(num:self.r!.eval(plugIn)).eval(plugIn)
                if denom == 0{
                    NSException(name:"Divide by zero", reason:"denominator evaluates to zero", userInfo:nil).raise()
                }
                return Operation(num:self.l!.eval(plugIn) / denom).eval(plugIn)
            
            case "+":
                return Operation(num:self.l!.eval(plugIn) + self.r!.eval(plugIn)).eval(plugIn)
                
            case "-":
                return Operation(num:self.l!.eval(plugIn) - self.r!.eval(plugIn)).eval(plugIn)
                
            default:
                return 0.0
            }
        }
        
        /*
        * FUNCTION: printOp()
        *      Prints option to given output.
        */
        func printOp(){
            if self.o == "i"{print(self.n!);return}
            if self.o == "x"{print("x");return}
            print("( ")
            if self.l == nil{
                print("_")
            }else{
                self.l!.printOp()
            }
            print(" ")
            print(self.o)
            print(" ")
            if self.l == nil{
                print("_")
            }else{
                self.r!.printOp()
            }
            print(" )")
        }
        
    }
    
    // --- PROPERTIES ---
    var ops: [Operation]
    var eql: Operation?
    let has_var:Bool
    
    
    
    
    // --- METHODS ---
    
    /*
    * EXPRESSION INIT:
    *      This init is for an expression with a nil eql property
    */
    init(){
        self.ops = []
        self.eql = nil
        self.has_var = false
    }
    
    /*
    * EQUATION INIT:
    *      This init is for an equation
    */
    init(eql_num:Double){
        self.ops = []
        self.eql = Operation(num:eql_num)
        self.has_var = true
    }
    
    
    /*
    * FUNCTION: appendOp()
    *      Create new variable operation and appends it to the back of the list.
    */
    func append(){
        let toAdd = Operation()
        self.ops.append(toAdd)
    }
    
    /*
    * FUNCTION: appendOp(number:Double)
    *      Create new id operation and appends it to the back of the list.
    */
    func append(number:Double){
        let toAdd = Operation(num: number)
        self.ops.append(toAdd)
    }
    
    /*
    * FUNCTION: appendOp(oper:Character)
    *      Create new operator operation and appends it to the back of the list.
    */
    func append(oper:Character){
        let toAdd = Operation(oper: oper)
        self.ops.append(toAdd)
    }
    
    /*
    * FUNCTION: delete(index:Int)
    *      Deletes operation at index 
    *
    * PRECONDITIONS:
    *       (1) Index must be in the range of the ops array
    */
    func delete(index:Int){

        if index < 0 || index >= self.ops.count{
            NSException(name:"Index Out of Bounds", reason:"Index must be within range of array", userInfo:nil).raise()
        }
        self.ops.removeAtIndex(index)
    }
    
    /*
    * FUNCTION: setLeftOperand(index:Int)
    *      Removes operation left of the operation at index and sets it as index's 
    *      left operand.
    *
    * PRECONDITIONS:
    *       (1) Index must be in the range of the ops array
    *       (2) Index cannot be the first element
    */
    func setLeftOperand(index:Int){

        if index < 1 || index >= self.ops.count{
            NSException(name:"Index Out of Bounds", reason:"Index must be within range of array but cannot be first element", userInfo:nil).raise()
        }
        
        self.ops[index].l = self.ops[index-1]
        self.delete(index-1)
    }
    
    /*
    * FUNCTION: setRightOperand(index:Int)
    *      Removes operation right of the operation at index and sets it as index's
    *      right operand.
    *
    * PRECONDITIONS:
    *       (1) Index must be in the range of the ops array
    *       (2) Index cannot be the last element
    */
    func setRightOperand(index:Int){
        if index < 0 || index >= self.ops.count-1{
            NSException(name:"Index Out of Bounds", reason:"Index must be within range of array but cannot be last element", userInfo:nil).raise()
        }
        
        self.ops[index].r = self.ops[index+1]
        self.delete(index+1)
    }
    
    
    /*
    * FUNCTION: setOperands(index:Int)
    *      Removes the operations right and light of the operation at index and sets 
    *      it as index's operands.
    *
    * PRECONDITIONS:
    *       (1) Index must be in the range of the ops array
    *       (2) Index cannot be the first or last element
    */
    func setOperands(index:Int){
        
        if index < 1 || index >= self.ops.count{
            NSException(name:"Index Out of Bounds", reason:"Index must be within range of array but cannot be first or last element", userInfo:nil).raise()
        }
        self.setLeftOperand(index)
        self.setRightOperand(index-1)
    }
    
    
    /*
    * FUNCTION: eval(num:Double) -> Double
    *      Evaluates the list to a double value by setting all operands and evaluating
    *      the top most operation.
    *
    * RETURNS: Double represenation or the evaluation
    */
    func eval(num:Double) -> Double{
        if self.ops.count != 1{
            self.setAllOperands()
        }
        return self.ops[0].eval(num)
    }
    
    /*
    * FUNCTION: printOpList()
    *      Prints list for debugging.
    */
    func printOpList(){
        if self.ops.count == 0{
            println("[]")
            return
        }
        print("[")
        
        for elem in self.ops{
            elem.printOp()
            if self.ops.last === elem{
                break
            }
            print(", ")
        }
        
        println("]")
    }
    
    /*
    * FUNCTION: highestPrc() -> Int
    *      Finds operation with highest prc and returns its index
    *
    * RETURNS: index of operation
    */
    func highestPrc() -> Int{
        var toRet = 1
        for var i = 1; i < self.ops.count; i+=2 {
            if self.ops[i].p > self.ops[toRet].p{
                toRet = i
            }
        }
        return toRet;
    }
    
    
    /*
    * FUNCTION: setAllOperands()
    *      Sets operands in the order of precedence (highest first) until only one 
    *      operation remains
    */
    func setAllOperands(){
        while self.ops.count != 1{
            setOperands(self.highestPrc())
        }
    }
    
    
    
    
    /*
    * FUNCTION: argToOpList(int argc, char **argv)
    *      Takes a string and its length and parses char
    *      representations of operations into an OpList.
    *
    *      NOTES:
    *             (1) This function checks for preconditions mostly in its body
    *                 rather than at the very beginning as this is more efficient
    *                 and more readable.
    *
    *             (2) The '-' is treated as a subtraction operation if a number
    *                 was last parsed. Otherwise it is a negative sign and parsed
    *                 as part of a number
    *
    *             (3) Parens alter precedence by 5, ensuring that the child
    *                 operations precede outer ones regardless of default
    *                 operator precedence
    *
    * RETURNS: A string with an error message if a precondition is violated, nil if no
    *          error
    *
    * PRECONDITIONS:
    *      (1) argv must contain at least one operation
    *      (2) no invalid chars
    *      (3) no consecutive numbers
    *      (4) no consecutive operators
    *      (5) no consecutive negative signs for a single number
    *      (6) no consecutive decimal points for a single number
    *      (7) each openning paren must be matched with a closing paren
    *      (8) only one unknown is allowed
    */
    func argToOpList(arg:String) -> String?{
        // --- error messages ---
        let consec_num = "consecutive numbers without operator"
        let consec_opr = "consecutive operators or operator without number"
        let invald_frm = "invalid format for number"
        let multip_var = "more than one variable"
        
        // --- variables ---
        let xtr_prc = 5
        var open_prn = 0
        var nxt_num = ""
        
        var dec_alr = false
        var lst_num = false // should keep track of number and variable
        var lst_spc = false
        var var_alr = false
        var lst_prnc = false
        var lst_prno = false
        var lst_opr = false
        var dig_alr = false
        var lst_var = false

        /*
         * INNER FUNCTION: addOpr
         */
        func addOpr(op:Character) -> String?{
            if !lst_num || lst_opr{
                return consec_opr
            }
            
            self.append(op)
            self.ops.last!.p += (xtr_prc * open_prn)
            
            lst_num = false
            nxt_num = ""
            dec_alr = false
            lst_spc = false
            
            lst_opr = true
            lst_prno = false
            lst_prnc = false
            lst_var = false
            return nil
        }
        
        /*
        * INNER FUNCTION: addNum
        */
        func addNum() -> String?{
            if lst_num && !lst_prnc && self.ops.last!.o != "x"{
                return consec_num
            }
            if nxt_num == ""{
                return nil
            }
            if lst_prnc {
                // --- copy of addOpr func with "*" hard set ---
                if !lst_num || lst_opr{
                    return consec_opr
                }
                
                self.append("*")
                self.ops.last!.p += (xtr_prc * open_prn)
                
                lst_opr = true
                // ---------------------------------------------
            }
            if self.ops.last != nil && self.ops.last!.o == "x" {
                self.append("*")
                self.ops.last!.p += (xtr_prc * open_prn)
                
                lst_opr = true

            }

            if nxt_num == "." || nxt_num == "-" || nxt_num == "-."{
                return invald_frm
            }
   
            
            self.append((nxt_num as NSString).doubleValue)
            self.ops.last!.p += (xtr_prc * open_prn)
            
            lst_num = true
            nxt_num = ""
            dec_alr = false
            lst_spc = false
            lst_prnc = false
            lst_prno = false
            lst_opr = false
            lst_var = false
            return nil
        }
        
        /*
        * INNER FUNCTION: addVar
        */
        func addVar() -> String?{
            if var_alr{
                return multip_var
            }
            if lst_num{
                // --- copy of addOpr func with "*" hard set ---
                if !lst_num || lst_opr{
                    return consec_num
                }
                
                self.append("*")
                self.ops.last!.p += (xtr_prc * open_prn)
                
                lst_opr = true
                // ---------------------------------------------
            }
            self.append()
            self.ops.last!.p += (xtr_prc * open_prn)
            
            lst_num = true
            dec_alr = false
            lst_spc = false
            lst_prnc = false
            lst_prno = false
            lst_opr = false
            lst_var = true
            return nil
        }
        
        var toRet:String? = nil
        
        for c in arg{
            switch c{
                
            case "0","1","2","3","4","5","6","7","8","9":
                nxt_num.append(c)
                lst_spc = false
                lst_prno = false
                dig_alr = true
                lst_var = false
                
            case ".":
                if dec_alr{
                    return consec_num
                }
                nxt_num.append(c)
                dec_alr = true
                lst_spc = false
                lst_prno = false
                lst_var = false
                
            case "-":
                if nxt_num != ""{
                    toRet = addNum()
                    if toRet != nil{ return toRet }
                }
                if lst_num{
                    toRet = addOpr("-")
                    if toRet != nil{ return toRet }
                }else{
                    nxt_num.append(c)
                }
                lst_spc = false
                lst_prno = false
                lst_var = false
                
            case "x":
                if self.eql == nil{
                    return "No knowns allowed in an expression"
                }
                if nxt_num != ""{
                    if nxt_num == "-"{
                        nxt_num += "1"
                    }
                    toRet = addNum()
                    if toRet != nil{ return toRet }
                }
                toRet = addVar()
                if toRet != nil{ return toRet }
                lst_spc = false
                lst_prno = false
                var_alr = true
                lst_var = true
                
            case "^","*","/","+":
                if c == "^" && self.eql != nil{
                    return "^ cannot be used with an unknown"
                }
                if c == "/" && self.eql != nil && !var_alr{
                    return "/ cannot be used before an unknown"
                }
                if nxt_num != ""{
                    toRet = addNum()
                    if toRet != nil{ return toRet }
                }
                toRet = addOpr(c)
                if toRet != nil{ return toRet }
                lst_prno = false
                lst_spc = false
                lst_var = false
                
            case "(":
                if nxt_num != "" {
                    if nxt_num == "-" {
                        nxt_num += "1"
                    }
                    toRet = addNum()
                    if toRet != nil{ return toRet }
                    
                    toRet = addOpr("*")
                    if toRet != nil{ return toRet }
                }
                if lst_num {
                    toRet = addOpr("*")
                    if toRet != nil{ return toRet }
                }
                open_prn++
                lst_prno = true
                lst_prnc = false
                lst_spc = false
                lst_var = false
                
            case ")":
                if lst_prno{
                    return "empty parens"
                }
                if nxt_num != ""{
                    toRet = addNum()
                    if toRet != nil{ return toRet }
                }
                if !lst_num {
                    return consec_opr
                }
                lst_num = true
                lst_prnc = true
                lst_prno = false
                lst_spc = false
                lst_var = false
                open_prn--
                
            case " ":
                if lst_spc {
                    continue
                }
                
                if nxt_num != "" {
                    toRet = addNum()
                    if toRet != nil{ return toRet }
                }
                lst_spc = true
                lst_prno = false
            default:
                return "Invalid char"
            }
        }
        
        if open_prn > 0{
            return "unclosed '('"
        }
        if open_prn < 0{
            return "unopenned ')'"
        }
        self.printOpList()
        if self.eql != nil && !var_alr{
            return "no unknown"
        }
        if !(dig_alr || var_alr){
            return invald_frm
        }
        
        if nxt_num == "" && !lst_num{
            return consec_opr
        }
    
        if nxt_num != ""{
            addNum()
            self.printOpList()
        }
        
        return nil
    }
    
    
    /*
    * FUNCTION: solve() -> Double
    *      Solves for variable by searching for correct value with binary
    *      search. Max and Min values for this function are defined as constants
    *      in the function body
    *
    * RETURNS: double representation of estimate for var value
    *
    * PRECONDITIONS:
    *      (1) List must contain at least one operation
    */
    func solve() -> Double{
        if self.ops.count == 0{
            NSException(name:"No Operation", reason:"OpList does not contain an operation", userInfo:nil).raise()
        }
        if self.ops.count != 1{
            self.setAllOperands()
        }
        
        // --- VARIABLES ---
        let max_val = 9999999999999978.99999
        let min_val = -9999999999999978.99999
        
        var tryVal = (min_val + max_val)/2
        var tryEvl = self.eval(tryVal)
        var eqlVal = self.eql!.eval(0.0)
        
        let frstTry  = tryVal
        let frstEvl = tryEvl
        
        var max = max_val
        var min = min_val
        
        // --- SEARCH ---
        while tryEvl != eqlVal {
            
            tryEvl = self.eval(tryVal)
            if tryEvl < eqlVal{
                min = tryVal
                tryVal = (min + max)/2
                if (frstEvl > tryEvl && frstTry < tryVal){
                    return solveSwitched()
                }
            }else{
                max = tryVal
                tryVal = (min + max)/2
                if (frstEvl < tryEvl && frstTry > tryVal){
                    return solveSwitched()
                }
            }
            
        }
        return tryVal
    }
    
    /*
    * HELPER FUNCTION: solveSwitched() -> Double
    *      Some operations result in a tryVal and tryEval that grow in opposite
    *      directions (e.i. one gets larger positively as the other gets larger
    *      negetively). This function is identical to solveVal(), except that
    *      the comparison operation is switched to prevent an infinite loop.
    *
    * RETURNS: double representation of estimate for var value
    */
    func solveSwitched() -> Double{
        if self.ops.count != 1{
            self.setAllOperands()
        }
        
        let max_val = 9999999999999978.99999
        let min_val = -9999999999999978.99999
        
        var tryVal = (min_val + max_val)/2
        var tryEvl = self.eval(tryVal)
        var eqlVal = self.eql!.eval(0.0)
        
        var max = max_val
        var min = min_val
        
        while tryEvl != eqlVal {
            
            tryEvl = self.eval(tryVal)
            if tryEvl > eqlVal{
                min = tryVal
                tryVal = (min + max)/2

            }else{
                max = tryVal
                tryVal = (min + max)/2

            }
            
        }
        return tryVal
    }

}

